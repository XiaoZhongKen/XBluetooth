//
//  NSError+OperationErrors.m
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/4.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import "NSError+OperationErrors.h"

static NSString *OperationErrorDomain = @"OperationErrors";

@implementation NSError (OperationErrors)

- (NSError *)initWithOperationCode:(OperationErrorCode)code userInfo:(NSDictionary *)userInfo {
    self = [self initWithDomain:OperationErrorDomain code:code userInfo:userInfo];
    return self;
}

@end
