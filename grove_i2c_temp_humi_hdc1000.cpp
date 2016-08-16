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
    uint8_t config = HDC1000_SINGLE_MEASUR|HDC1000_TEMP_11BIT|HDC1000_HEAT_ON;
    float offset;
    float humi_t;

    Wire.beginTransmission(_addr);
    Wire.write(HDC1000_CONFIG); //accessing the configuration register
    Wire.write(config); //sending the config bits (MSB)
    Wire.write(0x00); //the last 8 bits must be zeroes
    Wire.endTransmission();

    Wire.beginTransmission(_addr);
    Wire.write(HDC1000_HUMI);
    Wire.endTransmission();

    delay(30);

    uint8_t bytes = 2;
    uint16_t dest;

    Wire.requestFrom(_addr, bytes);
    if(Wire.available()>=bytes){
      dest = Wire.read()<<8;
      dest += Wire.read();
    }

    double humi = dest;
    humi_t = ((humi/65536.0)*100.0);

    if ((humi_t >= 20.00) && (humi_t <= 30.00)) {      
      offset = 5.50;
    } else if ((humi_t >= 31.00) && (humi_t <= 40.00)) {      
      offset = 10.50;
    } else if ((humi_t >= 41.00) && (humi_t <= 50.00)) {      
      offset = 12.50;
    } else if ((humi_t >= 51.00) && (humi_t <= 60.00)) {      
      offset = 16.50;
    } else if ((humi_t >= 61.00) && (humi_t <= 70.00)) {      
      offset = 18.50;
    } else if ((humi_t >= 71.00) && (humi_t <= 80.00)){      
      offset = 16.50;
    } else if ((humi_t >= 81.00) && (humi_t <= 90.00)){                
      offset = 5.50;
    } else {
      offset = 0.00;
    }

    *humidity = ((humi/65536.0)*100.0)-offset;

    return true;
}
