//
//  main.cpp
//  MIPSSimulator
//
//  Created by Yuping on 4/4/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#include "AppContext.h"
#include "basic.h"
#include "Unit.h"
#include "InstrCache.h"
#include "Memory.h"
#include "DataCache.h"
#include "Reg.h"
#include "Config.h"
#include "Operations.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <list>



///// GLOBAL /////
////time
int cycle = 0;
////PC (Program counter)
int PC = 0;


//labels
std::vector< std::pair<std::string, int> > labelcontainer;

//Unit* pipeline;  //header
//Unit* pplend;    //end
//IDUnit* idstage;
//EXUnit* exIU;

// Clean up any context data
AppContext::~AppContext()
{
    delete mMemory;
    delete mDataCache;
    delete mInstrCache;
}

std::vector<Result> output;


int main(int argc, const char * argv[])
{
    // collected data about application for use in callbacks
    AppContext appctx;
    
    if (argc < 5) {
        printf("Usage: simulator inst.txt data.txt reg.txt config.txt result.txt");
    }
    
    // Initialize the MIPS
    appctx.mMemory = new Memory("inst.txt", "data.txt");
    appctx.mConfig = new Config("config.txt");
    appctx.mDataCache = new DataCache();
    appctx.mInstrCache = new InstrCache();
    appctx.mRegister = new Reg("reg.txt");
    
    appctx.reqinstrcache = 0;
    appctx.instrcachehit = 0;
    appctx.reqdatacache = 0;
    appctx.datacachehit = 0;
    
    /////////////////////////////////////
    //construct_pipeline
    IFUnit ifstage = IFUnit();  //number of cycles depends on instruction cache
    IDUnit idstage = IDUnit(&ifstage);
    ifstage.next = &idstage;
    
    //  EX stage
    // EX stage 1  IU
    EXIUUnit exIU = EXIUUnit(&idstage);
    idstage.next = &exIU;
    EXMEMUnit exMEM = EXMEMUnit(&exIU);      //number of cycles depends on data cache
    exIU.next = &exMEM;
    
    // EX stage 2  ADD
    EXUnit *exADD; //end
    if (appctx.mConfig->FPadder_pipelined) {
        EXADDUnit *exADD1 = new EXADDUnit(&idstage);
        EXADDUnit *prev = exADD1;
        int num = appctx.mConfig->FPadder;
        for (int i=1; i < num-1; i++) {
            EXADDUnit* exADD2 = new EXADDUnit(prev);
            exADD2->placeholder = true;
            prev = exADD2;
        }
        exADD = prev;
    }
    else {
        EXADDUnit* exADD1 = new EXADDUnit(&idstage);
        exADD1->cycleft = appctx.mConfig->FPadder;
        exADD = exADD1;
    }
    
    // EX stage 3 MUL
    EXUnit *exMUL;
    if (appctx.mConfig->FPmultiplier_pipelined) {
        EXMULUnit *exMUL1 = new EXMULUnit(&idstage);
        EXMULUnit *prev = exMUL1;
        for (int i=1; i < appctx.mConfig->FPmultiplier-1; i++) {
            EXMULUnit* exMUL2 = new EXMULUnit(prev);
            exMUL2->placeholder = true;
            prev = exMUL2;
        }
        exMUL = prev;
    }
    else {
        EXMULUnit* exMUL1 = new EXMULUnit(&idstage);
        exMUL1->cycleft = appctx.mConfig->FPmultiplier;
        exMUL = exMUL1;
    }
    
    // EX stage 4 DIV
    EXUnit *exDIV;
    if (appctx.mConfig->FPdivider_pipelined) {
        EXDIVUnit *exDIV1 = new EXDIVUnit(&idstage);
        EXDIVUnit *prev = exDIV1;
        for (int i=1; i < appctx.mConfig->FPdivider-1; i++) {
            EXDIVUnit* exDIV2 = new EXDIVUnit(prev);
            exDIV2->placeholder = true;
            prev = exDIV2;
        }
        exDIV = prev;
    }
    else {
        EXDIVUnit* exDIV1 = new EXDIVUnit(&idstage);
        exDIV1->cycleft = appctx.mConfig->FPdivider;
        exDIV = exDIV1;
    }
    
    //  WB stage
    Unit *previous[4] = {&exMEM, exADD, exMUL, exDIV};
    
    WBUnit wbstage = WBUnit(previous);
    
    //////////////////////////////////////////////////////////////////////////////
    // main loop: run the instructions
    //////////////////////////////////////////////////////////////////////////////
    //todo
    //stop flag to mark the moment stop fetch new instruction
    //after all the instruction in the pipeline finish, stop the loop
    while (cycle < 100) {
        cycle++;
        printf("cycle: %d\n", cycle);
  
        wbstage.update(appctx, cycle, output);
        //exstage
//        EXUnit* pointer = &wbstage.prelist;
//        //check the ex stage, according to the privilege
//        pointer[0]->update(appctx, PC);
//        pointer[0]->pre->update(appctx, PC);
        
        exMEM.update(appctx, cycle);
        exIU.update(appctx, cycle);
        
        exADD->update(appctx, cycle);
        Unit* temp = exADD->pre;
        if (appctx.mConfig->FPadder_pipelined) {
            for (int i=1; i < appctx.mConfig->FPadder-1; i++) {
                temp->update(appctx, cycle);
                temp = temp->pre;
            }
        }

        exMUL->update(appctx, cycle);
        temp = exMUL->pre;
        if (appctx.mConfig->FPmultiplier_pipelined) {
            for (int i=0; i < appctx.mConfig->FPmultiplier-1; i++) {
                temp->update(appctx, cycle);
                temp = temp->pre;
            }
        }
   
        exDIV->update(appctx, cycle);
        temp = exDIV->pre;
        if (appctx.mConfig->FPdivider_pipelined) {
            for (int i=0; i < appctx.mConfig->FPdivider-1; i++) {
                temp->update(appctx, PC);
                temp = temp->pre;
            }
        }
        
        idstage.update(appctx, PC, cycle, labelcontainer);
   
        ifstage.update(appctx, PC);
        
    }
    
    //write result to file //std::vector<Result> output;
    std::ofstream myfile;
    myfile.open ("result.txt");
    for (std::vector<Result>::iterator i=output.begin(); i != output.end(); i++) {
        myfile << i->instruction <<" "<<i->ft<<" "<<i->ids<<" "<<i->ex<<" "<<i->wb<<" "<<i->raw<<" "<<i->war<<" "<<i->waw<<" "<<i->strhz << "\n";
    }
    
    myfile.close();
    
    return 0;
  
}

