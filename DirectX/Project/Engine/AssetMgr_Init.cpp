#include "pch.h"
#include "AssetMgr.h"

#include "assets.h"

void AssetMgr::Init()
{
	CreateEngineMesh();

	CreateEngineGraphicShader();

	CreateEngineMaterial();

	CreateEngineSprite();

	CreateEngineFlipbook();
}

void AssetMgr::CreateEngineMesh()
{
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	// =========
	// PointMesh
	// =========
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	UINT i = 0;

	Ptr<AMesh> pMesh = new AMesh;
	pMesh->Create(&v, 1, &i, 1);
	AddAsset(L"PointMesh", pMesh.Get());

	// ========
	// RectMesh
	// ========
	// 0 -- 1
	// |  \ |
	// 3 -- 2
	Vtx arrVtx[4] = {};
	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);

	UINT arrIdx[6] = {};
	arrIdx[0] = 0;
	arrIdx[1] = 1;
	arrIdx[2] = 2;

	arrIdx[3] = 0;
	arrIdx[4] = 2;
	arrIdx[5] = 3;

	pMesh = new AMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 6);
	AddAsset(L"RectMesh", pMesh.Get());



	// ==================
	// RectMesh_LineStrip
	// ==================
	vecIdx.clear();
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new AMesh;
	pMesh->Create(arrVtx, 4, vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"RectMesh_LineStrip", pMesh.Get());

	// ==========
	// CircleMesh
	// ==========
	vecVtx.clear();
	vecIdx.clear();

	float Radius = 0.5f;
	UINT Poly = 40;
	float Theta = XM_2PI / (float)Poly;

	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	for (int i = 0; i < Poly + 1; ++i)
	{
		v.vPos = Vec3(Radius * cosf((float)i * Theta), Radius * sinf((float)i * Theta), 0.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, 1.f - (v.vPos.y + 0.5f));
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);
	}

	for (int i = 0; i < Poly; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new AMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CircleMesh", pMesh.Get());

	// ====================
	// CircleMesh_LineStrip
	// ====================
	vecIdx.clear();
	for (int i = 0; i < Poly + 1; ++i)
	{
		vecIdx.push_back(i + 1);
	}

	pMesh = new AMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CircleMesh_LineStrip", pMesh.Get());
}

void AssetMgr::CreateEngineGraphicShader()
{
	// ===========
	// Std2DShader
	// ===========
	Ptr<AGraphicShader> pShader = new AGraphicShader;
	pShader->CreateVertexShader(L"HLSL//std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"HLSL//std2d.fx", "PS_Std2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	AddAsset(L"Std2DShader", pShader.Get());

	// ================
	// Std2D_AlphaBlend
	// ================
	pShader = new AGraphicShader;
	pShader->CreateVertexShader(L"HLSL//std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"HLSL//std2d.fx", "PS_Std2D_AlphaBlend");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	AddAsset(L"Std2D_AlphaBlend", pShader.Get());

	// ===================
	// Monochrome(Èæ¹éÈ¿°ú)
	// ===================
	pShader = new AGraphicShader;
	pShader->CreateVertexShader(L"HLSL//std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"HLSL//std2d.fx", "PS_Monochrome");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	AddAsset(L"MonochromeShader", pShader.Get());

	// =================================
	// Monochrome(Èæ¹éÈ¿°ú) - PostProcess
	// =================================
	pShader = new AGraphicShader;
	pShader->CreateVertexShader(L"HLSL//postprocess.fx", "VS_Monochrome");
	pShader->CreatePixelShader(L"HLSL//postprocess.fx", "PS_Monochrome");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	AddAsset(L"MonochromePPShader", pShader.Get());


	// ===========
	// DebugShader
	// ===========
	pShader = new AGraphicShader;
	pShader->CreateVertexShader(L"HLSL//debug.fx", "VS_Debug");
	pShader->CreatePixelShader(L"HLSL//debug.fx", "PS_Debug");
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	AddAsset(L"DebugShader", pShader.Get());
}

void AssetMgr::CreateEngineMaterial()
{
	// =========
	// Std2DMtrl
	// =========
	Ptr<AMaterial> pMtrl = new AMaterial;
	pMtrl->SetShader((AGraphicShader*)Find(ASSET_TYPE::GRAPHICSHADER, L"Std2DShader").Get());
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASK);
	AddAsset(L"Std2DMtrl", pMtrl.Get());

	// ================
	// Std2D_AlphaBlend
	// ================
	pMtrl = new AMaterial;
	pMtrl->SetShader((AGraphicShader*)Find(ASSET_TYPE::GRAPHICSHADER, L"Std2D_AlphaBlend").Get());
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_TRANSPARENT);
	AddAsset(L"Std2D_AlphaBlendMtrl", pMtrl.Get());

	// ==============
	// MonochromeMtrl
	// ==============
	pMtrl = new AMaterial;
	pMtrl->SetShader((AGraphicShader*)Find(ASSET_TYPE::GRAPHICSHADER, L"MonochormeShader").Get());
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_OPAQUE);
	AddAsset(L"MonochromeMtrl", pMtrl.Get());

	// ================
	// MonochromePPMtrl
	// ================
	pMtrl = new AMaterial;
	pMtrl->SetShader((AGraphicShader*)Find(ASSET_TYPE::GRAPHICSHADER, L"MonochromePPShader").Get());
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"MonochromePPMtrl", pMtrl.Get());

	// ==============
	// DebugShapeMtrl
	// ==============
	pMtrl = new AMaterial;
	pMtrl->SetShader((AGraphicShader*)Find(ASSET_TYPE::GRAPHICSHADER, L"DebugShader").Get());
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_DEBUG);
	AddAsset(L"DebugShapeMtrl", pMtrl.Get());
}

