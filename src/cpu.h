#ifndef CPU_H
#define CPU_H

#include <thread>
#include <chrono>

#include "definitions.h"
#include "register.h"
#include "memory.h"

namespace {
	const long CPU_CLOCK_SPEED_HZ =	1789773;
	const long PPU_CLOCK_SPEED_HZ =	CPU_CLOCK_SPEED_HZ * 3;
	const long APU_CLOCK_SPEED_HZ =	1789773;
 }

class CPU {
public:

	CPU(Memory& memory);
	~CPU();

	/*
	NES 6502 has a clock that runs at about 1.79MHz (1789773Hz)
	*/
	void tick();	// Emulates a single opcode execution

	/*
	Three general purpose 8-bit registers: A, X, and Y, with A being the accumulator
	one stack pointer register which is 8 bits long
	a status register which acts the same as any other 8-bit register
	and a program counter register that is 16-bits to keep track of the current
	execution address.
	*/

	ByteRegister regA, regX, regY;
	ByteRegister regSP;
	StatusRegister regStatus;
	WordRegister regPC;

	// The CPU has full access to the system's memory
	Memory& memory;

	u8 get_byte_from_pc();
	r8 get_signed_byte_from_pc();
	u16 get_word_from_pc();

private:
	// Timers and loop breaks
	unsigned long cpu_cycles;
	unsigned int loop_cycles;

	bool cpu_running;

	void execute_opcode(u8 opcode);

	// Stack operations
	u8 stack_pop();
	void stack_push(u8 byte);

	// Addressing modes
	u8 zero_page(unsigned int cycles);
	u8 zero_page_x(unsigned int cycles);
	u8 zero_page_y(unsigned int cycles);
	u16 pre_indexed_indirect(unsigned int cycles);
	u16 post_indexed_indirect(unsigned int cycles);
	u16 absolute(unsigned int cycles);
	u16 absolute_x(unsigned int cycles);
	u16 absolute_y(unsigned int cycles);

	// Utilities
	void set_flags_nz(u8 value);

	// General opcode commands

	// GENERAL LOGICAL AND ARITHMETIC COMMANDS
	void ORA(u8 byte);
	void AND(u8 byte);
	void EOR(u8 byte);
	void ADC(s8 byte);
	void SBC(s8 byte);
	void CMP(u8 byte);
	void CPX(u8 byte);
	void CPY(u8 byte);
	void DEC(u16 memory_address);
	void INC(u16 memory_address);
	void ASL(u16 memory_address);
	void ROL(u16 memory_address);
	void LSR(u16 memory_address);
	void ROR(u16 memory_address);

	// GENERAL MOVE COMMANDS
	void LDA(u8 byte);
	void STA(u16 memory_address);
	void LDX(u8 byte);
	void STX(u16 memory_address);
	void LDY(u8 byte);
	void STY(u16 memory_address);

	// BRANCH COMMANDS
	void branch(bool condition);

	// Note: Any missing general opcodes don't have enough unique addressing
	// modes to constitute a general function declaration.

	// LOGICAL AND ARITHMETIC COMMANDS
	void ORA_09();
	void ORA_05();
	void ORA_15();
	void ORA_01();
	void ORA_11();
	void ORA_0D();
	void ORA_1D();
	void ORA_19();

	void AND_29();
	void AND_25();
	void AND_35();
	void AND_21();
	void AND_31();
	void AND_2D();
	void AND_3D();
	void AND_39();

	void EOR_49();
	void EOR_45();
	void EOR_55();
	void EOR_41();
	void EOR_51();
	void EOR_4D();
	void EOR_5D();
	void EOR_59();

	void ADC_69();
	void ADC_65();
	void ADC_75();
	void ADC_61();
	void ADC_71();
	void ADC_6D();
	void ADC_7D();
	void ADC_79();

	void SBC_E9();
	void SBC_E5();
	void SBC_F5();
	void SBC_E1();
	void SBC_F1();
	void SBC_ED();
	void SBC_FD();
	void SBC_F9();

	void CMP_C9();
	void CMP_C5();
	void CMP_D5();
	void CMP_C1();
	void CMP_D1();
	void CMP_CD();
	void CMP_DD();
	void CMP_D9();

	void CPX_E0();
	void CPX_E4();
	void CPX_EC();

	void CPY_C0();
	void CPY_C4();
	void CPY_CC();

	void DEC_C6();
	void DEC_D6();
	void DEC_CE();
	void DEC_DE();

	void DEX_CA();
	
	void DEY_88();

	void INC_E6();
	void INC_F6();
	void INC_EE();
	void INC_FE();

	void INX_E8();

	void INY_C8();

	void ASL_0A();
	void ASL_06();
	void ASL_16();
	void ASL_0E();
	void ASL_1E();

	void ROL_2A();
	void ROL_26();
	void ROL_36();
	void ROL_2E();
	void ROL_3E();

	void LSR_4A();
	void LSR_46();
	void LSR_56();
	void LSR_4E();
	void LSR_5E();

	void ROR_6A();
	void ROR_66();
	void ROR_76();
	void ROR_6E();
	void ROR_7E();

	// MOVE COMMANDS
	void LDA_A9();
	void LDA_A5();
	void LDA_B5();
	void LDA_A1();
	void LDA_B1();
	void LDA_AD();
	void LDA_BD();
	void LDA_B9();

	void STA_85();
	void STA_95();
	void STA_81();
	void STA_91();
	void STA_8D();
	void STA_9D();
	void STA_99();

	void LDX_A2();
	void LDX_A6();
	void LDX_B6();
	void LDX_AE();
	void LDX_BE();

	void STX_86();
	void STX_96();
	void STX_8E();

	void LDY_A0();
	void LDY_A4();
	void LDY_B4();
	void LDY_AC();
	void LDY_BC();

	void STY_84();
	void STY_94();
	void STY_8C();

	void TAX_AA();

	void TXA_8A();

	void TAY_A8();

	void TYA_98();

	void TSX_BA();

	void TXS_9A();

	void PLA_68();

	void PHA_48();

	void PLP_28(); 

	void PHP_08();

	// JUMP/FLAG COMMANDS
	void BPL_10();
	void BMI_30();
	void BVC_50();
	void BVS_70();
	void BCC_90();
	void BCS_B0();
	void BNE_D0();
	void BEQ_F0();
	void BRK_00();
	void RTI_40();
	void JSR_20();
	void RTS_60();
	void JMP_4C();
	void JMP_6C();
	void BIT_24();
	void BIT_2C();
	void CLC_18();
	void SEC_38();
	void CLD_D8();
	void SED_F8();
	void CLI_58();
	void SEI_78();
	void CLV_B8();
	void NOP_EA();	// Always found NOP opcodes kinda weird
	              	// seems like they're just there to use up
	              	// cycles for some reason.
};

#endif // CPU_H