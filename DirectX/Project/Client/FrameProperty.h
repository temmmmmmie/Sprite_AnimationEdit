#pragma once
#include "EditorUI.h"
#include "AnimationEditor.h"
class FrameProperty :
    public EditorUI
{
public:
    AnimationEditor* animationedit;
private:
    virtual void Tick_UI() override;

public:
    FrameProperty();
    ~FrameProperty();
};

