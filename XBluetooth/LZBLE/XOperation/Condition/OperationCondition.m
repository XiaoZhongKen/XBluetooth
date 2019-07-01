//
//  OperationCondition.m
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/3.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import "OperationCondition.h"
#import "Operation.h"
#import "NSError+OperationErrors.h"

NSString *const OperationConditionKey = @"OperationCondition";

@implementation OperationCondition

@end

@implementation OperationConditionEvaluator

+ (void)evaluateCondition:(NSArray<id <OperationConditionDelegate>> *)conditions operation:(Operation *)operation completion:(void (^)(NSArray<NSError *> *))completion {
    dispatch_group_t conditionGroup = dispatch_group_create();
    NSMutableArray *results = [NSMutableArray arrayWithCapacity:conditions.count];
    for (int i = 0; i < conditions.count; i++) {
        dispatch_group_enter(conditionGroup);
        [conditions[i] evaluateForOperation:operation completion:^(NSError * result) {
            if (result) {
                results[i] = result;
            }
            dispatch_group_leave(conditionGroup);
        }];
    }
    
    dispatch_group_notify(conditionGroup, dispatch_get_global_queue(QOS_CLASS_DEFAULT, 0), ^{
        if (operation.cancelled) {
            [results addObject:[[NSError alloc] initWithOperationCode:ConditionFailed userInfo:nil]];
        }
        completion(results);
    });
}

@end
