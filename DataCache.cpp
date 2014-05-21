//
//  Data.cpp
//  MIPSSimulator
//
//  Created by Yuping on 4/4/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#include "DataCache.h"
#include "Memory.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>

DataCache::DataCache()
{
    for (int j=0; j<4; j++) {
        CacheBlocks[j] = DataBlock();
    }
    
}

bool DataCache::is_DCacheMiss(int address)
{   //decode
    bool cachemiss = true;
    int block_index = address/16;
//    int offset = address%16/4;
//    int block_start_address = block_index*16;
    int set_index = address%2;
    int tag = block_index;
    
    if (CacheBlocks[2*set_index].CacheTag == tag) {
        cachemiss = false;
    }
    else if (CacheBlocks[2*set_index+1].CacheTag == tag) {
        cachemiss = false;
    }
    return cachemiss;
}

void DataCache::fetchNewData(AppContext &appctx, int address)
{
    int block_index = address/16;
    int block_start_address = block_index*16;
    int block_start_index = block_start_address - 256;
    int set_index = address%2;
    int cacheindex = -1;
    if ( CacheBlocks[2*set_index].valid) {
        cacheindex = 2*set_index;
    }
    else if (CacheBlocks[2*set_index+1].valid) {
        cacheindex = 2*set_index + 1;
    }
    else {
        //todo
        //if the set is full, choose the least recent used and write back the current values
        cacheindex = 2*set_index;
    }
    //copy the block from memory
    for (int i=0; i<4; i++) {
        appctx.mDataCache->CacheBlocks[cacheindex].CacheData[i].address = block_start_address+i*4;
        appctx.mDataCache->CacheBlocks[cacheindex].CacheData[i].datacontent = appctx.mMemory->data[block_start_index+i];
    }
    appctx.mDataCache->CacheBlocks[cacheindex].CacheTag = block_index;
    appctx.mDataCache->CacheBlocks[cacheindex].valid = false;
    
}

int DataCache::getValue(int address)
{
    int block_index = address/16;
    int offset = address%16/4;
    int block_start_address = block_index*16;
    int set_index = address%2;
    
    int tag = block_index;
    int value;
    if (CacheBlocks[2*set_index].CacheTag == tag) {
        value = CacheBlocks[2*set_index].CacheData[offset].datacontent;
    }
    else if (CacheBlocks[2*set_index+1].CacheTag == tag) {
        value = CacheBlocks[2*set_index+1].CacheData[offset].datacontent;
    }
    return value;

}


