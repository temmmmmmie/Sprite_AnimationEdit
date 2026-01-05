#include "pch.h"
#include "RenderMgr.h"

#include "Device.h"
#include "StructuredBuffer.h"

RenderMgr::RenderMgr()
    : m_RenderMode(RENDER_MODE::EDITOR)
    , m_RenderFunc(nullptr)
{
    ChangeRenderMode(m_RenderMode);

    m_RTTex = AssetMgr::GetInst()->Find<ATexture>(L"RenderTargetTex");
    m_DSTex = AssetMgr::GetInst()->Find<ATexture>(L"DepthStencilTex");

    m_Light2DBuffer = new StructuredBuffer;
}

RenderMgr::~RenderMgr()
{
}


void RenderMgr::Init()
{
    // PostProcess 용도, RenderTarget 을 복사받을 텍스쳐    
    m_RTCopyTex = AssetMgr::GetInst()->CreateTexture(L"PostProcessTex"
                                                    , m_RTTex->GetWidth(), m_RTTex->GetHeight()
                                                    , m_RTTex->GetFormat(), D3D11_BIND_SHADER_RESOURCE);
    m_RTCopyTex->Binding(15);

    // Debug 렌더링 용도 게임 오브젝트
    m_DebugObj = new GameObject;
    m_DebugObj->AddComponent(new CTransform);
    m_DebugObj->AddComponent(new CMeshRender);    
}

void RenderMgr::Binding()
{
    // RenderMgr 에 등록된 광원의 데이터를 바인딩
    // 구조화버퍼의 크기가 모자라면 확장한다.
    if (m_Light2DBuffer->GetElementCount() < m_vecLight2D.size())
    {
        m_Light2DBuffer->Create(sizeof(tLight2DInfo), m_vecLight2D.size(), SB_TYPE::SRV_ONLY, true);
    }

    // RenderMgr 에 등록된 Light2D 컴포넌트가 보유한 Light2DInfo 정보를 벡터에 모은다.
    static vector<tLight2DInfo> vecLight2DInfo;
    vecLight2DInfo.clear();
    for (size_t i = 0; i < m_vecLight2D.size(); ++i)
    {
        vecLight2DInfo.push_back(m_vecLight2D[i]->GetLight2DInfo());
    }

    // 모아진 Light2DInfo 정보를 구조화버퍼에 보낸다.
    if (false == m_vecLight2D.empty())
    {
        m_Light2DBuffer->SetData(vecLight2DInfo.data(), sizeof(tLight2DInfo) * vecLight2DInfo.size());

        // 광원 정보를 전달받은 구조화버퍼를 t13 레지스터에 바인딩한다.
        m_Light2DBuffer->Binding(13);
    }

    // Global 데이터 상수 레지스터에 전달
    g_Global.Light2DCount = vecLight2DInfo.size();

    static Ptr<ConstBuffer> pBuffer = Device::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
    pBuffer->SetData(&g_Global);
    pBuffer->Binding();
    pBuffer->Binding_CS();
}

