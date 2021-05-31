#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1003.91)
static const int GPSRXPin = 2, GPSTXPin = 8;
static const uint32_t GPSBaud = 4800;
TinyGPSPlus gps;
SoftwareSerial GPSss(GPSRXPin, GPSTXPin);
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
Adafruit_BME280 bme;
#define XbeeRXPin 11
#define XbeeTXPin 12
SoftwareSerial Xbee(XbeeRXPin, XbeeTXPin);


void setup() {
  Serial.begin(9600);
  pinMode(XbeeRXPin, INPUT);
  pinMode(XbeeTXPin, OUTPUT);
  Xbee.begin(19200);
  GPSss.begin(GPSBaud);
  Xbee.println("");
  
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Xbee.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

   if (!bme.begin(0x77)) {
    Xbee.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
}

void loop() {
   //GPS Baglantisi
  while (GPSss.available() > 0)
    if (gps.encode(GPSss.read()))
      //gpsData();
      Xbee.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Xbee.print(gps.location.lat(), 6);
    Xbee.print(F(","));
    Xbee.print(gps.location.lng(), 6);
  }
  else
  {
    Xbee.print(F("INVALID"));
  }

  Xbee.println();


  
      /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);
  /* Display the floating point data */
  Xbee.print("BNO055 -> ");
  Xbee.print("");
  Xbee.print("X: ");
  Xbee.print(event.orientation.x, 4);
  Xbee.print("\tY: ");
  Xbee.print(event.orientation.y, 4);
  Xbee.print("\tZ: ");
  Xbee.print(event.orientation.z, 4);
  /* New line for the next sample */
  Xbee.println("");
  
  Xbee.print("BME280 -> ");
  Xbee.print("");
  Xbee.print("Temperature = ");
  Xbee.print(bme.readTemperature());
  Xbee.print("*C");

  Xbee.print("\tPressure = ");
  Xbee.print(bme.readPressure() / 100.0F);
  Xbee.print("hPa");

  Xbee.print("\tApprox. Altitude = ");
  Xbee.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Xbee.print("m");
  Xbee.println("");
  Xbee.println("****************************");
  delay(2000);



  
  /*//GPS Baglanti Hatası
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Xbee.println(F("No GPS detected: check wiring."));
    while(true);
  }*/

}
