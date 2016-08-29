#ifndef CPU_H
#define CPU_H

/*
 *	CPU.h
 *
 *	Class used to describe the R3051 MIPS processor.
 *
 */

// includes
#include "Coprocessor.h"
#include "Memory.h"
#include "Register.h"
#include <vector>
#include <functional>

// TODO: namespace!

typedef reg32 MIPSReg;

// class used to represent the R3051 CPU
class cpu
{
	/*** Registers ***/
	MIPSReg gp_reg[32];
	// program counter
	MIPSReg PC;
	// next value of program counter
	MIPSReg PC_Next;
	// HI and LO special registers
	MIPSReg HI;
	MIPSReg LO;

	/*** Coprocessors ***/
	coprocessor * cop[4];

	/*** Memory bus ***/
	memoryInterface *memory;

	// cache: 4kB instruction cache, 1kB data cache
	//		I have no idea how much of this we need to emulate

	/*** Status ***/
	bool little_endian;	// true if cpu is in little endian mode

	/*** Function Pointers ***/
	std::vector<std::function<void(cpu*, unsigned, unsigned, unsigned)>> r_type;
	//std::vector<std::function<void(cpu*, unsigned, unsigned, halfword)>> i_type;
	//std::vector<std::function<void(word)>> j_type;


public:
	// construct processor
	cpu(memoryInterface *memIn);

	~cpu();

	// steps the CPU by one tick.
	// pretty much everything is handled internally: instruction decoding, exception handling
	void stepCPU();

private:

	inline bool checkEndianness() { return little_endian; /*&& status bit*/ }
	// done at the end of every cpu tick.
	void incrementPCNext() { PC_Next.write(PC_Next.read() + 4); }

	// instructions
	void RESERVED(unsigned, unsigned, unsigned);
	void RESERVED(unsigned, unsigned, halfword);

	// f = function, o = opcode, t = target reg value, s = source reg value

	/********** Add/Subtract **************/
	void ADD(unsigned source, unsigned target, unsigned dest); // f 0x20
	void ADDU(unsigned source, unsigned target, unsigned dest); // f 0x21
	void ADDI(unsigned target, unsigned source, s_halfword immediate); // o 0x08
	void ADDIU(unsigned target, unsigned source, s_halfword immediate); // o 0x09
	void SUB(unsigned source, unsigned target, unsigned dest); // f 0x22
	void SUBU(unsigned source, unsigned target, unsigned dest); // f 0x23

	/********** Multiply/Divide ***********/
	void MULT(unsigned source, unsigned target, unsigned); // f 0x18
	void MULTU(unsigned source, unsigned target, unsigned); // f 0x19
	void DIV(unsigned source, unsigned target, unsigned); // f 0x1A
	void DIVU(unsigned source, unsigned target, unsigned); // f 0x1B

	/********** Move from/to HI/LO ********/
	void MFHI(unsigned, unsigned, unsigned dest); // f 0x10
	void MTHI(unsigned, unsigned, unsigned dest); // f 0x11
	void MFLO(unsigned, unsigned, unsigned dest); // f 0x12
	void MTLO(unsigned, unsigned, unsigned dest); // f 0x13

	/********** Bitwise Logic *************/
	void AND(unsigned source, unsigned target, unsigned dest); // f 0x24
	void ANDI(unsigned target, unsigned source, s_halfword immediate); // o 0x0C
	void OR(unsigned source, unsigned target, unsigned dest); // f 0x25
	void ORI(unsigned target, unsigned source, s_halfword immediate); // o 0x0D
	void XOR(unsigned source, unsigned target, unsigned dest); // f 0x26
	void XORI(unsigned target, unsigned source, s_halfword immediate); // o 0x0E
	void NOR(unsigned source, unsigned target, unsigned dest); // f 0x27

