//
//  BlockObserver.h
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/4.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "OperationObserver.h"

@interface BlockObserver : NSObject<OperationObserver>

- (instancetype)initWithStartHandler:(void (^)(Operation *op))start produceHandler:(void (^)(Operation *originalOp, NSOperation *producedOp))produce finishHandler:(void (^)(Operation *op, NSArray<NSError *> *errors))finish;

@end
