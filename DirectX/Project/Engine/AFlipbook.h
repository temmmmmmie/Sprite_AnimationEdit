#pragma once
#include "Asset.h"

#include "ASprite.h"

class AFlipbook :
    public Asset
{
private:
    vector<Ptr<ASprite>>    m_vecSprites;

public:
    void AddSprite(Ptr<ASprite> _Sprite) { 
        m_vecSprites.push_back(_Sprite); 
    }
    void SetSprite(int _Idx, Ptr<ASprite> _Sprite);
    void DeleteSprite(int _Idx) {
        
        m_vecSprites.erase(m_vecSprites.begin()+_Idx);
    }
    UINT GetSpriteCount() { return (UINT)m_vecSprites.size(); }
    Ptr<ASprite> GetSprite(int _Idx) { 
        if (_Idx >= m_vecSprites.size()) return nullptr;
        return m_vecSprites[_Idx]; 
    }

public:
    virtual int Save(const wstring& _FilePath);

private:
    virtual int Load(const wstring& _FilePath);


    CLONE_DISABLE(AFlipbook);
public:
    AFlipbook();
    virtual ~AFlipbook();
};

