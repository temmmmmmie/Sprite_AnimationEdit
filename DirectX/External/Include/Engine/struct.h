#pragma once

struct Vertex
{
	Vec3	vPos;
	Vec4	vColor;
	Vec2	vUV;
};
typedef Vertex Vtx;


struct tTaskInfo
{
	TASK_TYPE Type;
	DWORD_PTR Param0;
	DWORD_PTR Param1;
	DWORD_PTR Param2;
};

struct tDebugShapeInfo
{
	DEBUG_SHAPE		ShapeType;
	Vec4			Color;
	Vec3			WorldPos;
	Vec3			Scale;
	Vec3			Rotation;
	Matrix			matWorld;
	bool			DepthTest;
	float			Age;
	float			Life; 
};


// ===============
// 광원 관련 구조체
// ===============
struct tLight2DInfo
{
	LIGHT_TYPE	Type;		// 광원 타입
	Vec3		LightColor;	// 빛의 색상
	Vec3		Ambient;	// 빛으로 인해서 발생하는 최소한의 빛(환경광)
	Vec3		WorldPos;	// 광원의 위치
	Vec3		Direction;	// 광원의 빛이 향하는 방향
	float		Radius;		// 광원의 영향 범위
	float		Angle;		// 광원을 발사하는 각도범위
};



// =================
// 파티클 관련 구조체
// =================
struct tParticle
{
	Vec3	WorldPos;
	Vec3	WorldScale;
	Vec4	Color;

	Vec3	Velocity;

	float	Mass;
	float	Age;			// 파티클 나이(생성 이후 시간)
	float	Life;			// 파티클 수명(최대 유지시간)
	float	NormalizedAge;  // 파티클의 나이를 수명 대비 정규화한 값(Age/Life)

	int		Active;			// 활성화 상태
};







// ========================
// 상수버퍼에 대응하는 구조체
// ========================
struct tTransform
{
	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;

	Matrix  matWV;
	Matrix  matWVP;
};
extern tTransform g_Trans; // 전방선언


struct tMtrlConst
{
	int		iArr[4];
	float	fArr[4];
	Vec2	v2Arr[4];
	Vec4	v4Arr[4];
	Matrix  matArr[2];

	int		bTex[TEX_END];
	int		padding[2];
};


struct tGlobalData
{
	Vec2  RenderResolution;
	UINT  Light2DCount;
	UINT  Light3DCount;
	float DeltaTime;
	float Time;
	float EngineDT;
	float EngineTime;
};
extern tGlobalData g_Global;


struct Pixel
{
	BYTE b, g, r, a;
};