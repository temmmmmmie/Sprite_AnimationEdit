#include "pch.h"
#include "CFlipbookRender.h"

#include "AssetMgr.h"
#include "TimeMgr.h"

CFlipbookRender::CFlipbookRender()
	: CRenderComponent(COMPONENT_TYPE::FLIPBOOK_RENDER)
	, m_SpriteIdx(0)
	, m_AccTime(0.f)
	, m_RepeatCount(1)
	, m_FPS(30.f)
	, m_Finish(false)
{
	SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"RectMesh"));
	CreateMaterial();
}

CFlipbookRender::~CFlipbookRender()
{
}

void CFlipbookRender::FinalTick()
{
	if (m_Finish || nullptr == m_CurFlipbook || m_Stop)
		return;

	// FPS 역수, 1장 넘기는데 걸리는 시간
	float Limit = 1.f / m_FPS;

	if(IsEditorFlip)m_AccTime += ENGINE_DT;
	else m_AccTime += DT;

	if (Limit < m_AccTime)
	{
		m_AccTime -= Limit;
		++m_SpriteIdx;

		if (m_SpriteIdx == m_CurFlipbook->GetSpriteCount())
		{
			m_SpriteIdx = m_CurFlipbook->GetSpriteCount() - 1;

			if(m_RepeatCount == 1)
				m_Finish = true;
			else if (1 < m_RepeatCount)
			{
				--m_RepeatCount;
				m_SpriteIdx = 0;
			}
			else
			{
				m_SpriteIdx = 0;
			}
		}
	}
}

void CFlipbookRender::Render()
{
	if (nullptr == m_CurFlipbook || nullptr == m_CurFlipbook->GetSprite(m_SpriteIdx) || IsEditorFlip)
		return;

	if (nullptr != m_CurFlipbook)
	{
		GetMaterial()->SetScalar(INT_0, (int)m_IsEmissive);
		GetMaterial()->SetScalar(VEC2_0, m_CurFlipbook->GetSprite(m_SpriteIdx)->GetLeftTop());
		GetMaterial()->SetScalar(VEC2_1, m_CurFlipbook->GetSprite(m_SpriteIdx)->GetSlice());
		GetMaterial()->SetScalar(VEC2_2, m_CurFlipbook->GetSprite(m_SpriteIdx)->GetOffset());
		GetMaterial()->SetTexture(TEX_0, m_CurFlipbook->GetSprite(m_SpriteIdx)->GetAtlas());
	}

	GetMaterial()->Binding();

	GetMesh()->Render();
}

void CFlipbookRender::CreateMaterial()
{
	wstring MtrlName = L"FlipbookMtrl";
	wstring ShaderName = L"FlipbookShader";

	Ptr<AMaterial> pMtrl = AssetMgr::GetInst()->Find<AMaterial>(MtrlName);

	if (nullptr == pMtrl)
	{
		Ptr<AGraphicShader> pShader = AssetMgr::GetInst()->Find<AGraphicShader>(L"FlipbookShader");

		if (nullptr == pShader)
		{
			pShader = new AGraphicShader;
			pShader->SetName(L"FlipbookShader");
			pShader->CreateVertexShader(L"HLSL\\flipbook.fx", "VS_Flipbook");
			pShader->CreatePixelShader(L"HLSL\\flipbook.fx", "PS_Flipbook");
			pShader->SetRSType(RS_TYPE::CULL_NONE);
			pShader->SetBSType(BS_TYPE::ALPHABLEND);
			pShader->SetDSType(DS_TYPE::NO_WRITE);
			AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());
		}

		pMtrl = new AMaterial;
		pMtrl->SetName(MtrlName);
		pMtrl->SetShader(pShader);
		pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_TRANSPARENT);
		AssetMgr::GetInst()->AddAsset(pMtrl->GetName(), pMtrl.Get());
	}

	SetMaterial(AssetMgr::GetInst()->Find<AMaterial>(MtrlName));
}


void CFlipbookRender::AddFlipbook(Ptr<AFlipbook> _Flipbook)
{
	m_vecFlipbook.push_back(_Flipbook);
}

void CFlipbookRender::SetFlipbook(int _Idx, Ptr<AFlipbook> _Flipbook)
{
	if (m_vecFlipbook.size() <= _Idx)
	{
		m_vecFlipbook.resize(_Idx + 1);
	}

	m_vecFlipbook[_Idx] = _Flipbook;
}

void CFlipbookRender::Play(int _FlipbookIdx, float _FPS, int _RepeatCount, int _StartSpriteIdx)
{
	m_CurFlipbook = m_vecFlipbook[_FlipbookIdx];
	m_SpriteIdx = _StartSpriteIdx;
	m_RepeatCount = _RepeatCount;
	m_FPS = _FPS;
	m_Finish = false;
	m_AccTime = 0.f;
}

