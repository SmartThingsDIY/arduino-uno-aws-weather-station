#include <dht11.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#define DHT11PIN 4
dht11 sensor;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
SoftwareSerial WiFiBoard(2, 3);

void setup() {
  Serial.begin(9600);
  WiFiBoard.begin(115200);

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

  // print Hum on the serial monitor
  Serial.print("Humidity (%): ");
  Serial.println(humidity, 2);

  // print Temp on the serial monitor
  Serial.print("Temperature (C): ");
  Serial.println(temperature, 2);

  // sendDataToWiFiBoard(temperature, humidity);

  WiFiBoard.println(111);
  WiFiBoard.write(111);
  delay(2000); // take measurements every 2 sec
}

void sendDataToWiFiBoard(float temperature, float humidity)
{
  WiFiBoard.println(temperature);
}