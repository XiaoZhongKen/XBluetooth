//
//  GroupOperation.h
//  AOperation
//
//  Created by 肖忠肯 on 2018/8/21.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import "Operation.h"

@interface GroupOperation : Operation

- (instancetype)initWithOperations:(NSArray<NSOperation *> *)operations;

- (void)addOperation:(NSOperation *)operation;

- (void)operationDidFinished:(NSOperation *)operation withError:(NSArray<NSError *> *)errors;

@end
