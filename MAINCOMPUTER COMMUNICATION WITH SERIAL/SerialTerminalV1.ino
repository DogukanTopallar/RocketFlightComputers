#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1003.91)
static const int GPSRXPin = 10, GPSTXPin = 11;
static const uint32_t GPSBaud = 4800;
TinyGPSPlus gps;
SoftwareSerial GPSss(GPSRXPin, GPSTXPin);
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
Adafruit_BME280 bme;


void setup() {
  Serial.begin(9600);
  GPSss.begin(GPSBaud);
  Serial.println("");
  
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

   if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
}

void loop() {

  while (GPSss.available() > 0)
    if (gps.encode(GPSss.read()))
      displayInfo();
  
  if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      Serial.println(F("No GPS detected: check wiring."));
      while(true);
    }
    
  //GPS Baglantisi
  while (GPSss.available() > 0)
    if (gps.encode(GPSss.read()))
    Serial.print(F("Location: "));
      //gpsData();
  if (gps.location.isValid())
  
  {
    
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print("Location: ");
    Serial.print(F("INVALID"));
  }




  Serial.println();


  
      /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);
  /* Display the floating point data */
  Serial.print("BNO055 -> ");
  Serial.print("");
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  /* New line for the next sample */
  Serial.println("");
  
  Serial.print("BME280 -> ");
  Serial.print("");
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.print("*C");

  Serial.print("\tPressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.print("hPa");

  Serial.print("\tApprox. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.print("m");
  Serial.println("");
  Serial.println("****************************");
  delay(500);



  
  /*//GPS Baglanti Hatası
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }*/

}
void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID")); 
    Serial.println();
  }
}

 
