//
//  Unit.h
//  MIPSSimulator
//
//  Created by Yuping on 5/1/14.
//  Copyright (c) 2014 Yuping. All rights reserved.
//

#ifndef MIPSSimulator_Unit_h
#define MIPSSimulator_Unit_h

#include "AppContext.h"
#include "basic.h"

class Unit
{
public:
    Instr currentInstr;
    bool empty;
    bool ready;
    bool exe;
    bool finish;
    int cycleft;  //counting down how many cycle left
    Unit *pre;
    Unit *next;
    
public:
    Unit();
    ~Unit() {};
    virtual void execute(AppContext &appctx, int &cycle) {};
    virtual void update(AppContext &appctx, int &cycle) {};
    
};

class EXUnit : public Unit
{
public:
    EXUnit();
    ~EXUnit() {};
    virtual void execute(AppContext &appctx) {};
    virtual void update(AppContext &appctx, int cycle) {};
};

class IFUnit : public Unit
{
public:
    int HLT;
//    Unit *next;
public:
    IFUnit();
    ~IFUnit() {};
    void execute(AppContext &appctx, int &pc);
    void update(AppContext &appctx, int &pc);
};

class IDUnit : public Unit
{
public:
//    Unit *pre;
//    Unit *next;
public:
    IDUnit(Unit *previous );
    ~IDUnit() {};
    void execute(AppContext &appctx, int &pc, std::vector< std::pair<std::string, int> > &labelcontainer);
    void update(AppContext &appctx, int &pc, int cycle, std::vector< std::pair<std::string, int> > &labelcontainer);
};

class EXIUUnit : public EXUnit  //IU
{
public:
    EXIUUnit(Unit *previous );
    ~EXIUUnit() {};
    void execute(AppContext &appctx);
    void update(AppContext &appctx, int cycle);
};

class EXMEMUnit : public Unit
{
public:
    EXMEMUnit(Unit *previous );
    ~EXMEMUnit() {};
    void execute(AppContext &appctx);
    void update(AppContext &appctx, int cycle);
};

class EXADDUnit : public EXUnit
{
public:
    bool placeholder;
public:
    EXADDUnit(Unit *previous );
    ~EXADDUnit() {};
    void execute(AppContext &appctx);
    void update(AppContext &appctx, int cycle);
};

class EXMULUnit : public EXUnit
{
public:
    bool placeholder;
public:
    EXMULUnit(Unit *previous );
    ~EXMULUnit() {};
    void execute(AppContext &appctx);
    void update(AppContext &appctx, int cycle);
};

class EXDIVUnit : public EXUnit
{
public:
    bool placeholder;
public:
    EXDIVUnit(Unit *previous );
    ~EXDIVUnit() {};
    void execute(AppContext &appctx);
    void update(AppContext &appctx, int cycle);
};

class WBUnit : public Unit
{
public:
    Unit *prelist[4];
public:
    WBUnit(Unit **previous);
    ~WBUnit() {};
    void execute(AppContext &appctx );
    void update(AppContext &appctx, int cycle, std::vector<Result> &output);
};



#endif
