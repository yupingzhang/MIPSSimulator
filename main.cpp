//
//  main.cpp
//  MIPSSimulator
//
//  Created by Yuping on 4/4/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#include "AppContext.h"
#include "Instr.h"
#include "Memory.h"
#include "DataCache.h"
#include "Reg.h"
#include "Config.h"

#include <fstream>
#include <iostream>
#include <cstdio>

// Clean up any context data
AppContext::~AppContext()
{
    delete mTime;
    
    delete mMemory;
    delete mDataCache;
    delete mInstCache;
}

int main(int argc, const char * argv[])
{
    // collected data about application for use in callbacks
    AppContext appctx;

    if (argc < 5) {
        printf("Usage: simulator inst.txt data.txt reg.txt config.txt result.txt");
    }
    
//    FILE *fp1, *fp2, *fp3, *fp4;
//    fp1 = fopen("inst.txt", "r");
//    fp2 = fopen("data.txt", "r");
    //fp3 = fopen("config.txt", "r");
    //fp4 = fopen("result.txt", "w");
//    std::ifstream fp3("config.txt");
    
//    if (! fp1 ) {
//        cout << "unable to open file inst.txt\n" << endl;
//        return false;           // error
//    }
//    if (! fp2 ) {
//        cout << "unable to open file data.txt\n" << endl;
//        return false;           // error
//    }
//    if (! fp3 ) {
//        cout << "unable to open file config.txt\n" << endl;
//        return false;           // error
//    }
//    if (! fp4 ) {
//        fcout << "unable to open file result.txt\n" << endl;
//        return false;           // error
//    }
    
    // Initialize the MIPS
    //appctx->mMemory = new Memory(fp1, fp2);
    appctx.mConfig = new Config("config.txt");
    
    return 0;
  
}

