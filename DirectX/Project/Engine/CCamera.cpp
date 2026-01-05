#include "pch.h"
#include "CCamera.h"

#include "Device.h"
#include "RenderMgr.h"
#include "LevelMgr.h"
#include "Level.h"
#include "GameObject.h"
#include "CTransform.h"

CCamera::CCamera()
	: Component(COMPONENT_TYPE::CAMERA)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_OrthoScale(1.f)
	, m_AspectRatio(1.f)
	, m_Width(100)
	, m_Far(10000.f)
	, m_FOV(XM_PI / 2.f)
	, m_Priority(-1)
	, m_LayerCheck(0)
{
	Vec2 vRenderResol = Device::GetInst()->GetRenderResolution();
	m_Width = vRenderResol.x;
	m_AspectRatio = vRenderResol.x / vRenderResol.y;
}

CCamera::CCamera(const CCamera& _Origin)
	: Component(_Origin)
	, m_ProjType(_Origin.m_ProjType)
	, m_OrthoScale(_Origin.m_OrthoScale)
	, m_Width(_Origin.m_Width)
	, m_AspectRatio(_Origin.m_AspectRatio)
	, m_FOV(_Origin.m_FOV)
	, m_Far(_Origin.m_Far)
	, m_Priority(-1)
	, m_LayerCheck(_Origin.m_LayerCheck)
{
}

CCamera::~CCamera()
{
}

void CCamera::Begin()
{
	if(-1 != m_Priority)
		RenderMgr::GetInst()->RegisterCamera(this, m_Priority);	
}

void CCamera::FinalTick()
{
	// View 행렬 계산
	// 이동 x 회전(공전)
	// ViewSpace - 카메라가 기준이 되는 좌표계
	// 1. 카메라가 원점에 존재한다.
	// 2. 카메라가 바라보는 방향이 Z축이다.
	Vec3 WorldPos = Transform()->GetRelativePos();
	Matrix matTrans = XMMatrixTranslation(-WorldPos.x, -WorldPos.y, -WorldPos.z);

	Vec3 vR = Transform()->GetWorldDir(DIR::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR::FRONT);

	// 1행 vR
	// 2행 vU
	// 3행 vF
	// 으로 구성된 행렬의 역행렬이 View 회전행렬이다.
		
	// vR.x, vR.y, vR.z				  (1 0 0)
	// vU.x, vU.y, vU.z	 x	(vRot)  = (0 1 0)
	// vF.x, vF.y, vF.z				  (0 0 1)

	// 직교 행렬
	// 행렬 구성하고 있는 각 요소가, 행 단위로, 서로가 수직상태인 행렬
	// 직교행렬의 역행렬은 전치행렬이다.

	// vRot
	// vR.x, vU.x, vF.x
	// vR.y, vU.y, vF.y
	// vR.z, vU.z, vF.z
	Matrix matViewRot = XMMatrixIdentity();

	matViewRot._11 = vR.x;	matViewRot._12 = vU.x;	matViewRot._13= vF.x;
	matViewRot._21 = vR.y;	matViewRot._22 = vU.y;	matViewRot._23= vF.y;
	matViewRot._31 = vR.z;	matViewRot._32 = vU.z;	matViewRot._33= vF.z;

	m_matView = matTrans * matViewRot;


	// 투영(Projection) 행렬 계산
	// 1. 직교투영(2D 게임)
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{		
		m_matProj = XMMatrixOrthographicLH(m_Width * m_OrthoScale, (m_Width / m_AspectRatio) * m_OrthoScale, 1.f, m_Far);
	}

	// 2. 원근투영(3D 게임)
	else
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}
}

void CCamera::Render()
{
	// 이전프레임 정보 제거
	m_vecOpaque.clear();
	m_vecMask.clear();
	m_vecTransparent.clear();
	m_vecParticles.clear();
	m_vecPostProcess.clear();

	// 계산한 View, Proj 행렬 업데이트
	g_Trans.matView = m_matView;
	g_Trans.matProj = m_matProj;

	// 현재 레벨에 있는 물체들을, RenderDomain 에 따라 분류를 한다.
	SortObject();

	// Domain 순서대로 렌더링
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->Render();
	}

	for (size_t i = 0; i < m_vecMask.size(); ++i)
	{
		m_vecMask[i]->Render();
	}

	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->Render();
	}

	for (size_t i = 0; i < m_vecParticles.size(); ++i)
	{
		m_vecParticles[i]->Render();
	}

	// PostProcess
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		RenderMgr::GetInst()->CopyTarget();

		m_vecPostProcess[i]->Render();
	}
}

void CCamera::SortObject()
{
	Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (!(m_LayerCheck & (1 << i)))
			continue;

		Ptr<Layer> pLayer = pLevel->GetLayer(i);
		const vector<Ptr<GameObject>>& vecObjects = pLayer->GetObjects();

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (nullptr == vecObjects[j]->GetRenderCom()
				|| nullptr == vecObjects[j]->GetRenderCom()->GetMesh()
				|| nullptr == vecObjects[j]->GetRenderCom()->GetMaterial()
				|| nullptr == vecObjects[j]->GetRenderCom()->GetMaterial()->GetShader())
			{
				continue;
			}

			RENDER_DOMAIN domain = vecObjects[j]->GetRenderCom()->GetMaterial()->GetDomain();

			switch (domain)
			{
			case RENDER_DOMAIN::DOMAIN_OPAQUE:
				m_vecOpaque.push_back(vecObjects[j].Get());
				break;
			case RENDER_DOMAIN::DOMAIN_MASK:
				m_vecMask.push_back(vecObjects[j].Get());
				break;
			case RENDER_DOMAIN::DOMAIN_TRANSPARENT:
				m_vecTransparent.push_back(vecObjects[j].Get());
				break;
			case RENDER_DOMAIN::DOMAIN_PARTICLE:
				m_vecParticles.push_back(vecObjects[j].Get());
				break;
			case RENDER_DOMAIN::DOMAIN_POSTPROCESS:
				m_vecPostProcess.push_back(vecObjects[j].Get());
				break;
			}
		}
	}
}
