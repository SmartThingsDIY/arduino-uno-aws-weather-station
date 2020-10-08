/**
  Reads data from a DHT11 sensor, displays it on an LCD screen and at the same time
  sends it through pins 2 and 3 to a connected ESP8266 ESP-01 WiFi module
  @author MecaHumArduino
  @version 3.0
*/

#include <DHT.h>
#include <Arduino.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson (use v6.xx)
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#define DEBUG true

#define DHT_PIN 4 // pin connected to data pin of DHT11
#define DHT_TYPE DHT11  // Type of the DHT Sensor, DHT11/DHT22
int uvAnalogIn = A0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

DHT temperatureSensor(DHT_PIN, DHT_TYPE);

// ESP TX => Uno Pin 2
// ESP RX => Uno Pin 3
SoftwareSerial wifi(2, 3);

// **************
String sendDataToWiFiBoard(String command, const int timeout, boolean debug);
String prepareDataForWiFi(float humidity, float temperature, float heatIndex, float uvIndex);
void setup();
void loop();
// **************

/**
 * Build and return a JSON document from the sensor data
 * @param humidity
 * @param temperature
 * @param heatIndex
 * @return
 */
String prepareDataForWiFi(float humidity, float temperature, float heatIndex, float uvIndex)
{
  StaticJsonDocument<200> doc;

  doc["humidity"]    = String(humidity);
  doc["temperature"] = String(temperature);
  doc["heat_index"]  = String(heatIndex);
  doc["uv_index"]    = String(uvIndex);

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  return jsonBuffer;
}

/**
 * Send data through Serial to ESP8266 module
 * @param command
 * @param timeout
 * @param debug
 * @return
 */
String sendDataToWiFiBoard(String command, const int timeout, boolean debug)
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
  // read from GUVA-S12SD
  pinMode(uvAnalogIn, INPUT);

  Serial.begin(9600);
  wifi.begin(9600);

  // set up the LCD's number of columns and rows
  lcd.begin(16, 2);

  // put cursor at the begining of row 0, line 0
  lcd.setCursor(0, 0);

  // display a welcome message
  lcd.print("Temp, Humidity");
  // go to second line
  lcd.setCursor(0, 1);
  lcd.print("and UV Sensor");

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

      while((time+1000) > millis()) {
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

  // read from DHT11
  temperatureSensor.begin();

  float temperature = temperatureSensor.readTemperature(); // return temperature in °C
  float humidity    = temperatureSensor.readHumidity(); // return humidity in %
  // Compute heat index in Celsius (isFahrenheit = false)
  float heatIndex   = temperatureSensor.computeHeatIndex(temperature, humidity, false);
  float uvIndex     = analogRead(uvAnalogIn);

  // integer index, divide by 100!
  uvIndex /= 100.0;

  // check whether reading was successful or not
  if (temperature == NAN || humidity == NAN) { // NAN means no available data
    lcd.print("Reading failed.");
  } else {
    // display Humidity on the LCD screen
    lcd.setCursor(0, 0);
    lcd.print("Humidity:");
    lcd.print(String(humidity) + "%");

    // display Temperature on the LCD screen
    lcd.setCursor(0, 1);
    lcd.print("Temp:");
    lcd.print(String(temperature) + "C");

    // display UVIndex on the LCD screen
    // lcd.setCursor(0, 2);
    lcd.print(" - UV:");
    lcd.print(String(uvIndex));

    String preparedData = prepareDataForWiFi(humidity, temperature, heatIndex, uvIndex);
    if (DEBUG == true) {
      Serial.println(preparedData);
    }
    sendDataToWiFiBoard(preparedData, 1000, DEBUG);
  }

  delay(2000); // take measurements every 2 sec
}