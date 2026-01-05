#pragma once
#include "AssetUI.h"
class MaterialUI :
    public AssetUI
{

public:
    virtual void Tick_UI() override;

public:
    MaterialUI();
    virtual ~MaterialUI();
};

