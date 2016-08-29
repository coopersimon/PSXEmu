#include "CPU.h"
//#include "Memory.h"
#include "PSException.h"

// TODO: should these be defines? maybe just inline functions
// shifts the offset up by 2 bits, and adds to the next PC, minus one instruction
#define branchPC(x);	s_word result = s_halfword(x) << 2; \
			PC_Next.write((PC_Next.read() + result) - 4);

// shifts the jump value by 2 bits, and sets the next PC, minus one instruction
// upper 4 bits are the same as next PC
#define jumpPC(x);	word result = PC_Next.read() & 0xF0000000; \
			result |= x << 2; \
			PC_Next.write(result - 4);

// TODO: clever c++11 pointer stuff
cpu::cpu(memoryInterface *memIn)
{
	// this pointer should be handled better - unique ptr perhaps
	memory = memIn;
	
	// initialse coprocessors
	// cop[0] = new scc;
	// cop[2] = new gte;
	
	// reg 0 is always 0
	gp_reg[0].setVars(true, false);

	// initialise PC

	// function pointers
	r_type = {
		&cpu::SLL,
		&cpu::RESERVED,
		&cpu::SRL,
		&cpu::SRA,
		&cpu::SLLV,
		&cpu::RESERVED,
		&cpu::SRLV,
		&cpu::SRAV,
		&cpu::JR,
		&cpu::JALR,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::SYSCALL,
		&cpu::BREAK,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::MFHI,
		&cpu::MTHI,
		&cpu::MFLO,
		&cpu::MTLO,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::MULT,
		&cpu::MULTU,
		&cpu::DIV,
		&cpu::DIVU,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::ADD,
		&cpu::ADDU,
		&cpu::SUB,
		&cpu::SUBU,
		&cpu::AND,
		&cpu::OR,
		&cpu::XOR,
		&cpu::NOR,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::SLT,
		&cpu::SLTU,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED,
		&cpu::RESERVED
	};

	// TODO: add in other function pointer(s)
}

cpu::~cpu()
{
	// delete coprocessors
	// delete cop[0];
	// delete cop[2];
}

void cpu::stepCPU()
{
	word instruction = PC.read();
	// decode instruction
	word opcode = instruction >> 26;
	// TODO: use function pointer to find instruction, then increment PCs
		// remember to catch exceptions
}

void cpu::RESERVED(unsigned null1, unsigned null2, unsigned null3)
{
	throw riException();
}

void cpu::RESERVED(unsigned null1, unsigned null2, halfword null3)
{
	throw riException();
}

/*** Instructions **************/

/********** Add/Subtract **************/
void cpu::ADD(unsigned source, unsigned target, unsigned dest)
{
	word result = gp_reg[source].read() + gp_reg[target].read();
	if (0x80000000 & (gp_reg[source].read() ^ result)) // if output sign and input sign are different
	{	if (0x80000000 & gp_reg[source].read() & gp_reg[target].read()) // if the operand signs are the same
			throw ovfException();
	}
	gp_reg[dest].write(result);
}
void cpu::ADDU(unsigned source, unsigned target, unsigned dest)
{
	word result = gp_reg[source].read() + gp_reg[target].read();
	gp_reg[dest].write(result);
}
void cpu::ADDI(unsigned target, unsigned source, s_halfword immediate)
{
	word result = gp_reg[source].read() + immediate;
	if (0x80000000 & (gp_reg[source].read() ^ result)) // if output sign and input sign are different
	{
		if (0x80000000 & gp_reg[source].read() & gp_reg[target].read()) // if the operand signs are the same
			throw ovfException();
	}
	gp_reg[target].write(result);
}
void cpu::ADDIU(unsigned target, unsigned source, s_halfword immediate)
{
	word result = gp_reg[source].read() + immediate;
	gp_reg[target].write(result);
}
void cpu::SUB(unsigned source, unsigned target, unsigned dest)
{
	word result = gp_reg[source].read() - gp_reg[target].read();
	if (0x80000000 & (gp_reg[source].read() ^ result))  // if output sign and input sign are different
	{
		if (0x80000000 & (gp_reg[source].read() ^ gp_reg[target].read())) // if the operand signs are different
			throw ovfException();
	}
	gp_reg[dest].write(result);
}
void cpu::SUBU(unsigned source, unsigned target, unsigned dest)
{
	word result = gp_reg[source].read() - gp_reg[target].read();
	gp_reg[dest].write(result);
}

