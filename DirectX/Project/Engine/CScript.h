#pragma once
#include "Component.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Device.h"
#include "PathMgr.h"
#include "AssetMgr.h"

class CScript :
    public Component
{
private:

public:
    virtual void Tick() = 0;
    virtual void FinalTick() final;
    virtual CScript* Clone() = 0;

public:
    class CRenderComponent* GetRenderComponent();

public:
    CScript();
    virtual ~CScript();
};

