//
//  URLSessionTaskOperation.h
//  AOperation
//
//  Created by 肖忠肯 on 2018/8/22.
//  Copyright © 2018年 肖忠肯. All rights reserved.
//

#import "Operation.h"

@interface URLSessionTaskOperation : Operation

@property(strong, nonatomic) NSURLSessionTask *task;

- (instancetype)initWithTask:(NSURLSessionTask *)task;

@end
