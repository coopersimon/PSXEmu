// used for testing CPU

#include <CPU.h>
#include <Memory.h>
#include <vector>

namespace test
{

class CPUtestbench
{
private:
      cpu *device;
      unsigned instr_count;
      unsigned memory_location;

      void storeIntoMemory(word instruction);

public:
      CPUtestbench(cpu *test_device) : device(test_device), instr_count(0), memory_location(0) {}

      // construct instruction and load into memory
      void constructInstruction(unsigned opcode, unsigned source, unsigned target, unsigned dest, unsigned shamt, unsigned function); // rtype
      void constructInstruction(unsigned opcode, unsigned source, unsigned target, unsigned immediate); // itype
      void constructInstruction(unsigned opcode, unsigned jump); // jtype
      // load no op into memory
      void noOp();

      // run instructions in memory
      void run();

      // store into a GP register
      void storeReg(unsigned reg, word value);
      // retrieve the value of a GP register
      word getReg(unsigned reg);
      // self explanatory
      word getHI();
      word getLO();
};

enum
{
      SLL = 0,
      SRL = 2,
      SRA = 3,
      SLLV = 4,
      SRLV = 6,
      SRAV = 7,
      JR = 8,
      JALR = 9,
      SYSCALL = 12,
      BREAK = 13,
      MFHI = 16,
      MTHI = 17,
      MFLO = 18,
      MTLO = 19,
      MULT = 24,
      MULTU = 25,
      DIV = 26,
      DIVU = 27,
      ADD = 32,
      ADDU = 33,
      SUB = 34,
      SUBU = 35,
      AND = 36,
      OR = 37,
      XOR = 38,
      NOR = 39,
      SLT = 42,
      SLTU = 43
};

enum
{
      BCOND = 1,
      J = 2,
      JAL = 3,
      BEQ = 4,
      BNE = 5,
      BLEZ = 6,
      BGTZ = 7,
      ADDI = 8,
      ADDIU = 9,
      SLTI = 10,
      SLTIU = 11,
      ANDI = 12,
      ORI = 13,
      XORI = 14,
      LUI = 15,
      COP0 = 16,
      COP1 = 17,
      COP2 = 18,
      COP3 = 19,
      LB = 32,
      LH = 33,
      LWL = 34,
      LW = 35,
      LBU = 36,
      LHU = 37,
      LWR = 38,
      SB = 40,
      SH = 41,
      SWL = 42,
      SW = 43,
      SWR = 46,
      LWC0 = 48,
      LWC1 = 49,
      LWC2 = 50,
      LWC3 = 51,
      SWC0 = 56,
      SWC1 = 57,
      SWC2 = 58,
      SWC3 = 59
};

}; // namespace test
