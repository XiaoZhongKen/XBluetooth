//
//  ExclusivityController.m
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/4.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import "ExclusivityController.h"
#import "Operation.h"

@interface ExclusivityController()

@property(nonatomic) dispatch_queue_t serialQueue;
@property(strong, nonatomic) NSMutableDictionary<NSString *, NSArray<Operation *> *> *operations;

@end

@implementation ExclusivityController

+ (instancetype)sharedExclusivityController
{
    static ExclusivityController *sharedObj = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedObj = [[self alloc] initPrivate];
    });
    return sharedObj;
}

- (instancetype)initPrivate
{
    self = [super init];
    if (self) {
        _serialQueue = dispatch_queue_create("Operations.ExclusivityController", DISPATCH_QUEUE_SERIAL);
        _operations = [[NSMutableDictionary alloc] init];
    }
    return self;
}

// No one should call init
- (instancetype)init {
    @throw [NSException exceptionWithName:@"Singleton" reason:@"Use +[ClassName sharedObj]" userInfo:nil];
    return nil;
}

- (void)addOperation:(Operation *)operation categories:(NSArray<NSString *> *)categories {
    dispatch_sync(self.serialQueue, ^{
        for (NSString *category in categories) {
            [self noQueue_addOperation:operation category:category];
        }
    });
}

- (void)removeOperation:(Operation *)operation categories:(NSArray<NSString *> *)categories {
    dispatch_async(self.serialQueue, ^{
        for (NSString *category in categories) {
            [self noQueue_removeOperation:operation category:category];
        }
    });
}

#pragma mark - Operation Management

- (void)noQueue_addOperation:(Operation *)operation category:(NSString *)category {
    NSArray *operationWithThisCategory = self.operations[category] ?: @[];
    Operation *lastOp = operationWithThisCategory.lastObject;

    if (lastOp) {
        [operation addDependency:lastOp];
    }
   
    self.operations[category] = [operationWithThisCategory arrayByAddingObject:operation];
}

- (void)noQueue_removeOperation:(Operation *)operation category:(NSString *)category {
    NSArray *matchingOperations = self.operations[category];
    if (matchingOperations) {
        NSUInteger index = [matchingOperations indexOfObject:operation];
        NSMutableArray *operationWithThisCategory = [NSMutableArray arrayWithArray:matchingOperations];
        [operationWithThisCategory removeObjectAtIndex:index];
        self.operations[category] = [operationWithThisCategory copy];
    }
}

@end
