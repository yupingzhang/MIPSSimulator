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

#include "AppContext.h"
#include "basic.h"

struct DataBlock {
    int CacheTag;
    Data CacheData[4];
    int frequency;
    bool valid;
    
    DataBlock ()
    {
        CacheTag = 0;
        frequency = 0;
        valid = true;
        for(int i=0; i<4; i++)
        {
            CacheData[i] = Data();
        }
    }
};

class DataCache
{

public:
    DataBlock CacheBlocks[4];
    
public:
    DataCache();
    ~DataCache() {};
    
    bool is_DCacheMiss(int address);      
    void fetchNewData(AppContext &appctx, int memoryAddress);
    int getValue(int address);
    
};


#endif /* defined(__MIPSSimulator__Cache__) */
