#include "pch.h"
#include "FrameProperty.h"
#include "Engine/CFlipbookRender.h"
#include "Engine/AFlipbook.h"
#include "Engine/ASprite.h"

void FrameProperty::Tick_UI()
{
	Ptr<ASprite> sprite = animationedit->GetSelectedSprite();
	Ptr<CFlipbookRender> flip = animationedit->GetFlipBookRender();
	if (nullptr != sprite) {

		ImGui::Text("Name: ");
		ImGui::SameLine(70);
		ImGui::Text(GetString( sprite->GetKey()).c_str());

		ImGui::Text("LeftTop: ");
		ImGui::SameLine(70);
		string buffer = "(" + std::to_string(sprite->GetLeftTop().x) + ", " + std::to_string(sprite->GetLeftTop().y) + ")";
		ImGui::Text(buffer.c_str());
	
		ImGui::Text("Slice: ");
		ImGui::SameLine(70);
		buffer = "(" + std::to_string(sprite->GetSlice().x) + ", " + std::to_string(sprite->GetSlice().y) + ")";
		ImGui::Text( buffer.c_str() );


		ImGui::Text("Offset: ");
		ImGui::SameLine(70);
		Vec2 off = sprite->GetOffset();
		if (ImGui::DragFloat2("##Offset", off, 0.001f))
		{
			sprite->SetRawOffset(off);
		}

		if (ImGui::Button("Delete", Vec2(70, 20))) {
			int idx = flip->GetFrameIdx();
			flip->GetFlipbook()->DeleteSprite(idx);
			if(idx != 0)flip->SetFrame(--idx);
		};
	}
}

FrameProperty::FrameProperty()
	: EditorUI("FrameProperty")
{
}

FrameProperty::~FrameProperty()
{
}
