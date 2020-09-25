/**
  Reads data from a DHT11 sensor, displays it on an LCD screen and at the same time
  sends it through pins 2 and 3 to a connected ESP8266 ESP-01 WiFi module
  @author MecaHumArduino
  @version 3.0
*/

#include <Arduino.h>
#include <dht11.h>
#include <LiquidCrystal.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson (use v6.xx)
#include <SoftwareSerial.h>

#define DEBUG true
#define DHT11PIN 4
dht11 sensor;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// ESP TX => Uno Pin 2
// ESP RX => Uno Pin 3
SoftwareSerial wifi(2, 3);

// declaring custom function to follow C++ validation rules
String prepareDataForWiFi(float humidity, float temperature);
String sendDataToWiFi(String command, const int timeout, boolean debug);
String sendDataToWiFi(String command, const int timeout, boolean debug);


String prepareDataForWiFi(float humidity, float temperature)
{

  StaticJsonDocument<200> doc;

  doc["humidity"]    = (String)humidity;
  doc["temperature"] = (String)temperature;

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  return jsonBuffer;
}

String sendDataToWiFi(String command, const int timeout, boolean debug)
{
  String response = "";

  wifi.print(command); // send the read character to the esp8266

  long int time = millis();

  while((time+timeout) > millis()) {
    while(wifi.available()) {
      // The esp has data so display its output to the serial window
      char c = wifi.read(); // read the next character.
      response+=c;
    }
  }

  if (debug) {
    Serial.print(response);
  }

  return response;
}

void setup() {
  Serial.begin(9600);
  wifi.begin(9600);

  // set up the LCD's number of columns and rows
  lcd.begin(16, 2);

  // put cursor at the begining of row 0, line 0
  lcd.setCursor(0, 0);

  // display a welcome message
  lcd.print("Temp & Humidity");
  // go to second line
  lcd.setCursor(0, 1);
  lcd.print("Sensor");

  lcd.blink();
  delay(3000);
  lcd.noBlink();
}

void loop() {

  if (DEBUG == true) {
    Serial.print("buffer: ");
    if (wifi.available()) {
      String espBuf;
      long int time = millis();

      while( (time+1000) > millis()) {
        while (wifi.available()) {
          // The esp has data so display its output to the serial window
          char c = wifi.read(); // read the next character.
          espBuf += c;
        }
      }
      Serial.print(espBuf);
    }
    Serial.println(" endbuffer");
  }

  // read from the digital pin
  int   check       = sensor.read(DHT11PIN);
  float temperature = (float)sensor.temperature;
  float humidity    = (float)sensor.humidity;

  // display Humidity on the LCD screen
  lcd.setCursor(0, 0);
  lcd.print("Humidity (%): ");
  lcd.print(humidity, 2);

  // display Temperature on the LCD screen
  lcd.setCursor(0, 1);
  lcd.print("Temp (C): ");
  lcd.print(temperature, 2);

  String preparedData = prepareDataForWiFi(humidity, temperature);
  if (DEBUG == true) {
    Serial.println(preparedData);
  }
  sendDataToWiFi(preparedData, 1000, DEBUG);

  delay(2000); // take measurements every 2 sec
}