//
//  MutuallyExclusive.h
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/4.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "OperationCondition.h"

@interface MutuallyExclusive : NSObject<OperationConditionDelegate>

@property(nonatomic, readonly, copy) NSString *name;
@property(nonatomic, readonly) BOOL isMutuallyExclusive;

- (instancetype)initWithType:(NSString *)type;

@end
