//
//  lzProtocol.h
//  XBluetooth
//
//  Created by 肖忠肯 on 2019/6/28.
//  Copyright © 2019 肖忠肯. All rights reserved.
//

#ifndef lzProtocol_h
#define lzProtocol_h

#include <stdio.h>

#endif /* lzProtocol_h */

typedef struct {
    uint8_t data[30];
    uint8_t len;
} LZBLEProtocol;

typedef struct {
    uint8_t isValid;
    uint8_t type;
    uint8_t value;
} LZBLEParseResult;

LZBLEProtocol openDoorForSingleMode(const char *phone, const char *key);
LZBLEProtocol openDoorForDualMode(const char *phone, const char *key);

LZBLEParseResult resolveProtocol(const uint8_t *protocol, unsigned long len);
