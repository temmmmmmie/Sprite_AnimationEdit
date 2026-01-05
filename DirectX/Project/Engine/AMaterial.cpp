#include "pch.h"
#include "AMaterial.h"

#include "Device.h"
#include "ConstBuffer.h"


AMaterial::AMaterial()
	: Asset(ASSET_TYPE::MATERIAL)
	, m_Domain(RENDER_DOMAIN::NONE)
	, m_Const{}
{
}

AMaterial::~AMaterial()
{
}


void AMaterial::Binding()
{
	m_Shader->Binding();

	for (UINT i = 0; i < TEX_END; ++i)
	{
		if (nullptr == m_Tex[i])
		{
			ATexture::Clear(i);
			m_Const.bTex[i] = 0;
		}
		else
		{
			m_Tex[i]->Binding(i);
			m_Const.bTex[i] = 1;
		}
	}

	static Ptr<ConstBuffer> pCB = Device::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pCB->SetData(&m_Const);
	pCB->Binding();
}


int AMaterial::Save(const wstring& _FilePath)
{
	return 0;
}

int AMaterial::Load(const wstring& _FilePath)
{
	return 0;
}