/********** Multiply/Divide ***********/
void cpu::MULT(unsigned source, unsigned target, unsigned null)
{
	s_doubleword source64 = s_word(gp_reg[source].read()); // cast to signed 32 bit value, and then cast to 64 bit value
	s_doubleword target64 = s_word(gp_reg[target].read()); // consider including function which returns signed value? (need to cast to 64 bits either way)
	s_doubleword result = source64 * target64;
	LO = result & 0xFFFFFFFF;
	HI = result >> 32;
}
void cpu::MULTU(unsigned source, unsigned target, unsigned null)
{
	doubleword source64 = gp_reg[source].read(); // cast to 64 bit value
	doubleword target64 = gp_reg[target].read();
	doubleword result = source64 * target64;
	LO = result & 0xFFFFFFFF;
	HI = result >> 32;
}

void cpu::DIV(unsigned source, unsigned target, unsigned null)
{
	LO = s_word(gp_reg[source].read()) / s_word(gp_reg[target].read());
	HI = s_word(gp_reg[source].read()) % s_word(gp_reg[target].read());
}

void cpu::DIVU(unsigned source, unsigned target, unsigned null)
{
	LO = gp_reg[source].read() / gp_reg[target].read();
	HI = gp_reg[source].read() % gp_reg[target].read();
}

/********** Move from/to HI/LO ********/
void cpu::MFHI(unsigned null1, unsigned null2, unsigned dest)
{
	gp_reg[dest].write(HI.read());	// there are some access rules but I guess we can ignore these
}

void cpu::MTHI(unsigned null1, unsigned null2, unsigned dest)
{
	HI.write(gp_reg[dest].read());
}

void cpu::MFLO(unsigned null1, unsigned null2, unsigned dest)
{
	gp_reg[dest].write(LO.read());
}

void cpu::MTLO(unsigned null1, unsigned null2, unsigned dest)
{
	LO.write(gp_reg[dest].read());
}


/********** Bitwise Logic *************/
void cpu::AND(unsigned source, unsigned target, unsigned dest)
{
	word result = gp_reg[source].read() & gp_reg[target].read();
	gp_reg[dest].write(result);
}

void cpu::ANDI(unsigned target, unsigned source, s_halfword immediate)
{
	word imm32 = immediate & 0xFFFF;
	word result = gp_reg[source].read() & imm32;
	gp_reg[target].write(result);
}

void cpu::OR(unsigned source, unsigned target, unsigned dest)
{
	word result = gp_reg[source].read() | gp_reg[target].read();
	gp_reg[dest].write(result);
}

void cpu::ORI(unsigned target, unsigned source, s_halfword immediate)
{
	word imm32 = immediate & 0xFFFF;
	word result = gp_reg[source].read() | imm32;
	gp_reg[target].write(result);
}

void cpu::XOR(unsigned source, unsigned target, unsigned dest)
{
	word result = gp_reg[source].read() ^ gp_reg[target].read();
	gp_reg[dest].write(result);
}

void cpu::XORI(unsigned target, unsigned source, s_halfword immediate)
{
	word imm32 = immediate & 0xFFFF;
	word result = gp_reg[source].read() ^ imm32;
	gp_reg[target].write(result);
}

void cpu::NOR(unsigned source, unsigned target, unsigned dest)
{
	word result = gp_reg[source].read() | gp_reg[target].read();
	result ^= 0xFFFFFFFF;
	gp_reg[dest].write(result);
}


/********** Shifts ********************/
void cpu::SLL(unsigned shamt, unsigned target, unsigned dest)
{
	word result = gp_reg[target].read() << shamt;
	gp_reg[dest].write(result);
}

void cpu::SRL(unsigned shamt, unsigned target, unsigned dest)
{
	word result = gp_reg[target].read() >> shamt;
	word mask = (1 << (31 - shamt));
	mask |= mask - 1;
	result &= mask;
	gp_reg[dest].write(result);
}

void cpu::SRA(unsigned shamt, unsigned target, unsigned dest)
{
	word result = gp_reg[target].read() >> shamt;
	gp_reg[dest].write(result);
}

void cpu::SLLV(unsigned source, unsigned target, unsigned dest)
{
	unsigned shamt = source & 0x1F;
	SLL(dest, target, shamt);
}

