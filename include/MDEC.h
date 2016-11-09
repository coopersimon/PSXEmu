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
#include <vector>
#include <array>


class mdec : public dmaDevice, public memoryInterface
{
      // status register components
      bool data_out_fifo_empty;
      bool data_in_fifo_full;
      bool command_busy;
      bool data_in_request;
      bool data_out_request;
      unsigned data_output_depth;
      bool data_output_signed;
      bool data_output_bit_15;
      unsigned current_block;
      unsigned par_words_rem;

      // data FIFOs
      FIFOImpl<halfword> data_in_fifo;
      FIFOImpl<halfword> data_out_fifo;

      // internal data
      std::array<s_halfword, 64> decoded_cr;
      std::array<s_halfword, 64> decoded_cb;
      std::array<s_halfword, 64> decoded_y;

      // tables

      std::array<byte, 64> lum_quant_table;
      std::array<byte, 64> color_quant_table;
      std::array<halfword, 64> scale_table;

      // TODO: scale factor (?)

      // internal status
      unsigned opcode;
      bool command_executed;

public:
      // constructor
      mdec();

      // DMA functions: effectively identical to 0x1F801820 reg
      void wordFromDMA(word in) override;
      word wordToDMA() override;

      // memoryInterface functions
      word readWord(unsigned address) override;
      void writeWord(unsigned address, word in) override;

private:
      // 0x1F801820 (mem address)
      void setupCommand(word command);
      void executeCommand(word parameter);
      word readData();

      // 0x1F801824 (mem address)
      void setControl(word command);
      word readStatusReg();

      // internal decoding functions:

      // decodes a single block
      void decodeBlock();

      // inverse discrete cosine transform
      //void fast_idct(std::array<word, 64>& decoded_block);
      void idct(std::array<word, 64>& decoded_block);
      
      // conversions between colour spaces
      //void yuvToRGB15();
      //void yuvToRGB24();
      void yToMono8();
};


#endif
