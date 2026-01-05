#include "pch.h"
#include "SpriteWindow.h"
#include "ListUI.h"
#include "TreeUI.h"
#include "EditorMgr.h"


SpriteWindow::SpriteWindow()
	: EditorUI("SpriteWindow")
{
}

SpriteWindow::~SpriteWindow()
{
}

void SpriteWindow::Tick_UI()
{
	bool disabled = false;
	if (spriteedit->GetisSlicing()) {
		ImGui::BeginDisabled();
		disabled = true;
	}

	ImGui::Text("");
	ImGui::SameLine(80);
	if (ImGui::Button("DeleteAll")) {
		spriteedit->ClearSprites();
		spriteedit->ClearBoard();
		spriteedit->ResetHighlightedRect();
	}
	vector<Ptr<LocalSprite>> plocalsprites;
	if(!spriteedit->GetisSlicing()) plocalsprites = spriteedit->GetCurLocalSprites();
	UINT Flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
	
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) spriteedit->ResetHighlightedRect();
	
	for (size_t i = 0; i < plocalsprites.size(); i++)
	{
		if (plocalsprites.size() == 0 || spriteedit->GetisSlicing()) break;
		bool tree = ImGui::TreeNodeEx(GetString(plocalsprites[i]->Spritename).c_str(), Flags);
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered()) {
			spriteedit->SetHighlightedRect(i);
		}

		if(tree){
			Vec2 Startcor = Vec2(plocalsprites[i]->SelectStart.x, plocalsprites[i]->SelectStart.y);
			Vec2 Endcor = Vec2(plocalsprites[i]->SelectEnd.x, plocalsprites[i]->SelectEnd.y);
			ImGui::Spacing();
			ImGui::Text("Start");
			ImGui::SameLine(70);
			if(ImGui::DragFloat2("##SelectStart", Startcor, 0.01f)) {
				plocalsprites[i]->SelectStart = Startcor;
			};
			
			ImGui::Text("End");
			ImGui::SameLine(70);
			if (ImGui::DragFloat2("##SelectEnd", Endcor, 0.01f)) {
				plocalsprites[i]->SelectEnd = Endcor;
				
			};
			if (ImGui::Button("Delete")) {
				spriteedit->DeleteSprite(i);
				spriteedit->ResetHighlightedRect();
			}
			ImGui::TreePop();

		}

	}

	if (spriteedit->GetisSlicing() && disabled) {
		ImGui::EndDisabled();
	}
}



