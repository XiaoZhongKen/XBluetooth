//
//  TimeOutObserver.m
//  XAdvancedOperation
//
//  Created by 肖忠肯 on 2018/8/7.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import "TimeOutObserver.h"
#import "Operation.h"
#import "NSError+OperationErrors.h"

@interface TimeOutObserver()

@property(nonatomic) NSTimeInterval timeout;

@end

@implementation TimeOutObserver

- (NSString *)timeoutKey {
    return @"Timeout";
}

- (instancetype)initWithTimeout:(NSTimeInterval)timeout {
    self = [super init];
    if (self) {
        _timeout = timeout;
    }
    return self;
}

#pragma mark - OperationObserver

- (void)operationDidStart:(Operation *)operation {
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(self.timeout * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        if (!operation.finished && !operation.cancelled) {
            NSError *error = [[NSError alloc] initWithOperationCode:ExecutionFailed userInfo:@{self.timeoutKey: @(self.timeout)}];
            [operation cancelWithError:error];
        }
    });
}

- (void)opeartion:(Operation *)operation didProduceOperation:(NSOperation *)producedOperation {
    
}

- (void)operation:(Operation *)operation didFinishedWithError:(NSArray<NSError *> *)errors {
    
}

@end
