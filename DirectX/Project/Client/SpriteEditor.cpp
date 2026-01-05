#include "pch.h"
#include "SpriteEditor.h"
#include "Engine/AssetMgr.h"
#include "Engine/ATexture.h"
#include "Engine/Asprite.h"
#include "EditorMgr.h"
#include "TreeUI.h"
#include "Grid.h"
#include "SpriteWindow.h"

#include <thread>
#include <atomic>


void SpriteEditor::RemapAlpha()
{
	float width = CurAtlas->GetResol().x;
	float hight = CurAtlas->GetResol().y;
	Alphamap.resize(width * hight);

	Pixel* data = CurAtlas->GetPixels();

	for (int y = 0; y < hight; y++) {
		for (int x = 0; x < width; x++) {
			Alphamap[y * width + x] = data->a;
			++data;
		}
	}
}

void SpriteEditor::Refresh()
{
	ClearSprites();
	RemapAlpha();
	ClearBoard();
	ResetHighlightedRect();
}



void SpriteEditor::AddLocalSprite(Ptr<LocalSprite> sprite)
{
	sprite->SelectStart = ImVec2(
		(sprite->SelectStart.x - ImageOffset.x) / (CurAtlas->GetResol().x * Size),
		(sprite->SelectStart.y - ImageOffset.y) / (CurAtlas->GetResol().y * Size)
	);
	sprite->SelectEnd = ImVec2(
		(sprite->SelectEnd.x - ImageOffset.x) / (CurAtlas->GetResol().x * Size),
		(sprite->SelectEnd.y - ImageOffset.y) / (CurAtlas->GetResol().y * Size)
	);
	wchar_t szBuffer[256] = {};
	swprintf_s(szBuffer, 256, L"_%02d", 1);
	sprite->Spritename = Atlasname + szBuffer;
	CurLocalSprites.push_back(sprite);
	RenameLocalSprites();
}

void SpriteEditor::RenameLocalSprites()
{
	for (size_t i = 0; i < CurLocalSprites.size(); i++)
	{
		wchar_t szBuffer[256] = {};
		swprintf_s(szBuffer, 256, L"_%02d", i + 1);
		CurLocalSprites[i]->Spritename = Atlasname + szBuffer;
	}
}



void SpriteEditor::MenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load Atlas"))
			{
				LoadAtlas();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Grid", nullptr, GridWind->IsActive()))
			{
				if (GridWind->IsActive()) {
					GridWind->SetActive(false);
				}
				else {
					GridWind->SetActive(true);
				}
			}
			if (ImGui::MenuItem("SpriteWindow", nullptr, SpriteWind->IsActive()))
			{
				if (SpriteWind->IsActive()) {
					SpriteWind->SetActive(false);
				}
				else {
					SpriteWind->SetActive(true);
				}
			}
			if (ImGui::MenuItem("Slice", nullptr))
			{
				Slice();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}
void SpriteEditor::AtlasImage()
{
	if (CurAtlas != nullptr) {
		float size = Size;

		// 텍스쳐 해상도
		ImGui::Text("Size");
		ImGui::SameLine(100);
		ImGui::SetNextItemWidth(200);
		if (ImGui::DragFloat("##Width", &size, 0.05f)) {
			if(size > 0)Size = size;
		}
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
		ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);

		ImageOffset = ImGui::GetCursorScreenPos();

		ImGui::Image((ImTextureID)CurAtlas->GetSRV().Get(), ImVec2(CurAtlas->GetWidth() * Size, CurAtlas->GetHeight() * Size), Vec2(0.f, 0.f), Vec2(1.f, 1.f), tint_col, border_col);
		if (!DropCheck() && ImGui::IsItemHovered()) { DebugRectInput(); }
		if(!isSlicing)DrawDebugRect();
	}
}
bool SpriteEditor::DropCheck()
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
				if (ASSET_TYPE::TEXTURE == pAsset->GetType())
				{
					CurAtlas = (ATexture*) pAsset.Get();
					Atlasname = pAsset->GetKey();
					Refresh();
					ImGui::EndDragDropTarget();
					return true;
				}
			}
		}

		ImGui::EndDragDropTarget();
	}
	return false;
}

