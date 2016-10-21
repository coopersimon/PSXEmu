#ifndef DMA_H
#define DMA_H

/*
 *    DMA.h
 *
 *    Used to define the DMA: allows devices to stream data directly from RAM
 *    CPU --write--> DMA --command--> (GPU/SPU/MDEC..)
 */

// includes
#include <Memory.h>
#include <vector>


// TODO: all devices which utilise DMA need to inherit from the same class.
// TODO: should this class be declared in this header?

// all devices which have a DMA channel inherit from this class.
class dmaDevice
{
public:
      virtual void wordFromDMA(word in) = 0;
      virtual word wordToDMA() = 0;
};


// channel for individual device
class dmaChannel
{
      // some sort of device access.
      dmaDevice* device;
      RAMImpl* main_ram;

      unsigned base_address;
      word block_control;
      // TODO: should channel control be split into components?
      // (word?) channel control;

public:
      dmaChannel(dmaDevice* device_ptr, RAMImpl* ram_ptr);

      // transfers a block from RAM, into device 
      void transferBlockFromRAM();
      // transfers a block from device, into RAM
      void transferBlockToRAM();

      /*enum
      {
            DMABase = 0;
            DMABlockControl = 4;
            DMAChannelControl = 8;
      };*/

      // write base address, block control or channel control.
      void writeRegister(word data, unsigned reg);
      // read base address, block control or channel control.
      word readRegister(unsigned reg);

private:
      // transfers a single word from RAM, into device
      void transferWordFromRAM();
      // transfers a single word from device, into RAM
      void transferWordToRAM();

};


// dma will be accessed through memBus
// this works out which channel to send/receive data
class dma : public memoryInterface
{
      std::vector<dmaChannel> channel_list;

      word control_reg;
      word interrupt_reg;

public:
      dma(RAMImpl* main_ram);

      // memoryInterface functions
      byte readByte(unsigned address);
      void writeByte(unsigned address, byte in);
      
      halfword readHalfwordLittle(unsigned address);
      void writeHalfwordLittle(unsigned address, halfword in);
      halfword readHalfwordBig(unsigned address);
      void writeHalfwordBig(unsigned address, halfword in);
      
      word readWordLittle(unsigned address);
      void writeWordLittle(unsigned address, word in);
      word readWordBig(unsigned address);
      void writeWordBig(unsigned address, word in);
};

#endif
