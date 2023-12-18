#include <Arduino.h>
#include <Wire.h>

//Serial is computer serial
//Serial5 is ant
//Serial8 is GPS
// this is a test

void setup() {
  // make this baud rate fast enough to we aren't waiting on it
  //Serial.begin(9600); // computer

    // 9600 baud is the default rate for the Ultimate GPS
  Serial5.begin(9600); // antenna
  Serial.begin(9600); // monitor
  Serial8.begin(4800); // GPS

  // wait for hardware serial to appear
  //while (!Serial) delay(10);
  //while (!Serial5) delay(10);

  //resets to default
  Serial8.println("$PMTK314,-1*04");

  // turn on only the "minimum recommended" data
  Serial8.println("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29");
  // or put PMTK_SET_NMEA_OUTPUT_RMCONLY in the brackets 

  //turns off the antenna line that says the antenna is good eg: $PGTOP,11,3*6F
   Serial8.println("$PGCMD,33,0*6D");

  //turn off the NMEA (keep commented)
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_OFF);

  //sets GPS to 10HZ
  //GPS.sendCommand("$PMTK220,100*2F<CR><LF>");

  //sets GPS to 1HZ (default)
  Serial8.println("$PMTK220,1000*1F");

  //sets baude rate to 4800
  Serial8.println("$PMTK251,4800*14");

  //sets GPS to 0.5HZ 
  //Serial8.println("$PMTK220,5000*1B");

  //ABOVE IS FOR GPS
  //BELOW IS FOR IMU

  

}


void loop() {
  if (Serial8.available()) {
    char c = Serial8.read();
   // Serial.write(c); // Print received data from GPS to Serial Monitor
    Serial.print(c);
    Serial5.print(c);
  }
  delay(10);
}

