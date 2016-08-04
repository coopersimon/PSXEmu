#ifndef CPU_H
#define CPU_H

/*
 *	CPU.h
 *
 *	Class used to describe the R3051 MIPS processor.
 *
 */

// includes
#include "Processor.h"
#include "Coprocessor.h"
#include "Memory.h"

// class used to represent the R3051 CPU
class cpu {
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
	scc cop0;		// exceptions & page table management
	coprocessor cop1;	// empty coprocessor slot
	gte cop2;		// graphics maths
	coprocessor cop3;	// empty coprocessor slot

	/*** Memory bus ***/
	memBus memory;

	// cache: 4kB instruction cache, 1kB data cache
	//		I have no idea how much of this we need to emulate

	/*** Status ***/
	bool little_endian;	// true if cpu is in little endian mode

public:
	// construct processor with 32 general purpose registers
	cpu() : little_endian(true) { reg[0].set_vars(true, false); };

	~cpu();

	// this method returns the current PC value, as well as fetching the next value from memory.
	// the instruction can then be externally decoded and the correct instruction can be called.
	regData32 fetchPC(RAMImpl RAM);

private:

	bool checkEndianness() { return little_endian; /*&& status bit*/ }
	// done at the end of every cpu tick.
	void incrementPCNext() { PC_Next.write(PC_Next.read() + 4); }

	// instructions: might be made private
	// f = function, o = opcode, t = target reg value, s = source reg value

	/********** Add/Subtract **************/
	void ADD(unsigned dest, unsigned source, unsigned target); // f 0x20
	void ADDU(unsigned dest, unsigned source, unsigned target); // f 0x21
	void ADDI(unsigned target, unsigned source, halfword immediate); // o 0x08
	void ADDIU(unsigned target, unsigned source, halfword immediate); // o 0x09
	void SUB(unsigned dest, unsigned source, unsigned target); // f 0x22
	void SUBU(unsigned dest, unsigned source, unsigned target); // f 0x23

	/********** Multiply/Divide ***********/
	void MULT(unsigned source, unsigned target); // f 0x18
	void MULTU(unsigned source, unsigned target); // f 0x19
	void DIV(unsigned source, unsigned target); // f 0x1A
	void DIVU(unsigned source, unsigned target); // f 0x1B

	/********** Move from/to HI/LO ********/
	void MFHI(unsigned dest); // f 0x10
	void MTHI(unsigned dest); // f 0x11
	void MFLO(unsigned dest); // f 0x12
	void MTLO(unsigned dest); // f 0x13

	/********** Bitwise Logic *************/
	void AND(unsigned dest, unsigned source, unsigned target); // f 0x24
	void ANDI(unsigned target, unsigned source, halfword immediate); // o 0x0C
	void OR(unsigned dest, unsigned source, unsigned target); // f 0x25
	void ORI(unsigned target, unsigned source, halfword immediate); // o 0x0D
	void XOR(unsigned dest, unsigned source, unsigned target); // f 0x26
	void XORI(unsigned target, unsigned source, halfword immediate); // o 0x0E
	void NOR(unsigned dest, unsigned source, unsigned target); // f 0x27

	/********** Shifts ********************/
	void SLL(unsigned dest, unsigned target, unsigned shamt); // f 0x00
	void SRL(unsigned dest, unsigned target, unsigned shamt); // f 0x02
	void SRA(unsigned dest, unsigned target, unsigned shamt); // f 0x03
	void SLLV(unsigned dest, unsigned target, unsigned source); // f 0x04
	void SRLV(unsigned dest, unsigned target, unsigned source); // f 0x06
	void SRAV(unsigned dest, unsigned target, unsigned source); // f 0x07

	/********** Load/Store ****************/
	void LB(unsigned target, unsigned source, halfword offset); // o 0x20
	void LBU(unsigned target, unsigned source, halfword offset); // o 0x24
	void LH(unsigned target, unsigned source, halfword offset); // o 0x21
	void LHU(unsigned target, unsigned source, halfword offset); // o 0x25
	void LW(unsigned target, unsigned source, halfword offset); // o 0x23
	void LWL(unsigned target, unsigned source, halfword offset); // o 0x22
	void LWR(unsigned target, unsigned source, halfword offset); // o 0x26

	void SB(unsigned target, unsigned source, halfword offset); // o 0x28
	void SH(unsigned target, unsigned source, halfword offset); // o 0x29
	void SW(unsigned target, unsigned source, halfword offset); // o 0x2B
	void SWL(unsigned target, unsigned source, halfword offset); // o 0x2A
	void SWR(unsigned target, unsigned source, halfword offset); // o 0x2E

	void LUI(unsigned target, halfword offset); // o 0x0F

	/********** Branch ********************/
	void BEQ(unsigned source, unsigned target, halfword offset); // o 0x04
	void BNE(unsigned source, unsigned target, halfword offset); // o 0x05
	void BGEZ(unsigned source, halfword offset); // o 0x01, t 0x01
	void BGEZAL(unsigned source, halfword offset); // o 0x01, t 0x11
	void BLTZ(unsigned source, halfword offset); // o 0x01, t 0x00
	void BLTZAL(unsigned source, halfword offset); // o 0x01, t 0x10
	void BGTZ(unsigned source, halfword offset); // o 0x07
	void BLEZ(unsigned source, halfword offset); // o 0x06

	/********** Jump **********************/
	void J(word jump); // o 0x02
	void JAL(word jump); // o 0x03
	void JR(unsigned source); // f 0x08
	void JALR(unsigned source); // f 0x09

	/********** Set ***********************/
	void SLT(unsigned dest, unsigned source, unsigned target); // f 0x2A
	void SLTU(unsigned dest, unsigned source, unsigned target); // f 0x2B
	void SLTI(unsigned target, unsigned source, halfword immediate); // o 0x0A
	void SLTIU(unsigned target, unsigned source, halfword immediate); // o 0x0B

	/********** Special *******************/
	void SYSCALL(); // f 0x0C
	void BREAK(); // f 0x0D

	/********** Coprocessor ***************/
	void LWCz(unsigned target, unsigned source, halfword offset, unsigned coproc); // o 0x30-3
	void SWCz(unsigned target, unsigned source, halfword offset, unsigned coproc); // o 0x38-B
	void MTCz(unsigned target, unsigned dest, unsigned coproc); // o 0x10-3, s 0x04
	void MFCz(unsigned target, unsigned dest, unsigned coproc); // o 0x10-3, s 0x00
	void CTCz(unsigned target, unsigned dest, unsigned coproc); // o 0x10-3, s 0x06
	void CFCz(unsigned target, unsigned dest, unsigned coproc); // o 0x10-3, s 0x02
	//void COPz(unsigned instruction); // o 0x10-3 - called on the coprocessor directly

};

#endif
