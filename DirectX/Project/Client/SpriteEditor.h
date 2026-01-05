#pragma once
#include "EditorUI.h"
#include "Engine/ATexture.h"
#include "Engine/ASprite.h"

#include <mutex>


struct LocalSprite:
    public Entity
{
    wstring Spritename;
    ImVec2 SelectStart;
    ImVec2 SelectEnd;

    CLONE(LocalSprite);
};
class SpriteEditor :
    public EditorUI
{
private:
    Ptr<ATexture> CurAtlas;
    wstring Atlasname;
    float Size;
    ImVec2 ImageOffset;

    vector<Ptr<LocalSprite>> CurLocalSprites;
    vector<Ptr<ASprite>> CurSprites;
    int HighlightedRect;

    EditorUI* GridWind;
    EditorUI* SpriteWind;

    vector<unsigned char> Alphamap;
    vector<uint8_t> Board;

    std::atomic<bool> isSlicing = false;
    std::atomic<bool> sliceFinished = false;
public:
    ImVec2 GetImageOffset() { return ImageOffset; }
    float GetSize() { return Size; }
    Ptr<ATexture> GetCurAtlas() { return CurAtlas; }
    vector<Ptr<LocalSprite>> GetCurLocalSprites() {
        vector<Ptr<LocalSprite>> slicing;
        if (isSlicing) return slicing;

        return CurLocalSprites;
    }
    bool GetisSlicing() { return isSlicing; }
    void SetHighlightedRect(int i) { HighlightedRect = i; }
    void ResetHighlightedRect() { HighlightedRect = -1; }

    void ClearSprites() { 
        CurLocalSprites.clear(); 
    }
    void ClearBoard() {
        Board.clear();
        Board.resize(CurAtlas->GetHeight() * CurAtlas->GetWidth());
    }
    void DeleteSprite(int index) {
        auto iter = CurLocalSprites.begin();
        for (size_t i = 0; i < index; i++) ++iter;
        CurLocalSprites.erase(iter);
        RenameLocalSprites();
    }

    //local pos + imageoffset sprite
    void AddLocalSprite(Ptr<LocalSprite> sprite);

    void RenameLocalSprites();

    virtual void Tick() override;
    virtual void Tick_UI() override;

    void LoadAtlas();
    void MenuBar();
    void AtlasImage();
    bool DropCheck();
    void DebugRectInput();
    void DrawDebugRect();
    int GetAlpha(ImVec2 LocalPos);
    void RemapAlpha();
    void BFS(ImVec2 LocalPos);
    void Slice();

    wstring GetRelativepath(wchar_t Path[]);

    void CreateStagingTex();
    void Refresh();
    SpriteEditor();
    ~SpriteEditor();
};

