#ifndef MDEC_H
#define MDEC_H

/*
 *    MDEC.h
 *
 *    Declares the Motion Decoder Class
 *    Used for decoding images and FMV
 */

// includes
#include <DMA.h>
#include <FIFO.h>


class mdecBlock
{
      halfword data;

public:
      halfword readLEN()
      {
            return data >> 10;
      }
      s_halfword readAC()
      {
            return s_halfword(data & 0x3FF);
      }

      void write(halfword in)
      {
            data = in;
      }
};


class mdec : public dmaDevice
{
      // status register components
      unsigned data_out_fifo_empty : 1;
      unsigned data_in_fifo_full : 1;
      unsigned command_busy : 1;
      unsigned data_in_request : 1;
      unsigned data_out_request : 1;
      unsigned data_output_depth : 2;
      unsigned data_output_signed : 1;
      unsigned data_output_bit_15 : 1;
      unsigned : 4;
      unsigned current_block : 3;
      unsigned param_words_remaining : 16;

      // data FIFO
      FIFOImpl<word> out_data_FIFO;

      // tables
      halfword lum_quant_table[64];
      halfword color_quant_table[64];
      halfword scale_table[64];

      // TODO: scale factor (?)

public:
      // constructor

      // 0x1F801820 (mem address)
      void executeCommand(mdecBlock data);
      word readData();

      // 0x1F801824 (mem address)
      void setControl(word command);
      word readStatusReg();

      // DMA functions: effectively identical to executeCommand
      void wordFromDMA(word in);
      word wordToDMA();

private:
      // internal decoding functions:

      // decodes a single macroblock.
      void decodeColoredMacroblock();
      void decodeMonochromeMacroblock();

      // conversions between colour spaces
      void yuvToRgb(mdecBlock* dataBlocks);
      void yToMono(mdecBlock* dataBlocks);

};


#endif
