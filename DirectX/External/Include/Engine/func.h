#pragma once

void SpawnGameObject(int LayerIdx, class GameObject* _Object);
void DestroyGameObject(class GameObject* _Object);
void ChangeLevelState(LEVEL_STATE _NextState);
void ChangeLevel(class ALevel* pLevel, LEVEL_STATE _NextState);



const char* GetString(ASSET_TYPE _Type);
const wchar_t* GetWString(ASSET_TYPE _Type);
const char* GetString(COMPONENT_TYPE _Type);
const wchar_t* GetWString(COMPONENT_TYPE _Type);


void DrawDebugRect(Vec3 _WorldPos, Vec3 _WorldScale, Vec3 _WolrdRot, Vec4 _Color, bool _DepthTest, float _Duration = 0.f);
void DrawDebugRect(const Matrix& _matWorld, Vec4 _Color, bool _DepthTest, float _Duration = 0.f);
void DrawDebugCircle(Vec3 _WorldPos, float _Radius, Vec4 _Color, bool _DepthTest, float _Duration = 0.f);


