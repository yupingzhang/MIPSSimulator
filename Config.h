//
//  Config.h
//  MIPSSimulator
//
//  Created by Yuping on 4/26/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#ifndef __MIPSSimulator__Config__
#define __MIPSSimulator__Config__

#include <iostream>

class Config
{
public:
    unsigned int FPadder;  //cycle count
    unsigned int FPmultiplier;  //cycle count
    unsigned int FPdivider;   //cycle count
    bool FPadder_pipelined;
    bool FPmultiplier_pipelined;
    bool FPdivider_pipelined;
    
    unsigned int MemoryAccess;
    unsigned int ICacheAccess;
    unsigned int DCacheAccess;
    
public:
    Config(std::string filename);
    ~Config();
    
};

#endif /* defined(__MIPSSimulator__Config__) */
