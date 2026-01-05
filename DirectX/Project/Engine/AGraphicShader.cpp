#include "pch.h"
#include "AGraphicShader.h"

#include "Device.h"
#include "PathMgr.h"


AGraphicShader::AGraphicShader()
	: Asset(ASSET_TYPE::GRAPHICSHADER)
	, m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_RSType(RS_TYPE::CULL_BACK)
	, m_DSType(DS_TYPE::LESS)
	, m_BSType(BS_TYPE::DEFAULT)
{
}

AGraphicShader::~AGraphicShader()
{
}

int AGraphicShader::CreateVertexShader(const wstring& _RelativePath, const string& _FuncName)
{
	// 버텍스 쉐이더
	wstring HLSLPath = PathMgr::GetInst()->GetContentPath();
	HLSLPath += _RelativePath;

	int Flag = 0;
#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	// 버텍스 쉐이더 컴파일
	if (FAILED(D3DCompileFromFile(HLSLPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName.c_str(), "vs_5_0", Flag
		, 0, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr == m_ErrBlob)
			MessageBox(nullptr, L"쉐이더 파일을 찾을 수 없음", L"쉐이더 컴파일 실패", MB_OK);
		else
			MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer(), "쉐이더 컴파일 실패", MB_OK);

		return E_FAIL;
	}

	// 버텍스 쉐이더 객체 생성
	DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), nullptr, m_VS.GetAddressOf());

	// InputLayout 만들기
	D3D11_INPUT_ELEMENT_DESC arrElement[3] = {};

	arrElement[0].SemanticName = "POSITION";
	arrElement[0].SemanticIndex = 0;
	arrElement[0].AlignedByteOffset = 0;
	arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // 12
	arrElement[0].InputSlot = 0;
	arrElement[0].InstanceDataStepRate = 0;
	arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	arrElement[1].SemanticName = "COLOR";
	arrElement[1].SemanticIndex = 0;
	arrElement[1].AlignedByteOffset = 12;
	arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 16
	arrElement[1].InputSlot = 0;
	arrElement[1].InstanceDataStepRate = 0;
	arrElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	arrElement[2].SemanticName = "TEXCOORD";
	arrElement[2].SemanticIndex = 0;
	arrElement[2].AlignedByteOffset = 28;
	arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT; // 8
	arrElement[2].InputSlot = 0;
	arrElement[2].InstanceDataStepRate = 0;
	arrElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	DEVICE->CreateInputLayout(arrElement, 3, m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), m_Layout.GetAddressOf());

	return S_OK;
}

int AGraphicShader::CreateGeometryShader(const wstring& _RelativePath, const string& _FuncName)
{
	wstring HLSLPath = PathMgr::GetInst()->GetContentPath();
	HLSLPath += _RelativePath;

	int Flag = 0;
#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	// 픽셀 쉐이더 컴파일
	if (FAILED(D3DCompileFromFile(HLSLPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName.c_str(), "gs_5_0", Flag
		, 0, m_GSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr == m_ErrBlob)
			MessageBox(nullptr, L"쉐이더 파일을 찾을 수 없음", L"쉐이더 컴파일 실패", MB_OK);
		else
			MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer(), "쉐이더 컴파일 실패", MB_OK);
		return E_FAIL;
	}

	// 쉐이더 객체 생성
	DEVICE->CreateGeometryShader(m_GSBlob->GetBufferPointer(), m_GSBlob->GetBufferSize(), nullptr, m_GS.GetAddressOf());

	return S_OK;
}

int AGraphicShader::CreatePixelShader(const wstring& _RelativePath, const string& _FuncName)
{
	wstring HLSLPath = PathMgr::GetInst()->GetContentPath();
	HLSLPath += _RelativePath;

	int Flag = 0;
#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif

	// 픽셀 쉐이더 컴파일
	if (FAILED(D3DCompileFromFile(HLSLPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
								 , _FuncName.c_str(), "ps_5_0", Flag
								 , 0, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (nullptr == m_ErrBlob)
			MessageBox(nullptr, L"쉐이더 파일을 찾을 수 없음", L"쉐이더 컴파일 실패", MB_OK);
		else
			MessageBoxA(nullptr, (const char*)m_ErrBlob->GetBufferPointer(), "쉐이더 컴파일 실패", MB_OK);
		return E_FAIL;
	}

	// 픽셀 쉐이더 객체 생성
	DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf());


	return S_OK;
}

void AGraphicShader::Binding()
{
	// TriangleList : 정점 3개로 구성된 삼각형 면(Face, Plane)
	// LineStrip : 정점을 잇는 라인에 걸리는 픽셀
	CONTEXT->IASetPrimitiveTopology(m_Topology);

	// Layout
	// 하나의 정점이 어떻게 구성되어있는지에 대한 정보
	CONTEXT->IASetInputLayout(m_Layout.Get());

	// VertexShader Stage
	CONTEXT->VSSetShader(m_VS.Get(), 0, 0);
	
	// GeometryShader Stage
	CONTEXT->GSSetShader(m_GS.Get(), 0, 0);

	// RaterizerState
	CONTEXT->RSSetState(Device::GetInst()->GetRSState(m_RSType).Get());

	// PixelShader Stage
	CONTEXT->PSSetShader(m_PS.Get(), 0, 0);

	// DepthStencilState
	CONTEXT->OMSetDepthStencilState(Device::GetInst()->GetDSState(m_DSType).Get(), 0);

	// BlendState
	CONTEXT->OMSetBlendState(Device::GetInst()->GetBSState(m_BSType).Get(), nullptr, 0xffffffff);
}
