//
//  Memory.h
//  MIPSSimulator
//
//  Created by Yuping on 4/4/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//
//--------------------------------------------------
// Virtual memory
//--------------------------------------------------

#ifndef __MIPSSimulator__Memory__
#define __MIPSSimulator__Memory__

#include <iostream>
#include <vector>
#include "basic.h"

#include <utility>

class Memory
{
public:
    //instruction memory
    std::vector<Instr> instructions;    // index is the address
    //data memory
    std::vector<int> data;   //  address start at 256, +4 per line
    
    int mainbusoccupycycle;  //lock the memory
    
public:
    Memory(std::string instfile, std::string datafile);
    ~Memory() {};

    
};



#endif /* defined(__MIPSSimulator__Memory__) */
