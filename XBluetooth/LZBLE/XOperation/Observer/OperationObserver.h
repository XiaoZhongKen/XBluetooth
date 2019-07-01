//
//  OperationObserver.h
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/4.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#ifndef OperationObserver_h
#define OperationObserver_h

@class Operation;

@protocol OperationObserver

- (void)operationDidStart:(Operation *)operation;
- (void) opeartion:(Operation *)operation didProduceOperation:(NSOperation *)producedOperation;
- (void) operation:(Operation *)operation didFinishedWithError:(NSArray<NSError *> *)errors;

@end


#endif /* OperationObserver_h */
