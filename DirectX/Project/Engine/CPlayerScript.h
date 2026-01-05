#pragma once
#include "CScript.h"


class CPlayerScript :
    public CScript
{
private:
    float   m_Speed;
    float   m_Pow;


public:
    void BeginOverlap(CCollider2D* _OwnCollider, GameObject* _OtherObject, CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OwnCollider, GameObject* _OtherObject, CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OwnCollider, GameObject* _OtherObject, CCollider2D* _OtherCollider);


public:
    virtual void Begin() override;
    virtual void Tick() override;


    CLONE(CPlayerScript);
public:
    CPlayerScript();
    virtual ~CPlayerScript();
};

