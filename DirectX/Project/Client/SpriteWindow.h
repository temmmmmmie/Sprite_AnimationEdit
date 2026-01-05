#pragma once
#include "EditorUI.h"
#include "SpriteEditor.h"
class SpriteWindow :
    public EditorUI
{
public:
    SpriteEditor* spriteedit;
private:
    virtual void Tick_UI() override;
public:
    SpriteWindow();
    ~SpriteWindow();

friend class TreeUI;
};

