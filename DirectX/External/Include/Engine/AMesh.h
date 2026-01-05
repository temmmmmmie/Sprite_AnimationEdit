#pragma once
#include "Asset.h"


class AMesh :
    public Asset
{
private:
    ComPtr<ID3D11Buffer>    m_VB;
    D3D11_BUFFER_DESC       m_VBDesc;
    UINT                    m_VtxCount;
    Vtx*                    m_VtxSys;

    ComPtr<ID3D11Buffer>    m_IB;
    D3D11_BUFFER_DESC       m_IBDesc;
    UINT                    m_IdxCount;
    UINT*                   m_IdxSys;

public:
    int Create(Vtx* _VtxSysMem, UINT _VtxCount, UINT* _IdxSysMem, UINT _IdxCount);
    void Binding();
    void Render();
    void Render_Particle(int _Count);

public:
    Vtx* GetVtxSysMem() { return m_VtxSys; }
    UINT* GetIdxSysMem() { return m_IdxSys; }

    UINT GetVtxCount() { return m_VtxCount; }
    UINT GetIdxCount() { return m_IdxCount; }


public:
    virtual int Save(const wstring& _FilePath) override {return S_OK;};
    virtual int Load(const wstring& _FilePath) override {return S_OK;};

    CLONE_DISABLE(AMesh);
public:
    AMesh();
    virtual ~AMesh();
};

