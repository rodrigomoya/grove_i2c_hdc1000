/*
 * grove_i2c_temp_humi_hdc1000.cpp
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Lambor, Jack
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "suli2.h"
#include "grove_i2c_temp_humi_hdc1000.h" 


/*comment*/

GroveI2CTempHumiHdc1000::GroveI2CTempHumiHdc1000(int pinsda, int pinscl)
{
    this->i2c = (I2C_T *)malloc(sizeof(I2C_T));
    suli_i2c_init(i2c, pinsda, pinscl);
    _addr = HDC1000_ADDR;

    Wire.begin();
}

bool GroveI2CTempHumiHdc1000::read_temperature(float *temperature)
{
    uint8_t config = HDC1000_SINGLE_MEASUR|HDC1000_TEMP_11BIT|HDC1000_HEAT_ON;
    Wire.beginTransmission(_addr);
    Wire.write(HDC1000_CONFIG); //accessing the configuration register
    Wire.write(config); //sending the config bits (MSB)
    Wire.write(0x00); //the last 8 bits must be zeroes
    Wire.endTransmission();

    Wire.beginTransmission(_addr);
    Wire.write(HDC1000_TEMP);
    Wire.endTransmission();

    delay(30);

    uint8_t bytes = 2;
    uint16_t dest;

    Wire.requestFrom(_addr, bytes);
    if(Wire.available()>=bytes){
      dest = Wire.read()<<8;
      dest += Wire.read();
    }

    double temp = dest;
    *temperature = ((temp/65536.0)*165.0)-40.0;

    return true;
}

bool GroveI2CTempHumiHdc1000::read_humidity(float *humidity)
{
    // Configuracion 
    uint8_t config = HDC1000_SINGLE_MEASUR|HDC1000_HUMI_11BIT|HDC1000_HEAT_ON;
    Wire.beginTransmission(_addr);
    Wire.write(HDC1000_CONFIG); //accessing the configuration register
    Wire.write(config); //sending the config bits (MSB)
    Wire.write(0x00); //the last 8 bits must be zeroes
    Wire.endTransmission();

    delay(30);

    Wire.beginTransmission(_addr);
    Wire.write(HDC1000_HUMI);
    Wire.endTransmission();

    uint8_t bytes = 2;
    uint16_t dest;

    Wire.requestFrom(_addr, bytes);
    if(Wire.available()>=bytes){
      dest = Wire.read()<<8;
      dest += Wire.read();
    }

    double humi = ((humi/65536.0)*100.0);

    if (humi >= 20 && humi <= 30) {      
      // - 1
      humi = humi - 1.0;
    } else if (humi >= 31 && humi <= 40) {      
      // - 5
      humi = humi - 5.0;
    } else if (humi >= 41 && humi <= 50) {      
      // - 10    
      humi = humi - 10.0;
    } else if (humi > 51 && humi < 60) {      
      // - 15 
      humi = humi - 15.0;
    } else if (humi > 71 && humi < 80){      
      // - 10
      humi = humi - 10.0;
    } else if (humi > 81 && humi < 90){                
      // - 5      
      humi = humi - 5.0;
    }

    *humidity = humi;

    return true;
}
