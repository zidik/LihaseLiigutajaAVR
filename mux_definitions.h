// mux_definitions.h

#ifndef _MUX_DEFINITIONS_h
#define _MUX_DEFINITIONS_h
#include "pin.h"
#include "portb.h"
#include "portc.h"
#include "portd.h"
#include "porte.h"
#include "portf.h"
#include "ADG804.h"

//Pins
typedef pin<portf, 6> pin_A1_4;
typedef pin<portf, 7> pin_A0_4;

typedef pin<porte, 6> pin_EN_4;

typedef pin<portc, 7> pin_A0_1;
typedef pin<portc, 6> pin_EN_1;

typedef pin<portd, 7> pin_A0_2;
typedef pin<portd, 6> pin_EN_2;
typedef pin<portd, 4> pin_A0_3;
typedef pin<portd, 3> pin_EN_3;
typedef pin<portd, 2> pin_A1_3;
typedef pin<portd, 1> pin_A1_2;
typedef pin<portd, 0> pin_A1_0;

typedef pin<portb, 7> pin_A1_1;
typedef pin<portb, 6> pin_A0_0;
typedef pin<portb, 4> pin_EN_0;

//Muxes
typedef ADG804<pin_EN_0, pin_A0_0, pin_A1_0> mux_eap_out;
typedef ADG804<pin_EN_1, pin_A0_1, pin_A1_1> mux_flyback_out;
typedef ADG804<pin_EN_2, pin_A0_2, pin_A1_2> mux_gnd_1;
typedef ADG804<pin_EN_3, pin_A0_3, pin_A1_3> mux_gnd_2;
typedef ADG804<pin_EN_4, pin_A0_4, pin_A1_4> mux_flyback_in;

uint8_t map_output_pin_mux_channel(uint8_t output_pin);

/*
* Configure one mux to connect to specified output port (EAP)
* Should not be used on flyback input mux.
*/
template <typename mux>
void set_mux_output(uint8_t output_pin){
  if (output_pin >= 4)
    return;
  uint8_t channel = map_output_pin_mux_channel(output_pin);
  mux::disable();
  mux::set_channel(channel);
  mux::enable();
}
template <typename mux>
void enable_mux(const bool enable){
  mux::enable(enable);
}
template <typename mux>
void disable_mux(){
  mux::disable();
}

#endif

