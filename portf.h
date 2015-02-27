#ifndef PORTF_H
#define PORTF_H
#include <avr/io.h>
struct portf
{
  static uint8_t port() { return PORTF; }
  static void port(uint8_t value) { PORTF = value; }
  static uint8_t pin() { return PINF; }
  static void pin(uint8_t value) { PINF = value; }
  static uint8_t dir() { return DDRF; }
  static void dir(uint8_t value) { DDRF = value; }
};
#endif