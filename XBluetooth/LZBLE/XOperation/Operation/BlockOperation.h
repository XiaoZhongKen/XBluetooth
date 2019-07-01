//
//  BlockOperation.h
//  XAdvancedOperation
//
//  Created by 肖忠肯 on 2018/8/7.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import "Operation.h"

typedef void (^OperationBlock)(void(^continuationBlock)(void));

@interface BlockOperation : Operation

- (instancetype)initWithBlock:(OperationBlock)block;

- (instancetype)initWithMainQueueBlock:(dispatch_block_t)block;

@end
