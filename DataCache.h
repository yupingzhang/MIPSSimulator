//
//  DataCache.h
//  MIPSSimulator
//
//  Created by Yuping on 4/4/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//


#ifndef __MIPSSimulator__DataCache__
#define __MIPSSimulator__DataCache__

#include <iostream>
#include <cstdio>
#include <queue>

struct Data
{
    unsigned int dataaddress;
    float datacontent;
    
};

class DataCache
{
//data
public:
    std::queue<Data> *mDataCache;
//method
public:
    DataCache();
    ~DataCache() {};
    
    
};


#endif /* defined(__MIPSSimulator__Cache__) */
