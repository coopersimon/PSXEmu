#ifndef CONTROLLER_H
#define CONTROLLER_H

/*
 *    Controller.h
 *
 *    Declares the class used to interface with
 *    controller pads, and memory cards.
 *
 *    It connects the CPU/MemBus to the "hardware" pads
 *
 *    It's important for this file to stay OS/device-neutral
 */

// includes
#include <Memory.h>
#include <FIFO.h>


// represents a joypad.
// the classes that implement this are OS specific.
class joyPadInterface
{
public:
      virtual bool interruptTriggered() = 0;
      virtual byte transmitBytes(byte in) = 0;
};


// very similar (identical) to joypad.
class memCardInterface
{
public:
      virtual bool interruptTriggered() = 0;
      virtual byte transmitBytes(byte in) = 0;
};


// basically an interface between the system and the joypads/memory cards
class controllerIO : public memoryInterface
{
      // registers
      union {
            struct {
                  unsigned TX_ready_flag_1 : 1;
                  unsigned RX_fifo_not_empty : 1;
                  unsigned TX_ready_flag_2 : 1;
                  unsigned RX_parity_error : 1;
                  unsigned : 3;
                  unsigned ACK_input_level : 1;
                  unsigned : 1;
                  unsigned interrupt_request : 1;
                  unsigned : 1;
                  unsigned baudrate_timer : 21;
            } bit_fields;
            word data_field;
      } status_reg;

      union {
            struct {
                  unsigned baud_reload_factor : 2;
                  unsigned char_length : 2;
                  unsigned parity_enable : 1;
                  unsigned parity_length : 1;
                  unsigned : 2;
                  unsigned destroy_data : 1;
                  unsigned : 7;
                  unsigned TX_enable : 1;
                  unsigned JOY_n_output : 1;
                  unsigned RX_enable : 1;
                  unsigned : 1;
                  unsigned ack : 1;
                  unsigned : 1;
                  unsigned reset : 1;
                  unsigned : 1;
                  unsigned RX_interrupt_mode : 2;
                  unsigned TX_interrupt_enable : 1;
                  unsigned RX_interrupt_enable : 1;
                  unsigned ack_interrupt_enable : 1;
                  unsigned slot_number : 1;
            } bit_fields;
            word data_field;
      } control_reg;

      // controller pointers
      joyPadInterface* pad_1;
      joyPadInterface* pad_2;

      // memory cards
      memCardInterface* card_1;
      memCardInterface* card_2;

      // data received
      FIFOImpl<byte> RX_data;



public:
      // constructor
      

      // check for interrupt, so IRQ 7 can be set
      bool IRQ7Triggered();

      // memoryInterface functions
      word readWord(unsigned address) override;
      void writeWord(unsigned address, word in) override;
      
private:
      // internal functions

      // 0x1F801040 (mem address)
      void writeData(word data);
      word readData();

      // 0x1F801044
      word readStatusReg();

      // 0x1F801048
      void writeControlReg(word data);
      word readControlReg();
      
      // TODO: Baudrate stuff can be ignored?

};


#endif
