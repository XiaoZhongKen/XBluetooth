//
//  LZGateDevice.h
//  XBluetooth
//
//  Created by 肖忠肯 on 2019/6/26.
//  Copyright © 2019 肖忠肯. All rights reserved.
//

#import "LZBLEPeripheral.h"

NS_ASSUME_NONNULL_BEGIN

@interface LZGateDevice : LZBLEPeripheral

- (instancetype)initWithMac:(NSString *)mac key:(NSString *)key andPhone:(NSString *)phone;

- (void)open:(void (^)(NSInteger result, NSError *error))block;

@end

NS_ASSUME_NONNULL_END
