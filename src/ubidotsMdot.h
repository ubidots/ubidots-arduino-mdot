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
Made by Jose Garcia for Ubidots Inc

Maintainers: Jose Garcia <jose.garcia@ubidots.com>
             Maria Hernandez <maria@ubidots.com>
*/

#include "stdio.h"
#include "stdint.h"
#include <stdlib.h>
#include <FirmataMarshaller.h>
#include <Firmata.h>
#include <FirmataParser.h>
#include <Boards.h>
#include <FirmataDefines.h>
#include <FirmataConstants.h>

#ifndef USER_AGENT
#define USER_AGENT "arduinoLoRa"
#endif
#ifndef VERSION
#define VERSION "2.0"
#endif
#ifndef ANSWERLENGTH
#define ANSWERLENGTH 100
#endif
#ifndef PACKETSIZE
#define PACKETSIZE 11  // The max number of bytes allowed to send by mdot is 11
#endif

typedef struct Value {
  char varValue[PACKETSIZE];
} Value;

class Ubidots {
 public:
    void add(float value, uint8_t length = 4, uint8_t accuracy = 2);
    bool sendAll();
    Ubidots(const char * ssid, const char * pass, uint8_t band = 1);

 private:
    bool loraConnect(uint8_t band);
    bool readGateway(int delay_gateway = 200);
    void populatePacket(char* _packet);
    bool checkConnection();

    const char * _ssid;
    const char * _pass;

    int _index = 0;  // Index to populate char arrays

    uint8_t _maxValues;
    uint8_t _currentValue;
    uint8_t _band;

    Value * _val;
};