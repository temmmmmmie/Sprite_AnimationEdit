#pragma once

class Engine
	: public singleton<Engine>
{
	SINGLE(Engine)
private:
	HINSTANCE	m_hInst;
	HWND		m_hMainWnd;
	Vec2		m_WindowResolution;

public:
	int Init(HINSTANCE _Inst, const wstring& _TitleName, const WNDCLASSEXW& _WindowClass, Vec2 _WindResolution);
	void Progress();


public:
	void ChangeResolution(Vec2 _Resolution);
	HWND GetMainWnd() { return  m_hMainWnd; }

private:
	int CreateGameWindow(const wstring& _Title, const WNDCLASSEXW& _WindowClass, Vec2 _WindResolution);
};


