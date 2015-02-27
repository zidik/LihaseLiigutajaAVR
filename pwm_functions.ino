/*
* Initialize PWM registers
* Compare Output Mode, Fast PWM
*/
void initialize_PWM(){
  //set OC1A pin low
  PORTB &= (0 << PORTB5);
  //set OC1A pin output
  DDRB |= (1 << DDB5);

  //Fast PWM TOP value
  ICR1 = 0xFFFF;
  set_on_period(0);

  TCCR1A =
    (1 << COM1A1) | //Compare Output Mode for Channel A:
    (0 << COM1A0) | //Clear OC1A on compare match, set OC1A at TOP
    (0 << COM1B1) | //Compare Output Mode for Channel B:
    (0 << COM1B0) | //Normal port operation - OC Disconnected
    (0 << COM1C1) | //Compare Output Mode for Channel C:
    (0 << COM1C0) | //Normal port operation - OC Disconnected
    (1 << WGM11) |  //Fast PWM (TOP = ICR1)
    (0 << WGM10);   //Fast PWM (TOP = ICR1)

  TCCR1B =
    (0 << ICNC1) | // Disable Input Capture Noise Canceler
    (0 << ICES1) | // Input capture edge select - do not care
    (1 << WGM13) | //Fast PWM (TOP = ICR1)
    (1 << WGM12) | //Fast PWM (TOP = ICR1)
    (0 << CS12) | //Clock select:
    (0 << CS11) | //Timer stopped
    (0 << CS10);

}

/* Start PWM by connecting the clock throuh prescaler*/
void start_PWM(){
  uint8_t current_conf = TCCR1B & ((1 << ICNC1) | (1 << ICES1) | (1 << WGM13) | (1 << WGM12));
  uint8_t prescaling_conf =
    (0 << CS12) | //Clock select:
    (0 << CS11) | //No prescaling
    (1 << CS10);
  TCCR1B = current_conf + prescaling_conf;
}

/* Stop PWM by disconnecting the clock source, ensure OC1A pin is low (for safety)*/
void stop_PWM(){
  TCCR1B &=
    ~(1 << CS12) & //Clock select:
    ~(1 << CS11) & //Timer stopped
    ~(1 << CS10);

  //set OC1A pin low
  PORTB &= (0 << PORTB5);
}


void set_on_period(uint16_t value){
  if (value > 5) //Limit to 5 cycles (protection during development)
    return;
  //Output compare value
  //When the OCRnA I/O location is written the value written will be put into
  //the OCRnA Buffer Register.The OCRnA Compare Register will then be updated with the value
  //in the Buffer Register at the next timer clock cycle the TCNTn matches TOP.
  OCR1A = value;
}
