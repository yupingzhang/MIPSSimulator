//
//  Unit.cpp
//  MIPSSimulator
//
//  Created by Yuping on 5/7/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//


#include <iostream>
#include <cstring>
#include <vector>

#include "Unit.h"

#include "AppContext.h"
#include "basic.h"
#include "InstrCache.h"
#include "Memory.h"
#include "Config.h"
#include "DataCache.h"

Unit::Unit()
{
    empty = true;
    ready = false;
    exe = false;
    finish = false;
    cycleft = 1;
}

EXUnit::EXUnit()
{
//    pre = previous;
//    placeholder = false;
}

IFUnit::IFUnit()
{
    HLT = 0;
}

IDUnit::IDUnit(Unit *previous )
{
    pre = previous;
}

EXMEMUnit::EXMEMUnit(Unit *previous)
{
    pre = previous;
}

//////////////////////////////////////////////////////
EXIUUnit::EXIUUnit(Unit *previous )
{
    pre = previous;
}

EXADDUnit::EXADDUnit(Unit *previous )
{
    pre = previous;
    placeholder = false;
}

EXMULUnit::EXMULUnit(Unit *previous )
{
    pre = previous;
    placeholder = false;
}

EXDIVUnit::EXDIVUnit(Unit *previous )
{
    pre = previous;
    placeholder = false;
}


WBUnit::WBUnit(Unit **previous)
{
    for (int i=0; i<4; i++) {
        prelist[i] = previous[i];
    } 
}


void IFUnit::execute(AppContext &appctx, int &pc)
{
    exe = true;
    if ( HLT != 2 ) {
        pc += 1;
        printf("pc plus one\n");
    }
};

void IDUnit::execute(AppContext &appctx, int &pc, std::vector< std::pair<std::string, int> > &labelcontainer)
{
    exe = true;
    appctx.reqdatacache++;
    std::string oprt = currentInstr.oprt;
    if (currentInstr.s1 != "" && currentInstr.s1[0] != 'F') {
        currentInstr.s1_value = appctx.mRegister->intReg[currentInstr.s1[1]-'0']; //string to int
    }
    if (currentInstr.s2 != "" && currentInstr.s2[0] != 'F') {
        currentInstr.s2_value = appctx.mRegister->intReg[currentInstr.s2[1]-'0'];
    }
    if (oprt == "BNE" || oprt == "BEQ" || oprt == "J") {
        //find the label pc and update pc
        std::string lab = currentInstr.label;
        pc = label_pos(labelcontainer, lab);
    }
    
};

void EXIUUnit::execute(AppContext &appctx )
{
    exe = true;
    //call operation function
    std::string oprt = currentInstr.oprt;
    if (currentInstr.dest != "") {
        if (oprt == "LW" || oprt == "SW" || oprt == "L.D" || oprt == "L.D" ) {
            currentInstr.dest_add = currentInstr.s1_value + currentInstr.offset;
        }
        if (currentInstr.dest[0] != 'F') {
            //add, sub, and, or
            if (oprt == "DADD" || oprt == "DADDI" ) {
                currentInstr.dest_value = currentInstr.s1_value + currentInstr.s2_value;
            }
            if (oprt == "DSUB" || oprt == "DSUBI" ) {
                currentInstr.dest_value = currentInstr.s1_value - currentInstr.s2_value;
            }
            if ( oprt == "AND" || oprt == "ANDI" ) {
                currentInstr.dest_value = currentInstr.s1_value && currentInstr.s2_value;
            }
            if ( oprt == "OR" || oprt == "ORI" ) {
                currentInstr.dest_value = currentInstr.s1_value || currentInstr.s2_value;
            }
        }
  
    }
};

void EXMEMUnit::execute(AppContext &appctx )
{
    exe = true;
    cycleft = 0;
    std::string oprt = currentInstr.oprt;
    if (oprt == "L.W") {
        bool miss = appctx.mDataCache->is_DCacheMiss(currentInstr.dest_add);
        if (miss) {
            cycleft += 2*(appctx.mConfig->MemoryAccess + appctx.mConfig->DCacheAccess);
            appctx.mDataCache->fetchNewData(appctx, currentInstr.dest_add);
        }
        else
        {
            cycleft += appctx.mConfig->DCacheAccess;
        }
        //get the value
        currentInstr.dest_value = appctx.mDataCache->getValue(currentInstr.dest_add);
    }
    
    if (oprt == "L.D") {
        bool miss1 = appctx.mDataCache->is_DCacheMiss(currentInstr.dest_add);
        if (miss1) {
            cycleft += 2*(appctx.mConfig->MemoryAccess + appctx.mConfig->DCacheAccess);
            appctx.mDataCache->fetchNewData(appctx, currentInstr.dest_add);
        }
        else if ( !miss1 )
        {
            cycleft += appctx.mConfig->DCacheAccess;
        }
        bool miss2 = appctx.mDataCache->is_DCacheMiss(currentInstr.dest_add+4);
        if (miss2) {
            cycleft += 2*(appctx.mConfig->MemoryAccess + appctx.mConfig->DCacheAccess);
            appctx.mDataCache->fetchNewData(appctx, currentInstr.dest_add);
        }
        else if ( !miss2 )
        {
            cycleft += appctx.mConfig->DCacheAccess;
        }
        
        //value
        // no need to compute for float

    }
    
};

