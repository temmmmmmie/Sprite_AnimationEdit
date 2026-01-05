#pragma once
#include <Engine/CScript.h>

class CEditorCamMoveScript :
    public CScript
{
private:
    float   m_Speed;

public:
    virtual void Tick() override;


private:
    void MoveCamera2D();
    void MoveCamera3D();


    CLONE(CEditorCamMoveScript);
public:
    CEditorCamMoveScript();
    virtual ~CEditorCamMoveScript();
};

