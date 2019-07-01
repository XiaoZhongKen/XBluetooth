//
//  LZGateDevice.m
//  XBluetooth
//
//  Created by 肖忠肯 on 2019/6/26.
//  Copyright © 2019 肖忠肯. All rights reserved.
//

#import "LZGateDevice.h"
#import "DataSymthesizer/lzProtocol.h"

static NSString *const LZBluetoothProtocol = @"LZBluetoothProtocol";

@interface LZGateDevice ()

@property (nonatomic, copy) NSString *phone;
@property (nonatomic, copy) NSString *key;

@end

@implementation LZGateDevice

- (instancetype)initWithMac:(NSString *)mac key:(NSString *)key andPhone:(NSString *)phone {
    self = [super initWithMac:mac];
    if (self) {
        _key = key;
        _phone = phone;
    }
    return self;
}

- (void)open:(void (^)(NSInteger result, NSError *error))block {
    
    LZBLEProtocol p = openDoorForDualMode([self.phone cStringUsingEncoding:NSUTF8StringEncoding], [self.key cStringUsingEncoding:NSUTF8StringEncoding]);
    
    NSData *data = [[NSData alloc] initWithBytes:p.data length:p.len];
    
    if (CBPeripheralStateConnected == self.cperipheral.state) {
        
        [self writeData:data withResponseHandler:^(NSData * _Nonnull returnData) {
            if (block) {
                const uint8_t *protocol = (const uint8_t *)[returnData bytes];
                LZBLEParseResult result = resolveProtocol(protocol, [returnData length]);
                if (result.isValid) {
                    if (result.value == 1) {
                        block(1, nil);
                    } else if (result.value == 2) {
                        NSError *error = [[NSError alloc] initWithDomain:LZBluetoothProtocol code:2 userInfo:@{NSLocalizedDescriptionKey: @"opendoor failure"}];
                        block(0, error);
                    } else if (result.value == 3) {
                        NSError *error = [[NSError alloc] initWithDomain:LZBluetoothProtocol code:2 userInfo:@{NSLocalizedDescriptionKey: @"KEY error"}];
                        block(0, error);
                    } else {
                        NSError *error = [[NSError alloc] initWithDomain:LZBluetoothProtocol code:2 userInfo:@{NSLocalizedDescriptionKey: @"unknown error"}];
                        block(0, error);
                    }
                } else {
                    NSError *error = [[NSError alloc] initWithDomain:LZBluetoothProtocol code:2 userInfo:@{NSLocalizedDescriptionKey: @"protocol fcs invalid"}];
                    block(0, error);
                }
            }
        }];
    }
}


@end
