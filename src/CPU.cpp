#include "CPU.h"
//#include "Memory.h"
#include "PSException.h"
#include <iostream>


// TODO: clever c++11 pointer stuff
cpu::cpu(memoryInterface *memIn)
{
	// this pointer should be handled better - unique ptr perhaps
	memory = memIn;
	
	// initialse coprocessors
	cop[0] = &SCC;
	// cop[2] = new gte;
	
	// reg 0 is always 0
	gp_reg[0].setMask(0x00000000);

	// initialise PC
	PC.write(0);
	PC_next.write(4);

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

void cpu::stepCPU()
{
	// get instruction from mem & advance PC
	instruction = memory->readWordLittle(PC.read());
	PC.write(PC_next.read());
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
		// set BD
		// set CE
		SCC.CAUSE.writeBits(28, 2, coproc());
		// set IP
		// set SW
		// set EXECODE
		SCC.CAUSE.writeBits(2, 5, e.execode());
	}

	incrementPCNext();
}

void cpu::RESERVED()
{
	throw riException();
}


/*** Instructions **************/

/********** Add/Subtract **************/
void cpu::ADD()
{
	word result = source() + target();
	if (0x80000000 & (source() ^ result)) // if output sign and input sign are different
	{	if (0x80000000 & source() & target()) // if the operand signs are the same
			throw ovfException();
	}
	dest(result);
}
void cpu::ADDU()
{
	word result = source() + target();
	dest(result);
}
void cpu::ADDI()
{
	word result = source() + imm();
	if (0x80000000 & (source() ^ result)) // if output sign and input sign are different
	{
		if (0x80000000 & source() & target()) // if the operand signs are the same
			throw ovfException();
	}
	target(result);
}
void cpu::ADDIU()
{
	word result = source() + imm();
	target(result);
}
void cpu::SUB()
{
	word result = source() - target();
	if (0x80000000 & (source() ^ result))  // if output sign and input sign are different
	{
		if (0x80000000 & (source() ^ target())) // if the operand signs are different
			throw ovfException();
	}
	dest(result);
}
void cpu::SUBU()
{
	word result = source() - target();
	dest(result);
}

/********** Multiply/Divide ***********/
void cpu::MULT()
{
	s_doubleword source64 = s_word(source()); // cast to signed 32 bit value, and then cast to 64 bit value
	s_doubleword target64 = s_word(target()); // consider including function which returns signed value? (need to cast to 64 bits either way)
	s_doubleword result = source64 * target64;
	LO.write(result & 0xFFFFFFFF);
	HI.write(result >> 32);
}
void cpu::MULTU()
{
	doubleword source64 = source(); // cast to 64 bit value
	doubleword target64 = target();
	doubleword result = source64 * target64;
	LO.write(result & 0xFFFFFFFF);
	HI.write(result >> 32);
}

void cpu::DIV()
{
	LO.write(s_word(source()) / s_word(target()));
	HI.write(s_word(source()) % s_word(target()));
}

void cpu::DIVU()
{
	LO.write(source() / target());
	HI.write(source() % target());
}

/********** Move from/to HI/LO ********/
void cpu::MFHI()
{
	dest(HI.read());	// there are some access rules but I guess we can ignore these
}

void cpu::MTHI()
{
	HI.write(dest());
}

void cpu::MFLO()
{
	dest(LO.read());
}

void cpu::MTLO()
{
	LO.write(dest());
}


/********** Bitwise Logic *************/
void cpu::AND()
{
	word result = source() & target();
	dest(result);
}

void cpu::ANDI()
{
	word imm32 = imm() & 0xFFFF;
	word result = source() & imm32;
	target(result);
}

void cpu::OR()
{
	word result = source() | target();
	dest(result);
}

void cpu::ORI()
{
	word imm32 = imm() & 0xFFFF;
	word result = source() | imm32;
	target(result);
}

void cpu::XOR()
{
	word result = source() ^ target();
	dest(result);
}

void cpu::XORI()
{
	word imm32 = imm() & 0xFFFF;
	word result = source() ^ imm32;
	target(result);
}

void cpu::NOR()
{
	word result = source() | target();
	result ^= 0xFFFFFFFF;
	dest(result);
}


/********** Shifts ********************/
void cpu::SLL()
{
	word result = target() << shamt();
	dest(result);
}

void cpu::SRL()
{
	word result = target() >> shamt();
	word mask = (1 << (31 - shamt()));
	mask |= mask - 1;
	result &= mask;
	dest(result);
}

void cpu::SRA()
{
	word result = target() >> shamt();
	dest(result);
}

void cpu::SLLV()
{
	word result = target() << source();
	dest(result);
}

void cpu::SRLV()
{
	word result = target() >> source();
	word mask = (1 << (31 - source()));
	mask |= mask - 1;
	result &= mask;
	dest(result);
}

void cpu::SRAV()
{
	word result = target() >> source();
	dest(result);
}


/********** Load/Store ****************/
void cpu::LB()
{
	word address = source() + imm();
	word result = memory->readByte(address) & 0xFF;
	if (result >> 7)
		result |= 0xFFFFFF00;
	target(result);
}

