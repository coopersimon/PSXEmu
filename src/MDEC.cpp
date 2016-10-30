#include <MDEC.h>

mdec::mdec()
{
      // instantiate regs
      //
      // instantiate FIFOs
}

void mdec::executeCommand(word parameter)
{
      opcode = (parameter >> 29) & 0x7;

      // set parameters
      data_output_depth = (command >> 27) & 0x3;
      data_output_signed = (command >> 26) & 0x1;
      data_output_bit_15 = (command >> 25) & 0x1;

      if (opcode == 1)
      {
            param_words_remaining = command & 0xFFFF;
            param_words_remaining--;
      }

      /* set quant table
      else if (opcode == 2)
      {
            for (int i = 0; i < 64; i++)
            {
                  lum_quant_table[i] = data_blocks[i];
            }
            if (command & 1)
            {
                  for (int i = 0; i < 64; i++)
                  {
                        color_quant_table[i] = data_blocks[i + 64];
                  }
            }
      }

      // set scale table
      else if (opcode == 3)
      {
            for (int i = 0; i < 64; i++)
            {
                  scale_table[i] = data_blocks[i];
            }
      }*/
}





void mdec::yuvToRgb(mdecBlock* dataBlocks)
{
      
}

void mdec::yToMono(mdecBlock* dataBlocks)
{
      for (int i = 0; i < 64; i++)
      {
      }
}
