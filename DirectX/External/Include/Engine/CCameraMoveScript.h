#pragma once
#include "CScript.h"

class CCameraMoveScript :
    public CScript
{
private:
    float   m_Speed;

public:
    virtual void Tick() override;


private:
    void MoveCamera2D();
    void MoveCamera3D();


    CLONE(CCameraMoveScript);
public:
    CCameraMoveScript();
    virtual ~CCameraMoveScript();
};

