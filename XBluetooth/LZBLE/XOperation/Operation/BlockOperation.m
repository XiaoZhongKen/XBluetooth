//
//  BlockOperation.m
//  XAdvancedOperation
//
//  Created by 肖忠肯 on 2018/8/7.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import "BlockOperation.h"

@interface BlockOperation()

@property(nonatomic, copy) OperationBlock block;

@end

@implementation BlockOperation

- (instancetype)initWithBlock:(OperationBlock)block {
    self = [super init];
    if (self) {
        self.block = block;
    }
    return self;
}

- (instancetype)initWithMainQueueBlock:(dispatch_block_t)block {
    self = [self initWithBlock:^(void (^continuation)(void)) {
        dispatch_async(dispatch_get_main_queue(), ^{
            block();
            continuation();
        });
    }];
    return self;
}

- (void)execute {
    if (!self.block) {
        [self finish:nil];
        return;
    }
    self.block(^{
        [self finish:nil];
    });
}


@end
