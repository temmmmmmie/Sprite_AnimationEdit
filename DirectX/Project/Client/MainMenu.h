#pragma once

#include "EditorUI.h"

#include <Engine/Level.h>

class MainMenu :
    public EditorUI
{
private:
    Ptr<ALevel>  m_CopyLevel;


public:
    void File();
    void Level();
    void GameObject();
    void Asset();
    void Editor();


public:
    virtual void Tick() override;
    virtual void Tick_UI() override;

public:
    MainMenu();
    virtual ~MainMenu();
};

