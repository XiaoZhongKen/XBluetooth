//
//  Operation.h
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/3.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "NSError+OperationErrors.h"

@protocol OperationConditionDelegate, OperationObserver;

@interface Operation : NSOperation

@property(strong, readonly) NSMutableArray *conditions;

@property(strong, readonly) NSMutableArray *observers;

- (void)addObserver:(id <OperationObserver>)observer;

- (void)addCondition:(id <OperationConditionDelegate>)condition;

- (void)willEnqueue;

- (void)execute;

- (void)finish:(NSArray<NSError *> *)errors;

- (void)cancelWithError:(NSError *)error;

- (void)produceOperation:(NSOperation *)operation;

@end
