/*
Copyright (c) 2013-2016 Ubidots.
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:
The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "ArduinoMdot.h"
#include <SoftwareSerial.h>

/**
 * Constructor.
 */
Ubidots::Ubidots() {
    //_client.begin(BAOUD_RATE);
  _client.begin(BAOUD_RATE);
}
/** 
 * This function is to read the data from GPRS pins. This function is from Adafruit_FONA library
 * @arg timeout, time to delay until the data is transmited
 * @return replybuffer the data of the GPRS
 */
char* Ubidots::readData(uint16_t timeout) {
  uint16_t replyidx = 0;
  char replybuffer[500];
  while (timeout--) {
    if (replyidx >= 500) {
      break;
    }
    while (_client.available()) {
      char c =  _client.read();
      if (c == '\r') continue;
      if (c == 0xA) {
        if (replyidx == 0)   // the first 0x0A is ignored
          continue;
      }
      replybuffer[replyidx] = c;
      replyidx++;
    }

    if (timeout == 0) {
      break;
    }
    delay(1);
  }
  replybuffer[replyidx] = '\0';  // null term
#ifdef DEBUG_UBIDOTS
  Serial.println("Response of GPRS:");
  Serial.println(replybuffer);
#endif
  while (_client.available()) {
    _client.read();
  }
  return replybuffer;
}
bool Ubidots::loraSend(float data, byte name) {
    String dat;
    dat = String(data, 3);
    _client.write("AT+SEND\r\n");
    _client.print(dat);
    _client.write("\r\n");
    delay(1000);
}
/** 
 * This function is to set the APN, USER and PASSWORD
 * @arg apn the APN of your mobile
 * @arg user the USER of the APN
 * @arg pwd the PASSWORD of the APN
 * @return true upon success
 */
bool Ubidots::loraConnection(char* ssid, char* pass, , char* band) {
    _client.write("AT\r\n");
    if (strstr(readData(1000), "ERROR") != NULL) {
#ifdef DEBUG_UBIDOTS
        Serial.println(F("Error with AT+SAPBR CONTYPE"));
#endif
        return false;
    }
    _client.write("AT+NI=1,");
    _client.write(ssid);
    _client.write("\r\n");
    if (strstr(readData(1000), "ERROR") != NULL) {
#ifdef DEBUG_UBIDOTS
        Serial.println(F("Error with AT+SAPBR CONTYPE"));
#endif
        return false;
    }
    _client.write("AT+NK=1,");
    _client.write(pass);
    _client.write("\r\n");
    if (strstr(readData(1000), "ERROR") != NULL) {
#ifdef DEBUG_UBIDOTS
        Serial.println(F("Error with AT+SAPBR CONTYPE"));
#endif
        return false;
    }
    _client.write("AT+FSB=");
    _client.write(band);
    _client.write("\r\n");
    if (strstr(readData(1000), "ERROR") != NULL) {
#ifdef DEBUG_UBIDOTS
        Serial.println(F("Error with AT+SAPBR CONTYPE"));
#endif
        return false;
    }
    _client.write("AT+ACK=1\r\n");
    Serial.write("AT+ACK=1\r\n");
    if (strstr(readData(1000), "ERROR") != NULL) {
#ifdef DEBUG_UBIDOTS
        Serial.println(F("Error with AT+SAPBR CONTYPE"));
#endif
        return false;
    }
    _client.write("AT&W\r\n");
    if (strstr(readData(1000), "ERROR") != NULL) {
#ifdef DEBUG_UBIDOTS
        Serial.println(F("Error with AT+SAPBR CONTYPE"));
#endif
        return false;
    }
    _client.write("AT+NJM\r\n");
    if (strstr(readData(1000), "ERROR") != NULL) {
#ifdef DEBUG_UBIDOTS
        Serial.println(F("Error with AT+SAPBR CONTYPE"));
#endif
        return false;
    }
}

bool Ubidots::loraVerify(char* ssid, char* pass) {
    uint8_t flag = 0;
    while (flag < 6) {
        loraConnection(ssid, pass);
        flag++;
        _client.write("AT+NLC\r\n");
    }
}
