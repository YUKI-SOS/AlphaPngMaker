#pragma once

/// Direct2d 그래픽스 엔진
///
/// 인터페이스를 통해서, WinAPI 그래픽스 엔진과 교체 가능하다.
///
/// [11/16/2019 LeHideHome]

#include "IDREngine.h"
#include "DRMacro.h"
#include <d2d1.h>
#include <d2d1_1.h>
#include "d2d1helper.h"		// ColorF
#include <dwrite.h>			// writefactory
#include <wincodec.h>



using namespace D2D1;

class SREngine : public IDREngine
{
public:
	SREngine();
	virtual ~SREngine();

	/// IDREngine override
public:
	///--------------------------------------------------------------------------------
	/// 초기화 관련

	// 초기화 관련
	virtual void Initialize(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight) override;
	virtual void ChangeDisplay(int screenWidth, int screenHeight) override;
	virtual void RestoreDisplay() override;

	// Render 시작
	virtual void BeginRender() override;
	virtual void BeginRender(COLORREF clearColor) override;

	// Render 끝
	virtual void EndRender() override;

	// 엔진을 종료한다.
	virtual void Finalize() override;

private:
	HRESULT CreateDeviceResources(int width, int height);
	void DiscardDeviceResources();

	///--------------------------------------------------------------------------------
	/// 그리기 관련

	// 도형을 그리는 브러시 관련
	virtual void SetBrushColor(float r, float g, float b, float a) override;
	virtual void SetBrushColor(unsigned long dwRGBColor) override;

	// 지정된 좌표에 사각형을 출력한다. 이런식으로 장치 독립적으로 가야 한다.
	virtual void DrawRectangle(int x, int y, int x1, int y1, COLORREF crBrush) override;
	virtual void DrawRectangle(int x, int y, int x1, int y1) override;
	virtual void DrawFillRectangle(int x, int y, int x1, int y1, COLORREF crBrush);
	virtual void DrawLine(int x1, int y1, int x2, int y2) override;
	virtual void DrawLine(int x1, int y1, int x2, int y2, float width) override;
	virtual void DrawLine(int x1, int y1, int x2, int y2, float width, COLORREF crBrush) override;
	virtual void DrawEllipse(int x, int y, int size) override;
	virtual void DrawEllipse(int left, int top, int right, int bottom) override;
	virtual void DrawSelectRectangle(int x1, int y1, int x2, int y2) override;
	virtual void DrawSelectedCircle(int x, int y, int size) override;

	// 화면에 문자를 출력
	virtual bool DrawText(int x, int y, const char* pch, ...) override;
	virtual bool DrawText(int x, int y,COLORREF crBrush, const char* pch, ...) override;
	virtual bool DrawText(D2D1_RECT_F d2d1Rect, int fontSize, COLORREF crBrush, const char* pch, ...) override;
	virtual bool DrawText_Unicode(int x, int y, const wchar_t* unicodestr, ...) override;
	virtual bool DrawText_Unicode(D2D1_RECT_F d2d1Rect, const wchar_t* unicodestr, ...) override;

	//이미지
	HRESULT LoadMyBitMap(PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap);
	void DrawImage(float x, float y, float width, float height, ID2D1Bitmap* bitmap);

	/// TRS 관련
	virtual void SetTransform(const D2D1_MATRIX_3X2_F& transformMatrix) override;

private:
	HWND m_hWnd;								// 그리기를 할 윈도 (1개라고 가정)

	// Device Independant Resources
	ID2D1Factory* m_pD2DFactory;				// D2D는 Factory를 생성하면서 시작한다.

	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
	IDWriteTextFormat* m_pTempTextFormat;
	IWICImagingFactory* m_pWICFactory;


	// Device Dependant Resources
	ID2D1HwndRenderTarget* m_pRenderTarget;		// 그리기를 할 대상인 랜더 타겟


	// 브러시들
	ID2D1SolidColorBrush* m_pBlackBrush;
	ID2D1SolidColorBrush* m_pLightGreenBrush;
	ID2D1SolidColorBrush* m_pRedBrush;
	ID2D1SolidColorBrush* m_pWhiteBrush;
	ID2D1SolidColorBrush* m_pYellowBrush;
	ID2D1SolidColorBrush* m_pTempBrush;			// 그리기 함수 내에서만 사용의 용도
	ID2D1SolidColorBrush* m_pNowBrush;			// 바꿔가면서 여러 번 재사용의 용도



private:
	// For render
	HRESULT m_IsDDResReady;				// 디바이스 종속적인 자원들이 준비되었는가?
	D2D1_SIZE_F m_RenderTargetSize;

};

