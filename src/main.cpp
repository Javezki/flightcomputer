
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GPS.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);


#define GPSSerial Serial8
Adafruit_GPS GPS(&GPSSerial);

const unsigned long interval = 100;
unsigned long previousMillis = 0;

const int MPU = 0x68; // MPU6050 I2C address

float AccX, AccY, AccZ;
int16_t AccX_raw, AccY_raw, AccZ_raw;
String accelerometerData;


void setup()
{
  Serial5.begin(9600); // antenna
  Serial.begin(115200);
  GPS.begin(4800);
  bmp.begin();

  delay(1000);

  //IMU init
  Wire.begin(); //start communication 
  Wire.beginTransmission(MPU);
  
  Wire.write(0x6B);
  Wire.write(0x00); //reset device
  Wire.endTransmission(true);

  // BMP280 init
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */



}

String readAccValues() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers

  int16_t AccX_raw = (Wire.read() << 8 | Wire.read()); // X-axis value
  int16_t AccY_raw = (Wire.read() << 8 | Wire.read()); // Y-axis value
  int16_t AccZ_raw = (Wire.read() << 8 | Wire.read()); // Z-axis value

  double AccX = (AccX_raw / 16384.0) * 9.81; // Convert to actual accelerometer value
  double AccY = (AccY_raw / 16384.0) * 9.81;
  double AccZ = (AccZ_raw / 16384.0) * 9.81;

  String result = String(AccX) + "," + String(AccY) + "," + String(AccZ);
  return result;
}


void loop()
{
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; 
    accelerometerData = readAccValues();

    Serial.print(millis());
    Serial.print(",");

    Serial.print(accelerometerData);
    Serial.print(",");

    Serial.print(bmp.readTemperature());
    Serial.print(",");
    Serial.print(bmp.readPressure());
    Serial.print(",");
    Serial.println(bmp.readAltitude(1027)); /* Adjusted to local forecast! */


  }
    
    GPS.read();
    if (GPS.newNMEAreceived()) {

      accelerometerData = readAccValues();

      Serial.print(millis());
      Serial.print(",");

      Serial.print(accelerometerData);
      Serial.print(",");

      Serial.print(bmp.readTemperature());
      Serial.print(",");
      Serial.print(bmp.readPressure());
      Serial.print(",");
      Serial.print(bmp.readAltitude(1027)); /* Adjusted to local forecast! */
      Serial.print(",");

      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(",");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);


      if (!GPS.parse(GPS.lastNMEA())){} // this sets the newNMEAreceived() flag to false I cannot find a
      

    }
}
