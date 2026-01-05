#include "pch.h"
#include "GameObject.h"

#include "LevelMgr.h"
#include "Level.h"
#include "Layer.h"

GameObject::GameObject()
	: m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_IsDead(false)
{
}

GameObject::GameObject(const GameObject& _Origin)
	: Entity(_Origin)
    , m_LayerIdx(-1)
    , m_IsDead(false)
	, m_Parent(nullptr)
{
	// 컴포넌트를 복사받는다.
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == _Origin.m_arrCom[i])
			continue;

		AddComponent(_Origin.m_arrCom[i]->Clone());
	}

	// 스크립트를 복사받는다.
	for (size_t i = 0; i < _Origin.m_vecScript.size(); ++i)
	{
		AddComponent(_Origin.m_vecScript[i]->Clone());
	}

	// 자식 오브젝트 복사
	for (size_t i = 0; i < _Origin.m_vecChild.size(); ++i)
	{
		GameObject* pChild = _Origin.m_vecChild[i]->Clone();
		AddChild(pChild);
		pChild->m_LayerIdx = _Origin.m_vecChild[i]->m_LayerIdx;
	}	
}

GameObject::~GameObject()
{
}

void GameObject::AddComponent(Component* _Component)
{
	if (nullptr == _Component)
		return;

	COMPONENT_TYPE Type = _Component->GetType();

	if (COMPONENT_TYPE::SCRIPT != Type)
	{
		m_arrCom[(UINT)Type] = _Component;

		// 입력으로 들어오는 컴포넌트가 RenderComponent 중 하나다
		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_Component);
		if (pRenderCom)
		{
			// 하나의 게임오브젝트는 1개의 RenderCompnent 만 가질 수 있다.
			assert(!m_RenderCom.Get());

			m_RenderCom = pRenderCom;
		}
	}

	else
	{
		CScript* pScript = dynamic_cast<CScript*>(_Component);

		assert(pScript);

		m_vecScript.push_back(pScript);
	}

	_Component->SetOwner(this);
}

void GameObject::Begin()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if(m_arrCom[i].Get())
			m_arrCom[i]->Begin();
	}

	// 대본(Script) 수행
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Begin();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Begin();
	}
}

void GameObject::Tick()
{
	// 대본(Script) 수행
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Tick();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Tick();
	}
}

void GameObject::FinalTick()
{
	// Component
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrCom[i].Get())
			m_arrCom[i]->FinalTick();
	}

	// RegisterLayer
	RegisterToLayer();

	// Child Object
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->FinalTick();
	}

	// Transform 변경점 해제
	Transform()->Clear();
}


bool GameObject::IsAncestor(GameObject* _Other)
{
	GameObject* pAncestor = m_Parent;

	while (nullptr != pAncestor)
	{
		if (pAncestor == _Other)
			return true;

		pAncestor = pAncestor->m_Parent;
	}

	return false;
}

void GameObject::RegisterToLayer()
{
	Ptr<Layer> pLayer = LevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_LayerIdx);
	pLayer->RegisterObject(this);
}


void GameObject::Render()
{	
	Transform()->Binding();

	m_RenderCom->Render();
}

void GameObject::AddChild(Ptr<GameObject> _Child)
{
	m_vecChild.push_back(_Child);

	_Child->m_Parent = this;
}