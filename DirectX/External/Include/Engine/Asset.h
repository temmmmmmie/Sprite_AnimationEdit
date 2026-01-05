#pragma once
#include "Entity.h"


class Asset :
    public Entity
{
private:
    const ASSET_TYPE    m_Type;
    wstring             m_Key;
    wstring             m_RelativePath;

private:
    SET(wstring, Key)
    SET(wstring, RelativePath)

public:
    GET_REF(wstring, Key);
    GET_REF(wstring, RelativePath);
    GET(ASSET_TYPE, Type);

public:
    virtual int Save(const wstring& _FilePath) = 0;

private:
    virtual int Load(const wstring& _FilePath) = 0;

public:
    virtual Asset* Clone() = 0;
public:
    Asset(ASSET_TYPE _Type);
    Asset(const Asset& _Origin);
    virtual ~Asset();

    friend class AssetMgr;
};

