//
//  Config.cpp
//  MIPSSimulator
//
//  Created by Yuping on 4/26/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#include "Config.h"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>


Config::Config(std::string filename) {
    
    std::ifstream file(filename);
    std::string line;
    
    std::string tempString= "";
    int t1, t2;
    
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
        
        if (line.find("FP adder") != std::string::npos) {
            
            t1 = line.find_first_of(":")+2;
            t2 = line.find_first_of(",");
            tempString = line.substr(t1, t2-t1);
            FPadder = atoi(tempString.c_str());
            
            if (line.find("yes")!= std::string::npos) {
                FPadder_pipelined = true;
            }
            else if (line.find("no")!= std::string::npos) {
                FPadder_pipelined = false;
            }
        }
        
        if (line.find("FP Multiplier") != std::string::npos) {
            
            t1 = line.find_first_of(":")+2;
            t2 = line.find_first_of(",");
            tempString = line.substr(t1, t2-t1);
            FPmultiplier = atoi(tempString.c_str());
            
            if (line.find("yes")!= std::string::npos) {
                FPmultiplier_pipelined = true;
            }
            else if (line.find("no")!= std::string::npos) {
                FPmultiplier_pipelined = false;
            }
        }
        
        if (line.find("FP divider") != std::string::npos) {
            
            t1 = line.find_first_of(":")+2;
            t2 = line.find_first_of(",");
            tempString = line.substr(t1, t2-t1);
            FPdivider = atoi(tempString.c_str());
            
            if (line.find("yes")!= std::string::npos) {
                FPdivider_pipelined = true;
            }
            else if (line.find("no")!= std::string::npos) {
                FPdivider_pipelined = false;
            }
        }
        
        if (line.find("Main memory") != std::string::npos) {
            t1 = line.find_first_of(":")+2;
            tempString = line.substr(t1);
            MemoryAccess = atoi(tempString.c_str());

        }
        if (line.find("I-Cache") != std::string::npos) {
            t1 = line.find_first_of(":")+2;
            tempString = line.substr(t1);
            ICacheAccess = atoi(tempString.c_str());
        }
        if (line.find("D-Cache") != std::string::npos) {
            t1 = line.find_first_of(":")+2;
            tempString = line.substr(t1);
            DCacheAccess = atoi(tempString.c_str());
        }
        

    }
    //std::cout << FPadder_pipelined << " " <<FPmultiplier_pipelined << " "<< FPdivider_pipelined << std::endl;
    std::cout << MemoryAccess << " " <<ICacheAccess << " "<< DCacheAccess << std::endl;
    

}



