#pragma once
#include "CRenderComponent.h"

#include "AFlipbook.h"

class CFlipbookRender :
    public CRenderComponent
{
private:
    vector<Ptr<AFlipbook>>  m_vecFlipbook;
    Ptr<AFlipbook>          m_CurFlipbook;
    int                     m_SpriteIdx;
    float                   m_AccTime;
    float                   m_FPS;
    int                     m_RepeatCount;      // 반복 횟수
    bool                    m_Finish;
    bool                    m_IsEmissive;       // 스스로 발광체인지 체크
    bool                    m_Stop;

public:
    bool                    IsEditorFlip;

    void AddFlipbook(Ptr<AFlipbook> _Flipbook);
    void SetFlipbook(int _Idx, Ptr<AFlipbook> _Flipbook);
    Ptr<AFlipbook> GetFlipbook() { return m_CurFlipbook; }

    void Play(int _FlipbookIdx, float _FPS, int _RepeatCount = 0, int _StartSpriteIdx = 0);
    void Stop() { m_Stop = true; }
    void Resume() { m_Stop = false; }
    void PrevFrame() { 
        if (nullptr == m_CurFlipbook || 0 == m_CurFlipbook->GetSpriteCount()) return;

        if (m_SpriteIdx == 0) m_SpriteIdx = m_CurFlipbook->GetSpriteCount() - 1;
        else --m_SpriteIdx; 
    }
    void NextFrame() {
        if (nullptr == m_CurFlipbook || 0 == m_CurFlipbook->GetSpriteCount()) return;
        
        if (m_SpriteIdx == m_CurFlipbook->GetSpriteCount() - 1) m_SpriteIdx = 0;
        else ++m_SpriteIdx;
    }
    int GetFrameIdx() { return m_SpriteIdx; }
    void SetFrame(int frameidx) { m_SpriteIdx = frameidx; }
    void AddFrame() { 
        Ptr<ASprite> pSprite = new ASprite;
        m_CurFlipbook->AddSprite(pSprite);
    }
    void SetEmissive(bool _Emissive) { m_IsEmissive = _Emissive; }

    Ptr<ASprite> GetCurSprite() { 
        if (nullptr == m_CurFlipbook) return nullptr;
        return m_CurFlipbook->GetSprite(m_SpriteIdx); 
    };
    Ptr<ASprite> GetPrevSprite() { 
        if (m_SpriteIdx - 1 < 0 || nullptr == m_CurFlipbook) return nullptr;
        return m_CurFlipbook->GetSprite(m_SpriteIdx - 1); 
    };

public:
    virtual void FinalTick() override;
    virtual void Render() override;

private:
    void CreateMaterial();

public:
    CLONE(CFlipbookRender);
    CFlipbookRender();
    virtual ~CFlipbookRender();
};

