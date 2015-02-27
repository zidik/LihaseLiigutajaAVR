#include "mux_definitions.h"
#define IDENTIFICATION_STRING F("Lihaseliigutaja")
#define HELP_STRING ("\n\nHELP:\nUse LineFeed at the end of commands.\nCOMMANDS:\ni - identification\nv - version\nr0 - stop sending\nr1 - start sending\nh or ? - help")
#define SOTWARE_VERSION F("0.1.1")
#define SERIAL_INPUT_BUFFER_SIZE 256 //counter is uint8_t, so 256 is currently max

volatile uint16_t latest_readings[4] = { 0 };
volatile uint8_t ADC_channel = 0;
volatile uint8_t previous_reading_index = 0;

char serialInputBuffer[SERIAL_INPUT_BUFFER_SIZE];
uint8_t serial_counter = 0;
bool serialBufferComplete = false;

bool send_readings = false;

unsigned long current_step_period = 1000;
uint8_t next_step = 0;

//Add Streaming operator to Serial
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } //Add streaming operator to Serial


void setup()
{
  // Remove CLKDIV8
  CLKPR = 0x80;
  CLKPR = 0x00;

  // DISABLE JTAG - take control of F port
  MCUCR = _BV(JTD);
  MCUCR = _BV(JTD);

  //Debug LED
  DDRB |= (1 << DDB0);

  //Serial
  Serial.begin(500000);
  while (!Serial){
    delay(200);
    PINB |= (1 << PINB0);
  }
  // wait for serial port to connect.
  Serial << F("Booting...\n");

  Serial << F("Initializing MUXES\n");
  initialize_MUXes();
  Serial << F("Initializing ADC\n");
  init_ADC();
  Serial << F("Starting ADC\n");
  start_ADC();
  Serial << F("Initializing PWM\n");
  initialize_PWM();
  
  Serial << F("Boot complete\n");
  Serial << F(" --- --- --- \n");

  Serial << HELP_STRING << "\n";
}

void loop()
{
  PINB |= (1 << PINB0);
  
  EAP_control();
  ADC_reading_serial_output(); // If requested, send ADC readings to PC
  check_serial_buffer(); //Look into current buffer for a command
  handle_serial_input(); //Read data from serial to buffer
}

void EAP_control(){
  static unsigned long previous_tick = 0;
  unsigned long current_millis = millis();
  if ((current_millis - previous_tick) > current_step_period){
    previous_tick = current_millis;
    execute_step();
  }
}

void execute_step(){
  switch (next_step)
  {
  case 0:
    //Initial Charge :
    //Charge 0,1 from external (0-FB_OUT, 1-GND1)
    Serial << F("Initial charging\n");
    set_flyback_input(true, false);
    set_mux_output<mux_flyback_out>(0);
    set_mux_output<mux_gnd_1>(1);
    Serial << F("Starting PWM\n");
    start_PWM();
    current_step_period = 1000;
    next_step = 1;
    break;

  case 1:
    //Hold (0-NC, 1-GND1)
    Serial << F("Holding charge\n");
    stop_PWM();
    disable_mux<mux_flyback_out>();
    current_step_period = 1000;
    next_step = 2;
    break;

  case 2:
    Serial << F("Short-circuit\n");
    //Short-circuit (0-GND2, 1-GND1)
    set_mux_output<mux_gnd_2>(0);
    current_step_period = 1000;
    next_step = 3;
    break;

  case 3:
    Serial << F("Reverse charging\n");
    //Reverse charge (0-GND2, 1-FB_OUT)
    disable_mux<mux_gnd_1>();
    set_mux_output<mux_flyback_out>(1);
    start_PWM();
    current_step_period = 1000;
    next_step = 4;
    break;

  case 4:
    //Hold (0-GND2, 1-NC)
    Serial << F("Holding charge\n");
    stop_PWM();
    disable_mux<mux_flyback_out>();
    current_step_period = 1000;
    next_step = 5;
    break;

  case 5:
    //Short-circuit (0-GND2, 1-GND1)
    Serial << F("Short-circuit\n");
    set_mux_output<mux_gnd_1>(1);
    current_step_period = 1000;
    next_step = 6;
    break;

  case 6:
    //Charge (0-FB_OUT, 1-GND1)
    Serial << F("Charging\n");
    disable_mux<mux_gnd_2>();
    set_mux_output<mux_flyback_out>(0);
    current_step_period = 1000;
    next_step = 1;
    break;

  case 0xff:
    Serial << F("Initializing after invalid next step\n");
    disable_mux<mux_gnd_1>();
    disable_mux<mux_gnd_2>();
    current_step_period = 200;
    next_step = 0;

  default:
    Serial << F("Invalid next step: ") << next_step << "\n";
    disable_mux<mux_flyback_out>();
    disable_mux<mux_eap_out>();
    set_mux_output<mux_gnd_1>(1);
    set_mux_output<mux_gnd_2>(0);

    current_step_period = 5000;
    next_step = 0xff;
  }
}

void ADC_reading_serial_output(){
  if (!Serial) return;
  if (!send_readings) return;

  Serial << F("READ:") << latest_readings[0] << " " << latest_readings[1] << " " << latest_readings[2] << " " << latest_readings[3] << "\n";
}

void handle_serial_input(){
  while (Serial.available()) {
    if (serialBufferComplete)
      break;
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      serialBufferComplete = true;
    }
    else{
      serialInputBuffer[serial_counter++] = inChar;
    }
  }
}








ISR(ADC_vect)
{
  //The new ADC-round should be started by now already (?)

  //Get the last result
  latest_readings[previous_reading_index] = ADC;
  // let's choose next channel to sample
  choose_next_ADC_channel();
  set_ADC_channel(ADC_channel);
}