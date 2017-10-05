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
Made by Mateo Velez - Jose Garcia for Ubidots Inc

Maintainers: Jose Garcia <jose.garcia@ubidots.com>
             Maria Hernandez <maria@ubidots.com>
*/

#include "ubidotsMdot.h"

/**
 * Constructor.
 */
Ubidots::Ubidots(const char * ssid, const char * pass, uint8_t band) {
    _maxValues = 3;
    _currentValue = 0;
    _val = (Value *)malloc(_maxValues*sizeof(Value));
    _ssid = ssid;
    _pass = pass;
    _band = band;
}

/**
 * Attempts to make JOIN to the lora Network
 */

bool Ubidots::loraConnect(uint8_t band) {
    // Checks if AT commands are available
    Serial1.print("AT\r\n");
    if(!readGateway()){
        return false;
    }

    // Sets the ssid
    Serial1.print("AT+NI=1,");
    Serial1.print(_ssid);
    Serial1.print("\r\n");
    if(!readGateway()){
        return false;
    }

    // Sets the passphrase to connect
    Serial1.print("AT+NK=1,");
    Serial1.print(_pass);
    Serial1.print("\r\n");
    if(!readGateway()){
        return false;
    }

    // Sets the band number
    Serial1.print("AT+FSB=");
    Serial1.print(band);
    Serial1.print("\r\n");
    if(!readGateway()){
        return false;
    }

    // Attempts JOIN
    Serial1.print("AT+JOIN\r\n");
    delay(5000);
    if(!readGateway()){
        return false;
    }

    return true;
}

/**
 * Add a value of variable to save
 * @arg value variable value to save in a struct
 */

void Ubidots::add(float value, uint8_t length, uint8_t accuracy) {
    _currentValue++;
    dtostrf(value, length, accuracy, (_val + _currentValue)->varValue);
    Serial.println((_val + _currentValue)->varValue);
}

/**
 * Send all data of all variables that you saved
 * @return true upon success, false upon error.
 */
char* Ubidots::populatePacket() {
    char* _packet = (char *) malloc(sizeof(char) * PACKETSIZE);

    if (_currentValue > _maxValues) {
        _currentValue = 0;
        sprintf(_packet, "ERROR");
        return _packet;
    }

    sprintf(_packet, "");
    for (uint8_t i = 1; i <= _currentValue; i++){
        if (i == 1){
            sprintf(_packet, "%s", (_val + i)->varValue);
        }
        else{
            sprintf(_packet, "%s,%s", _packet, (_val + i)->varValue);
        }
    }
    Serial.println(_packet);
    _currentValue = 0;
    return _packet;
}

bool Ubidots::sendAll(){
    bool connected = checkConnection();
    char* packet = populatePacket();
    Serial.print("packet build: ");
    Serial.println(packet);
    if (strstr(packet, "ERROR") != NULL || strlen(packet) > PACKETSIZE){
        return false;  // Could not populate the array or max bytes size exceed
    }

    int timeout = 0;
    while (!connected && timeout < 5){
        loraConnect(_band);  //Attempts to connect
        connected = checkConnection();
        timeout++;
    }

    if (!connected){
        return false;  // The device is not connected
    }

    Serial.println("sending values as: ");
    Serial.println(packet);
    Serial1.print("AT+SEND=");
    Serial1.print(packet);
    Serial1.print("\r\n");

    if(!readGateway(1000)){
        return false;
    }

    Serial1.flush();
    delay(100);
    return true;
}


/************ Auxiliar Function *******************/

/**
 * Reads the answer obtained from the Serial Port
 */

bool Ubidots::readGateway(int delay_gateway){
    bool result = true;
    char* _gatewayAnswer = (char *) malloc(sizeof(char) * ANSWERLENGTH);
    for (uint8_t i = 0; i < 2; i++){
        while(Serial1.available() > 0){
            if (_index < ANSWERLENGTH - 1){
                _gatewayAnswer[_index++] = (char)Serial1.read();
                _gatewayAnswer[_index] = '\0';
            }
        }
        _index = 0;

        if (strstr(_gatewayAnswer, "ERROR") != NULL){
            result = false;
        }
        delay(delay_gateway);
    }
    free(_gatewayAnswer);
    return result;
}

/**
 * Checks if connection is available, returns false if it is not joined
 */

bool Ubidots::checkConnection(){
    Serial1.print("AT+NJS");
    Serial1.print("\r\n");
    delay(100);

    char* _gatewayAnswer = (char *) malloc(sizeof(char) * ANSWERLENGTH);
    while(Serial1.available() > 0){
        if (_index < ANSWERLENGTH - 1){
            _gatewayAnswer[_index++] = (char)Serial1.read();
            _gatewayAnswer[_index] = '\0';
        }
    }
    _index = 0;
    Serial.println("answer: ");
    Serial.println(_gatewayAnswer);

    if (strstr(_gatewayAnswer, "0") != NULL){
        free(_gatewayAnswer);
        return false;
    }

    free(_gatewayAnswer);
    return true;
}