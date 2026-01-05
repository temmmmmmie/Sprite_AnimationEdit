#pragma once
#include "EditorUI.h"
#include "Engine/CFlipbookRender.h"
class AnimationEditor :
    public EditorUI
{
private:
    Ptr<CFlipbookRender> Editorflip;
    int SelectedIdx;


    EditorUI* FramePropWind;
public:
    Ptr<class ASprite> GetSelectedSprite() {
        Ptr<AFlipbook> flip = Editorflip->GetFlipbook();
        if (nullptr == flip) return nullptr;
        return flip->GetSprite(SelectedIdx);
    }

    Ptr<CFlipbookRender> GetFlipBookRender() { return Editorflip; }

    virtual void Tick() override;
    virtual void Tick_UI() override;

    void Menubar();
    void AnimationPreview();
    void AniCtrlBtn();
    void FramePreview(bool _isplaying);
    bool FlipbookDropCheck();
    bool SpriteDropCheck(int idx);
    void SpriteDragCheck(int idx);

    void SyncScroll();

    AnimationEditor();
    ~AnimationEditor();
};

