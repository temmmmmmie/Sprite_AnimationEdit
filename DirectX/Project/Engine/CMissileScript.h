#pragma once
#include "CScript.h"
class CMissileScript :
    public CScript
{
private:
    float       m_Speed;

public:
    virtual void Begin() override;
    virtual void Tick() override;

public:
    void BeginOverlap(CCollider2D* _OwnCollider, GameObject* _OtherObject, CCollider2D* _OtherCollider);


    CLONE(CMissileScript);
public:
    CMissileScript();
    virtual ~CMissileScript();
};

