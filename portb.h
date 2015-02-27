#ifndef PORTB_H
#define PORTB_H
#include <avr/io.h>
struct portb
{
  static uint8_t port() { return PORTB; }
  static void port(uint8_t value) { PORTB = value; }
  static uint8_t pin() { return PINB; }
  static void pin(uint8_t value) { PINB = value; }
  static uint8_t dir() { return DDRB; }
  static void dir(uint8_t value) { DDRB = value; }
};
#endif