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
