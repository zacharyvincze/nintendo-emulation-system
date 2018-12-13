#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>
#include "definitions.h"
#include "bitwise.h"

class ByteRegister {
public:
	u8 value();
	void set(uint8_t value);
	void increment();
	void decrement();
	void reset();
protected:
	u8 reg_value;
};

class WordRegister {
public:
	u16 value();
	void set(uint16_t value);
	void increment();
	void decrement();
private:
	u16 reg_value;
};

class StatusRegister : public ByteRegister {
public:
	void set_carry(bool on) { reg_value = bitwise::set_bit_to(reg_value, 0, on); }
	void set_zero(bool on) { reg_value = bitwise::set_bit_to(reg_value, 1, on); }
	void set_interrupt_disable(bool on) { reg_value = bitwise::set_bit_to(reg_value, 2, on); }
	void set_decimal_mode(bool on) { reg_value = bitwise::set_bit_to(reg_value, 3, on); }
	void set_break_command(bool on) { reg_value = bitwise::set_bit_to(reg_value, 4, on); }
	void set_overflow(bool on) { reg_value = bitwise::set_bit_to(reg_value, 6, on); }
	void set_negative(bool on) { reg_value = bitwise::set_bit_to(reg_value, 7, on); }

	bool get_carry() { return bitwise::check_bit(reg_value, 0); }
	bool get_zero() { return bitwise::check_bit(reg_value, 1); }
	bool get_interrupt_disable() { return bitwise::check_bit(reg_value, 2); }
	bool get_decimal_mode() { return bitwise::check_bit(reg_value, 3); }
	bool get_break_command() { return bitwise::check_bit(reg_value, 4); }
	bool get_overflow() { return bitwise::check_bit(reg_value, 6); }
	bool get_negative() { return bitwise::check_bit(reg_value, 7); }
};

#endif // REGISTER_H