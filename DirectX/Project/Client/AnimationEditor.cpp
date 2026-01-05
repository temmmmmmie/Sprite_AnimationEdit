#include "pch.h"
#include "AnimationEditor.h"
#include "GameObjectEx.h"
#include "EditorMgr.h"
#include "Engine/ASprite.h"
#include "Engine/ATexture.h"
#include "TreeUI.h"
#include "FrameProperty.h"

AnimationEditor::AnimationEditor()
	: EditorUI("AnimationEditor")
{
	UseMenuBar(true);
	Ptr<GameObjectEx> pFlipbookobj = new GameObjectEx;
	pFlipbookobj->SetName(L"FlipbookObj");
	pFlipbookobj->AddComponent(new CTransform);
	pFlipbookobj->AddComponent(new CFlipbookRender);
	Editorflip = pFlipbookobj->FlipbookRender();
	Editorflip->IsEditorFlip = true;
	EditorMgr::GetInst()->AddEditorGameObj(pFlipbookobj);

	FrameProperty* frameprop = FIND_UI(FrameProperty, "FrameProperty");
	FramePropWind = frameprop;
	frameprop->animationedit = this;

	Ptr<AFlipbook> initialflip = new AFlipbook;
	Editorflip->AddFlipbook(initialflip);
	Editorflip->Play(0, 30);
	Editorflip->Stop();
}

AnimationEditor::~AnimationEditor()
{
}


void AnimationEditor::Tick()
{
	EditorUI::Tick();
}

void AnimationEditor::Tick_UI()
{
	Menubar();

	AnimationPreview();


}

void AnimationEditor::Menubar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("FrameProperty", nullptr, FramePropWind->IsActive()))
			{
				if (FramePropWind->IsActive()) {
					FramePropWind->SetActive(false);
				}
				else {
					FramePropWind->SetActive(true);
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void AnimationEditor::AnimationPreview()
{
	float windowWidth = ImGui::GetWindowSize().x;
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	Vec2 SpriteResol = Vec2(300, 300);
	ImGui::SetCursorPosX((windowWidth - SpriteResol.x) * 0.5f);
	Ptr<ASprite> cursprite = Editorflip->GetCurSprite();
	Ptr<ATexture> atlas;
	if(nullptr != cursprite) atlas = cursprite->GetAtlas();
	Ptr<ASprite> prevsprite = Editorflip->GetPrevSprite();

	if (nullptr != atlas) {
		
		ImVec2 pos = ImGui::GetCursorPos();
		if (nullptr != prevsprite && nullptr != prevsprite->GetAtlas())ImGui::Image((ImTextureID)prevsprite->GetAtlas()->GetSRV().Get(), SpriteResol, prevsprite->GetLeftTop() + prevsprite->GetOffset(), prevsprite->GetLeftTop() + prevsprite->GetSlice() + prevsprite->GetOffset(), ImVec4(0, 0, 0.5f, 0.5f), border_col);
		ImGui::SetCursorPos(pos);
		ImGui::Image((ImTextureID)atlas->GetSRV().Get(), SpriteResol, cursprite->GetLeftTop() + cursprite->GetOffset(), cursprite->GetLeftTop() + cursprite->GetSlice() + cursprite->GetOffset(), tint_col, border_col);
	}
	else
	{
		Ptr< ATexture> Exampletex = FIND(ATexture, L"SpriteExample");
		ImGui::Image((ImTextureID)Exampletex->GetSRV().Get(), SpriteResol, Vec2(0, 0), Vec2(1, 1), tint_col, border_col);
	}
	FlipbookDropCheck();

	AniCtrlBtn();
}

void AnimationEditor::AniCtrlBtn()
{
	float windowWidth = ImGui::GetWindowSize().x;
	bool disabled = false;
	if (0 == Editorflip->GetFlipbook()->GetSpriteCount()) {
		ImGui::BeginDisabled();
		disabled = true;
	}
		ImGui::SetCursorPosX((windowWidth - 220) * 0.5f);
		if (ImGui::Button("<", Vec2(70, 20))) {
			Editorflip->PrevFrame();
			SyncScroll();
		};
		ImGui::SameLine();
		static bool isplaying;
		if (isplaying) {
			if (ImGui::Button("Stop", Vec2(70, 20))) {
				Editorflip->Stop();
				isplaying = false;
			};
		}
		else {
			if (ImGui::Button("Play", Vec2(70, 20))) {
				Editorflip->Resume();
				isplaying = true;
			};
		}
		ImGui::SameLine();
		if (ImGui::Button(">", Vec2(70, 20))) {
			Editorflip->NextFrame();
			SyncScroll();
		};
	if (disabled) ImGui::EndDisabled();

	FramePreview(isplaying);
}

void AnimationEditor::FramePreview(bool _isplaying)
{
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	Vec2 SpriteResol = Vec2(100, 100);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImVec2 scrolling_child_size = ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7);
	ImGui::BeginChild("scrolling", scrolling_child_size, 0, ImGuiWindowFlags_HorizontalScrollbar);

	if (nullptr != Editorflip->GetCurSprite()) {
		Ptr<AFlipbook> flip = Editorflip->GetFlipbook();

		for (size_t i = 0; i < flip->GetSpriteCount(); i++)
		{
			Ptr<ASprite> sprite = flip->GetSprite(i);
			Ptr<ATexture> atlas = sprite->GetAtlas();
			if (nullptr == atlas) atlas = FIND(ATexture, L"SpriteExample");

			if (i == Editorflip->GetFrameIdx()) {
				border_col = ImGui::GetStyleColorVec4(ImGuiCol_FrameBgActive);
				SelectedIdx = i;
			}
			else border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);

			ImGui::Image((ImTextureID)atlas->GetSRV().Get(), SpriteResol, sprite->GetLeftTop(), sprite->GetLeftTop() + sprite->GetSlice(), tint_col, border_col);
			SpriteDragCheck(i);
			SpriteDropCheck(i);
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered()) {
				Editorflip->SetFrame(i);
			}
			
			ImGui::SetCursorPosX((SpriteResol.x * (i + 1)) - SpriteResol.x / 2);
			ImGui::Text("%d", i);
			ImGui::SetCursorPosX(SpriteResol.x * (i + 1));
			ImGui::SetCursorPosY(0);
		}

	}

	Ptr< ATexture> Add = FIND(ATexture, L"AddTex");
	ImGui::Image((ImTextureID)Add->GetSRV().Get(), SpriteResol, Vec2(0, 0), Vec2(1, 1), tint_col, ImGui::GetStyleColorVec4(ImGuiCol_Border));
	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered()) {
		Editorflip->AddFrame();
	}
	ImGui::EndChild();

	if (_isplaying) {
		SyncScroll();
	}
}

