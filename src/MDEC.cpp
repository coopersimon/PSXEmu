#include <MDEC.h>
#include <FixedPointMaths.h>

mdec::mdec()
{
      // instantiate FIFOs

}

// interface functions

void mdec::wordFromDMA(word in)
{
      // if MDEC isn't expecting a parameter, prepare command
      if (opcode == 0)
            setupCommand(in);
      // otherwise send parameter and execute
      else
            executeCommand(in);
}

word mdec::wordToDMA()
{
      // TODO: reorder blocks?
      return readData();
}

word mdec::readWord(unsigned address)
{
      if (address % 2 == 0)
            return readData();

      else if (address % 2 == 1)
            return readStatusReg();

      // TODO: throw error
}

void mdec::writeWord(unsigned address, word in)
{
      if (address % 2 == 0)
      {
            // if MDEC isn't expecting a parameter, prepare command
            if (opcode == 0)
                  setupCommand(in);
            // otherwise send parameter and execute
            else
                  executeCommand(in);
      }

      else if (address % 2 == 1)
            setControl(in);

      // TODO: throw error
}

// control functions

void mdec::setupCommand(word command)
{
      opcode = (command >> 29) & 0x7;

      // set parameters
      status_reg.bit_fields.data_output_depth = (command >> 27) & 0x3;
      status_reg.bit_fields.data_output_signed = (command >> 26) & 0x1;
      status_reg.bit_fields.data_output_bit_15 = (command >> 25) & 0x1;

      // decode
      if (opcode == 1)
      {
            status_reg.bit_fields.command_busy = 1;
            status_reg.bit_fields.current_block = 0;
            status_reg.bit_fields.par_words_rem = (command & 0xFFFF) - 1;

            command_executed = false;
      }

      // set quant table
      else if (opcode == 2)
      {
            status_reg.bit_fields.command_busy = 1;

            // set luminance and colour tables
            if (command & 0x1)
            {
                  opcode = 4;
                  status_reg.bit_fields.par_words_rem = 127;
            }
            // set only luminance table
            else
                  status_reg.bit_fields.par_words_rem = 63;
      }

      // set scale table
      else if (opcode == 3)
      {
            status_reg.bit_fields.command_busy = 1;
            status_reg.bit_fields.par_words_rem = 63;
      }

      // invalid instruction
      else
      {
            opcode = 0;
      }
}

void mdec::executeCommand(word parameter)
{
      unsigned param_num = status_reg.bit_fields.par_words_rem;

      /*  
       *  decode macroblock:
       *  each macroblock consists of 6 blocks
       *  each block consists of up to 32 words (8x8 halfwords)
       *    (potentially with extra empty block following)
       *  each block needs to be buffered before decoding
       */
      if (opcode == 1)
      {
            // first, fill FIFO
            // split into upper and lower halfwords
            data_in_fifo.write(parameter & 0xFFFF);
            data_in_fifo.write((parameter >> 16) & 0xFFFF);

            // if all data has been taken in:
            if (data_in_fifo.full())
            {
                  decodeBlock();
                  
                  // monochrome: depth = 4 or 8 bit
                  if (status_reg.bit_fields.data_output_depth == 0 ||
                      status_reg.bit_fields.data_output_depth == 1)
                  {
                        yToMono();
                  }
                  // colour: depth = 15 or 24 bit
                  else if(status_reg.bit_fields.current_block > 1)
                  {
                        yuvToRGB();
                  }

                  data_in_fifo.clear();
            }
      }

      // set luminance table
      else if (opcode == 2)
      {
            lum_quant_table[63 - param_num] = parameter & 0xFF;
      }

      // set luminance table
      else if (opcode == 4 && param_num >= 64)
      {
            lum_quant_table[127 - param_num] = parameter & 0xFF;
      }

      // set colour table
      else if (opcode == 4 && param_num < 64)
      {
            color_quant_table[63 - param_num] = parameter & 0xFF;
      }

      // set scale table
      else if (opcode == 3)
      {
            scale_table[63 - param_num] = parameter & 0xFFFF;
      }

      status_reg.bit_fields.par_words_rem--;
      
      // end execution
      if (status_reg.bit_fields.par_words_rem == 0xFFFF)
      {
            opcode = 0;
            status_reg.bit_fields.command_busy = 0;
      }
}

