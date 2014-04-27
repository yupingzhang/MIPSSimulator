//
//  AppContext.h
//  MIPSSimulator
//
//  Created by Yuping on 4/24/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#ifndef MIPSSimulator_AppContext_h
#define MIPSSimulator_AppContext_h

#include <queue>

struct AppContext {
  
    //GLOBAL Clock
    unsigned long int *mTime;
    //PC (Program counter)
    unsigned int PCounter;
    
    class Memory *mMemory;
    
    class Config *mConfig;
    class DataCache *mDataCache;
    class Reg *mInstCache;
    
    AppContext() : mTime(0), PCounter(0), mMemory(0), mDataCache(0), mInstCache(0), mConfig(0) {}
    ~AppContext();

};

#endif
