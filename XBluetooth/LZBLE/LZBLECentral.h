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

@property (nonatomic, strong) NSMutableArray *peripheralsOfInterest;

+ (instancetype)shareCentral;

- (void)scanPeripherals:(NSSet <LZBLEPeripheral *> *)peripherals;

- (void)scanPeripheralPromiscuousMode;

@end

NS_ASSUME_NONNULL_END
