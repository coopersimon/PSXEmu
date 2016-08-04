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
void r3051::ADD(unsigned dest, unsigned source, unsigned target) {
	regData32 result = gp_gp_reg[source].read() + gp_gp_reg[target].read();
	if (0x80000000 & (gp_reg[source].read() ^ result)) { // if output sign and input sign are different
		if (0x80000000 & gp_reg[source].read() & gp_reg[target].read()) // if the operand signs are the same
			throw new ovfException;
	}
	gp_reg[dest].write(result);
}
void r3051::ADDU(unsigned dest, unsigned source, unsigned target) {
	regData32 result = gp_reg[source].read() + gp_reg[target].read();
	gp_reg[dest].write(result);
}
void r3051::ADDI(unsigned target, unsigned source, shalfword immediate) {
	regData32 result = gp_reg[source].read() + immediate;
	if (0x80000000 & (gp_reg[source].read() ^ result)) { // if output sign and input sign are different
		if (0x80000000 & gp_reg[source].read() & gp_reg[target].read()) // if the operand signs are the same
			throw new ovfException;
	}
	gp_reg[target].write(result);
}
void r3051::ADDIU(unsigned target, unsigned source, shalfword immediate) {
	regData32 result = gp_reg[source].read() + immediate;
	gp_reg[target].write(result);
}
void r3051::SUB(unsigned dest, unsigned source, unsigned target) {
	regData32 result = gp_reg[source].read() - gp_reg[target].read();
	if (0x80000000 & (gp_reg[source].read() ^ result)) { // if output sign and input sign are different
		if (0x80000000 & (gp_reg[source].read() ^ gp_reg[target].read())) // if the operand signs are different
			throw new ovfException;
	gp_reg[dest].write(result);
}
void r3051::SUBU(unsigned dest, unsigned source, unsigned target) {
	regData32 result = gp_reg[source].read() - gp_reg[target].read();
	gp_reg[dest].write(result);
}

/********** Multiply/Divide ***********/
void MULT(unsigned source, unsigned target) {
	sdoubleword source64 = sword(gp_reg[source].read()); // cast to signed 32 bit value, and then cast to 64 bit value
	sdoubleword target64 = sword(gp_reg[target].read()); // consider including function which returns signed value? (need to cast to 64 bits either way)
	sdoubleword result = source64 * target64;
	LO = result & 0xFFFFFFFF;
	HI = result >> 32;
}

void MULTU(unsigned source, unsigned target) {
	doubleword source64 = gp_reg[source].read(); // cast to 64 bit value
	doubleword target64 = gp_reg[target].read();
	doubleword result = source64 * target64;
	LO = result & 0xFFFFFFFF;
	HI = result >> 32;
}

void DIV(unsigned source, unsigned target) {
	LO = sword(gp_reg[source].read()) / sword(gp_reg[target].read());
	HI = sword(gp_reg[source].read()) % sword(gp_reg[target].read());
}

void DIVU(unsigned source, unsigned target) {
	LO = gp_reg[source].read() / gp_reg[target].read();
	HI = gp_reg[source].read() % gp_reg[target].read();
}

/********** Move from/to HI/LO ********/
void MFHI(unsigned dest) {
	gp_reg[dest].write(HI.read());	// there are some access rules but I guess we can ignore these
}

void MTHI(unsigned dest) {
	HI.write(gp_reg[dest].read());
}

void MFLO(unsigned dest) {
	gp_reg[dest].write(LO.read());
}

void MTLO(unsigned dest) {
	LO.write(gp_reg[dest].read());
}


/********** Bitwise Logic *************/
void AND(unsigned dest, unsigned source, unsigned target) {
	word result = gp_reg[source].read() & gp_reg[target].read();
	gp_reg[dest].write(result);
}

void ANDI(unsigned target, unsigned source, halfword immediate) {
	word imm32 = immediate & 0xFFFF;
	word result = gp_reg[source].read() & imm32;
	gp_reg[target].write(result);
}

void OR(unsigned dest, unsigned source, unsigned target) {
	word result = gp_reg[source].read() | gp_reg[target].read();
	gp_reg[dest].write(result);
}

void ORI(unsigned target, unsigned source, halfword immediate) {
	word imm32 = immediate & 0xFFFF;
	word result = gp_reg[source].read() | imm32;
	gp_reg[target].write(result);
}

void XOR(unsigned dest, unsigned source, unsigned target) {
	word result = gp_reg[source].read() ^ gp_reg[target].read();
	gp_reg[dest].write(result);
}

void XORI(unsigned target, unsigned source, halfword immediate) {
	word imm32 = immediate & 0xFFFF;
	word result = gp_reg[source].read() ^ imm32;
	gp_reg[target].write(result);
}

void NOR(unsigned dest, unsigned source, unsigned target) {
	word result = gp_reg[source].read() | gp_reg[target].read();
	result ^= 0xFFFFFFFF;
	gp_reg[dest].write(result);
}


/********** Shifts ********************/
void SLL(unsigned dest, unsigned target, unsigned shamt) {
	word result = gp_reg[target].read() << shamt;
	gp_reg[dest].write(result);
}

void SRL(unsigned dest, unsigned target, unsigned shamt) {
	word result = gp_reg[target].read() >> shamt;
	word mask = (1 << (31 - shamt));
	mask |= mask - 1;
	result &= mask;
	gp_reg[dest].write(result);
}

void SRA(unsigned dest, unsigned target, unsigned shamt) {
	word result = gp_reg[target].read() >> shamt;
	gp_reg[dest].write(result);
}

void SLLV(unsigned dest, unsigned target, unsigned source) {
	unsigned shamt = source & 0x1F;
	SLL(dest, target, shamt);
}

void SRLV(unsigned dest, unsigned target, unsigned source) {
	unsigned shamt = source & 0x1F;
	SRL(dest, target, shamt);
}

void SRAV(unsigned dest, unsigned target, unsigned source) {
	unsigned shamt = source & 0x1F;
	SRA(dest, target, shamt);
}


/********** Load/Store ****************/
void LB(unsigned target, unsigned source, shalfword offset) {
	LBU(target, source, offset);
	if (target >> 7)
		target |= 0xFFFFFF00;	// sign extend
}

void LBU(unsigned target, unsigned source, shalfword offset) {
	word address = gp_reg[source].read() + offset;
	word result = memBus.readByte(address) & 0xFF;
	gp_reg[target].write(result);
}

void LH(unsigned target, unsigned source, shalfword offset) {
	LHU(target, source, offset);
	if (target >> 15)
		target |= 0xFFFF0000;	// sign extend
}

void LHU(unsigned target, unsigned source, shalfword offset) {
	word address = gp_reg[source].read() + offset;
	if (checkEndianness())
		word result = memBus.readHalfwordLittle(address) & 0xFFFF;
	else
		word result = memBus.readHalfwordBig(address) & 0xFFFF;
	gp_reg[target].write(result);
}

void LW(unsigned target, unsigned source, shalfword offset) {
	word address = gp_reg[source].read() + offset;
	if (checkEndianness())
		word result = memBus.readWordLittle(address);
	else
		word result = memBus.readWordBig(address);
	gp_reg[target].write(result);
}

// following 2 instructions are in big-endian mode. I believe PSX is little endian by default
void LWL(unsigned target, unsigned source, shalfword offset) {
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

void LWR(unsigned target, unsigned source, shalfword offset) {
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


void SB(unsigned target, unsigned source, shalfword offset) {
	word address = gp_reg[source].read() + offset;
	//byte data_in = gp_reg[target].read() & 0xFF;
	memBus.writeByte(address, byte(gp_reg[target].read()));
}

void SH(unsigned target, unsigned source, shalfword offset) {
	word address = gp_reg[source].read() + offset;
	if (checkEndianness)
		memBus.writeHalfwordLittle(address, halfword(gp_reg[target].read());
	else
		memBus.writeHalfwordBig(address, halfword(gp_reg[target].read());
}

void SW(unsigned target, unsigned source, shalfword offset) {
	word address = gp_reg[source].read() + offset;
	if (checkEndianness)
		memBus.writeWordLittle(address, gp_reg[target].read());
	else
		memBus.writeWordBig(address, gp_reg[target].read());
}

// next 2 instructions: little endian!
void SWL(unsigned target, unsigned source, shalfword offset) {

}

void SWR(unsigned target, unsigned source, shalfword offset) {
}


void LUI(unsigned target, halfword offset) {
	gp_reg[target].write(offset << 16);
}


/********** Branch ********************/
void BEQ(unsigned source, unsigned target, halfword offset) {
	if (gp_reg[source].read() != gp_reg[target].read())
		return;
	branchPC(offset);
}

void BNE(unsigned source, unsigned target, halfword offset) {
	if (gp_reg[source].read() == gp_reg[target].read())
		return;
	branchPC(offset);
}

void BGEZ(unsigned source, halfword offset); {
	if (gp_reg[source].read() < 0)
		return;
	branchPC(offset);
}

void BGEZAL(unsigned source, halfword offset); {
	if (gp_reg[source].read() < 0)
		return;
	gp_reg[31].write(PC.read() + 8);
	branchPC(offset);
}

void BLTZ(unsigned source, halfword offset); {
	if (gp_reg[source].read() >= 0)
		return;
	branchPC(offset);
}

void BLTZAL(unsigned source, halfword offset); {
	if (gp_reg[source].read() >= 0)
		return;
	gp_reg[31].write(PC.read() + 8);
	branchPC(offset);
}

void BGTZ(unsigned source, halfword offset) {
	if (gp_reg[source].read() <= 0)
		return
	branchPC(offset);
}

void BLEZ(unsigned source, halfword offset) {
	if (gp_reg[source].read() > 0)
		return;
	branchPC(offset);
}


/********** Jump **********************/
void J(word jump) {
	jumpPC(jump);
}

void JAL(word jump) {
	gp_reg[31].write(PC.read() + 8);
	jumpPC(jump);
}

void JR(unsigned source) {
	jumpPC(gp_reg[source].read());
}

void JALR(unsigned source) {
	gp_reg[31].write(PC.read() + 8);
	jumpPC(gp_reg[source].read());
}


/********** Set ***********************/
void SLT(unsigned dest, unsigned source, unsigned target) {
	word result = (sword(gp_reg[source].read()) < gp_reg[target].read());
	gp_reg[dest].write(result);
}

void SLTU(unsigned dest, unsigned source, unsigned target) {
	word result = (gp_reg[source].read() < gp_reg[target].read());
	gp_reg[dest].write(result);
}

void SLTI(unsigned target, unsigned source, halfword immediate) {
	sword imm32 = immediate;
	word result = (sword(gp_reg[source].read()) < imm32);
	gp_reg[target].write(result);
}

void SLTIU(unsigned target, unsigned source, halfword immediate) {
	sword imm32 = immediate;
	word result = (gp_reg[source].read() < imm32);
	gp_reg[target].write(result);
}


/********** Special *******************/
void SYSCALL() {
	throw new sysException;
}

void BREAK() {
	throw new bpException;
}


/********** Coprocessor ***************/
void LWCz(unsigned target, unsigned source, halfword offset, unsigned coproc) {
	word address = gp_reg[source].read() + offset;
	if (checkEndianness())
		word result = memBus.readWordLittle(address);
	else
		word result = memBus.readWordBig(address);
	// write coproc reg "target"
}

void SWCz(unsigned target, unsigned source, halfword offset, unsigned coproc) {
	word address = gp_reg[source].read() + offset;
	if (checkEndianness)
		memBus.writeWordLittle(address, /*coproc reg target*/);
	else
		memBus.writeWordBig(address, /*coproc reg target*/);
}

void MTCz(unsigned target, unsigned dest, unsigned coproc) {
	// write coproc reg "dest"
}

void MFCz(unsigned target, unsigned dest, unsigned coproc) {
	// write proc reg "dest"
}

void CTCz(unsigned target, unsigned dest, unsigned coproc) {
	// write control coproc reg "dest"
}

void CFCz(unsigned target, unsigned dest, unsigned coproc) {
	// write proc reg "dest"
}

