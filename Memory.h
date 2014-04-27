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

class Memory
{
private:
    //instruction memory
    std::vector<std::string> instructions;
    //data memory
    std::vector<int> data;
    
public:
    Memory();
    ~Memory() {};
    
    
    
};



#endif /* defined(__MIPSSimulator__Memory__) */
