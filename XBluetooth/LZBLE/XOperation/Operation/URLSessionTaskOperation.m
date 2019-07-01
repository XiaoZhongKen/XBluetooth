//
//  URLSessionTaskOperation.m
//  AOperation
//
//  Created by 肖忠肯 on 2018/8/22.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import "URLSessionTaskOperation.h"

static int URLSessionTaksOperationKVOContext = 0;

@implementation URLSessionTaskOperation

- (instancetype)initWithTask:(NSURLSessionTask *)task {
    NSAssert(task.state == NSURLSessionTaskStateSuspended, @"Tasks must be suspended.");
    
    self = [super init];
    if (self) {
        _task = task;
    }
    return self;
}

- (void)execute {
    NSString *warning = [NSString stringWithFormat:@"Task was resumed by something other than %@.", self];
    NSAssert(self.task.state == NSURLSessionTaskStateSuspended, warning);
    
    [self.task addObserver:self forKeyPath:@"state" options:0 context:&URLSessionTaksOperationKVOContext];
    [self.task resume];
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary<NSKeyValueChangeKey,id> *)change context:(void *)context {
    if (context != &URLSessionTaksOperationKVOContext) {
        return;
    }
    if (object == self.task && [keyPath isEqualToString:@"state"] && self.task.state == NSURLSessionTaskStateCompleted) {
        [self.task removeObserver:self forKeyPath:@"state"];
        [self finish:nil];
    }
}

- (void)cancel {
    [self.task cancel];
    [super cancel];
}

@end
