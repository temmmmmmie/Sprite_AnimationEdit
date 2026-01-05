#pragma once
#include "EditorUI.h"
#include "SpriteEditor.h"
class Grid :
    public EditorUI
{
public:
    SpriteEditor* spriteedit;
private:
    int Col;
    int Row;

    virtual void Tick_UI() override;
    void Slice();
    void Render();
public:
    Grid();
    ~Grid();
};

