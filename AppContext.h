//
//  AppContext.h
//  MIPSSimulator
//
//  Created by Yuping on 4/24/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#ifndef MIPSSimulator_AppContext_h
#define MIPSSimulator_AppContext_h
 
#include <vector>


struct AppContext {
    
    int reqinstrcache;
    int instrcachehit;
    int reqdatacache;
    int datacachehit;
    
    class Memory *mMemory;
    class Config *mConfig;
    class DataCache *mDataCache;
    class InstrCache *mInstrCache;
    class Reg *mRegister;
    
    AppContext() : mMemory(0), mConfig(0), mDataCache(0), mInstrCache(0), mRegister(0) {}
    ~AppContext();

};

#endif