void cpu::LBU()
{
	word address = source() + imm();
	word result = memory->readByte(address) & 0xFF;
	target(result);
}

void cpu::LH()
{
	word address = source() + imm();
	word result;
	if (checkEndianness())
		result = memory->readHalfwordLittle(address) & 0xFFFF;
	else
		result = memory->readHalfwordBig(address) & 0xFFFF;
	if (result >> 15)
		result |= 0xFFFF0000;
	target(result);
}

void cpu::LHU()
{
	word address = source() + imm();
	word result;
	if (checkEndianness())
		result = memory->readHalfwordLittle(address) & 0xFFFF;
	else
		result = memory->readHalfwordBig(address) & 0xFFFF;
	target(result);
}

void cpu::LW()
{
	word address = source() + imm();
	word result;
	if (checkEndianness())
		result = memory->readWordLittle(address);
	else
		result = memory->readWordBig(address);
	target(result);
}

// following 2 instructions are in big-endian mode. I believe PSX is little endian by default
void cpu::LWL()
{
	word address = source() + imm();
	word byte_number = address % 4;
	address -= byte_number;	// align address

	word load_data = memory->readWordBig(address);
	
	// shift word to make the selected byte the most significant, and fill in to the right
	load_data <<= byte_number * 8;

	// mask target() so it matches the empty bytes left by the load data
	word result = target() & (0xFFFFFFFF >> ((4 - byte_number) * 8));

	target(result | load_data);
}

void cpu::LWR()
{
	word address = source() + imm();
	word byte_number = address % 4;
	address -= byte_number;	// align address

	word load_data = memory->readWordBig(address);

	// shift word to make the selected byte the least significant, and fill in to the left
	load_data >>= (3 - byte_number) * 8;

	// mask target() so it matches the empty bytes left by the load data
	word result = target() & (0xFFFFFFFF << ((1 + byte_number) * 8));

	target(result | load_data);
}


void cpu::SB()
{
	word address = source() + imm();
	//byte data_in = gp_reg[target()].read() & 0xFF;
	memory->writeByte(address, byte(target()));
}

void cpu::SH()
{
	word address = source() + imm();
	if (checkEndianness())
		memory->writeHalfwordLittle(address, halfword(target()));
	else
		memory->writeHalfwordBig(address, halfword(target()));
}

void cpu::SW()
{
	word address = source() + imm();
	if (checkEndianness())
		memory->writeWordLittle(address, target());
	else
		memory->writeWordBig(address, target());
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
	target(imm() << 16);
}


/********** Branch ********************/
void cpu::BEQ()
{
	if (source() == target())
		branchRoutine(imm());
}

void cpu::BNE()
{
	if (source() != target())
		branchRoutine(imm());
}

void cpu::BCOND()
{
	bcond_type[target_val()](this);
}

void cpu::BGEZ()
{
	if (source() >= 0)
		branchRoutine(imm());
}

void cpu::BGEZAL()
{
	if (source() >= 0)
	{
		gp_reg[31].write(PC.read() + 8);
		branchRoutine(imm());
	}
}

void cpu::BLTZ()
{
	if (source() < 0)
		branchRoutine(imm());
}

void cpu::BLTZAL()
{
	if (source() < 0)
	{
		gp_reg[31].write(PC.read() + 8);
		branchRoutine(imm());
	}
}

void cpu::BGTZ()
{
	if (source() > 0)
		branchRoutine(imm());
}

void cpu::BLEZ()
{
	if (source() <= 0)
		branchRoutine(imm());
}


/********** Jump **********************/
void cpu::J()
{
	word result = PC.read() & 0xF0000000;
	result |= jump() << 2;
	PC_next.write(result - 4);
}

void cpu::JAL()
{
	gp_reg[31].write(PC.read() + 8);
	word result = PC.read() & 0xF0000000;
	result |= jump() << 2;
	PC_next.write(result - 4);
}

void cpu::JR()
{
	PC_next.write(source() - 4);
}

void cpu::JALR()
{
	gp_reg[31].write(PC.read() + 8);
	PC_next.write(source() - 4);
}


/********** Set ***********************/
void cpu::SLT()
{
	word result = (s_word(source()) < target());
	dest(result);
}

void cpu::SLTU()
{
	word result = (source() < target());
	dest(result);
}

void cpu::SLTI()
{
	s_word imm32 = imm();
	word result = (s_word(source()) < imm32);
	target(result);
}

void cpu::SLTIU()
{
	s_word imm32 = imm();
	word result = (source() < imm32);
	target(result);
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
	word address = source() + imm();
	word data;
	if (checkEndianness())
		data = memory->readWordLittle(address);
	else
		data = memory->readWordBig(address);
	cop[coproc()]->writeDataReg(data, target_val());
}

void cpu::SWCz()
{
	word address = source() + imm();
	if (checkEndianness())
		memory->writeWordLittle(address, cop[coproc()]->readDataReg(target_val()));
	else
		memory->writeWordBig(address, cop[coproc()]->readDataReg(target_val()));
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
