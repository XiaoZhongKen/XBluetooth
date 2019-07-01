//
//  LZBLECentral.m
//  LZMH
//
//  Created by 肖忠肯 on 2019/6/19.
//  Copyright © 2019 xmlzmh. All rights reserved.
//

#import "LZBLECentral.h"
#import <CoreBluetooth/CoreBluetooth.h>
#import <ReactiveObjC/ReactiveObjC.h>

#define kKownPeripherals_MAC_UUID @"kwonperipheralsIdentities"

typedef enum : NSUInteger {
    XBluetoothDefaultMode,
    XBluetoothPromiscuousMode,
} XBluetoothMode;

@interface LZBLECentral ()<CBCentralManagerDelegate>

@property (nonatomic, strong) CBCentralManager *centralManager;
@property (nonatomic, assign) NSInteger bluetoothStateOnSignal;
@property (nonatomic, assign) NSInteger scanPromiscuousSignal;
@property (nonatomic, strong) NSSet *tmpPeripherals;
@property (nonatomic, assign) XBluetoothMode mode;

@end

@implementation LZBLECentral

+ (instancetype)shareCentral {
    static LZBLECentral *central = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        central = [[self alloc] initPrivate];
    });
    return central;
}

- (instancetype)initPrivate {
    self = [super init];
    if (self) {
        self.centralManager = [[CBCentralManager alloc] initWithDelegate:self queue:nil options:@{CBCentralManagerOptionShowPowerAlertKey : @(NO)}];
    }
    return self;
}

// No one should call init
- (instancetype)init {
    @throw [NSException exceptionWithName:@"Singleton" reason:@"there must be one and only central instance, use +[shareCentral] instead" userInfo:nil];
    return nil;
}

- (void)scanPeripherals:(NSSet <LZBLEPeripheral *> *)peripherals {
    
    

    if ([self isBluetoothOn]) {
        
        [self.peripheralsOfInterest addObjectsFromArray:[peripherals allObjects]];
        
        NSInteger storePeripheralCount = 0;
        for (LZBLEPeripheral *p in peripherals) {
            CBPeripheral *cachePeripheral = [self retrievePeripheralWithLZPeripheral:p];
            if (cachePeripheral) {
                storePeripheralCount++;
                p.cperipheral = cachePeripheral;
                [self.centralManager connectPeripheral:cachePeripheral options:nil];
            }
        }
        
        if (storePeripheralCount < peripherals.count) {
            LZBLEPeripheral *peripheral = [peripherals anyObject];
            [self.centralManager scanForPeripheralsWithServices:@[peripheral.serviceOfInterest.service] options:@{CBCentralManagerScanOptionAllowDuplicatesKey: @NO}];
        }
    } else {
        self.tmpPeripherals = peripherals;
        self.bluetoothStateOnSignal = 1;
        
    }
}

- (void)scanPeripheralPromiscuousMode {
    
    self.mode = XBluetoothPromiscuousMode;
    if ([self isBluetoothOn]) {
        
        [self.centralManager scanForPeripheralsWithServices:@[[CBUUID UUIDWithString:@"FF00"]] options:@{CBCentralManagerScanOptionAllowDuplicatesKey: @NO}];
    } else {
        self.scanPromiscuousSignal = 1;
    }
}

- (BOOL)isBluetoothOn {
    if (@available(iOS 10, *)) {
        return CBManagerStatePoweredOn == self.centralManager.state;
    } else {
        return CBCentralManagerStatePoweredOn == self.centralManager.state;
    }
}

- (NSMutableArray *)peripheralsOfInterest {
    if (!_peripheralsOfInterest) {
        _peripheralsOfInterest = [[NSMutableArray alloc] init];
    }
    return _peripheralsOfInterest;
}

#pragma mark - CBCentralManagerDelegate