void SpriteEditor::DebugRectInput()
{
	ImVec2 mousePos = ImGui::GetMousePos();
	ImVec2 localPos = ImVec2(mousePos.x - ImageOffset.x, mousePos.y - ImageOffset.y);

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	static bool selecting = false;
	static ImVec2 selectStart;
	static ImVec2 selectEnd;
	static ImVec2 p1;
	static ImVec2 p2;
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)&& ImGui::IsItemHovered()) {
		selecting = true;
		selectStart = localPos;
	}
	if (selecting && ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsItemHovered()) {
		selectEnd = localPos;
		p1 = ImVec2(ImageOffset.x + selectStart.x, ImageOffset.y + selectStart.y);
		p2 = ImVec2(ImageOffset.x + selectEnd.x, ImageOffset.y + selectEnd.y);
		drawList->AddRect(p1, p2, IM_COL32(255, 255, 0, 255), 0.0f, 0, 2.0f);
	}

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && ImGui::IsItemHovered()) {
		selecting = false;
		if (abs(selectStart.x - selectEnd.x) < 20 && abs(selectStart.y - selectEnd.y) < 20) {
			BFS(localPos);
			return;
		}
		Ptr< LocalSprite> localsprite = new LocalSprite;
		localsprite->SelectStart = p1;
		localsprite->SelectEnd = p2;
		AddLocalSprite(localsprite);
	}
}

void SpriteEditor::DrawDebugRect()
{
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	for (size_t i = 0; i < CurLocalSprites.size(); i++)
	{
		ImVec2 p1 = CurLocalSprites[i]->SelectStart;
		p1.x = ImageOffset.x + p1.x * CurAtlas->GetResol().x * Size;
		p1.y = ImageOffset.y + p1.y * CurAtlas->GetResol().y * Size;

		ImVec2 p2 = CurLocalSprites[i]->SelectEnd;
		p2.x = ImageOffset.x + p2.x * CurAtlas->GetResol().x * Size;
		p2.y = ImageOffset.y + p2.y * CurAtlas->GetResol().y * Size;
		
		if(i == HighlightedRect )
			drawList->AddRect(p1, p2, IM_COL32(255, 0, 0, 255), 0.0f, 0, 2.0f);
		else
			drawList->AddRect(p1, p2, IM_COL32(255, 255, 0, 255), 0.0f, 0, 2.0f);
	}
}

inline int SpriteEditor::GetAlpha(ImVec2 LocalPos)
{
	return (int)Alphamap[LocalPos.y * CurAtlas->GetResol().x + LocalPos.x];
}


void SpriteEditor::BFS(ImVec2 LocalPos)
{
	LocalPos.x /= Size;
	LocalPos.y /= Size;

	if (GetAlpha(LocalPos) == 0) return;

	int width = CurAtlas->GetResol().x;
	int height = CurAtlas->GetResol().y;

	list<ImVec2> q;

	Board[(int)LocalPos.y * width + (int)LocalPos.x] = true;
	q.push_back(LocalPos);

	// Bounding box 초기화
	ImVec2 minPos = LocalPos;
	ImVec2 maxPos = LocalPos;

	// 상하좌우 방향 벡터
	const int dx[4] = { 1, -1, 0, 0 };
	const int dy[4] = { 0, 0, 1, -1 };

	while (!q.empty())
	{
		ImVec2 cur = q.front();
		q.pop_front();

		// Bounding box 갱신
		if (cur.x < minPos.x) minPos.x = cur.x;
		if (cur.y < minPos.y) minPos.y = cur.y;
		if (cur.x > maxPos.x) maxPos.x = cur.x;
		if (cur.y > maxPos.y) maxPos.y = cur.y;

		// 주변 4방향 탐색
		for (int dir = 0; dir < 4; dir++)
		{
			int nx = (int)cur.x + dx[dir];
			int ny = (int)cur.y + dy[dir];

			if (nx < 0 || ny < 0 || nx >= width || ny >= height)
				continue;
			
			if (GetAlpha(ImVec2(nx, ny)) < 5)
				continue;

			if (true == Board[ny * width + nx])
				continue;

			Board[ny * width + nx] = true;
			q.push_back(ImVec2(nx, ny));
		}
	}

	if (maxPos.x - minPos.x < 10 && maxPos.y - minPos.y < 10) return;


	LocalSprite* sprite = new LocalSprite;
	sprite->Spritename = L"a";
	sprite->SelectStart = ImVec2(ImageOffset.x + minPos.x * Size, ImageOffset.y + minPos.y * Size);
	sprite->SelectEnd = ImVec2(ImageOffset.x + (maxPos.x + 1) * Size, ImageOffset.y + (maxPos.y + 1) * Size);
	
	AddLocalSprite(sprite);
}