void AssetMgr::CreateEngineSprite()
{
	Load<ATexture>(L"SpriteExample", L"Texture//SpriteExample.jpg");
	Load<ATexture>(L"AddTex", L"Texture//Add.jpg");

	Ptr<ATexture> pAtlas = Load<ATexture>(L"ExplosionAtlas", L"Texture//Ex.png");

	int i = 0;
	for (UINT Row = 0; Row < 9; ++Row)
	{
		for (UINT Col = 0; Col < 10; ++Col, ++i)
		{
			wchar_t szBuffer[256] = {};
			swprintf_s(szBuffer, 256, L"Explosion_%02d", i);

			Ptr<ASprite> pSprite = new ASprite;
			pSprite->SetAtlas(pAtlas);
			pSprite->SetLeftTop(Vec2(Col * 320.f, Row * 240.f));
			pSprite->SetSlice(Vec2(320.f, 240.f));
			AddAsset(szBuffer, pSprite.Get());
		}
	}
	pAtlas = Load<ATexture>(L"TileAtlas", L"Texture//TILE.bmp");

	i = 0;
	for (UINT Row = 0; Row < 6; ++Row)
	{
		for (UINT Col = 0; Col < 8; ++Col, ++i)
		{
			wchar_t szBuffer[256] = {};
			swprintf_s(szBuffer, 256, L"TILE_%02d", i);

			Ptr<ASprite> pSprite = new ASprite;
			pSprite->SetAtlas(pAtlas);
			pSprite->SetLeftTop(Vec2(Col * 64.f, Row * 64.f));
			pSprite->SetSlice(Vec2(64.f, 64.f));
			AddAsset(szBuffer, pSprite.Get());
		}
	}
}

void AssetMgr::CreateEngineFlipbook()
{
	Ptr<AFlipbook> pFlipbook = new AFlipbook;

	int i = 0;
	for (UINT Row = 0; Row < 9; ++Row)
	{
		for (UINT Col = 0; Col < 10; ++Col, ++i)
		{
			wchar_t szBuffer[256] = {};
			swprintf_s(szBuffer, 256, L"Explosion_%02d", i);

			Ptr<ASprite> pSprite = Find<ASprite>(szBuffer);
			pFlipbook->AddSprite(pSprite);
		}
	}

	AddAsset(L"Explosion", pFlipbook.Get());
}
