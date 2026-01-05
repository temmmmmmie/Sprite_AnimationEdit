#include "pch.h"
#include "AMesh.h"

#include "Device.h"

AMesh::AMesh()
	: Asset(ASSET_TYPE::MESH)
	, m_VBDesc{}
	, m_VtxCount(0)
	, m_VtxSys(nullptr)
	, m_IBDesc{}
	, m_IdxCount(0)
	, m_IdxSys(nullptr)
{
}

AMesh::~AMesh()
{
	if (nullptr != m_VtxSys)
		delete[] m_VtxSys;
	if (nullptr != m_IdxSys)
		delete[] m_IdxSys;
}

int AMesh::Create(Vtx* _VtxSysMem, UINT _VtxCount, UINT* _IdxSysMem, UINT _IdxCount)
{
	m_VtxCount = _VtxCount;

	m_VBDesc.ByteWidth = sizeof(Vtx) * _VtxCount;
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA Sub = {};
	Sub.pSysMem = _VtxSysMem;
	if (FAILED(DEVICE->CreateBuffer(&m_VBDesc, &Sub, m_VB.GetAddressOf())))
	{
		MessageBox(nullptr, L"버텍스 버퍼 생성 실패", L"메쉬 생성 실패", MB_OK);
		return E_FAIL;
	}

	m_VtxSys = new Vtx[_VtxCount];
	for (UINT i = 0; i < m_VtxCount; ++i)
	{
		m_VtxSys[i] = _VtxSysMem[i];
	}

	m_IdxCount = _IdxCount;
	m_IBDesc.ByteWidth = sizeof(UINT) * m_IdxCount;
	m_IBDesc.Usage = D3D11_USAGE_DEFAULT;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	Sub = {};
	Sub.pSysMem = _IdxSysMem;
	if (FAILED(DEVICE->CreateBuffer(&m_IBDesc, &Sub, m_IB.GetAddressOf())))
	{
		MessageBox(nullptr, L"인덱스 버퍼 생성 실패", L"메쉬 생성 실패", MB_OK);
		return E_FAIL;
	}

	m_IdxSys = new UINT[m_IdxCount];
	for (UINT i = 0; i < m_IdxCount; ++i)
	{
		m_IdxSys[i] = _IdxSysMem[i];
	}


	return S_OK;
}

void AMesh::Binding()
{
	UINT Stride = sizeof(Vtx);
	UINT Offset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &Stride, &Offset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void AMesh::Render()
{
	Binding();

	CONTEXT->DrawIndexed(m_IdxCount, 0, 0); // 렌더링 시작
}

void AMesh::Render_Particle(int _Count)
{
	Binding();

	CONTEXT->DrawIndexedInstanced(m_IdxCount, _Count, 0, 0, 0); // 렌더링 시작
}

