/* Arduino JPeg Camera Library
 * Copyright 2010 SparkFun Electronics
 * Written by Ryan Owens
 * Modified by arms22
 * April 16th 2014 Modified by Yuuki Okamiya
 * August 1st 2014 Modified by Makoto Kurauchi
 */

#ifndef JPEGCamera_h
#define JPEGCamera_h

//#include <avr/pgmspace.h>
//#include <inttypes.h>
#include <rxduino.h>
//#include "Stream.h"

class JPEGCamera
{
public:
    enum ImageSize {
        IMG_SZ_160x120 = 0x22,
        IMG_SZ_320x240 = 0x11,
        IMG_SZ_640x480 = 0x00,
    };
//    JPEGCamera(Stream &port);
    bool reset(void);
    bool getSize(uint16_t * size);
    bool takePicture(void);
    bool stopPictures(void);
    uint16_t readData(uint8_t * buffer, uint16_t read_size, uint16_t address);
    bool imageSize(JPEGCamera::ImageSize size);
    bool imageSize2(JPEGCamera::ImageSize size);
    bool changeBaudRate(int baudrate);
private:
    uint8_t commandAndResponse(uint8_t cmd_sz,uint8_t res_sz);
    uint8_t recvResponse(uint8_t * response, uint8_t size, uint16_t timeout);
//    Stream &cameraPort;
    uint8_t cmdResBuffer[16];
};

#endif
