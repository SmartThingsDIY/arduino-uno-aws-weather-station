#include <dht11.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#define DEBUG true
#define DHT11PIN 4
dht11 sensor;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
SoftwareSerial wifi(2, 3);

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

  // read from the digital pin
  int   check       = sensor.read(DHT11PIN);
  float temperature = (float)sensor.temperature;
  float humidity    = (float)sensor.humidity;

  // display Hum on the LCD screen
  lcd.setCursor(0, 0);
  lcd.print("Humidity (%): ");
  lcd.print(humidity, 2);

  // display Temp on the LCD screen
  lcd.setCursor(0, 1);
  lcd.print("Temp (C): ");
  lcd.print(temperature, 2);

  // const char DATA_TO_SEND[] = "{\"Humidity\":\"" + (String)humidity + "\", \"Temperature\":\"" + (String)temperature + "\"} ";


  String sensorData = "{\"Humidity\":\"";
  sensorData += (String)humidity;
  sensorData += "\", \"Temperature\":\"";
  sensorData += (String)temperature;
  sensorData += "\"}";
  sensorData += "\r\n";

  Serial.println(sensorData);
  sendDataToWiFi(sensorData, 1000, DEBUG);

  delay(2000); // take measurements every 2 sec
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

    Serial.print(response);

    return response;
}