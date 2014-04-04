//
//  Reg.h
//  MIPSSimulator
//
//  Created by Yuping on 4/4/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#ifndef __MIPSSimulator__Reg__
#define __MIPSSimulator__Reg__

#include <iostream>

class Reg
{
private:
    int intReg[32];
    float floatReg[32];
    bool ocuppyflag[64];
    
public:
    Reg();
    ~Reg();
    
    void initialize(FILE *fp);
    
    Reg getReg(char*);
    Reg retrive(char*);
    
}


#endif /* defined(__MIPSSimulator__Reg__) */
