/*
 * grove_i2c_temp_humi_hdc1000.h
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


#ifndef __GROVE_I2C_TEMP_HUMI_HDC1000_H__
#define __GROVE_I2C_TEMP_HUMI_HDC1000_H__

#include "suli2.h"

//GROVE_NAME        "Grove - I2C TempHumi (HDC1000)"
//SKU               101020112396
//IF_TYPE           I2C
//IMAGE_URL         http://www.seeedstudio.com/wiki/images/thumb/5/5d/Grove_-_Temperature%26Humidity_Sensor_%28HDC1000%29_p1.jpg/500px-Grove_-_Temperature%26Humidity_Sensor_%28HDC1000%29_p1.jpg
//DESCRIPTION       "Driver for HDC1000 de SeeedStudio http://www.seeedstudio.com/wiki/Grove_-_Temperature&Humidity_Sensor_(HDC1000)"
//WIKI_URL          http://www.rizoma.io
//ADDED_AT          "2016-08-08"
//AUTHOR            "Rodrigo Moya Toro - rodrigo@rizoma.io"


#define HDC1000_ADDR                0x40

#define HDC1000_TEMP                0x00
#define HDC1000_HUMI                0x01
#define HDC1000_CONFIG              0x02

#define HDC1000_SERID_1             0xFB
#define HDC1000_SERID_2             0xFC
#define HDC1000_SERID_3             0xFD
#define HDC1000_MFID                0xFE
#define HDC1000_DEVID               0xFF

#define HDC1000_RST                 0x80
#define HDC1000_HEAT_ON             0x20
#define HDC1000_HEAT_OFF            0x00
#define HDC1000_BOTH_TEMP_HUMI      0x10
#define HDC1000_SINGLE_MEASUR       0x00
#define HDC1000_TEMP_HUMI_14BIT     0x00
#define HDC1000_TEMP_11BIT          0x04
#define HDC1000_HUMI_11BIT          0x01
#define HDC1000_HUMI_8BIT           0x02

// MARCA!

class GroveI2CTempHumiHdc1000
{
public:
    GroveI2CTempHumiHdc1000(int pinsda, int pinscl);
    uint16_t getRawTemp(void);
    uint16_t getRawHumi(void);
    /**
     * Read a rough temperature value of the envirenment
     *
     * @param temperature - unit: Celsius degree
     *
     * @return bool
     */
    bool read_temperature(float *temperature);

    /**
     * The basic temperature and humidity sensor will update the reading every 2 seconds.
     *
     * @param humidity - 0~100(%)
     *
     * @return bool
     */    
    bool read_humidity(float *humidity);

    uint16_t readConfig(void);


private:
    I2C_T *i2c;
    uint8_t _addr;
    int _drdyn_pin;
    uint16_t read16();
    void setReadRegister(uint8_t reg);
    void setConfig(uint8_t config);
};

#endif
