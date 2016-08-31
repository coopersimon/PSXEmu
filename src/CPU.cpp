#include "CPU.h"
//#include "Memory.h"
#include "PSException.h"
#include <iostream>

// TODO: should these be defines? maybe just inline functions
// shifts the imm() up by 2 bits, and adds to the next PC, minus one instruction
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
	gp_reg[0].setMask(0x00000000);

	// initialise PC
	PC.write(0);
	PC_Next.write(4);

	// function pointers
	r_type = {
		&cpu::SLL, &cpu::RESERVED, &cpu::SRL, &cpu::SRA,
		&cpu::SLLV, &cpu::RESERVED, &cpu::SRLV, &cpu::SRAV,
		&cpu::JR, &cpu::JALR, &cpu::RESERVED, &cpu::RESERVED,
		&cpu::SYSCALL, &cpu::BREAK, &cpu::RESERVED, &cpu::RESERVED,
		&cpu::MFHI, &cpu::MTHI, &cpu::MFLO, &cpu::MTLO,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED,
		&cpu::MULT, &cpu::MULTU, &cpu::DIV, &cpu::DIVU,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED,
		&cpu::ADD, &cpu::ADDU, &cpu::SUB, &cpu::SUBU,
		&cpu::AND, &cpu::OR, &cpu::XOR, &cpu::NOR,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::SLT, &cpu::SLTU,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED
	};

	i_type = {
		&cpu::RESERVED, &cpu::BCOND, &cpu::J, &cpu::JAL,
		&cpu::BEQ, &cpu::BNE, &cpu::BLEZ, &cpu::BGTZ,
		&cpu::ADDI, &cpu::ADDIU, &cpu::SLTI, &cpu::SLTIU,
		&cpu::ANDI, &cpu::ORI, &cpu::XORI, &cpu::LUI,
		//&cpu::COPz, &cpu::COPz, &cpu::COPz, &cpu::COPz,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED,	// TODO: remove when COPz is implemented
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, 
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, 
		&cpu::LB, &cpu::LH, &cpu::LWL, &cpu::LW,
		&cpu::LBU, &cpu::LHU, &cpu::LWR, &cpu::RESERVED,
		&cpu::SB, &cpu::SH, &cpu::SWL, &cpu::SW,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::SWR, &cpu::RESERVED,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED,  
		&cpu::LWCz, &cpu::LWCz, &cpu::LWCz, &cpu::LWCz,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, 
		&cpu::SWCz, &cpu::SWCz, &cpu::SWCz, &cpu::SWCz,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED 
	};

	bcond_type = {
		&cpu::BLTZ, &cpu::BGEZ, &cpu::RESERVED, &cpu::RESERVED,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED,
		&cpu::BLTZAL, &cpu::BGEZAL, &cpu::RESERVED, &cpu::RESERVED,
		&cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED, &cpu::RESERVED      
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
	instruction = memory->readWordLittle(PC.read());
	// decode instruction
	word opcode = (instruction >> 26) & 0x3F;
	
	try {
		if (opcode == 0)
		{
			// execute r-type instruction
			word function = instruction & 0x3F;
			r_type[function](this);
		}
		else
		{
			// execute i/j-type instruction
			i_type[opcode](this);
		}
	}
	catch (psException &e) {
		std::cout << e.execode() << std::endl;
	}

	PC.write(PC_Next.read());
	PC_Next.write(PC_Next.read() + 4);
}

void cpu::RESERVED()
{
	throw riException();
}


/*** Instructions **************/