// TODO: are all these functions necessary?
word mdec::readData()
{
      return data_out_fifo.read();
}

void mdec::setControl(word command)
{
      if (command >> 31)
      {
            // reset.
      }
      status_reg.bit_fields.data_in_request = (command >> 30) & 0x1;
      status_reg.bit_fields.data_out_request = (command >> 29) & 0x1;
}

word readStatusReg()
{
      return status_reg.data_field;
}

// Decoding functions

void mdec::decodeBlock()
{
      unsigned block = status_reg.bit_fields.current_block;
      unsigned q_scale = 0xFFFFFFFF;
      unsigned k = 0;

      std::array<s_halfword, 64> result_block = {0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 0, 0};

      while (k < 64)
      {
            halfword data = data_in_fifo.read();
            // ignore padding
            if (data == 0xFE00)
                  continue;
            // set q scale if necessary
            if (q_scale == 0xFFFFFFFF)
                  q_scale = (data >> 10) & 0x3F;

            // get data as signed value
            s_halfword value = (data << 6) >> 6;

            // multiply by table value
            if (q_scale == 0)
                  value *= 2;
            else if (block <= 1 && q_scale == 0xFFFFFFFF)
                  value *= color_quant_table[k];
            else if (block <= 1)
                  value *= (color_quant_table[k] * q_scale + 4) / 8;
            else if (block > 1 && q_scale == 0xFFFFFFFF)
                  value *= lum_quant_table[k];
            else if (block > 1)
                  value *= (lum_quant_table[k] * q_scale + 4) / 8;

            // saturate value
            if (value > 1023)
                  value = 1023;
            else if (value < -1024)
                  value = -1024;

            //value *= scale_table[k];
            result_block[k] = value;

            k += (data >> 10) & 0x3F;
            k++;
      }

      idct(result_block);

      if (block == 0)
            decoded_cr = result_block;
      else if (block == 1)
            decoded_cb = result_block;
      else
            decoded_y = result_block;
}

/*void mdec::fast_idct(std::array<word, 64>& block)
{
      for (unsigned i = 0; i < 8; ++i)
      {
            // if all are 0...

            
      }
}*/

void mdec::idct(std::array<word, 64>& block)
{
      std::array<word, 64> new_block;

      for (unsigned pass = 0; pass < 2; ++pass)
      {
            for (unsigned x = 0; x < 8; ++x)
            {
                  for (unsigned y = 0; y < 8; ++y)
                  {
                        word sum = 0;
                        for (unsigned z = 0; z < 8; ++z)
                        {
                              sum += block[y + z*8] * (scale_table[x + z*8] / 8);
                        }
                        new_block[x + y*8] = (sum + 0xFFF)/0x2000;
                  }
            }
            block.swap(new_block);
      }
}

void mdec::yuvToRGB24(unsigned x_base, unsigned y_base)
{
      for (unsigned y = 0 + y_base; y < 8 + y_base; ++y)
      {
            for (unsigned x = 0 + x_base; x < 8 + x_base; ++x)
            {
                  unsigned block_index = (x / 2) + ((y / 2) * 8);
                  s_halfword R = decoded_cr[block_index];
                  s_halfword B = decoded_cb[block_index];
                  s_halfword G = (R * 0xFF49) + (B * 0xFFA8); // R * -0.7143 + B * -0.3437
                  R *= 0x167; // R * 1.402
                  B *= 0x1C6; // B * 1.772

                  block_index = (x - x_base) + ((y - y_base) * 8);
                  s_halfword Y = decoded_y[block_index];
                  
                  // cut and saturate RGB values
                  // set to signed/unsigned
                  // write output
                  
            }
      }
}

void mdec::yToMono8()
{
      // TODO: this is 8 bit only. need to do 4 bit.
      for (unsigned i = 0; i < 64; ++i)
      {
            s_halfword Y = halfword(decoded_y[i]);
            
            // get as a signed 9 bit value
            Y &= 0x1FF;
            if (Y & 0x100)
                  Y |= 0xFE00;

            // saturate
            // Y = (Y << 8) >> 8;
            if (Y > 127)
                  Y = 127;
            else if (Y < -128)
                  Y = -128;

            if (!status_reg.bit_fields.data_output_signed)
                  Y += 128;

            data_out_fifo.write(Y);
      }
}




