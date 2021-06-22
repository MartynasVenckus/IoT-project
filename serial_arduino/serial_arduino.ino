#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include <Wire.h>
#include "SPI.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP280.h"

#include <OneWire.h>
#include <DallasTemperature.h>

SoftwareSerial s(5,6);

// Data wire is plugged into digital pin 9 on the Arduino
#define ONE_WIRE_BUS 9

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(ONE_WIRE_BUS);  

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

Adafruit_BMP280 bmp;

int sensorIn = A0;
int pwsp = 10;



void setup() {

Serial.begin(9600);
s.begin(9600);


sensors.begin();   //Begin the DS18B20 sensor
bmp.begin();       //Begin the BMP280 sensor


pinMode(pwsp, OUTPUT);     //settin pin 9 as output
  digitalWrite(pwsp, HIGH);  //setting pin 9 as HIGH for 5V supply

// Set the default voltage of the reference voltage
  analogReference(DEFAULT);

}
 StaticJsonBuffer<1000> jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
 
void loop() {
  float concentration;
  float pressure = bmp.readPressure();
  float temperature = bmp.readTemperature();
  int temp = 50;
 
 sensors.requestTemperatures();
  float temperatureDS = sensors.getTempCByIndex(0);    //DS18B20 temperature
  

  //Read voltage
  int sensorValue = analogRead(sensorIn);

  // The analog signal is converted to a voltage
  float voltage = sensorValue*(5000/1024.0);
  if(voltage == 0)
  {
  //  Serial.println("Fault");
  }
  else if(voltage < 400)
  {
    //Serial.println("preheating");
  }
  else
  {
    int voltage_diference=voltage-400;
     concentration=voltage_diference*50.0/16.0;
  }

  
  Serial.println(temperature);

//---------------------------------------------------------------------
 
  root["tempBMP"] = temperature;
  root["pressBMP"] = pressure;
  root["tempDS"] = temperatureDS;
  root["cons"] = concentration;
  
if(s.available()>0)
{
 root.printTo(s);
}
//---------------------------------------------------------------------


  delay(1000);


}