	/********** Shifts ********************/
	void SLL(unsigned shamt, unsigned target, unsigned dest); // f 0x00
	void SRL(unsigned shamt, unsigned target, unsigned dest); // f 0x02
	void SRA(unsigned shamt, unsigned target, unsigned dest); // f 0x03
	void SLLV(unsigned source, unsigned target, unsigned dest); // f 0x04
	void SRLV(unsigned source, unsigned target, unsigned dest); // f 0x06
	void SRAV(unsigned source, unsigned target, unsigned dest); // f 0x07

	/********** Load/Store ****************/
	void LB(unsigned target, unsigned source, s_halfword offset); // o 0x20
	void LBU(unsigned target, unsigned source, s_halfword offset); // o 0x24
	void LH(unsigned target, unsigned source, s_halfword offset); // o 0x21
	void LHU(unsigned target, unsigned source, s_halfword offset); // o 0x25
	void LW(unsigned target, unsigned source, s_halfword offset); // o 0x23
	void LWL(unsigned target, unsigned source, s_halfword offset); // o 0x22
	void LWR(unsigned target, unsigned source, s_halfword offset); // o 0x26

	void SB(unsigned target, unsigned source, s_halfword offset); // o 0x28
	void SH(unsigned target, unsigned source, s_halfword offset); // o 0x29
	void SW(unsigned target, unsigned source, s_halfword offset); // o 0x2B
	void SWL(unsigned target, unsigned source, s_halfword offset); // o 0x2A
	void SWR(unsigned target, unsigned source, s_halfword offset); // o 0x2E

	void LUI(unsigned target, unsigned, s_halfword offset); // o 0x0F

	/********** Branch ********************/
	void BEQ(unsigned target, unsigned source, s_halfword offset); // o 0x04
	void BNE(unsigned target, unsigned source, s_halfword offset); // o 0x05
	void BGEZ(unsigned, unsigned source, s_halfword offset); // o 0x01, t 0x01
	void BGEZAL(unsigned, unsigned source, s_halfword offset); // o 0x01, t 0x11
	void BLTZ(unsigned, unsigned source, s_halfword offset); // o 0x01, t 0x00
	void BLTZAL(unsigned, unsigned source, s_halfword offset); // o 0x01, t 0x10
	void BGTZ(unsigned, unsigned source, s_halfword offset); // o 0x07
	void BLEZ(unsigned, unsigned source, s_halfword offset); // o 0x06

	/********** Jump **********************/
	void J(word jump); // o 0x02
	void JAL(word jump); // o 0x03
	void JR(unsigned source, unsigned, unsigned); // f 0x08
	void JALR(unsigned source, unsigned, unsigned); // f 0x09

	/********** Set ***********************/
	void SLT(unsigned source, unsigned target, unsigned dest); // f 0x2A
	void SLTU(unsigned source, unsigned target, unsigned dest); // f 0x2B
	void SLTI(unsigned target, unsigned source, s_halfword immediate); // o 0x0A
	void SLTIU(unsigned target, unsigned source, s_halfword immediate); // o 0x0B

	/********** Special *******************/
	void SYSCALL(); // f 0x0C
	void BREAK(); // f 0x0D

	/********** Coprocessor ***************/
	/*void LWCz(unsigned target, unsigned source, halfword offset, unsigned coproc); // o 0x30-3
	void SWCz(unsigned target, unsigned source, halfword offset, unsigned coproc); // o 0x38-B
	void MTCz(unsigned target, unsigned dest, unsigned coproc); // o 0x10-3, s 0x04
	void MFCz(unsigned target, unsigned dest, unsigned coproc); // o 0x10-3, s 0x00
	void CTCz(unsigned target, unsigned dest, unsigned coproc); // o 0x10-3, s 0x06
	void CFCz(unsigned target, unsigned dest, unsigned coproc); // o 0x10-3, s 0x02*/
	//void COPz(unsigned instruction); // o 0x10-3 - called on the coprocessor directly

};

#endif