void cpu::SRLV(unsigned source, unsigned target, unsigned dest)
{
	unsigned shamt = source & 0x1F;
	SRL(dest, target, shamt);
}

void cpu::SRAV(unsigned source, unsigned target, unsigned dest)
{
	unsigned shamt = source & 0x1F;
	SRA(dest, target, shamt);
}


/********** Load/Store ****************/
void cpu::LB(unsigned target, unsigned source, s_halfword offset)
{
	LBU(target, source, offset);
	if (target >> 7)
		target |= 0xFFFFFF00;	// sign extend
}

void cpu::LBU(unsigned target, unsigned source, s_halfword offset)
{
	word address = gp_reg[source].read() + offset;
	word result = memory->readByte(address) & 0xFF;
	gp_reg[target].write(result);
}

void cpu::LH(unsigned target, unsigned source, s_halfword offset)
{
	LHU(target, source, offset);
	if (target >> 15)
		target |= 0xFFFF0000;	// sign extend
}

void cpu::LHU(unsigned target, unsigned source, s_halfword offset)
{
	word address = gp_reg[source].read() + offset;
	word result;
	if (checkEndianness())
		result = memory->readHalfwordLittle(address) & 0xFFFF;
	else
		result = memory->readHalfwordBig(address) & 0xFFFF;
	gp_reg[target].write(result);
}

void cpu::LW(unsigned target, unsigned source, s_halfword offset)
{
	word address = gp_reg[source].read() + offset;
	word result;
	if (checkEndianness())
		result = memory->readWordLittle(address);
	else
		result = memory->readWordBig(address);
	gp_reg[target].write(result);
}

// following 2 instructions are in big-endian mode. I believe PSX is little endian by default
void cpu::LWL(unsigned target, unsigned source, s_halfword offset)
{
	word address = gp_reg[source].read() + offset;
	word byte_number = address % 4;
	address -= byte_number;	// align address

	word load_data = memory->readWordBig(address);
	
	// shift word to make the selected byte the most significant, and fill in to the right
	load_data <<= byte_number * 8;

	// mask target so it matches the empty bytes left by the load data
	word result = gp_reg[target].read() & (0xFFFFFFFF >> ((4 - byte_number) * 8));

	gp_reg[target].write(result | load_data);
}

void cpu::LWR(unsigned target, unsigned source, s_halfword offset)
{
	word address = gp_reg[source].read() + offset;
	word byte_number = address % 4;
	address -= byte_number;	// align address

	word load_data = memory->readWordBig(address);

	// shift word to make the selected byte the least significant, and fill in to the left
	load_data >>= (3 - byte_number) * 8;

	// mask target so it matches the empty bytes left by the load data
	word result = gp_reg[target].read() & (0xFFFFFFFF << ((1 + byte_number) * 8));

	gp_reg[target].write(result | load_data);
}


void cpu::SB(unsigned target, unsigned source, s_halfword offset)
{
	word address = gp_reg[source].read() + offset;
	//byte data_in = gp_reg[target].read() & 0xFF;
	memory->writeByte(address, byte(gp_reg[target].read()));
}

void cpu::SH(unsigned target, unsigned source, s_halfword offset)
{
	word address = gp_reg[source].read() + offset;
	if (checkEndianness())
		memory->writeHalfwordLittle(address, halfword(gp_reg[target].read()));
	else
		memory->writeHalfwordBig(address, halfword(gp_reg[target].read()));
}

void cpu::SW(unsigned target, unsigned source, s_halfword offset)
{
	word address = gp_reg[source].read() + offset;
	if (checkEndianness())
		memory->writeWordLittle(address, gp_reg[target].read());
	else
		memory->writeWordBig(address, gp_reg[target].read());
}

// next 2 instructions: little endian!
void cpu::SWL(unsigned target, unsigned source, s_halfword offset)
{

}

void cpu::SWR(unsigned target, unsigned source, s_halfword offset)
{
}


void cpu::LUI(unsigned target, unsigned, s_halfword offset)
{
	gp_reg[target].write(offset << 16);
}


/********** Branch ********************/
void cpu::BEQ(unsigned source, unsigned target, s_halfword offset)
{
	if (gp_reg[source].read() != gp_reg[target].read())
		return;
	branchPC(offset);
}

void cpu::BNE(unsigned source, unsigned target, s_halfword offset)
{
	if (gp_reg[source].read() == gp_reg[target].read())
		return;
	branchPC(offset);
}

