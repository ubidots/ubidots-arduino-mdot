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

#ifndef __UbidotsMdot_H_
#define __UbidotsMdot_H_
#define DEBUG_UBIDOTS
#define BAOUD_RATE 115200

#include "Arduino.h"
#include <SoftwareSerial.h>

class Ubidots{
 private:
        SoftwareSerial _client = SoftwareSerial(10, 11);
        char* readData(uint16_t timeout);
        bool loraConnection(char* ssid, char* pass);
        void flushInput();

 public:
        Ubidots();
        bool loraVerify(char* ssid, char* pass);
        bool loraSend(float data, byte name, char* band);
};
#endif  // _UbidotsMdot_H_