void RenderMgr::TargetClear()
{
    // RenderTarget 클리어
    float arrColor[4] = { 0.f, 0.f, 0.f, 1.f };
    CONTEXT->ClearRenderTargetView(m_RTTex->GetRTV().Get(), arrColor);

    // DepthStencilTarget 클리어
    // 깊이의 범위는 0 ~ 1
    // 최대값으로 초기화 해야 그것보다 가까운 물체들이 깊이테스트를 통과해서 그려질 수 있다.
    // 통과한 물체는 자신이 그려지는 영역에 해당하는 부분에 깊이값을 자신의 것으로 대체한다.
    // (다음 렌더링 될 물체가 그것을 기반으로 깊이테스트를 할 수 있도록 하기 위함)
    CONTEXT->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void RenderMgr::TargetOMSet()
{
    // 출력타겟 지정
    CONTEXT->OMSetRenderTargets(1, m_RTTex->GetRTV().GetAddressOf(), m_DSTex->GetDSV().Get());
}

void RenderMgr::Render_Start()
{
    // 렌더링 시작전에 필요한 리소스를 레지스터에 바인딩
    Binding();

    // RenderTarget, DepthStencilTarget 클리어
    TargetClear();

    // RenderTarget, DepthStencilTarget 을 출력으로 지정
    TargetOMSet();
}

void RenderMgr::Render_Play()
{
    // 카메라로 Level 렌더링
    for (size_t i = 0; i < m_vecCam.size(); ++i)
    {
        if (nullptr == m_vecCam[i])
            continue;

        m_vecCam[i]->Render();
    }
}

void RenderMgr::Render_Editor()
{
    if (nullptr != m_EditorCam)
        m_EditorCam->Render();
}


void RenderMgr::Progress()
{
    // 전역 상수데이터 전달 및 타겟 클리어
    Render_Start();
    
    // 렌더링
    (this->*m_RenderFunc)();

    // DebugRender
    Render_Debug();

    // 리소스 클리어
    Clear();
}


void RenderMgr::Render_Debug()
{
    if (m_DebugShapeList.empty())
        return;

    list<tDebugShapeInfo>::iterator iter = m_DebugShapeList.begin();
    for (; iter != m_DebugShapeList.end(); )
    {
        // 쉐이프 타입에 맞는 메시 설정
        switch (iter->ShapeType)
        {
        case DEBUG_SHAPE::RECT:
            m_DebugObj->MeshRender()->SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"RectMesh_LineStrip"));
            break;
        case DEBUG_SHAPE::CIRCLE:
            m_DebugObj->MeshRender()->SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"CircleMesh_LineStrip"));
            break;
        case DEBUG_SHAPE::LINE:
            m_DebugObj->MeshRender()->SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"PointMesh"));
            break;
        case DEBUG_SHAPE::CUBE:
            m_DebugObj->MeshRender()->SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"CubeMesh"));
            break;
        case DEBUG_SHAPE::SPHERE:
            m_DebugObj->MeshRender()->SetMesh(AssetMgr::GetInst()->Find<AMesh>(L"SphereMesh"));
            break;
        }

        // 위치 크기 회전정보 세팅
        if (iter->matWorld != XMMatrixIdentity())
        {
            m_DebugObj->Transform()->SetmatWorld(iter->matWorld);
        }

        else
        {
            m_DebugObj->Transform()->SetRelativePos(iter->WorldPos);
            m_DebugObj->Transform()->SetRelativeScale(iter->Scale);            
            m_DebugObj->Transform()->SetRelativeRot(iter->Rotation);
            m_DebugObj->Transform()->FinalTick();            
        }
        
        // 재질 설정
        m_DebugObj->MeshRender()->SetMaterial(AssetMgr::GetInst()->Find<AMaterial>(L"DebugShapeMtrl"));

        // 쉐이더 깊이판정 옵션 설정
        if (iter->DepthTest)
            m_DebugObj->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::LESS);
        else
            m_DebugObj->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

        m_DebugObj->MeshRender()->GetMaterial()->SetScalar(VEC4_0, iter->Color);
        m_DebugObj->Render();

        iter->Age += ENGINE_DT;
        if (iter->Life < iter->Age)
        {
            iter = m_DebugShapeList.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void RenderMgr::Clear()
{
    // 등록받았던 광원을 해제한다.
    m_vecLight2D.clear();

    // 광원의 정보를 담아서 t13 에 바인딩했던 구조화버퍼를 클리어한다.
    m_Light2DBuffer->Clear();
}

bool RenderMgr::RegisterCamera(Ptr<CCamera> _Cam, int _Priority)
{
    if (_Priority < 0)
        return false;

    if (m_vecCam.size() < _Priority + 1)
    {
        m_vecCam.resize(_Priority + 1);
    }

    m_vecCam[_Priority] = _Cam;

    return true;
}

void RenderMgr::ChangeRenderMode(RENDER_MODE _RenderMode)
{
    m_RenderMode = _RenderMode;

    if (RENDER_MODE::PLAY == m_RenderMode)
        m_RenderFunc = &RenderMgr::Render_Play;
    else
        m_RenderFunc = &RenderMgr::Render_Editor;
}