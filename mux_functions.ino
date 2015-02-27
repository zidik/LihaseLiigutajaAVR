/*
* Initialize all 5 MUXes
*/
void initialize_MUXes(){
  mux_eap_out::initialize();
  mux_flyback_out::initialize();
  mux_gnd_1::initialize();
  mux_gnd_2::initialize();
  mux_flyback_in::initialize();
}

/*
* Configure flyback input MUX to either external (3v3 source) or internal (another EAP) source.
*/
void set_flyback_input(const bool external, const bool resistor){
  mux_flyback_in::disable();
  mux_flyback_in::set_channel((external ? 2 : 0) + (resistor ? 0 : 1));
  mux_flyback_in::enable();
}

/*
* This function returns the MUX channel number if given output pin on pinheader.
* MUX channels are routed to pinheader in this order: 4, 2, 3, 1
*/
uint8_t map_output_pin_mux_channel(uint8_t output_pin){
  uint8_t mux_channel;
  switch (output_pin)
  {
  case 0:
    mux_channel = 4;
    break;
  case 1:
    mux_channel = 2;
    break;
  case 2:
    mux_channel = 3;
    break;
  case 3:
    mux_channel = 1;
    break;
  default:
    mux_channel = 0xFF;//error
  }
  return mux_channel;
}
