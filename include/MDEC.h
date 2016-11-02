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
      union {
            struct {
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
                  unsigned par_words_rem : 16;
            } bit_fields;
            word data_field;
      } status_reg;

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

      // 0x1F801820 (mem address)
      void setupCommand(word command);
      void executeCommand(word parameter);
      word readData();

      // 0x1F801824 (mem address)
      void setControl(word command);
      word readStatusReg();

      // DMA functions: effectively identical to 0x1F801820 reg
      void wordFromDMA(word in) override;
      word wordToDMA() override;

      // memoryInterface functions
      word readWord(unsigned address) override;
      void writeWord(unsigned address, word in) override;

private:
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
