//
//  Memory.cpp
//  MIPSSimulator
//
//  Created by Yuping on 4/4/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//


#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>

#include "AppContext.h"
#include "basic.h"
#include "Memory.h"


int binaryToDec(std::string number)
{
    int result = 0, pow = 1;
    for ( int i = number.length() - 1; i >= 0; --i, pow <<= 1 )
        result += (number[i] - '0') * pow;
    
    return result;
}

Memory::Memory(std::string instfile, std::string datafile)
{
    std::ifstream finst(instfile);
    std::ifstream fdata(datafile);
    std::string line;
    
    int address = 0;
    //read the instruction into the memory
    while (std::getline(finst, line)) {
        Instr temp = Instr(line, address);
        instructions.push_back(temp);    //pc +1
        address++;
    }
    
    //address = 256;
    //read the data into the memory
    while (std::getline(fdata, line)) {
        data.push_back(binaryToDec(line));
        //address += 4;
    }
    //std::cout << instructions.front() << std::endl;
    //std::cout << data.front() << std::endl;
    std::cout << "Memory loaded." << std::endl;

}