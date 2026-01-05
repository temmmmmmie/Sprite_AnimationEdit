#pragma once
#include "Asset.h"

#include "AGraphicShader.h"
#include "ATexture.h"

// 물체의 재질을 설정한다.
// 렌더링 방식(파이프라인, 쉐이더) 와, 쉐이더에 전달할 리소스 데이터(텍스쳐, 상수 등) 를 묶은 개념
class AMaterial :
    public Asset
{
private:
    Ptr<AGraphicShader>     m_Shader;
    tMtrlConst              m_Const;
    Ptr<ATexture>           m_Tex[TEX_END];
	RENDER_DOMAIN			m_Domain; // 해당 재질의 렌더링 시점

public:
    GET_SET(Ptr<AGraphicShader>, Shader);

    template<typename T>
    void SetScalar(SCALAR_PARAM _Type, const T& _Data);

	void SetDomain(RENDER_DOMAIN _Domain) { m_Domain = _Domain; }
	RENDER_DOMAIN GetDomain() { return m_Domain; }

public:
    void SetTexture(TEX_PARAM _Param, Ptr<ATexture> _Tex) { m_Tex[_Param] = _Tex; }
    void Binding();

public:
    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;


	CLONE(AMaterial);
public:
    AMaterial();
    virtual ~AMaterial();

};

// 함수템플릿 안에서 T 타입 구별하기, T 타입에 따른 예외처리
template<typename T>
void AMaterial::SetScalar(SCALAR_PARAM _Type, const T& _Data)
{
	switch (_Type)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		if constexpr (std::is_same_v<T, int> || std::is_same_v<T, UINT>)
		{
			m_Const.iArr[_Type - INT_0] = _Data;
		}
		break;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		if constexpr (std::is_same_v<T, float>)
		{
			m_Const.fArr[_Type - FLOAT_0] = _Data;
		}
		break;
	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		if constexpr (std::is_same_v<T, Vec2>)
		{
			m_Const.v2Arr[_Type - VEC2_0] = _Data;
		}
		break;
	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		if constexpr (std::is_same_v<T, Vec4> || std::is_same_v<T, Vec3>)
		{
			m_Const.v4Arr[_Type - VEC4_0] = _Data;
		}
		break;
	case MAT_0:
	case MAT_1:
		if constexpr (std::is_same_v<T, Matrix>)
		{
			m_Const.matArr[_Type - MAT_0] = _Data;
		}
		break;
	}
}