/********** Add/Subtract **************/
void cpu::ADD()
{
	word result = gp_reg[source()].read() + gp_reg[target()].read();
	if (0x80000000 & (gp_reg[source()].read() ^ result)) // if output sign and input sign are different
	{	if (0x80000000 & gp_reg[source()].read() & gp_reg[target()].read()) // if the operand signs are the same
			throw ovfException();
	}
	gp_reg[dest()].write(result);
}
void cpu::ADDU()
{
	word result = gp_reg[source()].read() + gp_reg[target()].read();
	gp_reg[dest()].write(result);
}
void cpu::ADDI()
{
	word result = gp_reg[source()].read() + imm();
	if (0x80000000 & (gp_reg[source()].read() ^ result)) // if output sign and input sign are different
	{
		if (0x80000000 & gp_reg[source()].read() & gp_reg[target()].read()) // if the operand signs are the same
			throw ovfException();
	}
	gp_reg[target()].write(result);
}
void cpu::ADDIU()
{
	word result = gp_reg[source()].read() + imm();
	gp_reg[target()].write(result);
}
void cpu::SUB()
{
	word result = gp_reg[source()].read() - gp_reg[target()].read();
	if (0x80000000 & (gp_reg[source()].read() ^ result))  // if output sign and input sign are different
	{
		if (0x80000000 & (gp_reg[source()].read() ^ gp_reg[target()].read())) // if the operand signs are different
			throw ovfException();
	}
	gp_reg[dest()].write(result);
}
void cpu::SUBU()
{
	word result = gp_reg[source()].read() - gp_reg[target()].read();
	gp_reg[dest()].write(result);
}

/********** Multiply/Divide ***********/
void cpu::MULT()
{
	s_doubleword source64 = s_word(gp_reg[source()].read()); // cast to signed 32 bit value, and then cast to 64 bit value
	s_doubleword target64 = s_word(gp_reg[target()].read()); // consider including function which returns signed value? (need to cast to 64 bits either way)
	s_doubleword result = source64 * target64;
	LO.write(result & 0xFFFFFFFF);
	HI.write(result >> 32);
}
void cpu::MULTU()
{
	doubleword source64 = gp_reg[source()].read(); // cast to 64 bit value
	doubleword target64 = gp_reg[target()].read();
	doubleword result = source64 * target64;
	LO.write(result & 0xFFFFFFFF);
	HI.write(result >> 32);
}

void cpu::DIV()
{
	LO.write(s_word(gp_reg[source()].read()) / s_word(gp_reg[target()].read()));
	HI.write(s_word(gp_reg[source()].read()) % s_word(gp_reg[target()].read()));
}

void cpu::DIVU()
{
	LO.write(gp_reg[source()].read() / gp_reg[target()].read());
	HI.write(gp_reg[source()].read() % gp_reg[target()].read());
}

/********** Move from/to HI/LO ********/
void cpu::MFHI()
{
	gp_reg[dest()].write(HI.read());	// there are some access rules but I guess we can ignore these
}

void cpu::MTHI()
{
	HI.write(gp_reg[dest()].read());
}

void cpu::MFLO()
{
	gp_reg[dest()].write(LO.read());
}

void cpu::MTLO()
{
	LO.write(gp_reg[dest()].read());
}


/********** Bitwise Logic *************/
void cpu::AND()
{
	word result = gp_reg[source()].read() & gp_reg[target()].read();
	gp_reg[dest()].write(result);
}

void cpu::ANDI()
{
	word imm32 = imm() & 0xFFFF;
	word result = gp_reg[source()].read() & imm32;
	gp_reg[target()].write(result);
}

void cpu::OR()
{
	word result = gp_reg[source()].read() | gp_reg[target()].read();
	gp_reg[dest()].write(result);
}

void cpu::ORI()
{
	word imm32 = imm() & 0xFFFF;
	word result = gp_reg[source()].read() | imm32;
	gp_reg[target()].write(result);
}

void cpu::XOR()
{
	word result = gp_reg[source()].read() ^ gp_reg[target()].read();
	gp_reg[dest()].write(result);
}

void cpu::XORI()
{
	word imm32 = imm() & 0xFFFF;
	word result = gp_reg[source()].read() ^ imm32;
	gp_reg[target()].write(result);
}

void cpu::NOR()
{
	word result = gp_reg[source()].read() | gp_reg[target()].read();
	result ^= 0xFFFFFFFF;
	gp_reg[dest()].write(result);
}


