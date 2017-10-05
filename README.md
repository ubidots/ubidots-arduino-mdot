# Multitech mDot - LoRa with Arduino UNO

LoRa is a modulation technique with a significantly long range. The modulation is based on spread-spectrum techniques and a variation of chirp spread spectrum (CSS) with integrated forward error correction (FEC). Mdot is a node to connect an IoT project with a LoRa network.

Pre-requisites: You must have already set a LoRa network using a Conduit Gateway, for more information please refer to this tutorial [here](http://help.ubidots.com/connect-your-devices/multiconnect-conduit-lora-to-gprs-gateway).

## Requiremets

* [MultiTech mDot](http://www.multitech.com/models/94557148LF)
* [An Arduino BOARD](https://www.arduino.cc/en/Main/ArduinoBoardUno)
* [Arduino mDot library by Ubidots](https://github.com/ubidots/ubidots-arduino-mdot)

**IMPORTANT UPDATE NOTE:** Since version 2.0, the library will not make use of the Software Serial library due to multiple bugs found when trying to use it, because of this, the user **will not be able to make any debug**. We strongly advise to our users to use this library as reference for faster deployments but to develop their own firmware routine that fits their need once they think to go out to the market.

## Setup


1. With some wires connect the mDot device with the Arduino board as shown in the table below:

<center>

|ARDUINO |mDot    |
|:------:|:------:|
|5 V     |VCC     |
|Rx      |Tx - PA2|
|Tx      |Rx - PA3|
|GND     |GND     |

</center>

For more information about the PinOut of the mDot device, please refer [here](https://os.mbed.com/platforms/MTS-mDot-F411/#mdot-pinout-diagram).

2. Download the Ubidots ArduinoMdot library [here](https://github.com/ubidots/ubidots-arduino-mdot/archive/master.zip)
3. Now, click on **Sketch -> Include Library -> Add .ZIP Library**
4. Select the .ZIP file of ArduinoMdot and then "Accept" or "Choose"
5. Close the Arduino IDE and open it again.

# Docs

## Constructor

### Ubidots

```c++
Ubidots(const char * ssid, const char * pass, uint8_t band)
```

> Creates an Ubidots instance, you must setup as input your LoRa Network SSID and its password, the *band* argument is optional and is set to one by default.

## Methods

### Add
```c++
void add(float value, uint8_t length, uint8_t accuracy)
```

> Adds a value to be send. The mDot device lets you to send a packet with 11 bytes as limit, so in order to let to the user to set properly the decimal precision this method allows to set the value length (in bytes) and its accuracy; please **ensure that the total number of the bytes of the package do not exceed 11**, the library does not make any check of byte size. This method uses the ```dtostrf(float number, int size, int accuracy)``` C++ method to convert from float to char, so choose carefully the length and accuracy parameters, if you do not know how to use this function let the values by default.
Length and accuracy are set by default as 4 and 2 respectively.

### SendAll
```c++
bool Ubidots::sendAll()
```

> Sends the value added using the ```add()``` method to build the packet with this format: value1,value2,value3 . Returns true upon sucessfull package send.


## Example

This example is intended for sending two values to the conduit Gateway, don't forget to change the Network name, password and also sub band if you set a value different to one inside the gateway.

<aside class="warning">
Be careful if the firmware of your mDot isn't compatible with firmware of your MultiConnect Conduit Gateway, if it is different the mDot won't connect to your Conduit LoRa Network.

</aside>

```c++
/****************************************
 * Include Libraries
 ****************************************/
#include <ubidotsMdot.h>

/****************************************
 * Define Constants
 ****************************************/
char const * SSID_NAME = "...."; // Put here your LoRa SSID
char const * SSID_PASS = "...."; // Put here your LoRa Network password

Ubidots client(SSID_NAME, SSID_PASS);

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  float value_1 = 1;
  float value_2 = 25.234;
  client.add(value_1);
  client.add(value_2, 4, 2);  // char size is four, accuracy is 2 decimals
  client.sendAll();
  delay(5000);
}
```



