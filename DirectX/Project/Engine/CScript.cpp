#include "pch.h"
#include "CScript.h"

#include "GameObject.h"

CScript::CScript()
	: Component(COMPONENT_TYPE::SCRIPT)
{
}

CScript::~CScript()
{
}

void CScript::FinalTick()
{
}

CRenderComponent* CScript::GetRenderComponent()
{
	return GetOwner()->GetRenderCom().Get();
}
