#pragma once

#include "Component.h"


#include "AMesh.h"
#include "AMaterial.h"


class CRenderComponent
	: public Component
{
private:
	Ptr<AMesh>          m_Mesh;

	// 에셋 매니저를 통해서 관리되고 있는, 공유해서 사용할 수 있는 재질 에셋
	Ptr<AMaterial>		m_SharedMtrl;	

	// 게임이 플레이 도중, 오브젝트가 자신만의 재질을 임시로 가지게 됨, 
	// 본인만의 특정 전달 데이터를 동적재질을 이용해서 전달
	// 공유재질을 건드리면, 같은 재질을 사용하는 다른 오브젝트들도 모두 렌더링에 영향을 받기 때문
	Ptr<AMaterial>		m_DynamicMtrl;

	// 사용하고있는 현재 재질
	Ptr<AMaterial>		m_CurMtrl;

public:
	GET_SET(Ptr<AMesh>, Mesh);

	Ptr<AMaterial> GetMaterial() { return m_CurMtrl; }
	Ptr<AMaterial> GetDynamicMaterial();
	void SetMaterial(Ptr<AMaterial> _Mtrl) { m_CurMtrl = m_SharedMtrl = _Mtrl; }
	void SetSharedMaterial() { m_CurMtrl = m_SharedMtrl ; }
	void CreateDynamicMaterial();


public:
	virtual void Render() = 0;
	virtual CRenderComponent* Clone() = 0;

public:
	CRenderComponent(COMPONENT_TYPE _Type);
	CRenderComponent(const CRenderComponent& _Origin);
	virtual ~CRenderComponent();
};

