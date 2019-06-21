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
    
    LZBLEPeripheral *p = [[LZBLEPeripheral alloc] initWithMac:@"00:15:83:1B:34:F2" andKey:@"1736"];
    NSString *desc = [NSString stringWithFormat:@"MAC: %@\nKEY: %@", p.MAC, p.key];
    self.listing.text = desc;
    [[LZBLECentral shareCentral] scanPeripherals:[NSSet setWithObject:p]];
}

- (IBAction)stop:(id)sender {
    [self.bluetoothActivityIndicator stopAnimating];
    
    
}

@end
