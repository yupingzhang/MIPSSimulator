//
//  Reg.h
//  MIPSSimulator
//
//  Created by Yuping on 4/4/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

//registers

#ifndef __MIPSSimulator__Reg__
#define __MIPSSimulator__Reg__

#include <iostream>
#include <cstdio>

#include "basic.h"

class Reg
{
public:
    int intReg[32];
    int occupyflag[64];   // 1 is occupied, 0 is not.
    
public:
    Reg(std::string regst);
    ~Reg() {};
    
    //mark the in-use destination for hazard checking
    void setflag(std::string regname);
    bool checkflag(std::string regname);
    
};


#endif /* defined(__MIPSSimulator__Reg__) */
