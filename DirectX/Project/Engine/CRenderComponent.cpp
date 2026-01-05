#include "pch.h"
#include "CRenderComponent.h"


CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: Component(_Type)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _Origin)
	: Component(_Origin)
	, m_Mesh(_Origin.m_Mesh)
	, m_SharedMtrl(_Origin.m_SharedMtrl)
	, m_DynamicMtrl(nullptr)
	, m_CurMtrl(nullptr)
{
	if (_Origin.m_CurMtrl == _Origin.m_SharedMtrl)
	{
		m_CurMtrl = m_SharedMtrl;
	}
	else if(nullptr != _Origin.m_DynamicMtrl)
	{
		CreateDynamicMaterial();
	}
}

CRenderComponent::~CRenderComponent()
{
}

Ptr<AMaterial> CRenderComponent::GetDynamicMaterial()
{
	if (nullptr != m_DynamicMtrl)
	{
		m_CurMtrl = m_DynamicMtrl;
		return m_DynamicMtrl;
	}

	CreateDynamicMaterial();

	return m_DynamicMtrl;
}

void CRenderComponent::CreateDynamicMaterial()
{
	if (nullptr == m_SharedMtrl)
		return;

	m_CurMtrl = m_DynamicMtrl = m_SharedMtrl->Clone();
}