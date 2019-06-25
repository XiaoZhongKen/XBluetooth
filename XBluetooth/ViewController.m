//
//  ViewController.m
//  XBluetooth
//
//  Created by 肖忠肯 on 2019/6/21.
//  Copyright © 2019 肖忠肯. All rights reserved.
//

#import "ViewController.h"
#import "LZBLE/LZBLECentral.h"

@interface ViewController ()

@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *bluetoothActivityIndicator;
@property (weak, nonatomic) IBOutlet UITextView *listing;


@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.bluetoothActivityIndicator.hidden = YES;
    [self.bluetoothActivityIndicator setHidesWhenStopped:YES];
    
}

- (IBAction)start:(id)sender {
    self.bluetoothActivityIndicator.hidden = NO;
    [self.bluetoothActivityIndicator startAnimating];
    
    LZBLEPeripheral *p = [[LZBLEPeripheral alloc] initWithMac:@"C0:15:83:1B:34:F2" andKey:@"1736"];
    
    LZBLEPeripheral *p2 = [[LZBLEPeripheral alloc] initWithMac:@"C0:15:83:3A:11:94" andKey:@"1736"];
    
    LZBLEPeripheral *p3 = [[LZBLEPeripheral alloc] initWithMac:@"C0:C2:A1:60:01:6F" andKey:@"1736"];
    
    NSSet *peripherals = [NSSet setWithObjects:p, p2, p3, nil];
    
    NSMutableString *ms = [[NSMutableString alloc] init];
    for (LZBLEPeripheral *p in peripherals) {
        [ms appendString:[p description]];
        [ms appendFormat:@"\n"];
    }
    self.listing.text = ms;
    [[LZBLECentral shareCentral] scanPeripherals:peripherals];
}

- (IBAction)stop:(id)sender {
    [self.bluetoothActivityIndicator stopAnimating];
    
    
}

@end
