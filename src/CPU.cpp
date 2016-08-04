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
	regData32 result = reg[source].read() + reg[target].read();
	if (0x80000000 & (reg[source].read() ^ result)) { // if output sign and input sign are different
		if (0x80000000 & reg[source].read() & reg[target].read()) // if the operand signs are the same
			throw new ovfException;
	}
	reg[dest].write(result);
}
void r3051::ADDU(unsigned dest, unsigned source, unsigned target) {
	regData32 result = reg[source].read() + reg[target].read();
	reg[dest].write(result);
}
void r3051::ADDI(unsigned target, unsigned source, shalfword immediate) {
	regData32 result = reg[source].read() + immediate;
	if (0x80000000 & (reg[source].read() ^ result)) { // if output sign and input sign are different
		if (0x80000000 & reg[source].read() & reg[target].read()) // if the operand signs are the same
			throw new ovfException;
	}
	reg[target].write(result);
}
void r3051::ADDIU(unsigned target, unsigned source, shalfword immediate) {
	regData32 result = reg[source].read() + immediate;
	reg[target].write(result);
}
void r3051::SUB(unsigned dest, unsigned source, unsigned target) {
	regData32 result = reg[source].read() - reg[target].read();
	if (0x80000000 & (reg[source].read() ^ result)) { // if output sign and input sign are different
		if (0x80000000 & (reg[source].read() ^ reg[target].read())) // if the operand signs are different
			throw new ovfException;
	reg[dest].write(result);
}
void r3051::SUBU(unsigned dest, unsigned source, unsigned target) {
	regData32 result = reg[source].read() - reg[target].read();
	reg[dest].write(result);
}

/********** Multiply/Divide ***********/
void MULT(unsigned source, unsigned target) {
	sdoubleword source64 = sword(reg[source].read()); // cast to signed 32 bit value, and then cast to 64 bit value
	sdoubleword target64 = sword(reg[target].read()); // consider including function which returns signed value? (need to cast to 64 bits either way)
	sdoubleword result = source64 * target64;
	LO = result & 0xFFFFFFFF;
	HI = result >> 32;
}

void MULTU(unsigned source, unsigned target) {
	doubleword source64 = reg[source].read(); // cast to 64 bit value
	doubleword target64 = reg[target].read();
	doubleword result = source64 * target64;
	LO = result & 0xFFFFFFFF;
	HI = result >> 32;
}

void DIV(unsigned source, unsigned target) {
	LO = sword(reg[source].read()) / sword(reg[target].read());
	HI = sword(reg[source].read()) % sword(reg[target].read());
}

void DIVU(unsigned source, unsigned target) {
	LO = reg[source].read() / reg[target].read();
	HI = reg[source].read() % reg[target].read();
}

/********** Move from/to HI/LO ********/
void MFHI(unsigned dest) {
	reg[dest].write(HI.read());	// there are some access rules but I guess we can ignore these
}

void MTHI(unsigned dest) {
	HI.write(reg[dest].read());
}

void MFLO(unsigned dest) {
	reg[dest].write(LO.read());
}

void MTLO(unsigned dest) {
	LO.write(reg[dest].read());
}


/********** Bitwise Logic *************/
void AND(unsigned dest, unsigned source, unsigned target) {
	word result = reg[source].read() & reg[target].read();
	reg[dest].write(result);
}

void ANDI(unsigned target, unsigned source, halfword immediate) {
	word imm32 = immediate & 0xFFFF;
	word result = reg[source].read() & imm32;
	reg[target].write(result);
}

void OR(unsigned dest, unsigned source, unsigned target) {
	word result = reg[source].read() | reg[target].read();
	reg[dest].write(result);
}

void ORI(unsigned target, unsigned source, halfword immediate) {
	word imm32 = immediate & 0xFFFF;
	word result = reg[source].read() | imm32;
	reg[target].write(result);
}

void XOR(unsigned dest, unsigned source, unsigned target) {
	word result = reg[source].read() ^ reg[target].read();
	reg[dest].write(result);
}

void XORI(unsigned target, unsigned source, halfword immediate) {
	word imm32 = immediate & 0xFFFF;
	word result = reg[source].read() ^ imm32;
	reg[target].write(result);
}

void NOR(unsigned dest, unsigned source, unsigned target) {
	word result = reg[source].read() | reg[target].read();
	result ^= 0xFFFFFFFF;
	reg[dest].write(result);
}


/********** Shifts ********************/
void SLL(unsigned dest, unsigned target, unsigned shamt) {
	word result = reg[target].read() << shamt;
	reg[dest].write(result);
}

void SRL(unsigned dest, unsigned target, unsigned shamt) {
	word result = reg[target].read() >> shamt;
	word mask = (1 << (31 - shamt));
	mask |= mask - 1;
	result &= mask;
	reg[dest].write(result);
}

void SRA(unsigned dest, unsigned target, unsigned shamt) {
	word result = reg[target].read() >> shamt;
	reg[dest].write(result);
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
	word address = reg[source].read() + offset;
	word result = memBus.readByte(address) & 0xFF;
	reg[target].write(result);
}

void LH(unsigned target, unsigned source, shalfword offset) {
	LHU(target, source, offset);
	if (target >> 15)
		target |= 0xFFFF0000;	// sign extend
}

void LHU(unsigned target, unsigned source, shalfword offset) {
	word address = reg[source].read() + offset;
	if (checkEndianness())
		word result = memBus.readHalfwordLittle(address) & 0xFFFF;
	else
		word result = memBus.readHalfwordBig(address) & 0xFFFF;
	reg[target].write(result);
}

void LW(unsigned target, unsigned source, shalfword offset) {
	word address = reg[source].read() + offset;
	if (checkEndianness())
		word result = memBus.readWordLittle(address);
	else
		word result = memBus.readWordBig(address);
	reg[target].write(result);
}

// following 2 instructions are in big-endian mode. I believe PSX is little endian by default
void LWL(unsigned target, unsigned source, shalfword offset) {
	word address = reg[source].read() + offset;
	word byte_number = address % 4;
	address -= byte_number;	// align address

	word load_data = memBus.readWord(address);
	
	// shift word to make the selected byte the most significant, and fill in to the right
	load_data <<= byte_number * 8;

	// mask target so it matches the empty bytes left by the load data
	word result = reg[target].read() & (0xFFFFFFFF >> ((4 - byte_number) * 8));

	reg[target].write(result | load_data);
}

void LWR(unsigned target, unsigned source, shalfword offset) {
	word address = reg[source].read() + offset;
	word byte_number = address % 4;
	address -= byte_number;	// align address

	word load_data = memBus.readWord(address);

	// shift word to make the selected byte the least significant, and fill in to the left
	load_data >>= (3 - byte_number) * 8;

	// mask target so it matches the empty bytes left by the load data
	word result = reg[target].read() & (0xFFFFFFFF << ((1 + byte_number) * 8));

	reg[target].write(result | load_data);
}


void SB(unsigned target, unsigned source, shalfword offset) {
	word address = reg[source].read() + offset;
	//byte data_in = reg[target].read() & 0xFF;
	memBus.writeByte(address, byte(reg[target].read()));
}

void SH(unsigned target, unsigned source, shalfword offset) {
	word address = reg[source].read() + offset;
	if (checkEndianness)
		memBus.writeHalfwordLittle(address, halfword(reg[target].read());
	else
		memBus.writeHalfwordBig(address, halfword(reg[target].read());
}

void SW(unsigned target, unsigned source, shalfword offset) {
	word address = reg[source].read() + offset;
	if (checkEndianness)
		memBus.writeWordLittle(address, reg[target].read());
	else
		memBus.writeWordBig(address, reg[target].read());
}

// next 2 instructions: little endian!
void SWL(unsigned target, unsigned source, shalfword offset) {

}

void SWR(unsigned target, unsigned source, shalfword offset) {
}


void LUI(unsigned target, halfword offset) {
	reg[target].write(offset << 16);
}


/********** Branch ********************/
void BEQ(unsigned source, unsigned target, halfword offset) {
	if (reg[source].read() != reg[target].read())
		return;
	branchPC(offset);
}

void BNE(unsigned source, unsigned target, halfword offset) {
	if (reg[source].read() == reg[target].read())
		return;
	branchPC(offset);
}

void BGEZ(unsigned source, halfword offset); {
	if (reg[source].read() < 0)
		return;
	branchPC(offset);
}

void BGEZAL(unsigned source, halfword offset); {
	if (reg[source].read() < 0)
		return;
	reg[31].write(PC.read() + 8);
	branchPC(offset);
}

void BLTZ(unsigned source, halfword offset); {
	if (reg[source].read() >= 0)
		return;
	branchPC(offset);
}

void BLTZAL(unsigned source, halfword offset); {
	if (reg[source].read() >= 0)
		return;
	reg[31].write(PC.read() + 8);
	branchPC(offset);
}

void BGTZ(unsigned source, halfword offset) {
	if (reg[source].read() <= 0)
		return
	branchPC(offset);
}

void BLEZ(unsigned source, halfword offset) {
	if (reg[source].read() > 0)
		return;
	branchPC(offset);
}


/********** Jump **********************/
void J(word jump) {
	jumpPC(jump);
}

void JAL(word jump) {
	reg[31].write(PC.read() + 8);
	jumpPC(jump);
}

void JR(unsigned source) {
	jumpPC(reg[source].read());
}

void JALR(unsigned source) {
	reg[31].write(PC.read() + 8);
	jumpPC(reg[source].read());
}


/********** Set ***********************/
void SLT(unsigned dest, unsigned source, unsigned target) {
	word result = (sword(reg[source].read()) < reg[target].read());
	reg[dest].write(result);
}

void SLTU(unsigned dest, unsigned source, unsigned target) {
	word result = (reg[source].read() < reg[target].read());
	reg[dest].write(result);
}

void SLTI(unsigned target, unsigned source, halfword immediate) {
	sword imm32 = immediate;
	word result = (sword(reg[source].read()) < imm32);
	reg[target].write(result);
}

void SLTIU(unsigned target, unsigned source, halfword immediate) {
	sword imm32 = immediate;
	word result = (reg[source].read() < imm32);
	reg[target].write(result);
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
	word address = reg[source].read() + offset;
	if (checkEndianness())
		word result = memBus.readWordLittle(address);
	else
		word result = memBus.readWordBig(address);
	// write coproc reg "target"
}

void SWCz(unsigned target, unsigned source, halfword offset, unsigned coproc) {
	word address = reg[source].read() + offset;
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

