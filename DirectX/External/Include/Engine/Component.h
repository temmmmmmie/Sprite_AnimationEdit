#pragma once
#include "Entity.h"


#define GET_OTHER_COMPONENT(Type)  class C##Type* Type();

class GameObject;

class Component :
    public Entity
{
private:
    const COMPONENT_TYPE    m_Type;
    GameObject*             m_Owner;

public:
    GET(COMPONENT_TYPE, Type);
    GET(GameObject*, Owner);

    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(Light2D)
    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(SpriteRender);
    GET_OTHER_COMPONENT(FlipbookRender);
    GET_OTHER_COMPONENT(TileRender);
    GET_OTHER_COMPONENT(ParticleRender);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Collider2D);


private:
    SET(GameObject*, Owner);

public:
    virtual void Begin() {}
    virtual void FinalTick() = 0;


    virtual Component* Clone() = 0;
public:
    Component(COMPONENT_TYPE _Type);
    Component(const Component& _Origin);
    virtual ~Component();

    friend class GameObject;
};

