#pragma once
#include <windows.h>
#include <d2d1.h>

/// Darong 2D Engine Interface
///
/// 상속해서 WinAPI, Direct2D등을 사용 가능하다.
/// 모두 추상화가 되어 있으며, windows.h정도를 사용한다.
///
/// [12/27/2019 LeHide]
///

class IDREngine abstract
{
protected:
	static IDREngine* m_Instance;

public:
	//IDREngine();
	//~IDREngine();

	static IDREngine* GetInstance() { return m_Instance; }

	///--------------------------------------------------------------------------------
	/// 초기화, 그리기, 끝내기

	// 초기화 관련
	virtual void Initialize(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight) abstract;
	virtual void ChangeDisplay(int screenWidth, int screenHeight) abstract;
	virtual void RestoreDisplay() abstract;

	// Render 시작
	virtual void BeginRender() abstract;
	virtual void BeginRender(COLORREF clearColor) abstract;

	// Render 끝
	virtual void EndRender() abstract;

	// 엔진을 종료한다.
	virtual void Finalize() abstract;


	///--------------------------------------------------------------------------------
	/// 그리기 관련 Primitive

	// 도형을 그리는 브러시 관련
	virtual void SetBrushColor(float r, float g, float b, float a) abstract;
	virtual void SetBrushColor(unsigned long dwRGBColor) abstract;

	// 지정된 좌표에 사각형을 출력한다.
	virtual void DrawRectangle(int x, int y, int x1, int y1) abstract;
	virtual void DrawRectangle(int x, int y, int x1, int y1, COLORREF crBrush) abstract;
	virtual void DrawFillRectangle(int x, int y, int x1, int y1, COLORREF crBrush) abstract;
	virtual void DrawLine(int x1, int y1, int x2, int y2) abstract;
	virtual void DrawLine(int x1, int y1, int x2, int y2, float width) abstract;
	virtual void DrawLine(int x1, int y1, int x2, int y2, float width, COLORREF crBrush) abstract;
	virtual void DrawEllipse(int x, int y, int size) abstract;
	virtual void DrawEllipse(int left, int top, int right, int bottom) abstract;
	virtual void DrawSelectRectangle(int x1, int y1, int x2, int y2) abstract;
	virtual void DrawSelectedCircle(int x, int y, int size) abstract;

	// 화면에 문자를 출력
	virtual bool DrawText(int x, int y, const char* pch, ...) abstract;
	virtual bool DrawText(int x, int y, COLORREF crBrush, const char* pch, ...) abstract;
	virtual bool DrawText(D2D1_RECT_F d2d1Rect,int fontSize, COLORREF crBrush, const char* pch, ...) abstract;
	virtual bool DrawText_Unicode(int x, int y, const wchar_t* unicodestr, ...) abstract;
	virtual bool DrawText_Unicode(D2D1_RECT_F d2d1Rect, const wchar_t* unicodestr, ...) abstract;

	//이미지
	virtual	HRESULT LoadMyBitMap(PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap) abstract;
	virtual	void DrawImage(float x, float y, float width, float height, ID2D1Bitmap* bitmap) abstract;

	/// TRS 관련
	virtual void SetTransform(const D2D1_MATRIX_3X2_F& transformMatrix) abstract;
};

