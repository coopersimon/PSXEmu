// test functions for cpu

#include "TestCPU.h"

using namespace test;

void CPUtestbench::storeIntoMemory(word instruction)
{
      device->memory->writeWordLittle(memory_location, instruction);
      memory_location += 4;
}


void CPUtestbench::constructInstruction(unsigned opcode, unsigned source, unsigned target, 
                                          unsigned dest, unsigned shamt, unsigned function)
{
      word instruction = 0;
      instruction |= (opcode & 0x3F) << 26;
      instruction |= (source & 0x1F) << 21;
      instruction |= (target & 0x1F) << 16;
      instruction |= (dest & 0x1F) << 11;
      instruction |= (shamt & 0x1F) << 6;
      instruction |= (function & 0x3F);
      storeIntoMemory(instruction);
      instr_count++;
}

void CPUtestbench::constructInstruction(unsigned opcode, unsigned source, unsigned target, 
                                          unsigned immediate)
{
      word instruction = 0;
      instruction |= (opcode & 0x3F) << 26;
      instruction |= (source & 0x1F) << 21;
      instruction |= (target & 0x1F) << 16;
      instruction |= (immediate & 0xFFFF);
      storeIntoMemory(instruction);
      instr_count++;
}

void CPUtestbench::constructInstruction(unsigned opcode, unsigned jump)
{
      word instruction = 0;
      instruction |= (opcode & 0x3F) << 26;
      instruction |= (jump & 0x3FFFFFF);
      storeIntoMemory(instruction);
      instr_count++;
}

void CPUtestbench::noOp()
{
      storeIntoMemory(0);
}

void CPUtestbench::setMemLocation(int mem_location_in)
{
      memory_location = mem_location_in;
}

void CPUtestbench::run()
{
      for (unsigned i = 0; i < instr_count; i++)
            device->stepCPU();
}


void CPUtestbench::storeReg(unsigned reg, word value)
{
      device->gp_reg[reg].write(value);
}

word CPUtestbench::getReg(unsigned reg)
{
      return device->gp_reg[reg].read();
}

word CPUtestbench::getHI()
{
      return device->HI.read();
}

word CPUtestbench::getLO()
{
      return device->LO.read();
}