- (void)centralManagerDidUpdateState:(CBCentralManager *)central {
    NSLog(@"%ld", (long)central.state);
    
    if (self.bluetoothStateOnSignal == 1 && [self isBluetoothOn]) {
        self.bluetoothStateOnSignal = 0;
        
        [self scanPeripherals:self.tmpPeripherals];
    }
    
    if (self.scanPromiscuousSignal == 1 && [self isBluetoothOn]) {
        self.scanPromiscuousSignal = 0;
        [self scanPeripheralPromiscuousMode];
    }
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary<NSString *, id> *)advertisementData RSSI:(NSNumber *)RSSI {
    
    NSString *kCBAdvDataManufacturerData = [[advertisementData objectForKey:CBAdvertisementDataManufacturerDataKey] description];
    if (kCBAdvDataManufacturerData.length > 0) {
        NSString *MAC = [self extractMACFromAdvertisement:kCBAdvDataManufacturerData];
        MAC = [MAC stringByReplacingCharactersInRange:NSMakeRange(0, 1) withString:@"c"];
        MAC = [MAC uppercaseString];
        
        LZBLEPeripheral *didFoundPeripheral = nil;
        
        if (XBluetoothDefaultMode == self.mode) {
            for (LZBLEPeripheral *peripheral in self.peripheralsOfInterest) {
                if (peripheral.MAC.length > 0) {
                    NSString * MAC_dropColons = [peripheral.MAC stringByReplacingOccurrencesOfString:@":" withString:@""];
                    if ([MAC_dropColons isEqualToString:MAC]) {
                        didFoundPeripheral = peripheral;
                        break;
                    }
                }
            }
        } else if (XBluetoothPromiscuousMode == self.mode) {
            didFoundPeripheral = [[LZBLEPeripheral alloc] init];
            NSString *coloned = [self insertColonForMAC:MAC];
            didFoundPeripheral.MAC = coloned;
            [self.peripheralsOfInterest addObject:didFoundPeripheral];
        }

        if (didFoundPeripheral) {
            didFoundPeripheral.cperipheral = peripheral;
            [self.centralManager connectPeripheral:peripheral options:nil];
            [self recordDiscoveredPeripheral:didFoundPeripheral];
        }
    }
}

- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral {
    for (LZBLEPeripheral *per in self.peripheralsOfInterest) {
        if ([per.cperipheral.identifier isEqual:peripheral.identifier]) {
            peripheral.delegate = per;
            [peripheral discoverServices:@[per.serviceOfInterest.service]];
            break;
        }
    }
}

- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error {
    
}

- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(nullable NSError *)error {
    
}



- (CBPeripheral *)retrievePeripheralWithLZPeripheral:(LZBLEPeripheral *)lzPeripheral {
    NSDictionary *MAC_UUID_Pair = [[NSUserDefaults standardUserDefaults] dictionaryForKey:kKownPeripherals_MAC_UUID];
    if (MAC_UUID_Pair.count > 0) {
        NSString *uuidString = MAC_UUID_Pair[lzPeripheral.MAC];
        if (uuidString.length > 0) {
            NSUUID *uuid = [[NSUUID alloc] initWithUUIDString:uuidString];
            NSArray *knownPeripherals = [self.centralManager retrievePeripheralsWithIdentifiers:@[uuid]];
            return knownPeripherals.firstObject;
        }
    }
    return nil;
}


- (void)recordDiscoveredPeripheral:(LZBLEPeripheral *)peripheral {
    NSString *identity = peripheral.cperipheral.identifier.UUIDString;
    
    NSDictionary *dict = [[NSUserDefaults standardUserDefaults] dictionaryForKey:kKownPeripherals_MAC_UUID];
    if (!dict) {
        dict = @{peripheral.MAC : identity};
        [[NSUserDefaults standardUserDefaults] setObject:dict forKey:kKownPeripherals_MAC_UUID];
    } else {
        NSMutableDictionary *mdict = [NSMutableDictionary dictionaryWithDictionary:dict];
        [mdict setValue:identity forKey:peripheral.MAC];
        [[NSUserDefaults standardUserDefaults] setObject:mdict forKey:kKownPeripherals_MAC_UUID];
    }
}

- (NSString *)extractMACFromAdvertisement:(NSString *)ad {
    NSString *tmpStr = [NSString stringWithString:ad];
    tmpStr = [tmpStr stringByReplacingOccurrencesOfString:@" " withString:@""];
    tmpStr = [tmpStr stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"<>"]];
    tmpStr = [tmpStr substringFromIndex:4];
    return tmpStr;
}

- (NSString *)insertColonForMAC:(NSString *)MAC {
    NSMutableString *mStr = [NSMutableString stringWithString:MAC];
    for (int i = 2; i <= mStr.length; i += 3) {
        [mStr insertString:@":" atIndex:i];
    }
    return [[NSString stringWithString:mStr] substringToIndex:mStr.length - 1];
}

@end
