#include "pch.h"
#include "CPlayerScript.h"

#include "RenderMgr.h"
#include "TaskMgr.h"
#include "GameObject.h"
#include "CMissileScript.h"

CPlayerScript::CPlayerScript()
	: m_Speed(100)
	, m_Pow(0.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Begin()
{
	if (GetOwner()->Collider2D())
	{
		GetOwner()->Collider2D()->AddDelegate(this
			, (OVERLAP)&CPlayerScript::BeginOverlap
			, (OVERLAP)&CPlayerScript::Overlap
			, (OVERLAP)&CPlayerScript::EndOverlap);
	}
}

void CPlayerScript::Tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vScale = Transform()->GetRelativeScale();
	Vec3 vRot = Transform()->GetRelativeRot();

	if (KEY_PRESSED(KEY::LEFT))
		vPos.x -= DT * m_Speed;
	if (KEY_PRESSED(KEY::RIGHT))
		vPos.x += DT * m_Speed;
	if (KEY_PRESSED(KEY::UP))
		vPos.y += DT * m_Speed;
	if (KEY_PRESSED(KEY::DOWN))
		vPos.y -= DT * m_Speed;

	if (KEY_PRESSED(KEY::Z))
	{
		vRot.z += XM_PI * DT;
		vPos.z += 100 * DT;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeScale(vScale);
	Transform()->SetRelativeRot(vRot);


	if (KEY_TAP(KEY::SPACE))
	{
		GameObject* pMissileObj = new GameObject;
		pMissileObj->SetName(L"Missile");
		pMissileObj->AddComponent(new CTransform);
		pMissileObj->AddComponent(new CMeshRender);
		pMissileObj->AddComponent(new CCollider2D);
		pMissileObj->AddComponent(new CMissileScript);

		pMissileObj->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
		pMissileObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

		pMissileObj->GetRenderCom()->SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"RectMesh"));
		pMissileObj->GetRenderCom()->SetMaterial(AssetMgr::GetInst()->Find<AMaterial>(L"Std2DMtrl"));		

		SpawnGameObject(3, pMissileObj);
	}

	//if (nullptr != GetRenderComponent())
	//{
	//	m_Pow += DT * 0.3f;
	//	if (m_Pow > 1.f)
	//		m_Pow = 1.f;

	//	GetRenderComponent()->GetMaterial()->SetScalar(FLOAT_0, m_Pow);
	//}
}


void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, GameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	int a = 0;
}

void CPlayerScript::Overlap(CCollider2D* _OwnCollider, GameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _OwnCollider, GameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}
