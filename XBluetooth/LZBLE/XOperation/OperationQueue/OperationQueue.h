//
//  OperationQueue.h
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/4.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import <Foundation/Foundation.h>

@class OperationQueue;
@protocol OperationQueueDelegate

- (void)operationQueue:(OperationQueue *)operationQueue willAddOperation:(NSOperation *)operation;

- (void)operationQueue:(OperationQueue *)operationQueue didFinishOperation:(NSOperation *)operation withErrors:(NSArray<NSError *> *)errors;

@end

@interface OperationQueue : NSOperationQueue

@property(weak, nonatomic) id<OperationQueueDelegate> delegate;

@end
