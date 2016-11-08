#include <Controller.h>


bool controllerIO::IRQ7Triggered()
{
      // check pad1 for interrupt
      // check pad2 for interrupt
}

word controllerIO::readWord(unsigned address)
{
      if (address % 4 == 0)
      {
            return readData();
      }
      else if (address % 4 == 1)
      {
            return readStatusReg();
      }
      else if (address % 4 == 2)
      {
            return readModeReg();
      }
      else
            return 0;
}

void controllerIO::writeWord(unsigned address, word in)
{
      if (address % 4 == 0)
      {
            writeData(in);
      }
      else if (address % 4 == 2)
      {
            writeControlReg(in);
      }
}




void controllerIO::writeData(word data)
{
      // check that the data can be transferred
      if (control_reg.bit_fields.TX_enable)
      {
            byte receivedData;
            if (control_reg.bit_fields.slot_number == 0)
            {
                  receivedData = pad_1->transmitBytes(byte(data & 0xFF));
            }
            else
            {
                  receivedData = pad_2->transmitBytes(byte(data & 0xFF));
            }

            // write received data to fifo
            RX_data.write(receivedData);
      }
}

word controllerIO::readData()
{
      // simply returns value in fifo
      // when writing a byte this data will be destroyed-
      // however whenever a byte is written the fifo should be empty anyway
      return word(RX_data.read());
}


