#pragma once
#include "Asset.h"

class AGraphicShader :
    public Asset
{
private:
    ComPtr<ID3DBlob>			    m_VSBlob;
    ComPtr<ID3DBlob>			    m_GSBlob;
    ComPtr<ID3DBlob>		        m_PSBlob;
    ComPtr<ID3DBlob>		        m_ErrBlob;

    ComPtr<ID3D11VertexShader>	    m_VS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>	    m_PS;

    ComPtr<ID3D11InputLayout>	    m_Layout;
    D3D11_PRIMITIVE_TOPOLOGY        m_Topology;

    RS_TYPE                     m_RSType;
    DS_TYPE                     m_DSType;
    BS_TYPE                     m_BSType;

public:
    int CreateVertexShader(const wstring& _RelativePath, const string& _FuncName);
    int CreateGeometryShader(const wstring& _RelativePath, const string& _FuncName);
    int CreatePixelShader(const wstring& _RelativePath, const string& _FuncName);
    void SetRSType(RS_TYPE _Type) { m_RSType = _Type; }
    void SetDSType(DS_TYPE _Type) { m_DSType = _Type; }
    void SetBSType(BS_TYPE _Type) { m_BSType = _Type; }

    void Binding();

    GET_SET(D3D11_PRIMITIVE_TOPOLOGY, Topology);

public:
    virtual int Save(const wstring& _FilePath) override{return S_OK;};
    virtual int Load(const wstring& _FilePath) override{return S_OK;};


    CLONE_DISABLE(AGraphicShader);
public:
    AGraphicShader();
    virtual ~AGraphicShader();
};

