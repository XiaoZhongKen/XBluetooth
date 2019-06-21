//
//  LZBLECentral.h
//  LZMH
//
//  Created by 肖忠肯 on 2019/6/19.
//  Copyright © 2019 xmlzmh. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LZBLEPeripheral.h"

NS_ASSUME_NONNULL_BEGIN

@interface LZBLECentral : NSObject

+ (instancetype)shareCentral;

- (void)scanPeripherals:(NSSet <LZBLEPeripheral *> *)peripherals;

@end

NS_ASSUME_NONNULL_END
