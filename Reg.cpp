//
//  Reg.cpp
//  MIPSSimulator
//
//  Created by Yuping on 4/4/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#include "Reg.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>

int biToDec(std::string number)
{
    int result = 0, pow = 1;
    for ( int i = number.length() - 1; i >= 0; --i, pow <<= 1 )
        result += (number[i] - '0') * pow;
    
    return result;
}

Reg::Reg(std::string regfile)
{
    std::ifstream file(regfile);
    std::string line;
    //load the file
    for (int i=0; i<32; i++) {
        std::getline(file, line);
        intReg[i] = biToDec(line);
    }
    memset(occupyflag, 0, 64*sizeof(int));
    std::cout << "Register loaded." << std::endl;
    
}

void Reg::setflag(std::string regname)
{
     
    if (regname[0] == 'R') {
        occupyflag[regname[1]] = 1;
    }
    else if (regname[0] == 'F') {
        occupyflag[regname[1]+32] = 1;
    }
    
}

bool Reg::checkflag(std::string regname)
{
    bool check;
    if (regname == "") {
        return false;   //not occupied, == 0
    }
    if (regname[0] == 'R') {
        check = occupyflag[regname[1]];
    }
    else if (regname[0] == 'F') {
        check = occupyflag[regname[1]+32];
    }
    return check;
}



