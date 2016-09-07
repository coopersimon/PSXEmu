#ifndef CPU_H
#define CPU_H

/*
 *	CPU.h
 *
 *	Class used to describe the R3051 MIPS processor.
 *
 */

// includes
#include "SCC.h"
#include "Memory.h"
#include "Register.h"
#include <vector>
#include <functional>

// TODO: namespace!

// class used to represent the R3051 CPU
class cpu
{
	/*** Registers ***/
	MIPSReg gp_reg[32];
	// program counter
	MIPSReg PC;
	// next value of program counter
	MIPSReg PC_next;
	// HI and LO special registers
	MIPSReg HI;
	MIPSReg LO;

	/*** Coprocessors ***/
	scc SCC;
	// gte GTE;
	// generic pointers for load/store
	coprocessor * cop[4];

	/*** Memory bus ***/
	memoryInterface *memory;

	// cache: 4kB instruction cache, 1kB data cache
	//		I have no idea how much of this we need to emulate

	/*** Status ***/
	bool little_endian;	// true if cpu is in little endian mode
	unsigned instruction;

      /*** Exception related variables ***/
      bool branch_delay;      // true if instruction currently executing is in the branch delay slot

	/*** Function Pointers ***/
	std::vector<std::function<void(cpu*)>> r_type;
	std::vector<std::function<void(cpu*)>> i_type;
	std::vector<std::function<void(cpu*)>> bcond_type;


public:
	// construct processor
	cpu(memoryInterface *memIn);

	//~cpu();

	// steps the CPU by one tick.
	// pretty much everything is handled internally: instruction decoding, exception handling
	void stepCPU();

private:
	/********** INLINE FUNCTIONS **********/

	inline bool checkEndianness() { return little_endian; /*&& status bit*/ }
	
	// done at the end of every CPU step
	inline void incrementPCNext() { PC_next.write(PC_next.read() + 4); }

	// when branching
	inline void branchRoutine(s_halfword in)
	{
		s_word result = in << 2;
		PC_next.write(PC_next.read() + in - 4);
            //branch_delay = true;
	}

      // set every cycle, used for exception handling
      inline void setEPC()
      {
            if (PC_next.read() - PC.read() != 4)      // if PC is currently in the branch delay slot
            {
                  branch_delay = true;
            }
            else //if (!branch_delay)      // if PC is not currently in a branch delay slot
            {
                  SCC.data_reg[scc::EPC].writeBits(PC.read());
                  branch_delay = false;
            }
      }

	// read/write instruction registers
	inline unsigned source() const { return gp_reg[(instruction >> 21) & 0x1F].read(); }
	inline void source(word in) { gp_reg[(instruction >> 21) & 0x1F].write(in); }
	inline unsigned target() const { return gp_reg[(instruction >> 16) & 0x1F].read(); }
	inline void target(word in) { gp_reg[(instruction >> 16) & 0x1F].write(in); }
	inline unsigned dest() const { return gp_reg[(instruction >> 11) & 0x1F].read(); }
	inline void dest(word in) { gp_reg[(instruction >> 11) & 0x1F].write(in); }

	// instruction decoding
	inline unsigned target_val() const { return (instruction >> 16) & 0x1F; }
      inline unsigned dest_val() const { return (instruction >> 11) & 0x1F; }
	inline unsigned shamt() const { return (instruction >> 6) & 0x1F; }
	inline halfword imm() const { return instruction & 0xFFFF; }
	inline unsigned jump() const { return instruction & 0x3FFFFFF; }
	inline unsigned coproc() const { return (instruction >> 26) & 0x3; }

	/********** INSTRUCTIONS **************/
	void RESERVED();

	// f = function, o = opcode, t = target reg value, s = source reg value

	/********** Add/Subtract **************/
	void ADD(); // f 0x20
	void ADDU(); // f 0x21
	void ADDI(); // o 0x08
	void ADDIU(); // o 0x09
	void SUB(); // f 0x22
	void SUBU(); // f 0x23

	/********** Multiply/Divide ***********/
	void MULT(); // f 0x18
	void MULTU(); // f 0x19
	void DIV(); // f 0x1A
	void DIVU(); // f 0x1B

	/********** Move from/to HI/LO ********/
	void MFHI(); // f 0x10
	void MTHI(); // f 0x11
	void MFLO(); // f 0x12
	void MTLO(); // f 0x13

	/********** Bitwise Logic *************/
	void AND(); // f 0x24
	void ANDI(); // o 0x0C
	void OR(); // f 0x25
	void ORI(); // o 0x0D
	void XOR(); // f 0x26
	void XORI(); // o 0x0E
	void NOR(); // f 0x27

	/********** Shifts ********************/
	void SLL(); // f 0x00
	void SRL(); // f 0x02
	void SRA(); // f 0x03
	void SLLV(); // f 0x04
	void SRLV(); // f 0x06
	void SRAV(); // f 0x07

	/********** Load/Store ****************/
	void LB(); // o 0x20
	void LBU(); // o 0x24
	void LH(); // o 0x21
	void LHU(); // o 0x25
	void LW(); // o 0x23
	void LWL(); // o 0x22
	void LWR(); // o 0x26

	void SB(); // o 0x28
	void SH(); // o 0x29
	void SW(); // o 0x2B
	void SWL(); // o 0x2A
	void SWR(); // o 0x2E

	void LUI(); // o 0x0F

	/********** Branch ********************/
	void BEQ(); // o 0x04
	void BNE(); // o 0x05
	void BCOND(); // o 0x01
	void BGEZ(); // o 0x01, t 0x01
	void BGEZAL(); // o 0x01, t 0x11
	void BLTZ(); // o 0x01, t 0x00
	void BLTZAL(); // o 0x01, t 0x10
	void BGTZ(); // o 0x07
	void BLEZ(); // o 0x06

	/********** Jump **********************/
	void J(); // o 0x02
	void JAL(); // o 0x03
	void JR(); // f 0x08
	void JALR(); // f 0x09

	/********** Set ***********************/
	void SLT(); // f 0x2A
	void SLTU(); // f 0x2B
	void SLTI(); // o 0x0A
	void SLTIU(); // o 0x0B

	/********** Special *******************/
	void SYSCALL(); // f 0x0C
	void BREAK(); // f 0x0D

	/********** Coprocessor ***************/
	void LWCz(); // o 0x30-3
	void SWCz(); // o 0x38-B
	void MTCz(); // o 0x10-3, s 0x04
	void MFCz(); // o 0x10-3, s 0x00
	void CTCz(); // o 0x10-3, s 0x06
	void CFCz(); // o 0x10-3, s 0x02
	void COPz(); // o 0x10-3 - called on the coprocessor directly

};

#endif
