#ifndef JUMP_FLAG_CPP
#define JUMP_FLAG_CPP

#include "cpu.h"

// BRANCHES TODO: Implement cycles for branch opcodes
void CPU::BPL_10() {
	if (regStatus.get_negative() == 0) {
		r8 byte = get_signed_byte_from_pc();
		regPC.set(regPC.value() + byte);
	} else {
		regPC.increment();
	}
}

void CPU::BMI_30() {
	if (regStatus.get_negative()) {
		r8 byte = get_signed_byte_from_pc();
		regPC.set(regPC.value() + byte);
	} else {
		regPC.increment();
	}
}

void CPU::BVC_50() {
	if (regStatus.get_overflow() == 0) {
		r8 byte = get_signed_byte_from_pc();
		regPC.set(regPC.value() + byte);
	} else {
		regPC.increment();
	}
}

void CPU::BVS_70() {
	if (regStatus.get_overflow()) {
		r8 byte = get_signed_byte_from_pc();
		regPC.set(regPC.value() + byte);
	} else {
		regPC.increment();
	}
}

void CPU::BCC_90() {
	if (regStatus.get_carry() == 0) {
		r8 byte = get_signed_byte_from_pc();
		regPC.set(regPC.value() + byte);
	} else {
		regPC.increment();
	}
}

void CPU::BCS_B0() {
	if (regStatus.get_carry()) {
		r8 byte = get_signed_byte_from_pc();
		regPC.set(regPC.value() + byte);
	} else {
		regPC.increment();
	}
}

void CPU::BNE_D0() {
	if (regStatus.get_zero() == 0) {
		r8 byte = get_signed_byte_from_pc();
		regPC.set(regPC.value() + byte);
		printf("Branching %i\n", byte);
	} else {
		printf("Not branching\n");
		regPC.increment();
	}
}

void CPU::BEQ_F0() {
	if (regStatus.get_zero()) {
		r8 byte = get_signed_byte_from_pc();
		regPC.set(regPC.value() + byte);
	} else {
		regPC.increment();
	}
}

void CPU::BRK_00() {
	// TODO: Implement this BRK opcode
	printf("ERROR: Unimplemented opcode: BRK\n");
	exit(EXIT_FAILURE);
}

void CPU::RTI_40() {	// Implied
	// Returns from an interrupt, sets flags based on value stored in stack pointer memory location
	u8 status_byte = stack_pop();

	// Set bit 5 to 1, it does not actually exist in the flag register but
	// must always be set to 1 when the flag register is being set.
	status_byte = bitwise::set_bit_to(status_byte, 5, true);
	regStatus.set(status_byte);

	// Finally, set the address to return to after the operation
	u8 lower_return_byte = stack_pop();
	u8 upper_return_byte = stack_pop();

	u16 return_address = (upper_return_byte << 8) | lower_return_byte;
	regPC.set(return_address);
}

void CPU::JSR_20() {	// Absolute
	// Push program counter to stack and change pc to absolute value
	u16 address = absolute();
	u8 lower_stack = (regPC.value() - 1) >> 8;	// upper byte
	u8 upper_stack = (regPC.value() - 1) & 0xFF;	// lower byte
	
	// Stack is decremented before being pushed
	regSP.decrement();
	memory.writeByte(lower_stack, 0x101+regSP.value());
	regSP.decrement();
	memory.writeByte(upper_stack, 0x101+regSP.value());

	regPC.set(address);	// Jump to the absolute address
}

void CPU::RTS_60() {	// Implied
	// Pulling from the stack requires incrementing after the pull was made
	u8 lower_byte = stack_pop();
	u8 upper_byte = stack_pop();

	u16 return_address = (upper_byte << 8) | lower_byte;
	regPC.set(return_address + 1);
}

void CPU::JMP_4C() {	// Absolute
	// Sets the pc value to whatever the proceding bytes are
	regPC.set(absolute());
}

void CPU::JMP_6C() {	// Indirect (specific to this opcode apparently)
	// Get absolute address to get pointer location
	u16 address_pointer = get_word_from_pc();
	// Read address from return_address to return_address+1
	u8 lower_byte = memory.readByte(address_pointer);
	u8 upper_byte = memory.readByte(address_pointer+1);
	u16 return_address = (upper_byte << 8) | lower_byte;
	regPC.set(return_address);
}

void CPU::BIT_24() {	// Zero-page
	u8 byte = memory.readByte(zero_page());
	u8 result = regA.value() & byte;

	// Test zero using result
	regStatus.set_zero(result == 0);

	// Negative flag is equal to the value of byte (bit 7)
	regStatus.set_negative(bitwise::check_bit(byte, 7));

	// Overflow flag is equal to the 6th bit of byte
	regStatus.set_overflow(bitwise::check_bit(byte, 6));
}

void CPU::BIT_2C() {	// Absolute
	u8 byte = memory.readByte(absolute());
	u8 result = regA.value() & byte;

	regStatus.set_zero(result == 0);
	regStatus.set_negative(bitwise::check_bit(byte, 7));
	regStatus.set_overflow(bitwise::check_bit(byte, 6));
}

// CLC (CLEARS CARRY FLAG)
void CPU::CLC_18() {	// Implied
	regStatus.set_carry(false);
}

// SEC (SETS CARRY FLAG)
void CPU::SEC_38() {	// Implied
	regStatus.set_carry(true);
}

// CLD (CLEARS DECIMAL FLAG)
void CPU::CLD_D8() {	// Implied
	regStatus.set_decimal_mode(false);
}

// SED (SETS DECIMAL FLAG)
void CPU::SED_F8() {	// Implied
	regStatus.set_decimal_mode(true);
}

// CLI (CLEARS INTERRUPT DISABLE FLAG)
void CPU::CLI_58() {	// Implied
	regStatus.set_interrupt_disable(false);
}

// SEI (SETS INTERRUPT DISABLE FLAG)
void CPU::SEI_78() {	// Implied
	regStatus.set_interrupt_disable(true);
}

// CLV (CLEARS OVERFLOW FLAG)
void CPU::CLV_B8() {	// Implied
	regStatus.set_overflow(false);
}

// NOP (NO OPERATION)
void CPU::NOP_EA() {	// Implied
	// Just taking up CPU cycles I guess
}

#endif // JUMP_FLAG_CPP