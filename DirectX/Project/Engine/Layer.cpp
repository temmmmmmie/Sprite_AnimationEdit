#include "pch.h"
#include "Layer.h"

#include "GameObject.h"

Layer::Layer(int _Idx)
	: m_LayerIdx(_Idx)
{
}

Layer::Layer(const Layer& _Origin)
	: Entity(_Origin)
	, m_LayerIdx(_Origin.m_LayerIdx)
{
	for (size_t i = 0; i < _Origin.m_vecParents.size(); ++i)
	{
		AddObject(_Origin.m_vecParents[i]->Clone(), false);
	}
}

Layer::~Layer()
{

}

void Layer::AddObject(GameObject* _Object, bool _MoveWithChild)
{
	m_vecParents.push_back(_Object);

	// 부모오브젝트 포함, 자식 오브젝트들 모두 레벨순회로 접근
	static list<GameObject*> queue;
	queue.clear();
	queue.push_back(_Object);

	while (!queue.empty())
	{
		GameObject* Object = queue.front();
		queue.pop_front();

		// Layer 에 진입한 오브젝트가, 자신이 몇번 레이어 소속인지 알게 함
		// Layer 에 진입한 오브젝트 본인은 LayerIndex 갱신
		// Layer 에 진입한 오브젝트의 자식인 경우, 소속이 없거나 자식까지 같이 이동하기로 한 경우, 레이어Index 변경
		if(Object == _Object || Object->m_LayerIdx == -1 || _MoveWithChild)
			Object->m_LayerIdx = m_LayerIdx;

		for (size_t i = 0; i < Object->m_vecChild.size(); ++i)
		{
			queue.push_back(Object->m_vecChild[i].Get());
		}
	}
}

void Layer::Begin()
{
	for (size_t i = 0; i < m_vecParents.size(); ++i)
	{
		m_vecParents[i]->Begin();
	}
}

void Layer::Tick()
{
	for (size_t i = 0; i < m_vecParents.size(); ++i)
	{
		m_vecParents[i]->Tick();
	}
}

void Layer::FinalTick()
{
	vector<Ptr<GameObject>>::iterator iter = m_vecParents.begin();

	for (; iter != m_vecParents.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
		{
			iter = m_vecParents.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
