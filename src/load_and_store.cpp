#ifndef LOAD_AND_STORE_CPP
#define LOAD_AND_STORE_CPP

#include "cpu.h"

// Load and store opcodes

// LDA FUNCTIONS (MOVES VALUES INTO ACCUMULATOR)
void CPU::LDA(u8 byte) {
	regA.set(byte);
	set_flags_nz(regA.value());
}

void CPU::LDA_A9() { LDA(get_byte_from_pc()); }
void CPU::LDA_A5() { LDA(memory.readByte(zero_page())); }
void CPU::LDA_B5() { LDA(memory.readByte(zero_page_x())); }
void CPU::LDA_A1() { LDA(memory.readByte(pre_indexed_indirect())); }
void CPU::LDA_B1() { LDA(memory.readByte(post_indexed_indirect())); }
void CPU::LDA_AD() { LDA(memory.readByte(absolute())); }
void CPU::LDA_BD() { LDA(memory.readByte(absolute_x())); }
void CPU::LDA_B9() { LDA(memory.readByte(absolute_y())); }

// STA FUNCTIONS (MOVES ACCUMULATOR VALUE INTO MEMORY)
void CPU::STA(u16 memory_address) {
	memory.writeByte(regA.value(), memory_address);
}

void CPU::STA_85() { STA(zero_page()); }
void CPU::STA_95() { STA(zero_page_x()); }
void CPU::STA_81() { STA(pre_indexed_indirect()); }
void CPU::STA_91() { STA(post_indexed_indirect()); }
void CPU::STA_8D() { STA(absolute()); }
void CPU::STA_9D() { STA(absolute_x()); }
void CPU::STA_99() { STA(absolute_y()); }

// LDX (LOAD INTO X REGISTER)

void CPU::LDX(u8 byte) {
	regX.set(byte);
	set_flags_nz(regX.value());
}

void CPU::LDX_A2() { LDX(get_byte_from_pc()); }
void CPU::LDX_A6() { LDX(memory.readByte(zero_page())); }
void CPU::LDX_B6() { LDX(memory.readByte(zero_page_y())); }
void CPU::LDX_AE() { LDX(memory.readByte(absolute())); }
void CPU::LDX_BE() { LDX(memory.readByte(absolute_y())); }

// STX (MOVES X REGISTER INTO MEMORY)

void CPU::STX(u16 memory_address) {
	memory.writeByte(regX.value(), memory_address);
}

void CPU::STX_86() { STX(zero_page()); }
void CPU::STX_96() { STX(zero_page_y()); }
void CPU::STX_8E() { STX(absolute()); }

// LDY (LOAD INTO Y REGISTER)

void CPU::LDY(u8 byte) {
	regY.set(byte);
	set_flags_nz(regY.value());
}

void CPU::LDY_A0() { LDY(get_byte_from_pc()); }
void CPU::LDY_A4() { LDY(memory.readByte(zero_page())); }
void CPU::LDY_B4() { LDY(memory.readByte(zero_page_x())); }
void CPU::LDY_AC() { LDY(memory.readByte(absolute())); }
void CPU::LDY_BC() { LDY(memory.readByte(absolute_x())); }

// STY (MOVES Y REGISTER VALUE INTO MEMORY)

void CPU::STY(u16 memory_address) {
	memory.writeByte(regY.value(), memory_address);
}

void CPU::STY_84() { STY(zero_page()); }
void CPU::STY_94() { STY(zero_page_x()); }
void CPU::STY_8C() { STY(absolute()); }

// REGISTER TRANSFER OPERATIONS (THAT'S WHAT I'M CALLING IT ANYWAYS)
void CPU::TAX_AA() {	// Implied
	regX.set(regA.value());
	set_flags_nz(regX.value());
}

void CPU::TXA_8A() {	// Implied
	regA.set(regX.value());
	set_flags_nz(regA.value());
}

void CPU::TAY_A8() {	// Implied
	regY.set(regA.value());
	set_flags_nz(regY.value());
}

void CPU::TYA_98() {	// Implied
	regA.set(regY.value());
	set_flags_nz(regA.value());
}

// STACK OPERATIONS
void CPU::TSX_BA() {	// Implied
	regX.set(regSP.value());
	set_flags_nz(regX.value());
}

void CPU::TXS_9A() {	// Implied
	regSP.set(regX.value());
}

void CPU::PLA_68() {	// Implied
	// POP the stack into the A register
	regA.set(stack_pop());
	set_flags_nz(regA.value());
}

void CPU::PHA_48() {	// Implied
	// PUSH the value from the A register into the stack
	// decrement BEFORE the operation
	stack_push(regA.value());
}

void CPU::PLP_28() {	// Implied
    // POP the stack into the P register (ignore bits 5 and 4)
    bool bit5 = bitwise::check_bit(regStatus.value(), 5);
    bool bit4 = bitwise::check_bit(regStatus.value(), 4);
	regStatus.set(stack_pop());

	// Reset bits 5 and 4 because they should be ignored
	// TODO: Probably a much better way to get this done
	regStatus.set(bitwise::set_bit_to(regStatus.value(), 5, bit5));
	regStatus.set(bitwise::set_bit_to(regStatus.value(), 4, bit4));

	// Register flags should be set as soon as the value is loaded from memory
	// Nothing further needs to be done here, I think...
}

void CPU::PHP_08() {	// Implied
	// PUSH the stack from the P (status) register
	// decrement BEFORE the operation
	// According to NES dev wiki, when the P register
	// is pushed to the stack, bits 5 and 4 must be set to 1
	u8 result = regStatus.value() | 0b00110000;
	stack_push(result);

	// no flags need to be set!
}

#endif // LOAD_AND_STORE_CPP