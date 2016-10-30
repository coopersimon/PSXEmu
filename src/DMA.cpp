#include <DMA.h>

/*** DMA device channel ***/

void dmaChannel::transferBlock()
{
      // set start bits
      channel_control.bit_fields.start_busy = 1;
      channel_control.bit_fields.start_trigger = 0;

      // transfer all at once
      if (channel_control.bit_fields.sync_mode == 0)
      {
            // TODO: if chopping is enabled decrement block_control register
            unsigned block_size = block_control & 0xFFFF;
            if (block_size == 0)
                  block_size = 0x10000;

            // transfer to RAM
            if (channel_control.bit_fields.transfer_direction == 0)
            {
                  for (int i = 0; i < block_size; ++i)
                  {
                        transferWordToRAM();
                        // mem_address_step: if 0, +4; if 1, -4
                        base_address += (4 - (8 * channel_control.bit_fields.mem_address_step));
                  }
            }
            // transfer from RAM
            else
            {
                  for (int i = 0; i < block_size; ++i)
                  {
                        transferWordFromRAM();
                        // mem_address_step: if 0, +4; if 1, -4
                        base_address += (4 - (8 * channel_control.bit_fields.mem_address_step));
                  }
            }
      }

      /* //sync blocks to DMA requests
      if (channel_control.bit_fields.sync_mode == 1)
      {
            unsigned block_size = block_control & 0xFFFF;
            if (block_size == 0)
                  block_size = 0x10000;

            // transfer to RAM
            if (channel_control.bit_fields.transfer_direction == 0)
            {
                  for (int i = 0; i < block_size; ++i)
                  {
                        transferWordToRAM();
                        // mem_address_step: if 0, +4; if 1, -4
                        base_address += (4 - (8 * channel_control.bit_fields.mem_address_step));
                  }
            }
      }*/

      // TODO: deal with this when GPU work begins.
      // GPU command lists
}

void dmaChannel::writeRegister(word data, DMAReg reg)
{
      if (reg == DMABase)
            base_address = data;

      else if (reg == DMABlockControl)
            block_control = data;
      
      else if (reg == DMAChannelControl)
            channel_control.data_field = data;
}

word dmaChannel::readRegister(DMAReg reg)
{
      if (reg == DMABase)
            return base_address;
      
      else if (reg == DMABlockControl)
            return block_control;
      
      else if (reg == DMAChannelControl)
            return channel_control.data_field;

      // exception? idk.
      else
            return 0;
}

void dmaChannel::transferWordFromRAM()
{
      word data = main_ram->readWord(base_address);
      device->wordFromDMA(data);
}

void dmaChannel::transferWordToRAM()
{
      word data = device->wordToDMA();
      main_ram->writeWord(base_address, data);
}


/*** DMA controller ***/

dma::dma(RAMImpl* main_ram, std::vector<dmaDevice*> &devices)
{
      for (dmaDevice* d : devices)
      {
            channel_list.push_back(dmaChannel(d, main_ram));
      }

      control_reg = 0x07654321;
}

word dma::readWord(unsigned address)
{
      return readRegister(device);
}

void dma::writeWord(unsigned address, word in)
{
      writeRegister(device, in);
}
