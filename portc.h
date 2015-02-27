#ifndef PORTC_H
#define PORTC_H
#include <avr/io.h>
struct portc
{
  static uint8_t port() { return PORTC; }
  static void port(uint8_t value) { PORTC = value; }
  static uint8_t pin() { return PINC; }
  static void pin(uint8_t value) { PINC = value; }
  static uint8_t dir() { return DDRC; }
  static void dir(uint8_t value) { DDRC = value; }
};
#endif