void cpu::BGEZ(unsigned null, unsigned source, s_halfword offset)
{
	if (gp_reg[source].read() < 0)
		return;
	branchPC(offset);
}

void cpu::BGEZAL(unsigned null, unsigned source, s_halfword offset)
{
	if (gp_reg[source].read() < 0)
		return;
	gp_reg[31].write(PC.read() + 8);
	branchPC(offset);
}

void cpu::BLTZ(unsigned null, unsigned source, s_halfword offset)
{
	if (gp_reg[source].read() >= 0)
		return;
	branchPC(offset);
}

void cpu::BLTZAL(unsigned null, unsigned source, s_halfword offset)
{
	if (gp_reg[source].read() >= 0)
		return;
	gp_reg[31].write(PC.read() + 8);
	branchPC(offset);
}

void cpu::BGTZ(unsigned null, unsigned source, s_halfword offset)
{
	if (gp_reg[source].read() <= 0)
		return
	branchPC(offset);
}

void cpu::BLEZ(unsigned null, unsigned source, s_halfword offset)
{
	if (gp_reg[source].read() > 0)
		return;
	branchPC(offset);
}


/********** Jump **********************/
void cpu::J(word jump)
{
	jumpPC(jump);
}

void cpu::JAL(word jump)
{
	gp_reg[31].write(PC.read() + 8);
	jumpPC(jump);
}

void cpu::JR(unsigned source, unsigned null1, unsigned null2)
{
	jumpPC(gp_reg[source].read());
}

void cpu::JALR(unsigned source, unsigned null1, unsigned null2)
{
	gp_reg[31].write(PC.read() + 8);
	jumpPC(gp_reg[source].read());
}


/********** Set ***********************/
void cpu::SLT(unsigned source, unsigned target, unsigned dest)
{
	word result = (s_word(gp_reg[source].read()) < gp_reg[target].read());
	gp_reg[dest].write(result);
}

void cpu::SLTU(unsigned source, unsigned target, unsigned dest)
{
	word result = (gp_reg[source].read() < gp_reg[target].read());
	gp_reg[dest].write(result);
}

void cpu::SLTI(unsigned target, unsigned source, s_halfword immediate)
{
	s_word imm32 = immediate;
	word result = (s_word(gp_reg[source].read()) < imm32);
	gp_reg[target].write(result);
}

void cpu::SLTIU(unsigned target, unsigned source, s_halfword immediate)
{
	s_word imm32 = immediate;
	word result = (gp_reg[source].read() < imm32);
	gp_reg[target].write(result);
}


/********** Special *******************/
void cpu::SYSCALL()
{
	throw sysException();
}

void cpu::BREAK()
{
	throw bpException();
}


/********** Coprocessor ***************/
/*void cpu::LWCz(unsigned target, unsigned source, halfword offset, unsigned coproc)
{
	word address = gp_reg[source].read() + offset;
	if (checkEndianness())
		word data = memory->readWordLittle(address);
	else
		word data = memory->readWordBig(address);
	cop[coproc]->writeDataReg(result, target);
}

void cpu::SWCz(unsigned target, unsigned source, halfword offset, unsigned coproc)
{
	word address = gp_reg[source].read() + offset;
	if (checkEndianness())
		memory->writeWordLittle(address, cop[coproc]->readDataReg(target));
	else
		memory->writeWordBig(address, cop[coproc]->readDataReg(target));
}

void cpu::MTCz(unsigned target, unsigned dest, unsigned coproc)
{
	// write coproc reg "dest"
	cop[coproc]->storeDataReg(reg[target].read(), dest);
}

void cpu::MFCz(unsigned target, unsigned dest, unsigned coproc)
{
	// write proc reg "dest"
	word data = cop[coproc]->loadDataReg(target);
	reg[dest].write(data);
}

void cpu::CTCz(unsigned target, unsigned dest, unsigned coproc)
{
	// write control coproc reg "dest"
	cop[coproc]->storeControlReg(reg[target].read(), dest);
}

void cpu::CFCz(unsigned target, unsigned dest, unsigned coproc)
{
	// write proc reg "dest"
	word data = cop[coproc]->loadControlReg(target);
	reg[dest].write(data);
}

void cpu::COPz(unsigned coproc, unsigned instruction)
{
	cop[coproc]->executeInstruction(instruction);
}*/