/********** Shifts ********************/
void cpu::SLL()
{
	word result = gp_reg[target()].read() << shamt();
	gp_reg[dest()].write(result);
}

void cpu::SRL()
{
	word result = gp_reg[target()].read() >> shamt();
	word mask = (1 << (31 - shamt()));
	mask |= mask - 1;
	result &= mask;
	gp_reg[dest()].write(result);
}

void cpu::SRA()
{
	word result = gp_reg[target()].read() >> shamt();
	gp_reg[dest()].write(result);
}

void cpu::SLLV()
{
	word result = gp_reg[target()].read() << gp_reg[source()].read();
	gp_reg[dest()].write(result);
}

void cpu::SRLV()
{
	word result = gp_reg[target()].read() >> gp_reg[source()].read();
	word mask = (1 << (31 - gp_reg[source()].read()));
	mask |= mask - 1;
	result &= mask;
	gp_reg[dest()].write(result);
}

void cpu::SRAV()
{
	word result = gp_reg[target()].read() >> gp_reg[source()].read();
	gp_reg[dest()].write(result);
}


/********** Load/Store ****************/
void cpu::LB()
{
	word address = gp_reg[source()].read() + imm();
	word result = memory->readByte(address) & 0xFF;
	if (result >> 7)
		result |= 0xFFFFFF00;
	gp_reg[target()].write(result);
}

void cpu::LBU()
{
	word address = gp_reg[source()].read() + imm();
	word result = memory->readByte(address) & 0xFF;
	gp_reg[target()].write(result);
}

void cpu::LH()
{
	word address = gp_reg[source()].read() + imm();
	word result;
	if (checkEndianness())
		result = memory->readHalfwordLittle(address) & 0xFFFF;
	else
		result = memory->readHalfwordBig(address) & 0xFFFF;
	if (result >> 15)
		result |= 0xFFFF0000;
	gp_reg[target()].write(result);
}

void cpu::LHU()
{
	word address = gp_reg[source()].read() + imm();
	word result;
	if (checkEndianness())
		result = memory->readHalfwordLittle(address) & 0xFFFF;
	else
		result = memory->readHalfwordBig(address) & 0xFFFF;
	gp_reg[target()].write(result);
}

void cpu::LW()
{
	word address = gp_reg[source()].read() + imm();
	word result;
	if (checkEndianness())
		result = memory->readWordLittle(address);
	else
		result = memory->readWordBig(address);
	gp_reg[target()].write(result);
}

// following 2 instructions are in big-endian mode. I believe PSX is little endian by default
void cpu::LWL()
{
	word address = gp_reg[source()].read() + imm();
	word byte_number = address % 4;
	address -= byte_number;	// align address

	word load_data = memory->readWordBig(address);
	
	// shift word to make the selected byte the most significant, and fill in to the right
	load_data <<= byte_number * 8;

	// mask target() so it matches the empty bytes left by the load data
	word result = gp_reg[target()].read() & (0xFFFFFFFF >> ((4 - byte_number) * 8));

	gp_reg[target()].write(result | load_data);
}

void cpu::LWR()
{
	word address = gp_reg[source()].read() + imm();
	word byte_number = address % 4;
	address -= byte_number;	// align address

	word load_data = memory->readWordBig(address);

	// shift word to make the selected byte the least significant, and fill in to the left
	load_data >>= (3 - byte_number) * 8;

	// mask target() so it matches the empty bytes left by the load data
	word result = gp_reg[target()].read() & (0xFFFFFFFF << ((1 + byte_number) * 8));

	gp_reg[target()].write(result | load_data);
}


void cpu::SB()
{
	word address = gp_reg[source()].read() + imm();
	//byte data_in = gp_reg[target()].read() & 0xFF;
	memory->writeByte(address, byte(gp_reg[target()].read()));
}

void cpu::SH()
{
	word address = gp_reg[source()].read() + imm();
	if (checkEndianness())
		memory->writeHalfwordLittle(address, halfword(gp_reg[target()].read()));
	else
		memory->writeHalfwordBig(address, halfword(gp_reg[target()].read()));
}

