//
//  OperationCondition.h
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/3.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString *const OperationConditionKey;

typedef NS_ENUM(int, OperationConditionResult) {
    Satisfied,
    Fail,
};

@class Operation;

@protocol OperationConditionDelegate

@property(nonatomic, readonly, copy) NSString *name;
@property(nonatomic, readonly) BOOL isMutuallyExclusive;

- (NSOperation *)dependencyForOperation:(Operation *)operation;

- (void)evaluateForOperation:(Operation *)operation completion:(void (^)(NSError *))completion;

@end

@interface OperationConditionEvaluator : NSObject

+ (void)evaluateCondition:(NSArray<id <OperationConditionDelegate>> *)conditions operation:(Operation *)operation completion:(void (^)(NSArray<NSError *> *))completion;

@end

@interface OperationCondition: NSObject

@end
