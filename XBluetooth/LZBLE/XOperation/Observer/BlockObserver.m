//
//  BlockObserver.m
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/4.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import "BlockObserver.h"

@interface BlockObserver()

@property(nonatomic, copy) void (^startHandler)(Operation *);
@property(nonatomic, copy) void (^produceHandler)(Operation *, NSOperation *) ;
@property(nonatomic, copy) void (^finishHandler)(Operation *, NSArray<NSError *> *);

@end

@implementation BlockObserver

- (instancetype)initWithStartHandler:(void (^)(Operation *))start produceHandler:(void (^)(Operation *, NSOperation *))produce finishHandler:(void (^)(Operation *, NSArray<NSError *> *))finish {
    self = [super init];
    if (self) {
        self.startHandler = start;
        self.produceHandler = produce;
        self.finishHandler = finish;
    }
    return self;
}


#pragma mark - OperationObserver

- (void)operationDidStart:(Operation *)operation {
    if (self.startHandler) {
        self.startHandler(operation);
    }
}

- (void)opeartion:(Operation *)operation didProduceOperation:(NSOperation *)producedOperation {
    if (self.produceHandler) {
        self.produceHandler(operation, producedOperation);
    }
}

- (void) operation:(Operation *)operation didFinishedWithError:(NSArray<NSError *> *)errors {
    if (self.finishHandler) {
        self.finishHandler(operation, errors);        
    }
}

@end