void cpu::SW()
{
	word address = gp_reg[source()].read() + imm();
	if (checkEndianness())
		memory->writeWordLittle(address, gp_reg[target()].read());
	else
		memory->writeWordBig(address, gp_reg[target()].read());
}

// next 2 instructions: little endian!
void cpu::SWL()
{

}

void cpu::SWR()
{
}


void cpu::LUI()
{
	gp_reg[target()].write(imm() << 16);
}


/********** Branch ********************/
void cpu::BEQ()
{
	if (gp_reg[source()].read() == gp_reg[target()].read())
		branchPC(imm());
}

void cpu::BNE()
{
	if (gp_reg[source()].read() != gp_reg[target()].read())
		branchPC(imm());
}

void cpu::BCOND()
{
	bcond_type[target()];
}

void cpu::BGEZ()
{
	if (gp_reg[source()].read() >= 0)
		branchPC(imm());
}

void cpu::BGEZAL()
{
	if (gp_reg[source()].read() >= 0)
	{
		gp_reg[31].write(PC.read() + 8);
		branchPC(imm());
	}
}

void cpu::BLTZ()
{
	if (gp_reg[source()].read() < 0)
		branchPC(imm());
}

void cpu::BLTZAL()
{
	if (gp_reg[source()].read() < 0)
	{
		gp_reg[31].write(PC.read() + 8);
		branchPC(imm());
	}
}

void cpu::BGTZ()
{
	if (gp_reg[source()].read() > 0)
		branchPC(imm());
}

void cpu::BLEZ()
{
	if (gp_reg[source()].read() <= 0)
		branchPC(imm());
}


/********** Jump **********************/
void cpu::J()
{
	jumpPC(jump());
}

void cpu::JAL()
{
	gp_reg[31].write(PC.read() + 8);
	jumpPC(jump());
}

void cpu::JR()
{
	jumpPC(gp_reg[source()].read());
}

void cpu::JALR()
{
	gp_reg[31].write(PC.read() + 8);
	jumpPC(gp_reg[source()].read());
}


/********** Set ***********************/
void cpu::SLT()
{
	word result = (s_word(gp_reg[source()].read()) < gp_reg[target()].read());
	gp_reg[dest()].write(result);
}

void cpu::SLTU()
{
	word result = (gp_reg[source()].read() < gp_reg[target()].read());
	gp_reg[dest()].write(result);
}

void cpu::SLTI()
{
	s_word imm32 = imm();
	word result = (s_word(gp_reg[source()].read()) < imm32);
	gp_reg[target()].write(result);
}

void cpu::SLTIU()
{
	s_word imm32 = imm();
	word result = (gp_reg[source()].read() < imm32);
	gp_reg[target()].write(result);
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
void cpu::LWCz()
{
	word address = gp_reg[source()].read() + imm();
	word data;
	if (checkEndianness())
		data = memory->readWordLittle(address);
	else
		data = memory->readWordBig(address);
	cop[coproc()]->writeDataReg(data, target());
}

void cpu::SWCz()
{
	word address = gp_reg[source()].read() + imm();
	if (checkEndianness())
		memory->writeWordLittle(address, cop[coproc()]->readDataReg(target()));
	else
		memory->writeWordBig(address, cop[coproc()]->readDataReg(target()));
}

/*void cpu::MTCz()
{
	// write coproc reg "dest()"
	cop[coproc()]->storeDataReg(reg[target()].read(), dest());
}

void cpu::MFCz()
{
	// write proc reg "dest()"
	word data = cop[coproc()]->loadDataReg(target());
	reg[dest()].write(data);
}

void cpu::CTCz()
{
	// write control coproc reg "dest()"
	cop[coproc()]->storeControlReg(reg[target()].read(), dest());
}

void cpu::CFCz()
{
	// write proc reg "dest()"
	word data = cop[coproc()]->loadControlReg(target());
	reg[dest()].write(data);
}

void cpu::COPz()
{
	cop[coproc]->executeInstruction(instruction);
}*/
