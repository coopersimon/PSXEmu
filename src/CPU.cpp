#include "CPU.h"
#include "Memory.h"
#include "PSException.h"

// shifts the offset up by 2 bits, and adds to the next PC, minus one instruction
#define branchPC(x);	sword result = shalfword(x) << 2; \
			PC_Next.write((PC_Next.read() + result) - 4);

// shifts the jump value by 2 bits, and sets the next PC, minus one instruction
// upper 4 bits are the same as next PC
#define jumpPC(x);	word result = PC_Next & 0xF0000000; \
			result |= x << 2; \
			PC_Next.write(result - 4);

/*** Instructions **************/

/********** Add/Subtract **************/
void cpu::ADD(unsigned dest, unsigned source, unsigned target) {
	regData32 result = gp_gp_reg[source].read() + gp_gp_reg[target].read();
	if (0x80000000 & (gp_reg[source].read() ^ result)) { // if output sign and input sign are different
		if (0x80000000 & gp_reg[source].read() & gp_reg[target].read()) // if the operand signs are the same
			throw new ovfException;
	}
	gp_reg[dest].write(result);
}
void cpu::ADDU(unsigned dest, unsigned source, unsigned target) {
	regData32 result = gp_reg[source].read() + gp_reg[target].read();
	gp_reg[dest].write(result);
}
void cpu::ADDI(unsigned target, unsigned source, shalfword immediate) {
	regData32 result = gp_reg[source].read() + immediate;
	if (0x80000000 & (gp_reg[source].read() ^ result)) { // if output sign and input sign are different
		if (0x80000000 & gp_reg[source].read() & gp_reg[target].read()) // if the operand signs are the same
			throw new ovfException;
	}
	gp_reg[target].write(result);
}
void cpu::ADDIU(unsigned target, unsigned source, shalfword immediate) {
	regData32 result = gp_reg[source].read() + immediate;
	gp_reg[target].write(result);
}
void cpu::SUB(unsigned dest, unsigned source, unsigned target) {
	regData32 result = gp_reg[source].read() - gp_reg[target].read();
	if (0x80000000 & (gp_reg[source].read() ^ result)) { // if output sign and input sign are different
		if (0x80000000 & (gp_reg[source].read() ^ gp_reg[target].read())) // if the operand signs are different
			throw new ovfException;
	gp_reg[dest].write(result);
}
void cpu::SUBU(unsigned dest, unsigned source, unsigned target) {
	regData32 result = gp_reg[source].read() - gp_reg[target].read();
	gp_reg[dest].write(result);
}

/********** Multiply/Divide ***********/
void cpu::MULT(unsigned source, unsigned target) {
	sdoubleword source64 = sword(gp_reg[source].read()); // cast to signed 32 bit value, and then cast to 64 bit value
	sdoubleword target64 = sword(gp_reg[target].read()); // consider including function which returns signed value? (need to cast to 64 bits either way)
	sdoubleword result = source64 * target64;
	LO = result & 0xFFFFFFFF;
	HI = result >> 32;
}

void cpu::MULTU(unsigned source, unsigned target) {
	doubleword source64 = gp_reg[source].read(); // cast to 64 bit value
	doubleword target64 = gp_reg[target].read();
	doubleword result = source64 * target64;
	LO = result & 0xFFFFFFFF;
	HI = result >> 32;
}

void cpu::DIV(unsigned source, unsigned target) {
	LO = sword(gp_reg[source].read()) / sword(gp_reg[target].read());
	HI = sword(gp_reg[source].read()) % sword(gp_reg[target].read());
}

void cpu::DIVU(unsigned source, unsigned target) {
	LO = gp_reg[source].read() / gp_reg[target].read();
	HI = gp_reg[source].read() % gp_reg[target].read();
}

/********** Move from/to HI/LO ********/
void cpu::MFHI(unsigned dest) {
	gp_reg[dest].write(HI.read());	// there are some access rules but I guess we can ignore these
}

void cpu::MTHI(unsigned dest) {
	HI.write(gp_reg[dest].read());
}

void cpu::MFLO(unsigned dest) {
	gp_reg[dest].write(LO.read());
}

void cpu::MTLO(unsigned dest) {
	LO.write(gp_reg[dest].read());
}


/********** Bitwise Logic *************/
void cpu::AND(unsigned dest, unsigned source, unsigned target) {
	word result = gp_reg[source].read() & gp_reg[target].read();
	gp_reg[dest].write(result);
}

void cpu::ANDI(unsigned target, unsigned source, halfword immediate) {
	word imm32 = immediate & 0xFFFF;
	word result = gp_reg[source].read() & imm32;
	gp_reg[target].write(result);
}

void cpu::OR(unsigned dest, unsigned source, unsigned target) {
	word result = gp_reg[source].read() | gp_reg[target].read();
	gp_reg[dest].write(result);
}

void cpu::ORI(unsigned target, unsigned source, halfword immediate) {
	word imm32 = immediate & 0xFFFF;
	word result = gp_reg[source].read() | imm32;
	gp_reg[target].write(result);
}

void cpu::XOR(unsigned dest, unsigned source, unsigned target) {
	word result = gp_reg[source].read() ^ gp_reg[target].read();
	gp_reg[dest].write(result);
}

void cpu::XORI(unsigned target, unsigned source, halfword immediate) {
	word imm32 = immediate & 0xFFFF;
	word result = gp_reg[source].read() ^ imm32;
	gp_reg[target].write(result);
}

void cpu::NOR(unsigned dest, unsigned source, unsigned target) {
	word result = gp_reg[source].read() | gp_reg[target].read();
	result ^= 0xFFFFFFFF;
	gp_reg[dest].write(result);
}


/********** Shifts ********************/
void cpu::SLL(unsigned dest, unsigned target, unsigned shamt) {
	word result = gp_reg[target].read() << shamt;
	gp_reg[dest].write(result);
}

void cpu::SRL(unsigned dest, unsigned target, unsigned shamt) {
	word result = gp_reg[target].read() >> shamt;
	word mask = (1 << (31 - shamt));
	mask |= mask - 1;
	result &= mask;
	gp_reg[dest].write(result);
}

void cpu::SRA(unsigned dest, unsigned target, unsigned shamt) {
	word result = gp_reg[target].read() >> shamt;
	gp_reg[dest].write(result);
}

void cpu::SLLV(unsigned dest, unsigned target, unsigned source) {
	unsigned shamt = source & 0x1F;
	SLL(dest, target, shamt);
}

void cpu::SRLV(unsigned dest, unsigned target, unsigned source) {
	unsigned shamt = source & 0x1F;
	SRL(dest, target, shamt);
}

void cpu::SRAV(unsigned dest, unsigned target, unsigned source) {
	unsigned shamt = source & 0x1F;
	SRA(dest, target, shamt);
}


/********** Load/Store ****************/
void cpu::LB(unsigned target, unsigned source, shalfword offset) {
	LBU(target, source, offset);
	if (target >> 7)
		target |= 0xFFFFFF00;	// sign extend
}

void cpu::LBU(unsigned target, unsigned source, shalfword offset) {
	word address = gp_reg[source].read() + offset;
	word result = memBus.readByte(address) & 0xFF;
	gp_reg[target].write(result);
}

void cpu::LH(unsigned target, unsigned source, shalfword offset) {
	LHU(target, source, offset);
	if (target >> 15)
		target |= 0xFFFF0000;	// sign extend
}

void cpu::LHU(unsigned target, unsigned source, shalfword offset) {
	word address = gp_reg[source].read() + offset;
	if (checkEndianness())
		word result = memBus.readHalfwordLittle(address) & 0xFFFF;
	else
		word result = memBus.readHalfwordBig(address) & 0xFFFF;
	gp_reg[target].write(result);
}

void cpu::LW(unsigned target, unsigned source, shalfword offset) {
	word address = gp_reg[source].read() + offset;
	if (checkEndianness())
		word result = memBus.readWordLittle(address);
	else
		word result = memBus.readWordBig(address);
	gp_reg[target].write(result);
}

// following 2 instructions are in big-endian mode. I believe PSX is little endian by default
void cpu::LWL(unsigned target, unsigned source, shalfword offset) {
	word address = gp_reg[source].read() + offset;
	word byte_number = address % 4;
	address -= byte_number;	// align address

	word load_data = memBus.readWord(address);
	
	// shift word to make the selected byte the most significant, and fill in to the right
	load_data <<= byte_number * 8;

	// mask target so it matches the empty bytes left by the load data
	word result = gp_reg[target].read() & (0xFFFFFFFF >> ((4 - byte_number) * 8));

	gp_reg[target].write(result | load_data);
}

void cpu::LWR(unsigned target, unsigned source, shalfword offset) {
	word address = gp_reg[source].read() + offset;
	word byte_number = address % 4;
	address -= byte_number;	// align address

	word load_data = memBus.readWord(address);

	// shift word to make the selected byte the least significant, and fill in to the left
	load_data >>= (3 - byte_number) * 8;

	// mask target so it matches the empty bytes left by the load data
	word result = gp_reg[target].read() & (0xFFFFFFFF << ((1 + byte_number) * 8));

	gp_reg[target].write(result | load_data);
}


void cpu::SB(unsigned target, unsigned source, shalfword offset) {
	word address = gp_reg[source].read() + offset;
	//byte data_in = gp_reg[target].read() & 0xFF;
	memBus.writeByte(address, byte(gp_reg[target].read()));
}

void cpu::SH(unsigned target, unsigned source, shalfword offset) {
	word address = gp_reg[source].read() + offset;
	if (checkEndianness)
		memBus.writeHalfwordLittle(address, halfword(gp_reg[target].read());
	else
		memBus.writeHalfwordBig(address, halfword(gp_reg[target].read());
}

void cpu::SW(unsigned target, unsigned source, shalfword offset) {
	word address = gp_reg[source].read() + offset;
	if (checkEndianness)
		memBus.writeWordLittle(address, gp_reg[target].read());
	else
		memBus.writeWordBig(address, gp_reg[target].read());
}

// next 2 instructions: little endian!
void cpu::SWL(unsigned target, unsigned source, shalfword offset) {

}

void cpu::SWR(unsigned target, unsigned source, shalfword offset) {
}


void cpu::LUI(unsigned target, halfword offset) {
	gp_reg[target].write(offset << 16);
}


/********** Branch ********************/
void cpu::BEQ(unsigned source, unsigned target, halfword offset) {
	if (gp_reg[source].read() != gp_reg[target].read())
		return;
	branchPC(offset);
}

void cpu::BNE(unsigned source, unsigned target, halfword offset) {
	if (gp_reg[source].read() == gp_reg[target].read())
		return;
	branchPC(offset);
}

void cpu::BGEZ(unsigned source, halfword offset); {
	if (gp_reg[source].read() < 0)
		return;
	branchPC(offset);
}

void cpu::BGEZAL(unsigned source, halfword offset); {
	if (gp_reg[source].read() < 0)
		return;
	gp_reg[31].write(PC.read() + 8);
	branchPC(offset);
}

void cpu::BLTZ(unsigned source, halfword offset); {
	if (gp_reg[source].read() >= 0)
		return;
	branchPC(offset);
}

void cpu::BLTZAL(unsigned source, halfword offset); {
	if (gp_reg[source].read() >= 0)
		return;
	gp_reg[31].write(PC.read() + 8);
	branchPC(offset);
}

void cpu::BGTZ(unsigned source, halfword offset) {
	if (gp_reg[source].read() <= 0)
		return
	branchPC(offset);
}

void cpu::BLEZ(unsigned source, halfword offset) {
	if (gp_reg[source].read() > 0)
		return;
	branchPC(offset);
}


/********** Jump **********************/
void cpu::J(word jump) {
	jumpPC(jump);
}

void cpu::JAL(word jump) {
	gp_reg[31].write(PC.read() + 8);
	jumpPC(jump);
}

void cpu::JR(unsigned source) {
	jumpPC(gp_reg[source].read());
}

void cpu::JALR(unsigned source) {
	gp_reg[31].write(PC.read() + 8);
	jumpPC(gp_reg[source].read());
}


/********** Set ***********************/
void cpu::SLT(unsigned dest, unsigned source, unsigned target) {
	word result = (sword(gp_reg[source].read()) < gp_reg[target].read());
	gp_reg[dest].write(result);
}

void cpu::SLTU(unsigned dest, unsigned source, unsigned target) {
	word result = (gp_reg[source].read() < gp_reg[target].read());
	gp_reg[dest].write(result);
}

void cpu::SLTI(unsigned target, unsigned source, halfword immediate) {
	sword imm32 = immediate;
	word result = (sword(gp_reg[source].read()) < imm32);
	gp_reg[target].write(result);
}

void cpu::SLTIU(unsigned target, unsigned source, halfword immediate) {
	sword imm32 = immediate;
	word result = (gp_reg[source].read() < imm32);
	gp_reg[target].write(result);
}


/********** Special *******************/
void cpu::SYSCALL() {
	throw new sysException;
}

void cpu::BREAK() {
	throw new bpException;
}


/********** Coprocessor ***************/
void cpu::LWCz(unsigned target, unsigned source, halfword offset, unsigned coproc) {
	word address = gp_reg[source].read() + offset;
	if (checkEndianness())
		word result = memBus.readWordLittle(address);
	else
		word result = memBus.readWordBig(address);
	// write coproc reg "target"
}

void cpu::SWCz(unsigned target, unsigned source, halfword offset, unsigned coproc) {
	word address = gp_reg[source].read() + offset;
	if (checkEndianness)
		memBus.writeWordLittle(address, /*coproc reg target*/);
	else
		memBus.writeWordBig(address, /*coproc reg target*/);
}

void cpu::MTCz(unsigned target, unsigned dest, unsigned coproc) {
	// write coproc reg "dest"
}

void cpu::MFCz(unsigned target, unsigned dest, unsigned coproc) {
	// write proc reg "dest"
}

void cpu::CTCz(unsigned target, unsigned dest, unsigned coproc) {
	// write control coproc reg "dest"
}

void cpu::CFCz(unsigned target, unsigned dest, unsigned coproc) {
	// write proc reg "dest"
}

