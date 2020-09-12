#include <dht11.h>
#include <LiquidCrystal.h>

#define DHT11PIN 3
dht11 sensor;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  Serial.begin(9600);

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
  int check = sensor.read(DHT11PIN);

  // display Hum on the LCD screen
  lcd.setCursor(0, 0);
  lcd.print("Humidity (%): ");
  lcd.print((float)sensor.humidity, 2);

  // display Temp on the LCD screen
  lcd.setCursor(0, 1);
  lcd.print("Temp (C): ");
  lcd.print((float)sensor.temperature, 2);

  // print Hum on the serial monitor
  Serial.print("Humidity (%): ");
  Serial.println((float)sensor.humidity, 2);

  // print Temp on the serial monitor
  Serial.print("Temperature (C): ");
  Serial.println((float)sensor.temperature, 2);

  delay(2000); // take measurements every 2 sec
}