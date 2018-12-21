#include "cpu.h"

CPU::CPU(Memory& memory) : memory(memory) {
	cpu_running = true;

	printf("\n+----------------+\n");
	printf("|STARTING NES CPU|\n");
	printf("+----------------+\n\n");

	// Reset all registers
	regA.reset();
	regX.reset();
	regY.reset();
	regStatus.reset();
	regSP.reset();

	regStatus.set(0x24);	// Default flag register value (according to nestest rom)
	regSP.set(0xFD);		// Default SP register value (according to nestest rom)

	u16 reset_vector = 0xC000;	// For NES test ROM
	regPC.set(reset_vector);

	// Don't really know how to test this nes CPU so I'm just going to compare
	// the output with the log of a working emulator
	
	// This isn't a full while loop at the moment, I'm just trying to compare
	// lines of code from my emulator's log to the log of a well-known working
	// emulator to debug the CPU before going any further.
	for (int i = 0; i < 3200; i++) {
		if (cpu_cycles > 341) cpu_cycles -= 341;
		u16 current_pc = regPC.value();
		u8 opcode = get_byte_from_pc();

		// Display debugging information
		printf("%04X\t%02X\t%s\t\t\t", current_pc, opcode, opcode_names[opcode].c_str());
		printf("A:%02X X:%02X Y:%02X P:%02X SP:%02X CYCLES:%lu\n", regA.value(), regX.value(), regY.value(), regStatus.value(), regSP.value(), cpu_cycles);
		
		// Execute the opcode (switch case)
		execute_opcode(opcode);
		// Multiply loop_cycles by three because 1 ppu cycle is equal to
		// 3 cpu cycles
		cpu_cycles += loop_cycles*3;
		// Reset loop cycles back to zero so that it does not interfere with the
		// next executed opcode
		loop_cycles = 0;
	}
}

CPU::~CPU() {
	
}

u8 CPU::get_byte_from_pc() {
	u8 result = memory.readByte(regPC.value());
	regPC.increment();
	return result;
}

r8 CPU::get_signed_byte_from_pc() {
	r8 result = memory.readByte(regPC.value());
	regPC.increment();
	return result;
}

u16 CPU::get_word_from_pc() {
	u8 lower = get_byte_from_pc();
	u8 upper = get_byte_from_pc();
	return bitwise::combine_bytes(lower, upper);
}

void CPU::set_flags_nz(u8 value) {
	if (value == 0) regStatus.set_zero(true);
	else regStatus.set_zero(false);

	if ((value & 0x80) == 0x80) regStatus.set_negative(true);
	else regStatus.set_negative(false);
}

// Stack operations
u8 CPU::stack_pop() {
	// Stack must be incremented after the value is returned
	u8 byte = memory.readByte(0x101 + regSP.value());
	regSP.increment();
	return byte;
}

void CPU::stack_push(u8 byte) {
	// Stack must be decremented before the value is pushed
	regSP.decrement();
	memory.writeByte(byte, 0x101 + regSP.value());
}

