#ifndef PORTD_H
#define PORTD_H
#include <avr/io.h>
struct portd
{
  static uint8_t port() { return PORTD; }
  static void port(uint8_t value) { PORTD = value; }
  static uint8_t pin() { return PIND; }
  static void pin(uint8_t value) { PIND = value; }
  static uint8_t dir() { return DDRD; }
  static void dir(uint8_t value) { DDRD = value; }
};
#endif