#pragma once

#include "Level.h"

class LevelMgr
	: public singleton<LevelMgr>
{
	SINGLE(LevelMgr);
private:
	Ptr<ALevel>		m_CurLevel;

public:
	Ptr<ALevel> GetCurrentLevel() { return m_CurLevel; }
	Ptr<GameObject> FindObjectByName(const wstring& _Name);

private:
	void ChangeLevel(Ptr<ALevel> _NextLevel) { m_CurLevel = _NextLevel; }

public:
	void Init();
	void Progress();

	friend class TaskMgr;
};

