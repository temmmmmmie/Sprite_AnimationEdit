#include "pch.h"
#include "LevelMgr.h"

#include "device.h"
#include "AssetMgr.h"
#include "CollisionMgr.h"

#include "ATexture.h"
#include "CPlayerScript.h"
#include "CCameraMoveScript.h"
#include "CMonsterScript.h"

#include "AComputeShader.h"
#include "ASetColorCS.h"

LevelMgr::LevelMgr()
{}

LevelMgr::~LevelMgr()
{}

Ptr<GameObject> LevelMgr::FindObjectByName(const wstring& _Name)
{
	if (nullptr == m_CurLevel)
		return nullptr;

	return m_CurLevel->FindObjectByName(_Name);
}

void LevelMgr::Init()
{
	// StructruedBuffer UAV Test
	Ptr<StructuredBuffer> pBuffer = new StructuredBuffer;
	pBuffer->Create(sizeof(Vec4), 10, SRV_UAV, true);


	// 레벨 생성 및 초기오브젝트 설정
	m_CurLevel = new ALevel;
	m_CurLevel->SetName(L"TestLevel");
	m_CurLevel->GetLayer(0)->SetName(L"Default");
	m_CurLevel->GetLayer(1)->SetName(L"Background");
	m_CurLevel->GetLayer(2)->SetName(L"Tile");
	m_CurLevel->GetLayer(3)->SetName(L"Player");
	m_CurLevel->GetLayer(4)->SetName(L"Enemy");
	m_CurLevel->GetLayer(31)->SetName(L"UI");


	Ptr<GameObject> pObject = nullptr;

	// Camera 오브젝트
	pObject = new GameObject;
	pObject->SetName(L"MainCamera");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCamera);
	pObject->AddComponent(new CCameraMoveScript);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, -100.f));

	pObject->Camera()->SetOrthoScale(1.f);
	pObject->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pObject->Camera()->LayerCheckAll(); // 모든 레이어를 다 볼수 있도록 함
	pObject->Camera()->SetPriority(0); // 메인카메라로 설정
		
	m_CurLevel->AddObject(0, pObject.Get());



	// Light2D 광원
	pObject = new GameObject;
	pObject->SetName(L"Light");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight2D);

	pObject->Transform()->SetRelativePos(Vec3(-100.f, 0.f, 0.f));
		
	pObject->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pObject->Light2D()->SetLightColor(Vec3(0.8f, 0.8f, 0.8f));
	pObject->Light2D()->SetRadius(300.f);

	m_CurLevel->AddObject(0, pObject.Get());


	// 광원 하나더 추가해보기
	//pObject = pObject->Clone();
	//pObject->Transform()->SetRelativePos(Vec3(100.f, 0.f, 0.f));
	//pObject->Light2D()->SetLightColor(Vec3(0.2f, 0.2f, 0.8f));
	//m_CurLevel->AddObject(0, pObject.Get());



	// TileRender 오브젝트
	pObject = new GameObject;
	pObject->SetName(L"Tile");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CTileRender);

	pObject->Transform()->SetRelativePos(Vec3(-500.f, 500.f, 200.f));

	int Row = 5;
	int Col = 5;
	pObject->TileRender()->SetColRow(Col, Row);
	pObject->TileRender()->SetTileSize(Vec2(64.f, 64.f));

	pObject->TileRender()->SetAltas(FIND(ATexture, L"TileAtlas"));

	for (int i = 0; i < Row; ++i)
	{
		for (int j = 0; j < Col; ++j)
		{
			pObject->TileRender()->SetSprite(j, i, FIND(ASprite, L"TILE_24"));
		}
	}

	m_CurLevel->AddObject(0, pObject.Get());


	// 타일랜더 복사 테스트
	pObject = pObject->Clone();
	m_CurLevel->AddObject(0, pObject.Get());
	pObject->Transform()->SetRelativePos(Vec3(500.f, 500.f, 200.f));



	// Player 오브젝트
	pObject = new GameObject;
	pObject->SetName(L"Player");
	pObject->AddComponent(new CTransform); 
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativePos(Vec3(200.f, 0.f, 100.f));
	pObject->Transform()->SetRelativeScale(Vec3(320.f, 240.f, 1.f));

	pObject->MeshRender()->SetMesh(FIND(AMesh, L"RectMesh"));
	pObject->MeshRender()->SetMaterial(FIND(AMaterial, L"Std2DMtrl"));
	pObject->MeshRender()->GetMaterial()->SetTexture(TEX_0, LOAD(ATexture, L"Texture\\Character.png", L"Texture\\Character.png"));

	pObject->Collider2D()->SetIndependentScale(false);
	pObject->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetScale(Vec2(1.f, 1.f));

	Ptr<GameObject> pChild = pObject->Clone();
	pChild->Transform()->SetIndependentScale(true);
	pChild->Transform()->SetRelativePos(Vec3(-200.f, 0.f, 0.f));


	pObject->AddChild(pChild);

	// 최상위 부모인 Player 를 0번 레이어에 넣는다.
	m_CurLevel->AddObject(3, pObject.Get());





	// Monster 오브젝트
	pObject = new GameObject;
	pObject->SetName(L"Monster");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CFlipbookRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CMonsterScript);

	pObject->Transform()->SetRelativePos(Vec3(-200.f, 0.f, 200.f));
	pObject->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	pObject->Collider2D()->SetOffset(Vec2(0.f, 0.f));
	pObject->Collider2D()->SetScale(Vec2(1.f, 1.f));

	pObject->FlipbookRender()->SetFlipbook(0, FIND(AFlipbook, L"Explosion"));
	pObject->FlipbookRender()->Play(0, 60);

	m_CurLevel->AddObject(4, pObject.Get());


	pObject = pObject->Clone();
	m_CurLevel->AddObject(4, pObject.Get());
	pObject->Transform()->SetRelativePos(Vec3(200.f, 0.f, 100.f));


	// Particle Object
	pObject = new GameObject;
	pObject->SetName(L"Particle");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CParticleRender);

	pObject->Transform()->SetRelativePos(Vec3(-300.f, 0.f, 200.f));
	pObject->ParticleRender()->SetParticleTexture(LOAD(ATexture, L"Texture\\particle\\CartoonSmoke.png", L"Texture\\particle\\Bubbles99px.png"));

	m_CurLevel->AddObject(0, pObject.Get());


	// PostProcess Object
	/*pObject = new GameObject;
	pObject->SetName(L"PostProcess");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));
	pObject->MeshRender()->SetMesh(FIND(AMesh, L"RectMesh"));
	pObject->MeshRender()->SetMaterial(FIND(AMaterial, L"MonochromePPMtrl"));

	pObject->MeshRender()->GetMaterial()->SetTexture(TEX_0, LOAD(ATexture, L"Texture\\noise\\noise_03.jpg", L"Texture\\noise\\noise_03.jpg"));


	m_CurLevel->AddObject(0, pObject.Get());*/

	// 충돌 체크 지정
	CollisionMgr::GetInst()->CollisionCheck(3, 4);

	m_CurLevel->ChangeLevelState(LEVEL_STATE::STOP);
}

void LevelMgr::Progress()
{
	// 레벨 업데이트
	m_CurLevel->Deregister();
	
	if (LEVEL_STATE::PLAY == m_CurLevel->GetState())
	{
		m_CurLevel->Tick();
	}
	
	m_CurLevel->FinalTick();

	// 충돌 검사 진행
	CollisionMgr::GetInst()->Progress();
}
