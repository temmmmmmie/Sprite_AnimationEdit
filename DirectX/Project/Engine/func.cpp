#include "pch.h"
#include "func.h"

#include "RenderMgr.h"
#include "TaskMgr.h"

void SpawnGameObject(int LayerIdx, GameObject* _Object)
{
	tTaskInfo info = {};
	info.Type = TASK_TYPE::CREATE_OBJECT;
	info.Param0 = LayerIdx;
	info.Param1 = (DWORD_PTR)_Object;
	TaskMgr::GetInst()->AddTask(info);
}

void DestroyGameObject(GameObject* _Object)
{
	tTaskInfo info = {};
	info.Type = TASK_TYPE::DELETE_OBJECT;
	info.Param0 = (DWORD_PTR)_Object;
	TaskMgr::GetInst()->AddTask(info);
}

#include "LevelMgr.h"
void ChangeLevelState(LEVEL_STATE _NextState)
{
	tTaskInfo info = {};

	info.Type = TASK_TYPE::CHANGE_LEVEL_STATE;
	info.Param0 = (DWORD_PTR)_NextState;

	TaskMgr::GetInst()->AddTask(info);
}

void ChangeLevel(ALevel* pLevel, LEVEL_STATE _NextState)
{
	tTaskInfo info = {};

	info.Type = TASK_TYPE::CHANGE_LEVEL;
	info.Param0 = (DWORD_PTR)pLevel;
	info.Param1 = (DWORD_PTR)_NextState;

	TaskMgr::GetInst()->AddTask(info);
}

const char* GetString(ASSET_TYPE _Type)
{
	return ASSET_TYPE_STR[(UINT)_Type];
}

const wchar_t* GetWString(ASSET_TYPE _Type)
{
	return ASSET_TYPE_WSTR[(UINT)_Type];
}

const char* GetString(COMPONENT_TYPE _Type)
{
	return COMPONENT_TYPE_STR[(UINT)_Type];
}

const wchar_t* GetWString(COMPONENT_TYPE _Type)
{
	return COMPONENT_TYPE_WSTR[(UINT)_Type];
}

void DrawDebugRect(Vec3 _WorldPos, Vec3 _WorldScale, Vec3 _WolrdRot, Vec4 _Color, bool _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};

	info.ShapeType = DEBUG_SHAPE::RECT;
	info.Color = _Color;
	info.DepthTest = _DepthTest;
	info.WorldPos = _WorldPos;
	info.Scale = _WorldScale;
	info.Rotation = _WolrdRot;
	info.Age = 0.f;
	info.Life = _Duration;
	info.matWorld = XMMatrixIdentity();

	RenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugRect(const Matrix& _matWorld, Vec4 _Color, bool _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};

	info.ShapeType = DEBUG_SHAPE::RECT;
	info.Color = _Color;
	info.DepthTest = _DepthTest;
	info.Age = 0.f;
	info.Life = _Duration;
	info.matWorld = _matWorld;

	RenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCircle(Vec3 _WorldPos, float _Radius, Vec4 _Color, bool _DepthTest, float _Duration)
{
	tDebugShapeInfo info = {};

	info.ShapeType = DEBUG_SHAPE::CIRCLE;
	info.Color = _Color;
	info.DepthTest = _DepthTest;
	info.Age = 0.f;
	info.Life = _Duration;
	info.WorldPos = _WorldPos;
	info.Scale = Vec3(_Radius * 2.f, _Radius * 2.f, 1.f);
	info.Rotation = Vec3(0.f, 0.f, 0.f);
	info.matWorld = XMMatrixIdentity();

	RenderMgr::GetInst()->AddDebugShapeInfo(info);
}

