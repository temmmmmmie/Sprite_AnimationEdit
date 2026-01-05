#include "pch.h"
#include "MainMenu.h"

#include <Engine/LevelMgr.h>
#include <Engine/Level.h>
#include <Engine/Layer.h>
#include <Engine/GameObject.h>
#include <Engine/assets.h>

#include "EditorMgr.h"
#include "Inspector.h"
#include "SpriteEditor.h"
#include "AnimationEditor.h"

MainMenu::MainMenu()
	: EditorUI("MainMenu")
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::Tick()
{
	if (ImGui::BeginMainMenuBar())
	{
		File();

		Level();

		GameObject();

		Asset();

		Editor();

		ImGui::EndMainMenuBar();
	}
}

void MainMenu::Tick_UI()
{
}

void MainMenu::File()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Level Save"))
		{

		}

		if (ImGui::MenuItem("Level Load"))
		{

		}

		ImGui::EndMenu();
	}
}

void MainMenu::Level()
{
	if (ImGui::BeginMenu("Level"))
	{
		Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();

		bool IsPlay = pCurLevel.Get() && LEVEL_STATE::PLAY == pCurLevel->GetState();
		bool IsPause = pCurLevel.Get() && LEVEL_STATE::PAUSE == pCurLevel->GetState();
		bool IsStop = pCurLevel.Get() && LEVEL_STATE::STOP == pCurLevel->GetState();

		if (ImGui::MenuItem("Play", nullptr, nullptr, !IsPlay))
		{
			// 현재 레벨을 복사해둔다.
			if (IsStop)
			{
				m_CopyLevel = pCurLevel->Clone();
			}			

			ChangeLevelState(LEVEL_STATE::PLAY);
		}

		if (ImGui::MenuItem("Pause", nullptr, nullptr, IsPlay))
		{
			ChangeLevelState(LEVEL_STATE::PAUSE);
		}

		if (ImGui::MenuItem("Stop", nullptr, nullptr, !IsStop))
		{
			ChangeLevel(m_CopyLevel.Get(), LEVEL_STATE::STOP);
			FIND_UI(Inspector, "Inspector")->SetTargetObject(nullptr);
		}

		ImGui::EndMenu();
	}
}

void MainMenu::GameObject()
{
	if (ImGui::BeginMenu("GameObject"))
	{
		ImGui::EndMenu();
	}
}

void MainMenu::Asset()
{
	if (ImGui::BeginMenu("Asset"))
	{
		ImGui::EndMenu();
	}
}

void MainMenu::Editor()
{
	if (ImGui::BeginMenu("Editor"))
	{
		EditorUI* SpriteEditorWind = FIND_UI(SpriteEditor, "SpriteEditor");
		if (ImGui::MenuItem("SpriteEditor", nullptr, SpriteEditorWind->IsActive()))
		{
			if (SpriteEditorWind->IsActive()) {
				SpriteEditorWind->SetActive(false);
			}
			else {
				SpriteEditorWind->SetActive(true);
			}
		}

		EditorUI* AnimationEditorWind = FIND_UI(AnimationEditor, "AnimationEditor");
		if (ImGui::MenuItem("AnimationEditor", nullptr, AnimationEditorWind->IsActive()))
		{
			if (AnimationEditorWind->IsActive()) {
				AnimationEditorWind->SetActive(false);
			}
			else {
				AnimationEditorWind->SetActive(true);
			}
		}
		ImGui::EndMenu();
	}
}