void SpriteEditor::Slice() {
	if (isSlicing) return;

	isSlicing = true;
	sliceFinished = false;

	std::thread SliceThread([this]()
	{
		ClearSprites();
		ClearBoard();
		ResetHighlightedRect();

		int width = CurAtlas->GetResol().x;
		int height = CurAtlas->GetResol().y;

		for (size_t x = 0; x < width; x++)
		{
			for (size_t y = 0; y < height; y++)
			{
				if (true == Board[y * width + x]) continue;

				if (GetAlpha(ImVec2(x, y)) > 5 && false == Board[y * width + x]) {
					BFS(ImVec2(x * Size, y * Size));
				}
				else {
					Board[y * width + x] = true; // 투명한 곳도 방문 완료로 간주
				}
			}
		}
		isSlicing = false;
		sliceFinished = true;
	});

	SliceThread.detach();
}


//path
wstring SpriteEditor::GetRelativepath(wchar_t Path[])
{
	wstring contentpath = PathMgr::GetInst()->GetContentPath();
	int length = contentpath.length();
	wstring tempFilePath;
	int i = 0;
	while (Path[i + length] != 0)
	{
		tempFilePath.push_back( Path[i + length]);
		++i;
	}
	return tempFilePath;
}

void SpriteEditor::LoadAtlas()
{
	// 파일경로를 저장할 배열
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};
	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;	// 최종적으로 고른 경로를 받아낼 목적지
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"PNG\0*.png\0JPG\0*.jpg\0BMP\0*.bmp\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = PathMgr::GetInst()->GetContentPath().c_str();


	if (GetOpenFileName(&Desc))
	{
		Ptr< ATexture> tex = AssetMgr::GetInst()->Load<ATexture>(GetRelativepath(szFilePath), GetRelativepath(szFilePath));
		CurAtlas = tex;
		Atlasname = tex->GetKey();
		Refresh();
	}
}

void SpriteEditor::Tick()
{
	EditorUI::Tick();

}

void SpriteEditor::Tick_UI()
{
	bool disabled = false;
	if (isSlicing) {
		ImGui::BeginDisabled();
		disabled = true;
	}

	MenuBar();

	AtlasImage();

	if (isSlicing && disabled) {
		ImGui::EndDisabled();
	}
}

SpriteEditor::SpriteEditor()
	: EditorUI("SpriteEditor"),
	Size(1),
	Atlasname(L"SpriteExample"),
	HighlightedRect(-1)
{
	UseMenuBar(true);
	CurAtlas = FIND(ATexture, L"SpriteExample");
	GridWind = FIND_UI(Grid, "Grid");
	SpriteWind = FIND_UI(SpriteWindow, "SpriteWindow");
	Grid* g = (Grid*)GridWind;
	SpriteWindow* s = (SpriteWindow*)SpriteWind;
	g->spriteedit = this;
	s->spriteedit = this;

	Refresh();
}

SpriteEditor::~SpriteEditor()
{
}