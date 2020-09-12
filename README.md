# Weather Station with Arduino Uno board, DHT11 Sensor, LCD screen and AWS IOT

This tutorial shows you how to use the [DHT11](https://amzn.to/2Qs9fcV) temperature and humidity sensors with the [Arduino Uno](https://amzn.to/3jmCpqx) board to build a simple weather station that displays current temperature and humidity on an [LCD](https://amzn.to/2FzJdT6) screen and pushes the data to AWS IoT through an [ESP8266](https://amzn.to/3hqqWom) board.

In the **first part**, we'll go through a quick introduction to the sensor, pinout, wiring diagram, and finally the Arduino sketch.

In the **second part**, we'll see how to connect out DYI weather station to AWS IoT in order to store the data, visualize it and run some analytics on it.

PART ONE
========

⚡️ COMPONENTS AND SUPPLIES
--------------------------

<img align="right" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/esp-32.jpg?raw=true" style="max-width:100%;" height="200">

*   [ESP32 Board](https://amzn.to/3jmCpqx)
*   [Breadboard](https://amzn.to/2Ei40tP)
*   [Jumper Wires](https://amzn.to/2Ehh2ru)
*   [DHT11](https://amzn.to/2Qs9fcV) or [DHT22](https://amzn.to/31t7P8l) Temp & Humidity Sensor
*   [10k Ohm resistor](https://amzn.to/2Qo7vkW)


EDIT: Or use this [KIT from WayinTop](https://amzn.to/3hxR01A) that already contains everything

🚀APPS
------

*   [Arduino IDE](https://www.arduino.cc/en/main/software)
*   [AWS cli](https://docs.aws.amazon.com/cli/latest/userguide/cli-chap-install.html)

DHT11 vs DHT22 SENSORS
----------------------

<img align="center" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/DHT11-DHT22-Temperature-Humidity-Sensor.jpg?raw=true">

The [DHT11](https://amzn.to/2Qs9fcV) and [DHT22](https://amzn.to/31t7P8l) sensors are used to measure temperature and relative humidity. These are very popular among makers and electronics hobbyists.

These sensors contain a chip that does analog to digital conversion and spit out a digital signal with the temperature and humidity. This makes them very easy to use with any microcontroller.

The DHT11 and DHT22 are very similar, but differ in their specifications.

The DHT22 sensor has a better resolution and a wider temperature and humidity measurement range. However, it is a bit more expensive, and you can only request readings with 2 seconds interval.

The DHT11 has a smaller range and it's less accurate. However, you can request sensor readings every second. It's also a bit cheaper.

Despite their differences, they work in a similar way, and you can use the same code to read temperature and humidity. You just need to select in the code the sensor type you're using.

Schematic Diagram
-----------------

Wire the [DHT11](https://amzn.to/2Qs9fcV) or [DHT22](https://amzn.to/31t7P8l) sensor to the [ESP32](https://amzn.to/3jmCpqx) development board as shown in the following schematic diagram.

<img align="center" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/schematic.png?raw=true" style="max-width:100%;" height="600">

Installing Libraries
--------------------

To read from the DHT sensor, we'll use the [DHT library from Adafruit](https://github.com/adafruit/DHT-sensor-library). To use this library you also need to install the [Adafruit Unified Sensor library](https://github.com/adafruit/Adafruit_Sensor). Follow the next steps to install those libraries.

Open your Arduino IDE and go to **Sketch > Include Library > Manage Libraries**. The Library Manager should open.

Search for **"DHT"** on the Search box and install the DHT library from Adafruit.

<img align="center" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/adafruit_dht_library.png?raw=true" style="max-width:100%;" height="600">

After installing the DHT library from Adafruit, type **"Adafruit Unified Sensor"** in the search box. Scroll all the way down to find the library and install it.

<img align="center" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/adafruit_unified_sensor_library.png?raw=true" style="max-width:100%;" height="600">

After installing the libraries, restart your Arduino IDE.

THE CODE
--------

Rename the file [secret.h.public](https://github.com/isbkch/esp32-aws-weather-station/blob/master/secrets.h.public) to **secret.h** and edit it with your information:

*   1\. Select your thing from [this page](https://console.aws.amazon.com/iot/home?region=us-east-1#/thinghub)
*   2\. From the left menu, select **Security** and click the button "**Create Certificate**"
*   3\. Now download both files *.cert.pem and *.private.key and replace their content in the **secret.h**. And don't forget to activate your Certificate.
*   4\. Back to the left menu, click **Interact** and copy the HTTPS link then paste it into **secret.h** as well.

Finally, open the file [esp32-aws-weather-station.ino](https://github.com/isbkch/esp32-aws-weather-station/blob/master/esp32-aws-weather-station.ino) with Arduino IDE

PART TWO
========

AWS SETUP
---------

Now that parts are connected and the libraries are installed, it's time to setup the AWS part.

As you see in the diagram below, the ESP board will be sending data through an MQTT topic to AWS IoT Core, we will be applying rules on the raw data and invoking a Lambda that writes the data to a dynamoDB table. Afterwards, we can read that data and display it on a webpage

<img align="center" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/aws-architecture.png?raw=true" style="max-width:100%;" height="350">
