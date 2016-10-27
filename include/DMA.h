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
      virtual void executeCommand() = 0;
};


// channel for individual device
class dmaChannel
{
      // some sort of device access.
      dmaDevice* device;
      RAMImpl* main_ram;

      // address where reading begins
      unsigned base_address;
      // number and size of blocks to transfer
      word block_control;
      
      // channel control
      union {
            struct {
                  unsigned transfer_direction : 1;
                  unsigned mem_address_step : 1;
                  unsigned : 6;
                  unsigned chopping : 1;
                  unsigned sync_mode : 2;
                  unsigned : 5;
                  unsigned chop_dma_window_size : 3;
                  unsigned : 1;
                  unsigned chop_cpu_window_size : 3;
                  unsigned : 1;
                  unsigned start_busy : 1;
                  unsigned : 3;
                  unsigned start_trigger : 1;
                  unsigned : 3;
            } bit_fields;
            word data_field;
      } channel_control;

public:
      dmaChannel(dmaDevice* device_ptr, RAMImpl* ram_ptr) :
            device(device_ptr), main_ram(ram_ptr) {}

      // transfers a block between RAM and device 
      void transferBlock();

      enum DMAReg
      {
            DMABase = 0;
            DMABlockControl = 4;
            DMAChannelControl = 8;
      };

      // write base address, block control or channel control.
      void writeRegister(word data, DMAReg reg);
      // read base address, block control or channel control.
      word readRegister(DMAReg reg);

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
      // constructor creates dmaChannel for each device
      dma(RAMImpl* main_ram, std::vector<dmaDevice*> &devices);

      // memoryInterface functions
      byte readByte(unsigned address) override;
      void writeByte(unsigned address, byte in) override;
      
      halfword readHalfwordLittle(unsigned address) override;
      void writeHalfwordLittle(unsigned address, halfword in) override;
      halfword readHalfwordBig(unsigned address) override;
      void writeHalfwordBig(unsigned address, halfword in) override;
      
      word readWordLittle(unsigned address) override;
      void writeWordLittle(unsigned address, word in) override;
      word readWordBig(unsigned address) override;
      void writeWordBig(unsigned address, word in) override;

private:
      inline word readRegister(unsigned device)
      {
            if ((device < 0xF) && (device >= 0x8))
                  return channel_list[device - 8].readRegister(address % 0xF);
            
            else if (address == 0xF0)
                  return control_reg;
            
            else if (address == 0xF4)
                  return interrupt_reg;
            
            // exception? idk.
            else
                  return 0;
      }

      inline void writeRegister(unsigned device, word in)
      {
            if ((device < 0xF) && (device >= 0x8))
                  channel_list[device - 8].writeRegister(in, address % 0xF);

            else if (address == 0xF0)
                  control_reg = in;

            else if (address == 0xF4)
                  interrupt_reg = in;
      }
};

#endif
