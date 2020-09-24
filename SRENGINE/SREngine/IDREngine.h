#pragma once
#include <windows.h>
#include <d2d1.h>

/// Darong 2D Engine Interface
///
/// ����ؼ� WinAPI, Direct2D���� ��� �����ϴ�.
/// ��� �߻�ȭ�� �Ǿ� ������, windows.h������ ����Ѵ�.
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
	/// �ʱ�ȭ, �׸���, ������

	// �ʱ�ȭ ����
	virtual void Initialize(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight) abstract;
	virtual void ChangeDisplay(int screenWidth, int screenHeight) abstract;
	virtual void RestoreDisplay() abstract;

	// Render ����
	virtual void BeginRender() abstract;
	virtual void BeginRender(COLORREF clearColor) abstract;

	// Render ��
	virtual void EndRender() abstract;

	// ������ �����Ѵ�.
	virtual void Finalize() abstract;


	///--------------------------------------------------------------------------------
	/// �׸��� ���� Primitive

	// ������ �׸��� �귯�� ����
	virtual void SetBrushColor(float r, float g, float b, float a) abstract;
	virtual void SetBrushColor(unsigned long dwRGBColor) abstract;

	// ������ ��ǥ�� �簢���� ����Ѵ�.
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

	// ȭ�鿡 ���ڸ� ���
	virtual bool DrawText(int x, int y, const char* pch, ...) abstract;
	virtual bool DrawText(int x, int y, COLORREF crBrush, const char* pch, ...) abstract;
	virtual bool DrawText(D2D1_RECT_F d2d1Rect,int fontSize, COLORREF crBrush, const char* pch, ...) abstract;
	virtual bool DrawText_Unicode(int x, int y, const wchar_t* unicodestr, ...) abstract;
	virtual bool DrawText_Unicode(D2D1_RECT_F d2d1Rect, const wchar_t* unicodestr, ...) abstract;

	//�̹���
	virtual	HRESULT LoadMyBitMap(PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap) abstract;
	virtual	void DrawImage(float x, float y, float width, float height, ID2D1Bitmap* bitmap) abstract;

	/// TRS ����
	virtual void SetTransform(const D2D1_MATRIX_3X2_F& transformMatrix) abstract;
};
