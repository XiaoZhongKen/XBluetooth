//
//  NSError+OperationErrors.h
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/4.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(int, OperationErrorCode) {
    ConditionFailed,
    ExecutionFailed
};

@interface NSError (OperationErrors)

- (NSError *)initWithOperationCode:(OperationErrorCode)code userInfo:(NSDictionary *)userInfo;

@end
