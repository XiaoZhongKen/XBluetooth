//
//  GroupOperation.m
//  AOperation
//
//  Created by 肖忠肯 on 2018/8/21.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import "GroupOperation.h"
#import "OperationQueue.h"

@interface GroupOperation()<OperationQueueDelegate>

@property(strong, nonatomic) OperationQueue *internalQueue;
@property(strong, nonatomic) NSBlockOperation *startingOperation;
@property(strong, nonatomic) NSBlockOperation *finishingOperation;

@property(strong, nonatomic) NSMutableArray *aggregateErrors;

@end

@implementation GroupOperation

- (instancetype)initWithOperations:(NSArray<NSOperation *> *)operations {
    self = [super init];
    if (self) {
        _internalQueue = [[OperationQueue alloc] init];
        _internalQueue.suspended = YES;
        _internalQueue.delegate = self;
        _aggregateErrors = [[NSMutableArray alloc] init];
        _startingOperation = [NSBlockOperation blockOperationWithBlock:^{
            
        }];
        
        _finishingOperation = [NSBlockOperation blockOperationWithBlock:^{
            
        }];
        
        [_internalQueue addOperation:_startingOperation];
        for (NSOperation *operation in operations) {
            [_internalQueue addOperation:operation];
        }
    }
    return self;
}

- (void)cancel {
    [self.internalQueue cancelAllOperations];
    [super cancel];
}

- (void)execute {
    self.internalQueue.suspended = NO;
    [self.internalQueue addOperation:self.finishingOperation];
}

- (void)addOperation:(NSOperation *)operation {
    [self.internalQueue addOperation:operation];
}

- (void)aggregateError:(NSError *)error {
    [self.aggregateErrors addObject:error];
}

- (void)operationDidFinished:(NSOperation *)operation withError:(NSArray<NSError *> *)errors {
    
}

#pragma mark - OperationQueueDelegate

- (void)operationQueue:(OperationQueue *)operationQueue willAddOperation:(NSOperation *)operation {
    NSAssert(!self.finishingOperation.finished && !self.finishingOperation.executing, @"cannot add new operations to a group after the group has completed");
    
    if (operation != self.finishingOperation) {
        [self.finishingOperation addDependency:operation];
    }
    
    if (operation != self.startingOperation) {
        [operation addDependency:self.startingOperation];
    }
}

- (void)operationQueue:(OperationQueue *)operationQueue didFinishOperation:(NSOperation *)operation withErrors:(NSArray<NSError *> *)errors {
    [self.aggregateErrors addObjectsFromArray:errors];
    
    if (operation == self.finishingOperation) {
        self.internalQueue.suspended = YES;
        [self finish:self.aggregateErrors];
    } else if (operation != self.startingOperation) {
        [self operationDidFinished:operation withError:errors];
    }
}

@end
