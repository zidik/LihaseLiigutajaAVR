void init_ADC(){
  //Set pins as inputs
  DDRF &=
    ~(1 << DDF5) &
    ~(1 << DDF4) &
    ~(1 << DDF1) &
    ~(1 << DDF0);

  //Turn off pullups
  PORTF &=
    ~(1 << PORTF5) &
    ~(1 << PORTF4) &
    ~(1 << PORTF1) &
    ~(1 << PORTF0);

  //Disable digital inputs on the pins (Power reduction)
  DIDR0 |=
    (1 << ADC5D) |
    (1 << ADC4D) |
    (1 << ADC1D) |
    (1 << ADC0D);

  ADCSRA =
    (1 << ADEN) | //Enable ADC
    (0 << ADSC) | //Do not start conversion yet.
    (1 << ADATE) | //Enable Auto Trigger
    (1 << ADIF) | //Clear interrupt flag
    (1 << ADIE) | //Enable interrupt
    (1 << ADPS2) | //Prescaling: 64   8Mhz/64=125kHz
    (1 << ADPS1) |
    (0 << ADPS0);

  ADCSRB =
    (0 << ADHSM) | //Disable ADC High Speed Mode
    (0 << MUX5) |
    (0 << ADTS3) | //ADC Auto Trigger Source - free-running
    (0 << ADTS2) |
    (0 << ADTS1) |
    (0 << ADTS0);

  ADMUX =
    (0 << REFS1) |
    (1 << REFS0) | //AVCC with external capacitor on AREF pin
    (0 << ADLAR) | //Right-aligned
    (0 << MUX4) | //channel 0, single-ended
    (0 << MUX3) |
    (0 << MUX2) |
    (0 << MUX1) |
    (0 << MUX0);
  choose_next_ADC_channel();
  set_ADC_channel(ADC_channel);
}

void start_ADC(){
  ADCSRA |= (1 << ADSC);
}

/* Select ADC0, ADC1, ADC4, ADC5, ADC6 or ADC7 */
void set_ADC_channel(uint8_t channel_no){
  //configuration without MUX
  uint8_t configuration = ADMUX       & ((1 << REFS1) | (1 << REFS0) | (1 << ADLAR));
  //Mask input to only use MUX bits
  uint8_t new_mux_setting = channel_no  & ((1 << MUX4) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
  //New setting:
  ADMUX = configuration + new_mux_setting;
}

void choose_next_ADC_channel(){
  //Previous reading index: where in the array should we put the previous reading
  switch (ADC_channel) {
  case 0:
    ADC_channel = 1;
    previous_reading_index = 0;
    break;
  case 1:
    ADC_channel = 4;
    previous_reading_index = 1;
    break;
  case 4:
    ADC_channel = 5;
    previous_reading_index = 3;
    break;
  case 5:
    ADC_channel = 0;
    previous_reading_index = 2;
    break;
  default:
    ADC_channel = 0; //Should not happen
    previous_reading_index = 0;
    break;
  }
}