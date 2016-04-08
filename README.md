# Multitech mDot - LoRa with Arduino UNO

LoRa is a modulation technique with a significantly long range. The modulation is based on spread-spectrum techniques and a variation of chirp spread spectrum (CSS) with integrated forward error correction (FEC). Mdot is a node to connect an IoT project with a LoRa network.

<aside class="notice">
Before to di this tutorial, you need to do the MultiConnect Conduit tutorial, and set the Network name, password and sub-band channel. This tutorial is <p href="../devices/conduit.html">here</p>
</aside>

## Requiremets

* [MultiTech mDot](http://www.multitech.com/models/94557148LF)
* [An Arduino UNO](https://www.arduino.cc/en/Main/ArduinoBoardUno)
* [An UartsBee](http://www.seeedstudio.com/wiki/UartSBee)
* [ArduiniMdot library by Ubidtos](https://github.com/ubidots/ubidots-arduino-mdot)

## Setup

1. Put the mDot on the UartsBee
2. With some wires connect the UartsBee with Arduino UNO, follor the next table:
|||
|:---:|:---:|
|UartsBee|Arduino UNO|
|GND|GND|
|VCC|5V|
|TX|11|
|RX|10|
3. Download the Ubidots ArduinoMdot library [here](https://github.com/ubidots/ubidots-arduino-mdot)
4. Now, click on **Sketch -> Include Library -> Add .ZIP Library**
5. Select the .ZIP file of ArduinoMdot and then "Accept" or "Choose"
6. Close the Arduino IDE and open it again.


## Send one value to Ubidots

To send one value to Ubidots API you need to open the Arduino IDE and paste the next code, don't forget to change the Network name, password and sub band.

<aside class="warning">
Be careful if the firmware of your mDot isn't compatible with firmware of your MultiConnect Conduit, the mDot won't connect to your Conduit LoRa Network.
</aside>

```c++
#include "ArduinoMdot.h"

#define SSID "Conduit_LoRa_Network_Name"
#define PASS "Conduit_LoRa_Network_Password"
#define SUB_BAND "1" // Number of your sub band of LoRa 1-8
#define VARIABLE_NAME 1  // Depends of the number it'll be send a setted name

Ubidots client;
void setup() {
  Serial.begin(115200);
}

void loop() {
  float value = analogRead(A0);
  client.loraVerify(SSID, PASS, SUB_BAND);
  client.loraSend(2.1230, VARIABLE_NAME);
}
```



