//
//  OperationQueue.m
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/4.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import "OperationQueue.h"
#import "Operation.h"
#import "BlockObserver.h"
#import "OperationCondition.h"
#import "ExclusivityController.h"

@implementation OperationQueue

- (void)addOperation:(NSOperation *)op {
    if ([op isKindOfClass:[Operation class]]) {
        Operation *operation = (Operation *)op;
        BlockObserver *delegate = [[BlockObserver alloc] initWithStartHandler:nil produceHandler:^(Operation *originalOp, NSOperation *producedOp) {
            [self addOperation:producedOp];
        } finishHandler:^(Operation *op, NSArray<NSError *> *errors) {
            [self.delegate operationQueue:self didFinishOperation:op withErrors:errors];
        }];
        [operation addObserver:delegate];
        
        NSMutableArray *dependencies = [[NSMutableArray alloc] init];
        NSArray *conditions = operation.conditions;
        for (id<OperationConditionDelegate> condition in conditions) {
            NSOperation *producedOperation = [condition dependencyForOperation:operation];
            if (producedOperation) {
                [dependencies addObject:producedOperation];
            }
        }
        
        for (NSOperation *dependency in dependencies) {
            [operation addDependency:dependency];
            
            [self addOperation:dependency];
        }
        
        /// MutuallyExclusive
        
        NSMutableArray<NSString *> *concurencyCategories = [[NSMutableArray alloc] init];
        for (id<OperationConditionDelegate> condition in conditions) {
            if (condition.isMutuallyExclusive) {
                [concurencyCategories addObject:condition.name];
            }
        }
        
        if (concurencyCategories.count != 0) {
            ExclusivityController *exclusivityController = [ExclusivityController sharedExclusivityController];
            [exclusivityController addOperation:operation categories:concurencyCategories];
            
            BlockObserver *observer = [[BlockObserver alloc] initWithStartHandler:nil produceHandler:nil finishHandler:^(Operation *op, NSArray<NSError *> *errors) {
                [exclusivityController removeOperation:op categories:concurencyCategories];
            }];
            [operation addObserver:observer];
        }
        
        [operation willEnqueue];

    } else {
        __weak OperationQueue *weakSelf = self;
        __weak NSOperation *weakOp = op;
        op.completionBlock = ^{
            if (!weakSelf || !weakOp) {
                return;
            }
            [weakSelf.delegate operationQueue:weakSelf didFinishOperation:weakOp withErrors:nil];
        };
    }
    
    [self.delegate operationQueue:self willAddOperation:op];
    [super addOperation:op];
}

@end
