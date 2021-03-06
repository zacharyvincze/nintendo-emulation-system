#include "cpu.h"

// ORA (OR FUNCTION ON ACCUMULATOR)

void CPU::ORA(u8 byte) {
	regA.set(regA.value() | byte);
	set_flags_nz(regA.value());
}

void CPU::ORA_09() { ORA(get_byte_from_pc()); loop_cycles += 2; }
void CPU::ORA_05() { ORA(memory.readByte(zero_page(3))); }
void CPU::ORA_15() { ORA(memory.readByte(zero_page_x(4))); }
void CPU::ORA_01() { ORA(memory.readByte(pre_indexed_indirect(6))); }
void CPU::ORA_11() {
	u16 before_page = 0xFF00 & regPC.value();
	ORA(memory.readByte(post_indexed_indirect(5)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}
void CPU::ORA_0D() { ORA(memory.readByte(absolute(4))); }
void CPU::ORA_1D() {
	u16 before_page = 0xFF00 & regPC.value();
	ORA(memory.readByte(absolute_x(4))); 
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}
void CPU::ORA_19() {
	u16 before_page = 0xFF00 & regPC.value();
	ORA(memory.readByte(absolute_y(4))); 
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}

// AND (AND FUNCTION ON ACCUMULATOR)
void CPU::AND(u8 byte) {
	regA.set(regA.value() & byte);
	set_flags_nz(regA.value());
}

void CPU::AND_29() { AND(get_byte_from_pc()); loop_cycles += 2; }
void CPU::AND_25() { AND(memory.readByte(zero_page(3))); }
void CPU::AND_35() { AND(memory.readByte(zero_page_x(4))); }
void CPU::AND_21() { AND(memory.readByte(pre_indexed_indirect(6))); }
void CPU::AND_31() {
	u16 before_page = 0xFF00 & regPC.value();
	AND(memory.readByte(post_indexed_indirect(5)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}
void CPU::AND_2D() { AND(memory.readByte(absolute(4))); }
void CPU::AND_3D() {
	u16 before_page = 0xFF00 & regPC.value();
	AND(memory.readByte(absolute_x(4)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}
void CPU::AND_39() {
	u16 before_page = 0xFF00 & regPC.value();
	AND(memory.readByte(absolute_y(4)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}

// EOR (EXLUSIVE OR OPERATION ON ACCUMULATOR)

void CPU::EOR(u8 byte) {
	regA.set(regA.value() ^ byte);
	set_flags_nz(regA.value());
}

void CPU::EOR_49() { EOR(get_byte_from_pc()); loop_cycles += 2; }
void CPU::EOR_45() { EOR(memory.readByte(zero_page(3))); }
void CPU::EOR_55() { EOR(memory.readByte(zero_page_x(4))); }
void CPU::EOR_41() { EOR(memory.readByte(pre_indexed_indirect(6))); }
void CPU::EOR_51() {
	u16 before_page = 0xFF00 & regPC.value();
	EOR(memory.readByte(post_indexed_indirect(5)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}
void CPU::EOR_4D() { EOR(memory.readByte(absolute(4))); }
void CPU::EOR_5D() {
	u16 before_page = 0xFF00 & regPC.value();
	EOR(memory.readByte(absolute_x(4)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}
void CPU::EOR_59() {
	u16 before_page = 0xFF00 & regPC.value();
	EOR(memory.readByte(absolute_y(4)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}

// ADC (Add M to A with carry)
void CPU::ADC(s8 byte) {
	s8 value = byte;
	u8 reg_value = regA.value();
	u8 result = regA.value() + regStatus.get_carry() + value;

	// Set flags
	regStatus.set_overflow((reg_value ^ result) & (value ^ result) & 0x80);
	unsigned int carry_check = reg_value + static_cast<u8>(value+regStatus.get_carry());
	regStatus.set_carry(carry_check > 0xFF);
	regA.set(result);
	set_flags_nz(regA.value());
}

void CPU::ADC_69() { ADC(get_byte_from_pc()); loop_cycles += 2; }
void CPU::ADC_65() { ADC(memory.readByte(zero_page(3))); }
void CPU::ADC_75() { ADC(memory.readByte(zero_page_x(4))); }
void CPU::ADC_61() { ADC(memory.readByte(pre_indexed_indirect(6))); }
void CPU::ADC_71() {
	u16 before_page = 0xFF00 & regPC.value();
	ADC(memory.readByte(post_indexed_indirect(5)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}
void CPU::ADC_6D() { ADC(memory.readByte(absolute(4))); }
void CPU::ADC_7D() {
	u16 before_page = 0xFF00 & regPC.value();
	ADC(memory.readByte(absolute_x(4)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}
void CPU::ADC_79() {
	u16 before_page = 0xFF00 & regPC.value();
	ADC(memory.readByte(absolute_y(4)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}

// SBC (Subtract M from A with borrow)

void CPU::SBC(s8 byte) {
	s8 value = byte ^ 0xFF;
	u8 reg_value = regA.value();
	u8 result = regA.value() + value + regStatus.get_carry();

	// Set flags
	regStatus.set_overflow((reg_value ^ result) & (value ^ result) & 0x80);
	unsigned int carry_check = u8(reg_value) + u8(value) + regStatus.get_carry();
	regStatus.set_carry(carry_check > 0xFF);
	regA.set(result);
	set_flags_nz(regA.value());
}

void CPU::SBC_E9() { SBC(get_byte_from_pc()); loop_cycles += 2; }
void CPU::SBC_E5() { SBC(memory.readByte(zero_page(3))); }
void CPU::SBC_F5() { SBC(memory.readByte(zero_page_x(4))); }
void CPU::SBC_E1() { SBC(memory.readByte(pre_indexed_indirect(6))); }
void CPU::SBC_F1() {
	u16 before_page = 0xFF00 & regPC.value();
	SBC(memory.readByte(post_indexed_indirect(5)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}
void CPU::SBC_ED() { SBC(memory.readByte(absolute(4))); }
void CPU::SBC_FD() {
	u16 before_page = 0xFF00 & regPC.value();
	SBC(memory.readByte(absolute_x(4)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}
void CPU::SBC_F9() {
	u16 before_page = 0xFF00 & regPC.value();
	SBC(memory.readByte(absolute_y(4)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}

// CMP (COMPARE M TO A)

void CPU::CMP(u8 byte) {
	u8 result = regA.value() - byte;
	regStatus.set_carry(regA.value() >= byte);
	set_flags_nz(result);
}

void CPU::CMP_C9() { CMP(get_byte_from_pc()); loop_cycles += 2; }
void CPU::CMP_C5() { CMP(memory.readByte(zero_page(3))); }
void CPU::CMP_D5() { CMP(memory.readByte(zero_page_x(4))); }
void CPU::CMP_C1() { CMP(memory.readByte(pre_indexed_indirect(6))); }
void CPU::CMP_D1() {
	u16 before_page = 0xFF00 & regPC.value();
	CMP(memory.readByte(post_indexed_indirect(5)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}
void CPU::CMP_CD() { CMP(memory.readByte(absolute(4))); }
void CPU::CMP_DD() {
	u16 before_page = 0xFF00 & regPC.value();
	CMP(memory.readByte(absolute_x(4)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}
void CPU::CMP_D9() {
	u16 before_page = 0xFF00 & regPC.value();
	CMP(memory.readByte(absolute_y(4)));
	if (before_page != (0xFF00 & regPC.value())) loop_cycles += 1;
}

// CPX (COMPARE X WITH M)

void CPU::CPX(u8 byte) {
	u8 result = regX.value() - byte;
	regStatus.set_carry(regX.value() >= byte);
	set_flags_nz(result);
}

void CPU::CPX_E0() { CPX(get_byte_from_pc()); loop_cycles += 2; }
void CPU::CPX_E4() { CPX(memory.readByte(zero_page(3))); }
void CPU::CPX_EC() { CPX(memory.readByte(absolute(4))); }

// CPY (COMPARE M WITH Y)

void CPU::CPY(u8 byte) {
	u8 result = regY.value() - byte;
	regStatus.set_carry(regY.value() >= byte);
	set_flags_nz(result);
}

void CPU::CPY_C0() { CPY(get_byte_from_pc()); loop_cycles += 2; }
void CPU::CPY_C4() { CPY(memory.readByte(zero_page(3))); }
void CPU::CPY_CC() { CPY(memory.readByte(absolute(4))); }

// DEC (DECREMENT VALUE IN SYSTEM MEMORY)

void CPU::DEC(u16 memory_address) {
	u8 result = memory.readByte(memory_address) - 1;
	memory.writeByte(result, memory_address);
	set_flags_nz(result);
}

void CPU::DEC_C6() { DEC(zero_page(5)); }
void CPU::DEC_D6() { DEC(zero_page_x(6)); }
void CPU::DEC_CE() { DEC(absolute(6)); }
void CPU::DEC_DE() { DEC(absolute_x(7)); }

// DEX (DECREMENT X REGISTER)
void CPU::DEX_CA() {	// Implied
	regX.decrement();
	set_flags_nz(regX.value());
	loop_cycles += 2;
}

// DEY (DECREMENT Y REGISTER)
void CPU::DEY_88() {	// Implied
	regY.decrement();
	set_flags_nz(regY.value());
	loop_cycles += 2;
}

// INC (INCREMENT MEMORY ADDRESS)
void CPU::INC(u16 memory_address) {
	u8 result = memory.readByte(memory_address) + 1;
	memory.writeByte(result, memory_address);
	set_flags_nz(result);
}

void CPU::INC_E6() { INC(zero_page(5)); }
void CPU::INC_F6() { INC(zero_page_x(6)); }
void CPU::INC_EE() { INC(absolute(6)); }
void CPU::INC_FE() { INC(absolute_x(7)); }

// INX (INCREMENT X REGISTER)
void CPU::INX_E8() {	// Implied
	regX.increment();
	set_flags_nz(regX.value());
	loop_cycles += 2;
}

// INY (INCREMENT Y REGISTER)
void CPU::INY_C8() {	// Implied
	regY.increment();
	set_flags_nz(regY.value());
	loop_cycles += 2;
}

// ASL (Shift M or A left one bit)
void CPU::ASL_0A() {	// Implied (shift the accumulator)
	// Set the carry flag if the 7th bit is set
	// 10000000 << 1 = 00000000 (that last one is carried)
	if (regA.value() & 0x80) regStatus.set_carry(true);
	else regStatus.set_carry(false);

	// Shift the accumulator left by one
	regA.set(regA.value() << 1);

	set_flags_nz(regA.value());

	loop_cycles += 2;
}

void CPU::ASL(u16 memory_address) {
	u8 result = memory.readByte(memory_address);
	regStatus.set_carry(result & 0x80);
	result <<= 1;
	set_flags_nz(result);
	memory.writeByte(result, memory_address);
}

void CPU::ASL_06() { ASL(zero_page(5)); }
void CPU::ASL_16() { ASL(zero_page_x(6)); }
void CPU::ASL_0E() { ASL(absolute(6)); }
void CPU::ASL_1E() { ASL(absolute_x(7)); }

// ROL (ROTATE A OR M LEFT)
void CPU::ROL_2A() {	// Implied (accumulator)
	bool newCarry = regA.value() & 0x80;

	// Rotate bits left (multiply them by 2 and add carry)
	u8 result = (regA.value() * 2) + regStatus.get_carry();
	regA.set(result);

	regStatus.set_carry(newCarry);
	set_flags_nz(result);
	loop_cycles += 2;
}

void CPU::ROL(u16 memory_address) {
	u8 result = memory.readByte(memory_address);
	regStatus.set_carry(result & 0x80);
	result = (result * 2) + regStatus.get_carry();
	set_flags_nz(result);
	memory.writeByte(result, memory_address);
}

void CPU::ROL_26() { ROL(zero_page(5)); }
void CPU::ROL_36() { ROL(zero_page_x(6)); }
void CPU::ROL_2E() { ROL(absolute(6)); }
void CPU::ROL_3E() { ROL(absolute_x(7)); }

// LSR (SHIFT A OR M RIGHT BY 1)
void CPU::LSR_4A() {	// Implied (accumulator)
	u8 result = regA.value();
	if (result & 0x1) regStatus.set_carry(true);
	else regStatus.set_carry(false);

	regA.set(result >> 1);

	set_flags_nz(regA.value());
	loop_cycles += 2;
}

void CPU::LSR(u16 memory_address) {
	u8 result = memory.readByte(memory_address);
	regStatus.set_carry(result & 0x1);
	result >>= 1;
	set_flags_nz(result);
	memory.writeByte(result, memory_address);
}

void CPU::LSR_46() { LSR(zero_page(5)); }
void CPU::LSR_56() { LSR(zero_page_x(6)); }
void CPU::LSR_4E() { LSR(absolute(6)); }
void CPU::LSR_5E() { LSR(absolute_x(7)); }

// ROR (ROTATE M OR A RIGHT)
void CPU::ROR_6A() {	// Implied (accumulator)
	u8 result = regA.value();
	bool newCarry = result & 0x1;

	result = (result / 2) + (regStatus.get_carry() * 128);

	regA.set(result);

	regStatus.set_carry(newCarry);
	set_flags_nz(regA.value());
	loop_cycles += 2;
}

void CPU::ROR(u16 memory_address) {
	u8 result = memory.readByte(memory_address);
	bool newCarry = result & 0x1;
	result = (result/2) + (regStatus.get_carry() * 128);
	set_flags_nz(result);
	regStatus.set_carry(newCarry);
	memory.writeByte(result, memory_address);
}

void CPU::ROR_66() { ROR(zero_page(5)); }
void CPU::ROR_76() { ROR(zero_page_x(6)); }
void CPU::ROR_6E() { ROR(absolute(6)); }
void CPU::ROR_7E() { ROR(absolute_x(7)); }