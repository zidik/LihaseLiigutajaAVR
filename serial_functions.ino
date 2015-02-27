//NB! Add this in main INO:
//Add Streaming operator to Serial
//template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } //Add streaming operator to Serial

void check_serial_buffer(){
  //Serial buffer has to be ready before processing it
  if (!serialBufferComplete)
    return;
  serialBufferComplete = false;

  bool command_successful = false;
  // asking for identification string
  if (serial_counter == 1 && (serialInputBuffer[0] == '?' || serialInputBuffer[0] == 'h')){
    Serial << HELP_STRING << '\n';
    command_successful = true;
  }
  // asking for identification string
  else if (serial_counter == 1 && serialInputBuffer[0] == 'i'){
    Serial << IDENTIFICATION_STRING << '\n';
    command_successful = true;
  }
  // asking for  version
  else if (serial_counter == 1 && serialInputBuffer[0] == 'v'){
    Serial << SOTWARE_VERSION << '\n';
    command_successful = true;
  }
  else if (serial_counter == 2 && serialInputBuffer[0] == 'r'){
    if (serialInputBuffer[1] == '0'){
      Serial << "Stopped sending readings\n";
      send_readings = false;
      command_successful = true;
    }
    else if (serialInputBuffer[1] == '1'){
      Serial << "Starting to send readings\n";
      send_readings = true;
      command_successful = true;
    }
  }

  if (!command_successful){
    Serial << F("e") << F("Did not understand the command: ");
    Serial.write(serialInputBuffer, serial_counter);
    Serial << "\n";
  }

  serial_counter = 0;
  serialBufferComplete = false;
}
