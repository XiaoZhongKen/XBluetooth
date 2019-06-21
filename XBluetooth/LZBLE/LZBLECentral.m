//
//  LZBLECentral.m
//  LZMH
//
//  Created by 肖忠肯 on 2019/6/19.
//  Copyright © 2019 xmlzmh. All rights reserved.
//

#import "LZBLECentral.h"
#import <CoreBluetooth/CoreBluetooth.h>

#define kKownPeripherals_MAC_UUID @"kwonperipheralsIdentities"

@interface LZBLECentral ()<CBCentralManagerDelegate>

@property (nonatomic, strong) CBCentralManager *centralManager;
@property (nonatomic, strong) NSMutableArray *peripheralsOfInterest;

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
            CBPeripheral *storePeripheral = [self retrievePeripheralWithLZPeripheral:p];
            if (storePeripheral) {
                storePeripheralCount++;
                p.cperipheral = storePeripheral;
                [self.centralManager connectPeripheral:storePeripheral options:nil];
            }
        }
        
        if (storePeripheralCount < peripherals.count) {
            LZBLEPeripheral *peripheral = [peripherals anyObject];
            [self.centralManager scanForPeripheralsWithServices:@[peripheral.serviceOfInterest.service] options:@{CBCentralManagerScanOptionAllowDuplicatesKey: @NO}];
        }
    } else {
        
        
        
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
}

- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary<NSString *, id> *)advertisementData RSSI:(NSNumber *)RSSI {
    
    NSString *MAC = [[advertisementData objectForKey:CBAdvertisementDataManufacturerDataKey] description];
    
    LZBLEPeripheral *didFoundPeripheral = [[self.peripheralsOfInterest filteredArrayUsingPredicate:[NSPredicate predicateWithFormat:@"MAC == %@", MAC]] firstObject];
    
    if (didFoundPeripheral) {
        didFoundPeripheral.cperipheral = peripheral;
        [self.centralManager connectPeripheral:peripheral options:nil];
        [self recordDiscoveredPeripheral:didFoundPeripheral];
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

@end
