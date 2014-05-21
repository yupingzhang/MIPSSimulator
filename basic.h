//
//  basic.h
//  MIPSSimulator
//
//  Created by Yuping on 4/30/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#ifndef MIPSSimulator_basic_h
#define MIPSSimulator_basic_h

#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <utility>

#include "AppContext.h"


//class Label
//{
//public:
//    std::string label_name;
//    int lablePointor;
//    
//public:
//    Label(std::string str="", int pc=0)
//    {
//        label_name = str;
//        lablePointor = pc;
//    }
//    
//};
//
typedef std::pair<std::string, int> Label;


inline int label_pos(std::vector<Label> labelcontainer, std::string name)
{
    int i;
    for (std::vector<Label>::iterator it=labelcontainer.begin(); it !=labelcontainer.end(); it++) {
		if(!strcmp(it->first.c_str(), name.c_str()))
			return it->second;
	}
    return -1;   //cannot find the label, print out Error
}

struct Data
{
    unsigned int address;
    float datacontent;
    
    Data (int add=0, float data=0 )
    {
        address = add;
        datacontent = data;
    }
};


struct Result
{
    std::string instruction;
    int ft, ids, ex, wb;
    char raw, war, waw, strhz;
    Result(std::string instr="", int iFT=0, int iID=0, int iEX=0, int iWB=0, char iraw='N', char iwar='N', char iwaw='N', char istrhz='N') {
        instruction = instr;
        ft = iFT;
        ids = iID;
        ex = iEX;
        wb = iWB;
        raw = iraw;
        war = iwar;
        waw = iwaw;
        strhz = istrhz;
    }
    
    void operator=(Result rlt)
    {
        instruction = rlt.instruction;
        ft = rlt.ft;
        ids = rlt.ids;
        ex = rlt.ex;
        wb = rlt.wb;
        raw = rlt.raw;
        war = rlt.war;
        waw = rlt.waw;
        strhz = rlt.strhz;
    }
    
};


struct Instr
{
    int pc;
    std::string oprt;
    std::string dest;
    std::string s1;
    std::string s2;
    int offset;
    std::string label;

    Result instresult;
    
    int dest_value;
    int dest_add;
    int s1_value;
    int s2_value;
    
    Instr(std::string str="", int address=-1);
    
    void operator=(Instr newinstr)
    {
        pc = newinstr.pc;
        oprt = newinstr.oprt;
        dest = newinstr.dest;
        s1 = newinstr.s1;
        s2 = newinstr.s2;
        offset = newinstr.offset;
        label = newinstr.label;
        
        dest_value = newinstr.dest_value;
        dest_add = newinstr.dest_add;
        s1_value = newinstr.s1_value;
        s2_value = newinstr.s2_value;
        
        instresult = newinstr.instresult;
        
    }
};


inline Instr::Instr(std::string str, int address)
{
    int len = str.length();
    //initialize the result(output)
    instresult = Result(str);  
    
    //check if there is any label
    std::size_t found = str.find(":");
    if (found!=std::string::npos)
    {
        std::string ss = str.substr(0, found);
        label = ss;
        
        //std::pair<std::string, int> lab(ss, address);
        //labelcontainer.push_back(lab);
        
        str = str.substr(found+1, len-found-1);   //start from actual instruction
    }
    
    std::stringstream ssin(str);
    std::string temp;
    pc = address;
    offset = 0;
    
    ssin >> oprt;     //operation
    
    if (oprt == "LW" || oprt == "SW" || oprt == "L.D" || oprt == "S.D" ) {
        
        ssin >> temp;
        dest = temp.substr(0, temp.length()-1);  //dest register, excluse ","

        ssin >> temp;
        std::size_t loc1 = temp.find("(");
        std::size_t loc2 = temp.find(")");
        if (loc1 != std::string::npos && loc2 != std::string::npos) {
            offset = atoi(temp.substr(0, loc1).c_str());
            s1 = temp.substr(loc1+1, loc2-loc1-1);
        }
    }
    else if(oprt == "DADD" || oprt == "DADDI" || oprt == "DSUB" || oprt == "DSUBI" || oprt == "AND" || oprt == "ANDI" || oprt == "OR" || oprt == "ORI" || oprt == "ADD.D" || oprt == "SUB.D" || oprt == "MUL.D" || oprt == "DIV.D"  )
        
    {
               
                ssin >> temp;
                dest = temp.substr(0, temp.length()-1);  //dest register, excluse ","
                
                ssin >> temp;
                s1 = temp.substr(0, temp.length()-1);
                
                ssin >> temp;
                s2 = temp.substr(0, temp.length());
                
    }
    else if(oprt == "BNE" || oprt == "BEQ")
    {
                
                ssin >> temp;
                dest = temp.substr(0, temp.length()-1);  //dest register, excluse ","
                
                ssin >> temp;
                s1 = temp.substr(0, temp.length()-1);
                
                ssin >> label;
    }
    else if(oprt == "J")
    {
                ssin >> label;
    }
    else if(oprt == "HLT")
    {//HLT
    }
    
    
//    Instr newinstr = new Instr(pc, oprt, dest, s1, s2, offset, label);
//    return newinstr;
}



#endif