void CPU::execute_opcode(u8 opcode) {
	switch (opcode) {
		// ORA
		case 0x09: ORA_09(); break;
		case 0x05: ORA_05(); break;
		case 0x15: ORA_15(); break;
		case 0x01: ORA_01(); break;
		case 0x11: ORA_11(); break;
		case 0x0D: ORA_0D(); break;
		case 0x1D: ORA_1D(); break;
		case 0x19: ORA_19(); break;

		// AND
		case 0x29: AND_29(); break;
		case 0x25: AND_25(); break;
		case 0x35: AND_35(); break;
		case 0x21: AND_21(); break;
		case 0x31: AND_31(); break;
		case 0x2D: AND_2D(); break;
		case 0x3D: AND_3D(); break;
		case 0x39: AND_39(); break;

		// EXCLUSIVE OR
		case 0x49: EOR_49(); break;
		case 0x45: EOR_45(); break;
		case 0x55: EOR_55(); break;
		case 0x41: EOR_41(); break;
		case 0x51: EOR_51(); break;
		case 0x4D: EOR_4D(); break;
		case 0x5D: EOR_5D(); break;
		case 0x59: EOR_59(); break;

		// ADD M AND CARRY TO ACCUMULATOR
		case 0x69: ADC_69(); break;
		case 0x65: ADC_65(); break;
		case 0x75: ADC_75(); break;
		case 0x61: ADC_61(); break;
		case 0x71: ADC_71(); break;
		case 0x6D: ADC_6D(); break;
		case 0x7D: ADC_7D(); break;
		case 0x79: ADC_79(); break;

		// SBC
		case 0xE9: SBC_E9(); break;
		case 0xE5: SBC_E5(); break;
		case 0xF5: SBC_F5(); break;
		case 0xE1: SBC_E1(); break;
		case 0xF1: SBC_F1(); break;
		case 0xED: SBC_ED(); break;
		case 0xFD: SBC_FD(); break;
		case 0xF9: SBC_F9(); break;

		// CMP
		case 0xC9: CMP_C9(); break;
		case 0xC5: CMP_C5(); break;
		case 0xD5: CMP_D5(); break;
		case 0xC1: CMP_C1(); break;
		case 0xD1: CMP_D1(); break;
		case 0xCD: CMP_CD(); break;
		case 0xDD: CMP_DD(); break;
		case 0xD9: CMP_D9(); break;

		// CPX
		case 0xE0: CPX_E0(); break;
		case 0xE4: CPX_E4(); break;
		case 0xEC: CPX_EC(); break;

		// CPY
		case 0xC0: CPY_C0(); break;
		case 0xC4: CPY_C4(); break;
		case 0xCC: CPY_CC(); break;

		// DEC
		case 0xC6: DEC_C6(); break;
		case 0xD6: DEC_D6(); break;
		case 0xCE: DEC_CE(); break;
		case 0xDE: DEC_DE(); break;

		// DEX
		case 0xCA: DEX_CA(); break;

		// DEY
		case 0x88: DEY_88(); break;

		// INC
		case 0xE6: INC_E6(); break;
		case 0xF6: INC_F6(); break;
		case 0xEE: INC_EE(); break;
		case 0xFE: INC_FE(); break;

		// INX
		case 0xE8: INX_E8(); break;

		// INY
		case 0xC8: INY_C8(); break;

		// ASL
		case 0x0A: ASL_0A(); break;
		case 0x06: ASL_06(); break;
		case 0x16: ASL_16(); break;
		case 0x0E: ASL_0E(); break;
		case 0x1E: ASL_1E(); break;

		// ROL
		case 0x2A: ROL_2A(); break;
		case 0x26: ROL_26(); break;
		case 0x36: ROL_36(); break;
		case 0x2E: ROL_2E(); break;
		case 0x3E: ROL_3E(); break;

		// LSR
		case 0x4A: LSR_4A(); break;
		case 0x46: LSR_46(); break;
		case 0x56: LSR_56(); break;
		case 0x4E: LSR_4E(); break;
		case 0x5E: LSR_5E(); break;

		// ROR
		case 0x6A: ROR_6A(); break;
		case 0x66: ROR_66(); break;
		case 0x76: ROR_76(); break;
		case 0x6E: ROR_6E(); break;
		case 0x7E: ROR_7E(); break;

		// LDA
		case 0xA9: LDA_A9(); break;
		case 0xA5: LDA_A5(); break;
		case 0xB5: LDA_B5(); break;
		case 0xA1: LDA_A1(); break;
		case 0xB1: LDA_B1(); break;
		case 0xAD: LDA_AD(); break;
		case 0xBD: LDA_BD(); break;
		case 0xB9: LDA_B9(); break;

		// STA
		case 0x85: STA_85(); break;
		case 0x95: STA_95(); break;
		case 0x81: STA_81(); break;
		case 0x91: STA_91(); break;
		case 0x8D: STA_8D(); break;
		case 0x9D: STA_9D(); break;
		case 0x99: STA_99(); break;

		// LDX
		case 0xA2: LDX_A2(); break;
		case 0xA6: LDX_A6(); break;
		case 0xB6: LDX_B6(); break;
		case 0xAE: LDX_AE(); break;
		case 0xBE: LDX_BE(); break;

		// STX
		case 0x86: STX_86(); break;
		case 0x96: STX_96(); break;
		case 0x8E: STX_8E(); break;

		// LDY
		case 0xA0: LDY_A0(); break;
		case 0xA4: LDY_A4(); break;
		case 0xB4: LDY_B4(); break;
		case 0xAC: LDY_AC(); break;
		case 0xBC: LDY_BC(); break;

		// STY
		case 0x84: STY_84(); break;
		case 0x94: STY_94(); break;
		case 0x8C: STY_8C(); break;

		// STACK OPERATIONS AND REGISTER TRANSFERS
		case 0xAA: TAX_AA(); break;
		case 0x8A: TXA_8A(); break;
		case 0xA8: TAY_A8(); break;
		case 0x98: TYA_98(); break;
		case 0xBA: TSX_BA(); break;
		case 0x9A: TXS_9A(); break;
		case 0x68: PLA_68(); break;
		case 0x48: PHA_48(); break;
		case 0x28: PLP_28(); break;
		case 0x08: PHP_08(); break;

		// JUMP/FLAGS
		case 0x10: BPL_10(); break;
		case 0x30: BMI_30(); break;
		case 0x50: BVC_50(); break;
		case 0x70: BVS_70(); break;
		case 0x90: BCC_90(); break;
		case 0xB0: BCS_B0(); break;
		case 0xD0: BNE_D0(); break;
		case 0xF0: BEQ_F0(); break;
		case 0x00: BRK_00(); break;
		case 0x40: RTI_40(); break;
		case 0x20: JSR_20(); break;
		case 0x60: RTS_60(); break;
		case 0x4C: JMP_4C(); break;
		case 0x6C: JMP_6C(); break;
		case 0x24: BIT_24(); break;
		case 0x2C: BIT_2C(); break;
		case 0x18: CLC_18(); break;
		case 0x38: SEC_38(); break;
		case 0xD8: CLD_D8(); break;
		case 0xF8: SED_F8(); break;
		case 0x58: CLI_58(); break;
		case 0x78: SEI_78(); break;
		case 0xB8: CLV_B8(); break;
		case 0xEA: NOP_EA(); break;
	}
}