void EXADDUnit::execute(AppContext &appctx)
{
    exe = true;
};

void EXMULUnit::execute(AppContext &appctx)
{
    exe = true;
};

void EXDIVUnit::execute(AppContext &appctx)
{
    exe = true;
};


void WBUnit::execute(AppContext &appctx )
{
    exe = true;
    // write dest_value back
    if (currentInstr.dest != "" && currentInstr.dest[0] != 'F' ) {
        appctx.mRegister->intReg[currentInstr.dest[1]] = currentInstr.dest_value;
    }
};


void IFUnit::update(AppContext &appctx, int &pc)
{

    if ( HLT != 2 ) {
        //check if it's empty
        if (empty) {
            appctx.reqinstrcache++;
            int index = appctx.mInstrCache->getInstr(pc);
            //debug
            printf("InstrCacheindex:  %d\n", index);
            printf("pc:  %d\n", pc);
            
            cycleft = 1;
            ready = false;
            exe = false;
            finish = false;
            if (index == -1) {
                //cache miss
                appctx.instrcachehit++;
                std::cout << "Instruction cache miss.\n";
                index = appctx.mInstrCache->fetchNewInstr(appctx, pc);  //from memory, set that memory block valid to false during reading
                cycleft += 5;
            }
            currentInstr = appctx.mInstrCache->instrCache[index];
            //it's not empty now
            empty = false;
            if (currentInstr.oprt == "HLT") {
                HLT++;
            }
        }
    }
    
    if (!empty &&  !ready) {
         ready = true;
    }
    if (!empty &&  ready && !exe ) {
        //fetch a new instruction
        execute(appctx, pc);
    }
    if (!empty && exe && !finish) {
        cycleft--;
        if (cycleft == 0) {
            finish = true;
        }
    }
 
};


void IDUnit::update(AppContext &appctx, int &pc, int cycle, std::vector< std::pair<std::string, int> > &labelcontainer)
{
    if ( empty ) {
        //get the next instruction
        if (pre->finish) {
            currentInstr = pre->currentInstr;
            pre->empty = true;
            ready = false;
            exe = false;
            finish = false;
            empty = false;
            cycleft = 1;
            // push result
            currentInstr.instresult.ft = cycle - 1;
        }
    }
    if (!empty && !ready) {    //stall
        //if there is raw hazard, need to wait until the resource register ready.
        if ( appctx.mRegister->checkflag(currentInstr.s1) || appctx.mRegister->checkflag(currentInstr.s2)) {
            currentInstr.instresult.raw = true;
        }
        else {
            ready = true;
        }
    }
    if (!empty && ready && !exe ) { // execute
        //set a flag for dest reg
        if ( !appctx.mRegister->checkflag(currentInstr.dest) ) {
            appctx.mRegister->setflag(currentInstr.dest);   //if no flag, set a flag there.
            //if there's a flag already, will cause waw hazard.
        }
        execute(appctx, pc, labelcontainer);
    }
    if (!empty && exe && !finish) {  //wait, counting down cycle left
        cycleft--;
        if (cycleft == 0) {
            finish = true;
        }
    }
};


void EXIUUnit::update(AppContext &appctx, int cycle)
{
    if (empty) {
        std::string opt = pre->currentInstr.oprt;
        if ( opt == "LW" || opt == "SW" || opt == "L.D" || opt == "S.D" || opt == "DADD" || opt == "DADDI" || opt == "DSUB" || opt == "DSUBI" || opt == "ADD" || opt == "ADDI" || opt == "OR" || opt == "ORI") {
            if (pre->finish) {
                currentInstr = pre->currentInstr;
                pre->empty = true;
                empty = false;
                ready = false;
                exe = false;
                finish = false;
                cycleft = 1;
            }
            currentInstr.instresult.ids = cycle - 1;
        }
    }
    if (!empty && !ready) {
        ready = true;
    }
    if (!empty &&  ready && !exe ) { // execute
        execute(appctx);
    }
    if (!empty && exe && !finish) {  //wait, counting down cycle left
        cycleft--;
        if (cycleft == 0) {
            finish = true;
        }
    }

};

