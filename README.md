# Weather Station with Arduino Uno board, DHT11 Sensor, LCD screen and AWS IOT

This tutorial shows you how to use the [DHT11](https://amzn.to/2Qs9fcV) temperature and humidity sensors with the [Arduino Uno](https://amzn.to/2RnHhPY) board to build a simple weather station that displays current temperature and humidity on an [LCD](https://amzn.to/2FzJdT6) screen and pushes the data to AWS IoT through an [ESP8266](https://amzn.to/3hqqWom) board.

In the **first part**, we'll go through a quick introduction to the sensor, pinout, wiring diagram, and finally the Arduino sketch.

In the **second part**, WIP...

PART ONE
========

⚡️ COMPONENTS AND SUPPLIES
--------------------------

<img align="right" src="https://store-cdn.arduino.cc/usa/catalog/product/cache/1/image/1000x750/f8876a31b63532bbba4e781c30024a0a/a/0/a000066_iso_1_3.jpg" style="max-width:100%;" height="300">


*   [Arduino Uno](https://amzn.to/2RnHhPY)
*   [LCD Screen](https://amzn.to/2FzJdT62Ehh2ru)
*   [Breadboard](https://amzn.to/2Ei40tP)
*   [Jumper Wires](https://amzn.to/2Ehh2ru)
*   [DHT11](https://amzn.to/2Qs9fcV) Temp & Humidity Sensor
*   [10k Ohm resistor](https://amzn.to/2Qo7vkW)

🚀APPS
------

*   [PlatformIO](https://platformio.org/)
*   [AWS cli](https://docs.aws.amazon.com/cli/latest/userguide/cli-chap-install.html)

What is humidity?
-----------------
Humidity is the water vapor around you mixed with air. It is measured in %. So, if the humidity is 60%, then 60% of the air around you is water vapor. If it is 100%, then it means either the sensor is not correct, the sensor is broken/damaged, the Arduino crashed, the Arduino can't receive any signal, there's an error in the code or you're underwater(1). If it's 0%, it means all the reasons above except the last one, you're in space or you're in the middle of a desert(2).

(1) Correction: it means the air cannot hold any more water.<br><br>

(2) The air in a desert does contain some water but it is a very little amount compared to a normal place. The Sahara Desert has a mean humidity of 25%.</p>

<img align="right" src="https://github.com/MecaHumArduino/arduino-uno-aws-weather-station/blob/master/docs/dht11.jpeg?raw=true" style="max-width:100%;" height="350">

How to measure Temperature & Humidity
-------------------------------------

To the right, is a [DHT11](https://amzn.to/2Qs9fcV) sensor, it is a temperature and humidity combined sensor with calibrated digital signal output. It's is a reliable sensor for determining temperature and humidity. There is a newer and more accurate module on the market called DHT22 but we will stick with the DHT11 for this demo

DHT11 - Schematic Diagram
-------------------------

Wire the [DHT11](https://amzn.to/2Qs9fcV) sensor to the [Arduino Uno](https://amzn.to/2RnHhPY) development board as shown in the following schematic diagram.

<img align="center" src="https://github.com/MecaHumArduino/arduino-uno-aws-weather-station/blob/master/docs/dht11-wiring.png?raw=true" style="max-width:100%;" height="350">

LCD Screen - Schematic Diagram
------------------------------

The LCD display needs six Arduino pins, all set to be digital outputs. It also needs 5V and GND connections.

There are a number of connections to be made. Lining up the display with the top of the breadboard helps to identify its pins without too much counting, especially if the breadboard has its rows numbered with row 1 as the top row of the board. Do not forget, the long yellow lead that links the slider of the pot to pin 3 of the display. The potentiometer is used to control the contrast of the display.

<img align="center" src="https://github.com/MecaHumArduino/arduino-uno-aws-weather-station/blob/master/docs/lcd-schema.png?raw=true" style="max-width:100%;" height="600">