//
//  Operation.m
//  AdvanceNSOperation
//
//  Created by 肖忠肯 on 2018/8/3.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import "Operation.h"
#import "OperationCondition.h"
#import "OperationObserver.h"


typedef NS_ENUM(int, State) {
    Initialized,
    Pending,
    EvaluatingConditions,
    Ready,
    Executing,
    Finishing,
    Finished
};

@interface Operation()

@property(strong, nonatomic) NSLock *stateLock;
@property(nonatomic) BOOL userInitiated;
@property(nonatomic) State state;
@property(strong, nonatomic) NSMutableArray *internalErrors;
@property(nonatomic) BOOL hasFinishedAlready;

@end

@implementation Operation

@synthesize userInitiated = _userInitiated, state = _state, conditions = _conditions, observers = _observers;

+ (NSSet *)keyPathsForValuesAffectingIsReady {
    return [NSSet setWithObject:@"state"];
}

+ (NSSet *)keyPathsForValuesAffectingIsExecuting {
    return [NSSet setWithObject:@"state"];
}

+ (NSSet *)keyPathsForValuesAffectingIsFinished {
    return [NSSet setWithObject:@"state"];
}

- (BOOL)canTransitionToState:(State)target {
    if (_state == Initialized && target == Pending) {
        return YES;
    } else if (_state == Pending && target == EvaluatingConditions) {
        return YES;
    } else if (_state == EvaluatingConditions && target == Ready) {
        return YES;
    } else if (_state == Ready && target == Executing) {
        return YES;
    } else if (_state == Ready && target == Finishing) {
        return YES;
    } else if (_state == Executing && target == Finishing) {
        return YES;
    } else if (_state == Finishing && target == Finished) {
        return YES;
    } else {
        return NO;
    }
}

- (void)willEnqueue {
    self.state = Pending;
}

- (State)state {
    [self.stateLock lock];
    State currentState = _state;
    [self.stateLock unlock];
    return currentState;
}

- (void)setState:(State)state {
    [self willChangeValueForKey:@"state"];
    
    [self.stateLock lock];
    if (_state == Finished) {
        return;
    }
    NSAssert([self canTransitionToState:state], @"Performing invalid state transition.");
    _state = state;
    [self.stateLock unlock];
    [self didChangeValueForKey:@"state"];
}

- (NSLock *)stateLock {
    if (!_stateLock) {
        _stateLock = [[NSLock alloc] init];
    }
    return _stateLock;
}

- (BOOL)isReady {
    switch (self.state) {
        case Initialized:
            return self.cancelled;
            break;
        case Pending:
            if (self.cancelled) {
                return YES;
            }
            
            if (super.isReady) {
                [self evaluateConditions];
            }
            
            return NO;
            break;
        case Ready:
            return super.isReady || self.cancelled;
        default:
            return NO;
            break;
    }
}

- (BOOL)userInitiated {
    return self.qualityOfService == NSQualityOfServiceUserInitiated;
}

- (void)setUserInitiated:(BOOL)userInitiated {
    NSAssert(self.state < Executing, @"Cannot modify userInitiated after execution has begun.");
    self.qualityOfService = userInitiated ? NSQualityOfServiceUserInitiated : NSQualityOfServiceDefault;
}

- (BOOL)isExecuting {
    return self.state == Executing;
}

- (BOOL)isFinished {
    return self.state == Finished;
}


- (void)evaluateConditions {
    NSAssert(self.state == Pending && !self.cancelled, @"evaluateConditions() was called out-of-order");
    self.state = EvaluatingConditions;
    
    [OperationConditionEvaluator evaluateCondition:self.conditions operation:self completion:^(NSArray<NSError *> *errors) {
        if (errors.count > 0) {
            [self.internalErrors addObjectsFromArray:errors];
        }
        self.state = Ready;
    }];
}

- (void)addCondition:(id<OperationConditionDelegate>)condition {
    NSAssert(self.state < Executing, @"Cannot modify condition after execution has begun.");
    [self.conditions addObject:condition];
}

- (void)addObserver:(id<OperationObserver>)observer {
    NSAssert(self.state < Executing, @"Cannot modify observers after execution has begun.");
    [self.observers addObject:observer];
}

- (void)addDependency:(NSOperation *)op {
    NSAssert(self.state < Executing, @"Dependencies cannot be modified after execution has begun.");
    [super addDependency:op];
}

- (void)start {
    [super start];
    
    if (self.cancelled) {
        [self finish:nil];
    }
}

- (void)main {
    NSAssert(self.state == Ready, @"This operation must be performed on an operation queue.");
    
    if (self.internalErrors.count == 0 && !self.cancelled) {
        self.state = Executing;
        
        for (id observer in self.observers) {
            [observer operationDidStart:self];
        }
        
        [self execute];
    } else {
        [self finish:nil];
    }
}

- (void)execute {
    NSLog(@"%@ must override `execute()`.", NSStringFromClass([self class]));
    [self finish:nil];
}

- (void)cancelWithError:(NSError *)error {
    if (error) {
        [self.internalErrors addObject:error];
    }
    [self cancel];
}

- (void)produceOperation:(NSOperation *)operation {
    for (id observer in self.observers) {
        [observer opeartion:self didProduceOperation:operation];
    }
}

- (void)finishWithError:(NSError *)error {
    if (error) {
        [self finish:@[error]];
    } else {
        [self finish:nil];
    }
}

- (void)finish:(NSArray<NSError *> *)errors {
    if (!self.hasFinishedAlready) {
        self.hasFinishedAlready = YES;
        self.state = Finishing;
        
        [self.internalErrors addObjectsFromArray:errors];
        NSArray *combinedErrors = [self.internalErrors copy];
        [self finished:combinedErrors];
        
        for (id observer in self.observers) {
            [observer operation:self didFinishedWithError:combinedErrors];
        }
        
        self.state = Finished;
    }
}

- (void)finished:(NSArray<NSError *> *)errors {
    
}

- (NSMutableArray *)observers {
    if (!_observers) {
        _observers = [[NSMutableArray alloc] init];
    }
    return _observers;
}

- (NSMutableArray *)conditions {
    if (!_conditions) {
        _conditions = [[NSMutableArray alloc] init];
    }
    return _conditions;
}

- (NSMutableArray *)internalErrors {
    if (!_internalErrors) {
        _internalErrors = [[NSMutableArray alloc] init];
    }
    return _internalErrors;
}

@end
