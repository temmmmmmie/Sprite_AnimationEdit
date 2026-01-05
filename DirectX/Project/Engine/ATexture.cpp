#include "pch.h"
#include "ATexture.h"

#include "Device.h"



ATexture::ATexture()
	: Asset(ASSET_TYPE::TEXTURE)
	, m_Desc{}
	, m_RecentSRVNum(-1)
	, m_RecentUAVNum(-1)
{
}

ATexture::~ATexture()
{
}

int ATexture::Load(const wstring& _FilePath)
{
	path Ext = path(_FilePath).extension();

	HRESULT hr = S_OK;

	// DirectxTex 라이브러리 함수
	// .dds
	if (Ext == L".dds" || Ext == L".DDS")
		hr = LoadFromDDSFile(_FilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	// .tga
	else if (Ext == L".tga" || Ext == L".TGA")
		hr = LoadFromTGAFile(_FilePath.c_str(), nullptr, m_Image);
	// WIC(Window Image Component) - bmp, jpg, jpeg, png
	else if (Ext == L".png" || Ext == L".PNG"
		|| Ext == L".jpg" || Ext == L".JPG"
		|| Ext == L".jpeg" || Ext == L".JPEG"
		|| Ext == L".bmp" || Ext == L".BMP")
		hr = LoadFromWICFile(_FilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"시스템 메모리 로딩 실패", L"텍스쳐 로딩 실패", MB_OK);
		return E_FAIL;
	}

	// 텍스쳐 2D 생성
	m_Desc.Width = m_Image.GetMetadata().width;
	m_Desc.Height = m_Image.GetMetadata().height;
	m_Desc.Format = m_Image.GetMetadata().format;
	m_Desc.ArraySize = m_Image.GetMetadata().arraySize;
	m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_Desc.Usage = D3D11_USAGE_DEFAULT;
	m_Desc.CPUAccessFlags = 0;
	m_Desc.MiscFlags = 0;
	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;
	m_Desc.MipLevels = 1;    // 원본 텍스쳐의 저화질 버전 이미지 생성, 1 == 원본만

	// DepthStencil 용 Texture 생성
	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = m_Image.GetPixels();
	tSub.SysMemPitch = m_Image.GetImages()->rowPitch;
	tSub.SysMemSlicePitch = m_Image.GetImages()->slicePitch;

	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, &tSub, m_Tex2D.GetAddressOf())))
	{
		return E_FAIL;
	}

	// ShaderResourceView 생성
	DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf());

	return S_OK;
}

int ATexture::Create(ComPtr<ID3D11Texture2D> _Tex2D)
{
	m_Tex2D = _Tex2D;

	m_Tex2D->GetDesc(&m_Desc);

	if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf());
	}

	return S_OK;
}

int ATexture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _Usage)
{
	m_Desc.Width = _Width;
	m_Desc.Height = _Height;
	m_Desc.Format = _Format;
	m_Desc.ArraySize = 1;
	m_Desc.BindFlags = _BindFlag;

	m_Desc.Usage = _Usage;
	if (m_Desc.Usage == D3D11_USAGE_DYNAMIC)
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else
		m_Desc.CPUAccessFlags = 0;

	m_Desc.MiscFlags = 0;
	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;
	m_Desc.MipLevels = 1;    // 원본 텍스쳐의 저화질 버전 이미지 생성, 1 == 원본만

	// Texture2D 생성
	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
		return E_FAIL;

	// Texture 의 용도에 맞는 View 를 생성한다.
	if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
		DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
	if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf());
	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf());

	return S_OK;
}


void ATexture::Binding(UINT _RegisterNum)
{
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void ATexture::Binding_CS_SRV(UINT _RegisterNum)
{
	m_RecentSRVNum = _RegisterNum;
	CONTEXT->CSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void ATexture::Binding_CS_UAV(UINT _RegisterNum)
{
	m_RecentUAVNum = _RegisterNum;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, m_UAV.GetAddressOf(), &i);
}

void ATexture::Clear_CS_SRV()
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->CSSetShaderResources(m_RecentSRVNum, 1, &pSRV);
	m_RecentSRVNum = -1;
}

void ATexture::Clear_CS_UAV()
{
	ID3D11UnorderedAccessView* pUAV = nullptr;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(m_RecentUAVNum, 1, &pUAV, &i);
	m_RecentUAVNum = -1;
}

void ATexture::Clear(UINT _RegisterNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, &pSRV);
}


int ATexture::Save(const wstring& _FilePath)
{
	return S_OK;
}
