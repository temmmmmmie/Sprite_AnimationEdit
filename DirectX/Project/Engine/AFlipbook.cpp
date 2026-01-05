#include "pch.h"
#include "AFlipbook.h"


AFlipbook::AFlipbook()
	: Asset(ASSET_TYPE::FLIPBOOK)
{
}

AFlipbook::~AFlipbook()
{
}

void AFlipbook::SetSprite(int _Idx, Ptr<ASprite> _Sprite)
{
	if (m_vecSprites.size() <= _Idx)
	{
		m_vecSprites.resize(_Idx + 1);
	}

	m_vecSprites[_Idx] = _Sprite;
}

int AFlipbook::Save(const wstring& _FilePath)
{
	return 0;
}

int AFlipbook::Load(const wstring& _FilePath)
{
	return 0;
}
