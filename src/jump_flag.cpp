#ifndef JUMP_FLAG_CPP
#define JUMP_FLAG_CPP

#include "cpu.h"

// BRANCHES

// Branch cycles, default to 2 cycles, add 1 cycles if the branch
// succeeds, add another 2 cycles if a page is crossed during the branch
void CPU::branch(bool condition) {
	u16 before_page = 0xFF00 & regPC.value();
	// Default to 2 cycles
	loop_cycles += 2;
	
	if (condition) {
		r8 byte = get_signed_byte_from_pc();
		regPC.set(regPC.value() + byte);
		// Add one cycle since the branch was successful
		loop_cycles += 1;

		// Check if page is crossed, if so, add 2 more cycles
		if (before_page != (0xFF00 & regPC.value()))
			loop_cycles += 2;
	} else {
		regPC.increment();
		regPC.increment();
	}
}

void CPU::BPL_10() { branch(!regStatus.get_negative()); }
void CPU::BMI_30() { branch(regStatus.get_negative()); }
void CPU::BVC_50() { branch(!regStatus.get_overflow()); }
void CPU::BVS_70() { branch(regStatus.get_overflow()); }
void CPU::BCC_90() { branch(!regStatus.get_carry()); }
void CPU::BCS_B0() { branch(regStatus.get_carry()); }
void CPU::BNE_D0() { branch(!regStatus.get_zero()); }
void CPU::BEQ_F0() { branch(regStatus.get_zero()); }

void CPU::BRK_00() {
	// TODO: Implement this BRK opcode
	printf("ERROR: Unimplemented opcode: BRK\n");
	exit(EXIT_FAILURE);
	// BRK opcode takes 7 CPU cycles to complete
	loop_cycles += 7;
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

	// This opcode takes 6 CPU cycles to complete
	loop_cycles += 6;
}

void CPU::JSR_20() {	// Absolute
	// Push program counter to stack and change pc to absolute value
	u16 address = absolute(6);
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
	regPC.set(absolute(3));
}

void CPU::JMP_6C() {	// Indirect (specific to this opcode apparently)
	// Get absolute address to get pointer location
	u16 address_pointer = get_word_from_pc();
	// Read address from return_address to return_address+1
	u8 lower_byte = memory.readByte(address_pointer);
	u8 upper_byte = memory.readByte(address_pointer+1);
	u16 return_address = (upper_byte << 8) | lower_byte;
	regPC.set(return_address);
	// Indirect addressing mode (exclusive to this specific opcode
	// requires 5 CPU cycles)
	loop_cycles += 5;
}

void CPU::BIT_24() {	// Zero-page
	u8 byte = memory.readByte(zero_page(3));
	u8 result = regA.value() & byte;

	// Test zero using result
	regStatus.set_zero(result == 0);

	// Negative flag is equal to the value of byte (bit 7)
	regStatus.set_negative(bitwise::check_bit(byte, 7));

	// Overflow flag is equal to the 6th bit of byte
	regStatus.set_overflow(bitwise::check_bit(byte, 6));
}

void CPU::BIT_2C() {	// Absolute
	u8 byte = memory.readByte(absolute(4));
	u8 result = regA.value() & byte;

	regStatus.set_zero(result == 0);
	regStatus.set_negative(bitwise::check_bit(byte, 7));
	regStatus.set_overflow(bitwise::check_bit(byte, 6));
}

// CLC (CLEARS CARRY FLAG)
void CPU::CLC_18() {	// Implied
	regStatus.set_carry(false);
	loop_cycles += 2;
}

// SEC (SETS CARRY FLAG)
void CPU::SEC_38() {	// Implied
	regStatus.set_carry(true);
	loop_cycles += 2;
}

// CLD (CLEARS DECIMAL FLAG)
void CPU::CLD_D8() {	// Implied
	regStatus.set_decimal_mode(false);
	loop_cycles += 2;
}

// SED (SETS DECIMAL FLAG)
void CPU::SED_F8() {	// Implied
	regStatus.set_decimal_mode(true);
	loop_cycles += 2;
}

// CLI (CLEARS INTERRUPT DISABLE FLAG)
void CPU::CLI_58() {	// Implied
	regStatus.set_interrupt_disable(false);
	loop_cycles += 2;
}

// SEI (SETS INTERRUPT DISABLE FLAG)
void CPU::SEI_78() {	// Implied
	regStatus.set_interrupt_disable(true);
	loop_cycles += 2;
}

// CLV (CLEARS OVERFLOW FLAG)
void CPU::CLV_B8() {	// Implied
	regStatus.set_overflow(false);
	loop_cycles += 2;
}

// NOP (NO OPERATION)
void CPU::NOP_EA() {	// Implied
	// Just taking up CPU cycles I guess
	loop_cycles += 2;
}

#endif // JUMP_FLAG_CPP