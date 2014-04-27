//
//  Instr.h
//  MIPSSimulator
//
//  Created by Yuping on 4/4/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#ifndef __MIPSSimulator__Instr__
#define __MIPSSimulator__Instr__

#include <iostream>
#include <cstring>
#include "Reg.h"

#define IF 1
#define ID 2
#define EX 3
#define ME 4
#define WR 5

class Instr
{
private:
    std::string oprt;
    std::string dest;
    std::string s1;
    std::string s2;
    int offset;
    
public:
    Instr();
    ~Instr() {};
    int status;
    int harzard_flag;
    void operate(Reg dest, Reg source1, Reg source2);  //arithmetic
    void operate(Reg dest, Reg source1);   //data transfers
    void operate(Reg dest);  //control
    void operate();  //special
};

#endif /* defined(__MIPSSimulator__Instr__) */