bool AnimationEditor::FlipbookDropCheck()
{
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentUI");

		if (payload)
		{
			TreeNode* pNode = *((TreeNode**)payload->Data);

			if (0 != pNode->GetData())
			{
				Ptr<Asset> pAsset = (Asset*)pNode->GetData();
				if (ASSET_TYPE::FLIPBOOK == pAsset->GetType())
				{
					Editorflip->SetFlipbook(0, (AFlipbook*)pAsset.Get());
					Editorflip->SetFrame(0);
					Editorflip->Play(0, 30);
					Editorflip->Stop();
					SyncScroll();
					ImGui::EndDragDropTarget();
					return true;
				}
			}
		}

		ImGui::EndDragDropTarget();
	}
	return false;
}

bool AnimationEditor::SpriteDropCheck(int idx)
{
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentUI");

		if (payload)
		{
			TreeNode* pNode = *((TreeNode**)payload->Data);

			if (0 != pNode->GetData())
			{
				Ptr<Asset> pAsset = (Asset*)pNode->GetData();
				if (ASSET_TYPE::SPRITE == pAsset->GetType())
				{
					Editorflip->GetFlipbook()->SetSprite(idx, (ASprite*)pAsset.Get());
					ImGui::EndDragDropTarget();
					return true;
				}
			}
		}

		ImGui::EndDragDropTarget();
	}
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASprite");

		if (payload)
		{
			ASprite* pSprite = *((ASprite**)payload->Data);

			if (nullptr != pSprite)
			{
				if (ASSET_TYPE::SPRITE == pSprite->GetType())
				{
					Editorflip->GetFlipbook()->SetSprite(idx, pSprite);
					ImGui::EndDragDropTarget();
					return true;
				}
			}
		}

		ImGui::EndDragDropTarget();
	}
	return false;
}

void AnimationEditor::SpriteDragCheck(int idx)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		Ptr<ASprite> target = Editorflip->GetFlipbook()->GetSprite(idx);
		ImGui::Text(GetString( target->GetKey()).c_str());

		ImGui::SetDragDropPayload("ASprite", &target, sizeof(ASprite*));

		ImGui::EndDragDropSource();
	}
}

void AnimationEditor::SyncScroll()
{
	ImGui::BeginChild("scrolling");
	if (nullptr != Editorflip->GetCurSprite()) {
		float x = ImGui::GetScrollMaxX();
		float ScrollPiece = x / Editorflip->GetFlipbook()->GetSpriteCount();
		ImGui::SetScrollX(ScrollPiece * Editorflip->GetFrameIdx() + 80);
	}
	ImGui::EndChild();
}
