#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

SoftwareSerial s(D6,D5);

char ssid[] = "AndroidAP";   // your network SSID (name) 
char pass[] = "dvso4065";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = 1323193;
const char * myWriteAPIKey = "U6CYMZCPID28YZ2F";
 
void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  s.begin(9600);
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  while (!Serial) continue;

  

}
 
void loop() {
  
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
   // Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
 
  if (root == JsonObject::invalid())
  {
    return;
  }
  //Print the data in the serial monitor
  Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  Serial.println("");
  Serial.print("TemperatureBMP ");
  float data1=root["tempBMP"];
  Serial.println(data1);
  Serial.print("PressureBMP    ");
  float data2=root["pressBMP"];
  Serial.println(data2);
  Serial.print("TemperatureDS  ");
  float data3=root["tempDS"];
  Serial.println(data3);
  Serial.print("Consentration  ");
  float data4=root["cons"];
  Serial.println(data4);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
 Serial.println("");

 // set the fields with the values
  ThingSpeak.setField(1, data1);
  ThingSpeak.setField(2, data2);
  ThingSpeak.setField(3, data3);
  ThingSpeak.setField(4, data4);

   // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(20000);
 
}
