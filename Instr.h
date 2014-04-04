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

#define IF 1
#define ID 2
#define EX 3
#define ME 4
#define WR 5

class Instr
{
private:
    reg dest;
    reg s1;
    reg s2;
    
    
public:
    Instr();
    ~Instr() {};
    int status;
    int harzard_flag;
    void operate(reg dest, reg source1, reg source2);  //arithmetic
    void operate(reg dest, reg source1);   //data transfers
    void operate(reg dest);  //control
    void operate();  //special
}

#endif /* defined(__MIPSSimulator__Instr__) */
