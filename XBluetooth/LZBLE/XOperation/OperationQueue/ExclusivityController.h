//
//  ExclusivityController.h
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/4.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import <Foundation/Foundation.h>

@class Operation;

@interface ExclusivityController : NSObject

+ (instancetype)sharedExclusivityController;

- (void)addOperation:(Operation *)operation categories:(NSArray<NSString *> *)categories;

- (void)removeOperation:(Operation *)operation categories:(NSArray<NSString *> *)categories;

@end
