//
//  MutuallyExclusive.m
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/4.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import "MutuallyExclusive.h"

@interface MutuallyExclusive()

@property(nonatomic, copy) NSString *type;

@end

@implementation MutuallyExclusive


- (NSOperation *)dependencyForOperation:(Operation *)operation {
    return nil;
}

- (void)evaluateForOperation:(Operation *)operation completion:(void (^)(NSError *))completion {
    completion(nil);
}

- (instancetype)initWithType:(NSString *)type {
    self = [super init];
    if (self) {
        _type = type;
    }
    return self;
}

- (NSString *)name {
    return [NSString stringWithFormat:@"%@%@", NSStringFromClass([self class]), self.type];
}

- (BOOL)isMutuallyExclusive {
    return YES;
}

@end