void EXMEMUnit::update(AppContext &appctx, int cycle )
{
    if (empty) {
        if (pre->finish) {
            currentInstr = pre->currentInstr;
            pre->empty = true;
            empty = false;
            ready = false;
            exe = false;
            finish = false;
        }
    }
    if (!empty && !ready) {
        ready = true;
    }
    if (!empty &&  ready && !exe ) { // execute
        execute(appctx);
        exe = true;
    }
    if (!empty && exe && !finish) {
        cycleft--;
        if (cycleft == 0) {
            finish = true;
        }
    }
    
};

void EXADDUnit::update(AppContext &appctx, int cycle)
{
    if (empty) {
        if (pre->currentInstr.oprt == "ADD.D" || pre->currentInstr.oprt == "SUB.D" ) {
            if (pre->finish) {
                currentInstr = pre->currentInstr;
                pre->empty = true;
                empty = false;
                ready = false;
                exe = false;
                finish = false;
                if (appctx.mConfig->FPadder_pipelined) {
                    cycleft = 1;
                }
                else {
                    cycleft = appctx.mConfig->FPadder;}
                
            }
            if ( !placeholder) {
                currentInstr.instresult.ids = cycle - 1;
            }
        }
    }
    
    if (!empty && !ready) {
        ready = true;
    }
    if (!empty &&  ready && !exe ) { // execute
        if ( !placeholder ) {
            execute(appctx);
        }
        else {
            exe = true;
        }
        
    }
    if (!empty && exe && !finish) {  //wait, counting down cycle left
        cycleft--;
        if (cycleft == 0) {
            finish = true;
        }
    }
    
};

void EXMULUnit::update(AppContext &appctx, int cycle)
{
    if (empty) {
        if (pre->currentInstr.oprt == "MUL.D") {
            if (pre->finish) {
                currentInstr = pre->currentInstr;
                pre->empty = true;
                empty = false;
                ready = false;
                exe = false;
                finish = false;
                if (appctx.mConfig->FPmultiplier_pipelined) {
                    cycleft = 1;
                }
                else {
                    cycleft = appctx.mConfig->FPmultiplier; }
                
            }
            if ( !placeholder) {
                currentInstr.instresult.ids = cycle - 1;
            }
        }
    }
    
    if (!empty && !ready) {
        ready = true;
    }
    if (!empty &&  ready && !exe ) { // execute
        if ( !placeholder ) {
            execute(appctx);
        }
        else {
            exe = true;
        }
        
    }
    if (!empty && exe && !finish) {  //wait, counting down cycle left
        cycleft--;
        if (cycleft == 0) {
            finish = true;
        }
    }
    
};

void EXDIVUnit::update(AppContext &appctx, int cycle)
{
    if (empty) {
        if (pre->currentInstr.oprt == "DIV.D") {
            if (pre->finish) {
                currentInstr = pre->currentInstr;
                pre->empty = true;
                empty = false;
                ready = false;
                exe = false;
                finish = false;
                if (appctx.mConfig->FPdivider_pipelined) {
                    cycleft = 1;
                }
                else {
                    cycleft = appctx.mConfig->FPdivider; }
                
            }
            if ( !placeholder) {
                currentInstr.instresult.ids = cycle - 1;
            }
        }
    }
    
    if (!empty && !ready) {
        ready = true;
    }
    if (!empty &&  ready && !exe ) { // execute
        if ( !placeholder ) {
            execute(appctx);
        }
        else {
            exe = true;
        }
        
    }
    if (!empty && exe && !finish) {  //wait, counting down cycle left
        cycleft--;
        if (cycleft == 0) {
            finish = true;
        }
    }
    
};

void WBUnit::update(AppContext &appctx, int cycle, std::vector<Result> &output)
{
    if (empty) {
        //get a new instruction in the previous stage
        //Todo: based on the write back priority: not pipelined > pipelined with more execution cycles //or earlier issued instruction
        //Instr temp;
        int i=0;
        for (; i<4; i++) {
            if (prelist[i]->finish) {
                break;
            }
        }
        if (i != 4) {
            prelist[i]->empty = true;
            
            currentInstr = prelist[i]->currentInstr;
            empty = false;
            ready = false;
            exe = false;
            finish = false;
            currentInstr.instresult.ex = cycle-1;
            
            cycleft = 1;
        }
    }
    if (!empty && !ready) {    //check waw hazard  //stall
        ready = true;
    }
    if (!empty &&  ready && !exe ) { // execute
        execute(appctx);
    }
    if (!empty && exe && !finish) {  //wait, counting down cycle left
        cycleft--;
        if (cycleft == 0) {
            finish = true;
            empty = true;
            currentInstr.instresult.wb = cycle;
            output.push_back(currentInstr.instresult);   //push to the final output
        }
    }
    
};

// be careful about the pipelined type (if previous unit is the same?), only the first unit in the pipeline execute(once)

