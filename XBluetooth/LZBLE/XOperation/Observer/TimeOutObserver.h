//
//  TimeOutObserver.h
//  XAdvancedOperation
//
//  Created by 肖忠肯 on 2018/8/7.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "OperationObserver.h"

@interface TimeOutObserver : NSObject<OperationObserver>

@property(nonatomic, copy, readonly) NSString *timeoutKey;

@end
