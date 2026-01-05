#pragma once
#include "singleton.h"

#include "GameObject.h"
#include "StructuredBuffer.h"

enum class RENDER_MODE
{
    PLAY,
    EDITOR,
};

class RenderMgr :
    public singleton<RenderMgr>
{
    SINGLE(RenderMgr);
private:
    vector<Ptr<CCamera>>    m_vecCam;       // Level 안에 있는 카메라
    Ptr<CCamera>            m_EditorCam;    // Editor 용 카메라

    vector<Ptr<CLight2D>>   m_vecLight2D;   // Level 안에 있는 광원
    Ptr<StructuredBuffer>   m_Light2DBuffer; // 광원 정보를 담아서 Register 에 바인딩

    Ptr<GameObject>         m_DebugObj;
    list<tDebugShapeInfo>   m_DebugShapeList;

    Ptr<ATexture>           m_RTTex;
    Ptr<ATexture>           m_DSTex;
    Ptr<ATexture>           m_RTCopyTex;

    RENDER_MODE             m_RenderMode;
    void(RenderMgr::*       m_RenderFunc)(void);

public:
    bool RegisterCamera(Ptr<CCamera> _Cam, int _Priority);
    void RegisterEditorCamera(Ptr<CCamera> _Cam) { m_EditorCam = _Cam; }

    void RegisterLight2D(Ptr<CLight2D> _Light) { m_vecLight2D.push_back(_Light); }

    void AddDebugShapeInfo(const tDebugShapeInfo& _Info) { m_DebugShapeList.push_back(_Info); }
    void ChangeRenderMode(RENDER_MODE _RenderMode);
    void CopyTarget() { CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), m_RTTex->GetTex2D().Get()); }



public:
    void Init();
    void Progress();
     
private:
    void Binding();
    void TargetClear();
    void TargetOMSet();

    void Render_Start();
    void Render_Play();
    void Render_Editor();
    void Render_Debug();
    void Clear();

};

