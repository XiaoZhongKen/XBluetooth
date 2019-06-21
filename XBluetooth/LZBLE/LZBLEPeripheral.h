//
//  LZBLEPeripheral.h
//  LZMH
//
//  Created by 肖忠肯 on 2019/6/19.
//  Copyright © 2019 xmlzmh. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>


NS_ASSUME_NONNULL_BEGIN

@interface LZService : NSObject

@property (strong, readonly) CBUUID *service;
@property (strong, readonly) CBUUID *writeCharacteristic;
@property (strong, readonly) CBUUID *notifyCharacteristic;

@end



@interface LZBLEPeripheral : NSObject <CBPeripheralDelegate>

@property (strong, readonly) LZService *serviceOfInterest;
@property (strong, nonatomic) CBPeripheral *cperipheral;
@property (nonatomic, copy) NSString *MAC;
@property (nonatomic, copy) NSString *key;

- (instancetype)initWithMac:(NSString *)mac andKey:(NSString *)key;

@end

NS_ASSUME_NONNULL_END
