//
//  InstrCache.cpp
//  MIPSSimulator
//
//  Created by Yuping on 4/4/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#include <iostream>
#include <cstring>


#include "AppContext.h"
#include "basic.h"
#include "InstrCache.h"
#include "Memory.h"
#include "DataCache.h"



InstrCache::InstrCache()
{

    for (int i=0; i<16; i++) {
        instrCache[i] = Instr();
    }
    writepoint = 0;
}


int InstrCache::fetchNewInstr(AppContext appctx, int pc)
{
    //fetch a block instruction from memory
    //write to the writepoint in the cache
    int cachewriteindex = appctx.mInstrCache->writepoint;
    int memoryblockindex = pc/4;
    int index = pc%4;
    for (int i=0; i<4; i++) {
        appctx.mInstrCache->instrCache[cachewriteindex+i] = appctx.mMemory->instructions[memoryblockindex*4+i];
    }
    writepoint = (writepoint+4)%16;    
    return (cachewriteindex+index);
    
    // occupy the bus and memory 6 cycles
    
}

// find the instruction index in the cache
// if cannot find, fetch and return the index
int InstrCache::getInstr(int address)
{
    for (int i=0; i<16; i++) {
        if (instrCache[i].pc == address) {
            return i;
        }
    }
    //if cannot find
    return -1;
  
}







