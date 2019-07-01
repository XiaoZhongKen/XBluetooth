//
//  PeripheralCell.h
//  XBluetooth
//
//  Created by 肖忠肯 on 2019/6/25.
//  Copyright © 2019 肖忠肯. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface PeripheralCell : UITableViewCell

@property (weak, nonatomic) IBOutlet UILabel *macLabel;
@property (weak, nonatomic) IBOutlet UILabel *keyLabel;

@property (nonatomic, copy) void (^actionBlock)(id sender);

@end

NS_ASSUME_NONNULL_END
