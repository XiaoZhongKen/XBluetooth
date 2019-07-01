//
//  PeripheralCell.m
//  XBluetooth
//
//  Created by 肖忠肯 on 2019/6/25.
//  Copyright © 2019 肖忠肯. All rights reserved.
//

#import "PeripheralCell.h"

@implementation PeripheralCell

- (void)awakeFromNib {
    [super awakeFromNib];
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

- (IBAction)action:(id)sender {
    if (self.actionBlock) {
        self.actionBlock(sender);
    }
}


@end
