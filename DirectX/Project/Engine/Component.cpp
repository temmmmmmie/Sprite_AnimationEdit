#include "pch.h"
#include "Component.h"

#include "GameObject.h"

#define GET_OTHER_COMPONENT_BODY(Type) C##Type* Component::Type() { return m_Owner->Type(); }

Component::Component(COMPONENT_TYPE _Type)
	: m_Type(_Type)
	, m_Owner(nullptr)
{
}

Component::Component(const Component& _Origin)
	: Entity(_Origin)
	, m_Type(_Origin.m_Type)
	, m_Owner(nullptr)
{
}

Component::~Component()
{
}

GET_OTHER_COMPONENT_BODY(Transform);
GET_OTHER_COMPONENT_BODY(MeshRender);
GET_OTHER_COMPONENT_BODY(SpriteRender);
GET_OTHER_COMPONENT_BODY(FlipbookRender);
GET_OTHER_COMPONENT_BODY(TileRender);
GET_OTHER_COMPONENT_BODY(ParticleRender);
GET_OTHER_COMPONENT_BODY(Camera);
GET_OTHER_COMPONENT_BODY(Collider2D); 
GET_OTHER_COMPONENT_BODY(Light2D);