//
//  main.cpp
//  MIPSSimulator
//
//  Created by Yuping on 4/4/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#include <iostream>
#include <cstdio>

void main(int argc, const char * argv[])
{
    if (argc < 5) {
        printf("Usage: simulator inst.txt data.txt reg.txt config.txt result.txt");
    }
    
    FILE *fp1, *fp2, *fp3, *fp4;
    fp1 = fopen("inst.txt", "r");
    fp2 = fopen("data.txt", "r");
    fp3 = fopen("config.txt", "r");
    fp4 = fopen("result.txt", "w");


  
}

