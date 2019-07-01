//
//  ViewController.m
//  XBluetooth
//
//  Created by 肖忠肯 on 2019/6/21.
//  Copyright © 2019 肖忠肯. All rights reserved.
//

#import "ViewController.h"
#import "LZBLE/LZBLECentral.h"
#import "PeripheralCell.h"

static NSString *const reuseCellIdentifier = @"kPeripheralCell";

@interface ViewController ()<UITableViewDelegate, UITableViewDataSource>

@property (weak, nonatomic) IBOutlet UIActivityIndicatorView *bluetoothActivityIndicator;
@property (weak, nonatomic) IBOutlet UITextView *listing;


@property (weak, nonatomic) IBOutlet UITableView *pTableView;

@property (nonatomic, strong) NSTimer *timer;



@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.bluetoothActivityIndicator.hidden = YES;
    [self.bluetoothActivityIndicator setHidesWhenStopped:YES];
    
    [self.pTableView registerNib:[UINib nibWithNibName:@"PeripheralCell" bundle:nil] forCellReuseIdentifier:reuseCellIdentifier];
    self.pTableView.delegate = self;
    self.pTableView.dataSource = self;
    
    self.pTableView.estimatedRowHeight = 100;
    self.pTableView.rowHeight = UITableViewAutomaticDimension;
    self.pTableView.allowsSelection = NO;
    
    [[LZBLECentral shareCentral] scanPeripheralPromiscuousMode];
    
    self.timer = [NSTimer scheduledTimerWithTimeInterval:2 target:self selector:@selector(updateTable) userInfo:nil repeats:YES];
}

- (void)updateTable {
    [self.pTableView reloadData];
}

- (IBAction)start:(id)sender {
    self.bluetoothActivityIndicator.hidden = NO;
    [self.bluetoothActivityIndicator startAnimating];
    
    LZBLEPeripheral *p = [[LZBLEPeripheral alloc] initWithMac:@"C0:15:83:1B:34:F2"];
    
    LZBLEPeripheral *p2 = [[LZBLEPeripheral alloc] initWithMac:@"C0:15:83:3A:11:94"];
    
    LZBLEPeripheral *p3 = [[LZBLEPeripheral alloc] initWithMac:@"C0:C2:A1:60:01:6F"];
    
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

#pragma mark - UITableViewDatasource

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    
    return [LZBLECentral shareCentral].peripheralsOfInterest.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    PeripheralCell *cell = [tableView dequeueReusableCellWithIdentifier:reuseCellIdentifier];
    LZBLEPeripheral *p = [LZBLECentral shareCentral].peripheralsOfInterest[indexPath.row];
    cell.macLabel.text = p.MAC;
    if (CBPeripheralStateConnected == p.cperipheral.state) {
        cell.backgroundColor = [UIColor greenColor];
    } else {
        cell.backgroundColor = [UIColor grayColor];
    }
    
    cell.actionBlock = ^(id  _Nonnull sender) {
        
    };
    return cell;
}


#pragma mark - UITableViewDelegate


@end
