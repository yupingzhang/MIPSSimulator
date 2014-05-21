//
//  Instr.h
//  MIPSSimulator
//
//  Created by Yuping on 4/4/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#ifndef __MIPSSimulator__InstrCache__
#define __MIPSSimulator__InstrCache__

#include <iostream>
#include <cstring>
#include <vector>

#include "AppContext.h"
#include "Reg.h"
#include "basic.h"


class InstrCache
{
//Instruction cache has 4 blocks with 4 words.
public:
    Instr instrCache[16];
    int writepoint;  // where to write/overwrite in the instrCache
    
public:
    InstrCache();
    ~InstrCache() {};
 
    int fetchNewInstr(AppContext appctx, int pc);
    int getInstr(int pc);
    //After the resources are in the cache, execute the oprt
   
};



#endif /* defined(__MIPSSimulator__Instr__) */
