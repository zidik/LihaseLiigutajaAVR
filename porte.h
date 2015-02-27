#ifndef PORTE_H
#define PORTE_H
#include <avr/io.h>
struct porte
{
  static uint8_t port() { return PORTE; }
  static void port(uint8_t value) { PORTE = value; }
  static uint8_t pin() { return PINE; }
  static void pin(uint8_t value) { PINE = value; }
  static uint8_t dir() { return DDRE; }
  static void dir(uint8_t value) { DDRE = value; }
};
#endif