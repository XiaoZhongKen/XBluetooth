//
//  lzProtocol.c
//  XBluetooth
//
//  Created by 肖忠肯 on 2019/6/28.
//  Copyright © 2019 肖忠肯. All rights reserved.
//

#include "lzProtocol.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static void strbcd(const char *decimalStr, uint8_t *bcd);

static uint8_t fcsModularSum(uint8_t *src, int len);

static LZBLEProtocol generalProtocolWithAddr(uint8_t cmdType, uint8_t sendAddr[4], uint8_t receiveAddr[4], LZBLEProtocol p);

static LZBLEProtocol generalProtocolWithoutAddr(uint8_t cmdType, LZBLEProtocol p);

static int validateFCS(const uint8_t *protocol, unsigned long len);

LZBLEProtocol openDoorForSingleMode(const char *phone, const char *key) {
    uint8_t tmpStream[12] = {0};
    tmpStream[0] = 0x01; // bluetooth open door
    memcpy(tmpStream+1, key, sizeof(char) * strlen(key));
    uint8_t phoneBCD[6] = {0};
    strbcd(phone, phoneBCD);
    memcpy(tmpStream+5, phoneBCD, 6);
    tmpStream[11] = 0x01;
    
    LZBLEProtocol protocol = {};
    memcpy(protocol.data, tmpStream, 12);
    protocol.len = 12;
    
    //    for (int i = 0; i < protocol.len; i++) {
    //        printf("%x ", protocol.data[i]);
    //    }
    
    uint8_t receiveAddr[] = { 0x0a, 0x0a, 0x0a, 0x0a };
    return generalProtocolWithAddr(0x40, NULL, receiveAddr, protocol);
}

LZBLEProtocol openDoorForDualMode(const char *phone, const char *key) {
    uint8_t tmpStream[12] = {0};
    tmpStream[0] = 0x01; // bluetooth open door
    memcpy(tmpStream+1, key, sizeof(char) * strlen(key));
    uint8_t phoneBCD[6] = {0};
    strbcd(phone, phoneBCD);
    memcpy(tmpStream+5, phoneBCD, 6);
    tmpStream[11] = 0x01;
    
    LZBLEProtocol protocol = {};
    memcpy(protocol.data, tmpStream, 12);
    protocol.len = 12;
    
    return generalProtocolWithoutAddr(0x40, protocol);
}

LZBLEParseResult resolveProtocol(const uint8_t *protocol, unsigned long len) {
    
    LZBLEParseResult result = {};
    if (validateFCS(protocol, len) == 1) {
        result.isValid = 1;
        result.type = protocol[11];
        result.value = protocol[len-2];
    } else {
        result.isValid = 0;
    }
    return result;
}

static int validateFCS(const uint8_t *protocol, unsigned long len) {
    uint8_t sum = 0;
    if (len == protocol[1]) {
        for (int i = 0; i < len - 1; i++) {
            sum += protocol[i];
        }
        return sum == protocol[len-1];
    } else {
        return 0;
    }
}


static void strbcd(const char *decimalStr, uint8_t *bcd) {
    
    long len = strlen(decimalStr);
    long remindLen = len;
    long desLen = ceill(len / 2.0);
    long descIndex = desLen - 1;
    
    while (remindLen > 0) {
        if (remindLen > 1) {
            remindLen -= 2;
            char octectStr[3] = "\0";
            memcpy(octectStr, &decimalStr[remindLen], 2);
            uint8_t decLong = (int8_t)(strtol(octectStr, NULL, 10));
            uint8_t packedBCDByte = (((decLong / 10) & 0x0f) << 4) | ((decLong % 10) & 0x0f);
            bcd[descIndex--] = packedBCDByte;
        } else if (remindLen == 1) {
            remindLen -= 1;
            char octectStr[3] = "\0";
            memcpy(octectStr, &decimalStr[remindLen], 1);
            long decLong = strtol(octectStr, NULL, 10);
            int8_t packedBCDByte = (decLong % 10) & 0x0f;
            bcd[descIndex--] = packedBCDByte;
        }
    }
}

static uint8_t fcsModularSum(uint8_t *src, int len) {
    uint8_t sum = 0;
    for (int i = 0; i < len; i++) {
        sum += src[i];
    }
    return sum;
}

static LZBLEProtocol generalProtocolWithAddr(uint8_t cmdType, uint8_t sendAddr[4], uint8_t receiveAddr[4], LZBLEProtocol p) {
    // 0xBD, len, cmdType, 4 * sendAddr, 4 * receiveAddr, n * data, fcs
    uint8_t tmp[128] = {0};
    tmp[0] = 0xBD; // frame start
    tmp[1] = 12 + p.len;
    tmp[2] = cmdType; // cmd type
    if (sendAddr == NULL) {
        memset(tmp+3, 0, 4); // sender addr
    } else {
        memcpy(tmp+3, sendAddr, 4);
    }
    
    if (receiveAddr == NULL) {
        uint8_t receiveAddr[] = { 0x00, 0x00, 0x10, 0x01 };
        memcpy(tmp+7, receiveAddr, 4); // receiver addr
    }
    memcpy(tmp+7, receiveAddr, 4); // receiver addr
    
    memcpy(tmp+11, p.data, p.len); // spec
    uint8_t fcs = fcsModularSum(tmp, tmp[1] - 1);
    memset(tmp+11+p.len, fcs, 1);
    
    LZBLEProtocol com = {};
    memcpy(com.data, tmp, tmp[1]);
    com.len = tmp[1];
    return com;
}

static LZBLEProtocol generalProtocolWithoutAddr(uint8_t cmdType, LZBLEProtocol p) {
    uint8_t tmp[128] = {0};
    tmp[0] = 0xBD; // frame start
    tmp[1] = 4 + p.len;
    tmp[2] = cmdType; // cmd type
    memcpy(tmp+3, p.data, p.len);
    uint8_t fcs = fcsModularSum(tmp, tmp[1]-1);
    memset(tmp+3+p.len, fcs, 1);
    LZBLEProtocol com = {};
    memcpy(com.data, tmp, tmp[1]);
    com.len = tmp[1];
    return com;
}
