#include "pch.h"
#include "Grid.h"
#include "EditorMgr.h"
#include "Engine/ATexture.h"

Grid::Grid()
	:EditorUI("Grid"),
	Col(0),
	Row(0)
{
}

Grid::~Grid()
{
}

void Grid::Tick_UI()
{
	bool disabled = false;
	if (spriteedit->GetisSlicing()) {
		ImGui::BeginDisabled();
		disabled = true;
	}

	int col = Col;
	int row = Row;
	ImGui::Text("	Col");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(100);
	if (ImGui::DragInt("##Col", &col)) {
		if(col > 0)Col = col;
	}
	ImGui::Text("	Row");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(100);
	if (ImGui::DragInt("##Row", &row)) {

		if (row > 0)Row = row;
	}

	if(col > 0 && row > 0)Render();
	ImGui::Text("		");
	ImGui::SameLine(70);
	if(ImGui::Button("Slice", Vec2(100, 20)) && Col > 0 && Row > 0) {
		Slice();
	}
	if (spriteedit->GetisSlicing() && disabled) {
		ImGui::EndDisabled();
	}
}

void Grid::Slice()
{
	spriteedit->ClearSprites();

	ImVec2 ImageOffset = spriteedit->GetImageOffset();
	float size = spriteedit->GetSize();
	Ptr<ATexture> atlas = spriteedit->GetCurAtlas();

	Vec2 resol = atlas->GetResol()* size;

	float width = resol.x / Col;
	float height = resol.y / Row;

	ImVec2 SelectStartPoint = ImVec2(ImageOffset.x, ImageOffset.y);
	ImVec2 SelectEndPoint = ImVec2(width + ImageOffset.x, height + ImageOffset.y);
	while (SelectEndPoint.y <= resol.y + ImageOffset.y + 0.05f)
	{
		LocalSprite* sprite = new LocalSprite;
		sprite->SelectStart = ImVec2(SelectStartPoint.x, SelectStartPoint.y);
		sprite->SelectEnd = ImVec2(SelectEndPoint.x, SelectEndPoint.y);
		spriteedit->AddLocalSprite(sprite);

		SelectStartPoint.x += width;		
		SelectEndPoint.x += width;
		if (SelectEndPoint.x > resol.x + ImageOffset.x + 0.05f) {
			SelectStartPoint.x = ImageOffset.x;
			SelectEndPoint.x = width + ImageOffset.x;
			SelectStartPoint.y += height; 
			SelectEndPoint.y += height; 
		}
	}

}

void Grid::Render() {

}


