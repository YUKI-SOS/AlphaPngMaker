#include "SREngine.h"
#include <stdio.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")	// writefactory


SREngine::SREngine()
	: m_hWnd(nullptr),
	m_pD2DFactory(nullptr),
	m_pDWriteFactory(nullptr),
	m_pTextFormat(nullptr),
	m_pRenderTarget(nullptr), m_pWICFactory(nullptr),
	m_pWhiteBrush(nullptr), m_pYellowBrush(nullptr), m_pTempBrush(nullptr), m_pNowBrush(nullptr),
	m_IsDDResReady(S_FALSE), m_RenderTargetSize(D2D1_SIZE_F())
{

}

SREngine::~SREngine()
{
	// 각 자원들의 명시적인 릴리즈 타이밍을 Finalize()로 옮겼음.

	// 명시적인 Finalize() 호출 이후에 엔진 소멸 시 필요한 처리
}

void SREngine::Initialize(HINSTANCE hinst, HWND hWnd, int screenWidth, int screenHeight)
{
	/// Initializes the COM library on the current thread and identifies the concurrency model as single-thread apartment (STA).
	HRESULT _hResult = CoInitialize(NULL);
	//CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	m_hWnd = hWnd;

	HRESULT hr = S_OK;

	// 1. Direct2D Factory를 만든다.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

	static const WCHAR msc_fontName[] = L"굴림";
	static const FLOAT msc_fontSize = 12;
	static const FLOAT msc_uifontSize = 15;

	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		reinterpret_cast<void**>(&m_pWICFactory)
	);

	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite factory.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_pDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
		);
	}

	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = m_pDWriteFactory->CreateTextFormat(
			msc_fontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			msc_fontSize,
			L"", //locale
			&m_pTextFormat
		);
	}


	// 텍스트 정렬 방식
	if (SUCCEEDED(hr))
	{
		// Left-Top the text horizontally and vertically.
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	}


	CreateDeviceResources(screenWidth, screenHeight);

}

void SREngine::ChangeDisplay(int screenWidth, int screenHeight)
{
}

void SREngine::RestoreDisplay()
{
}

void SREngine::BeginRender()
{
	m_IsDDResReady = S_OK;

	// 디바이스 종속적 자원들이 문제 없고, 랜더링을 할 수 있는 상황일 때
	if (SUCCEEDED(m_IsDDResReady) && !(m_pRenderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
	{
		// Retrieve the size of the render target.
		m_RenderTargetSize = m_pRenderTarget->GetSize();

		// 그리기는 BegineDraw()와 EndDraw()사이에
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		//m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	}
}
void SREngine::BeginRender(COLORREF clearColor)
{
	m_IsDDResReady = S_OK;

	// 디바이스 종속적 자원들이 문제 없고, 랜더링을 할 수 있는 상황일 때
	if (SUCCEEDED(m_IsDDResReady) && !(m_pRenderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
	{
		// Retrieve the size of the render target.
		m_RenderTargetSize = m_pRenderTarget->GetSize();

		// 그리기는 BegineDraw()와 EndDraw()사이에
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF(D2D1::ColorF(clearColor))));
	}
}

void SREngine::EndRender()
{
	if (SUCCEEDED(m_IsDDResReady) && !(m_pRenderTarget->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED))
	{
		// 그리기는 BegineDraw()와 EndDraw()사이에
		HRESULT hr = m_pRenderTarget->EndDraw();

		// 자원을 복구해야 할 경우
		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
		}
	}
}

HRESULT SREngine::CreateDeviceResources(int width, int height)
{
	HRESULT hr = S_OK;

	// 2. Direct2D 랜더 타겟을 생성한다.
	if (m_pRenderTarget == nullptr)
	{
		RECT rc;
		GetClientRect(m_hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			width,
			height);

		hr = m_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hWnd, size),
			&m_pRenderTarget);

		if (SUCCEEDED(hr))
		{
			// Create brushes.
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pBlackBrush);
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGreen), &m_pLightGreenBrush);
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &m_pRedBrush);
			hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pWhiteBrush);
			hr = m_pRenderTarget->CreateSolidColorBrush(ColorF(1.0f, 1.0f, 0.f), &m_pYellowBrush);
			hr = m_pRenderTarget->CreateSolidColorBrush(ColorF(0.0f, 0.0f, 0.0f), &m_pNowBrush);
		}

	}

	return hr;
}

void SREngine::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pWhiteBrush);
	SafeRelease(&m_pYellowBrush);
	SafeRelease(&m_pBlackBrush);
	SafeRelease(&m_pLightGreenBrush);
	SafeRelease(&m_pRedBrush);

}

void SREngine::Finalize()
{
	SafeRelease(&m_pD2DFactory);	//m_pD2DFactory->Release();
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pDWriteFactory);
	SafeRelease(&m_pTextFormat);
	SafeRelease(&m_pTempTextFormat);
	SafeRelease(&m_pWhiteBrush);
	SafeRelease(&m_pYellowBrush);

	CoUninitialize();
}

void SREngine::SetBrushColor(float r, float g, float b, float a)
{
	SafeRelease(&m_pNowBrush);

	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(r, g, b, a)), &m_pNowBrush);
}

void SREngine::SetBrushColor(unsigned long dwRGBColor)
{
	SafeRelease(&m_pNowBrush);

	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(dwRGBColor)), &m_pNowBrush);
}

void SREngine::DrawRectangle(int x, int y, int x1, int y1, COLORREF crBrush)
{
	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(crBrush)), &m_pTempBrush);

	if (m_pTempBrush != NULL)
	{
		D2D1_RECT_F _rt;
		_rt.left = (float)x;
		_rt.top = (float)y;
		_rt.right = (float)x1;
		_rt.bottom = (float)y1;

		m_pRenderTarget->DrawRectangle(_rt, m_pTempBrush, 1.0f);
	}

	SafeRelease(&m_pTempBrush);
}

void SREngine::DrawRectangle(int x, int y, int x1, int y1)
{
	D2D1_RECT_F _rt;
	_rt.left = (float)x;
	_rt.top = (float)y;
	_rt.right = (float)x1;
	_rt.bottom = (float)y1;

	m_pRenderTarget->DrawRectangle(_rt, m_pNowBrush, 2.0f);
}

void SREngine::DrawFillRectangle(int x, int y, int x1, int y1, COLORREF crBrush)
{
	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(D2D1::ColorF(crBrush))),
		&m_pTempBrush
	);

	if (m_pTempBrush != NULL)
	{
		D2D1_RECT_F _rt;
		_rt.left = (float)x;
		_rt.top = (float)y;
		_rt.right = (float)x1;
		_rt.bottom = (float)y1;

		m_pRenderTarget->FillRectangle(_rt, m_pTempBrush);
	}

	SafeRelease(&m_pTempBrush);
}

void SREngine::DrawLine(int x1, int y1, int x2, int y2)
{
	D2D1_POINT_2F _point;
	_point.x = (float)x1;
	_point.y = (float)y1;
	D2D1_POINT_2F _point2;
	_point2.x = (float)x2;
	_point2.y = (float)y2;

	m_pRenderTarget->DrawLine(_point, _point2, m_pNowBrush, 1.0f);
}
void SREngine::DrawLine(int x1, int y1, int x2, int y2, float width)
{
	D2D1_POINT_2F _point;
	_point.x = (float)x1;
	_point.y = (float)y1;
	D2D1_POINT_2F _point2;
	_point2.x = (float)x2;
	_point2.y = (float)y2;

	m_pRenderTarget->DrawLine(_point, _point2, m_pNowBrush, width);
}
void SREngine::DrawLine(int x1, int y1, int x2, int y2, float width, COLORREF crBrush)
{

	m_pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF(D2D1::ColorF(crBrush))),
		&m_pTempBrush
	);

	if (m_pTempBrush != NULL)
	{
		D2D1_POINT_2F _point;
		_point.x = (float)x1;
		_point.y = (float)y1;
		D2D1_POINT_2F _point2;
		_point2.x = (float)x2;
		_point2.y = (float)y2;

		m_pRenderTarget->DrawLine(_point, _point2, m_pTempBrush, width);
	}
	SafeRelease(&m_pTempBrush);

}

void SREngine::DrawEllipse(int x, int y, int size)
{
	this->DrawEllipse(x, y, x + size, y + size);
}

void SREngine::DrawEllipse(int left, int top, int right, int bottom)
{
	D2D1_ELLIPSE _region;
	_region.point.x = (float)left;
	_region.point.y = (float)top;
	_region.radiusX = (float)(right - left);
	_region.radiusY = (float)(bottom - top);

	m_pRenderTarget->DrawEllipse(_region, m_pNowBrush);
}

void SREngine::DrawSelectRectangle(int x1, int y1, int x2, int y2)
{
	this->DrawRectangle(x1, y1, x2, y2, COLORREF(RGB(0, 255, 0)));
}

void SREngine::DrawSelectedCircle(int x, int y, int size)
{
	this->DrawEllipse(x, y, x + size, y + size);
}

bool SREngine::DrawText(int x, int y, const char* pch, ...)
{
	// 포맷 스트링 관련 버퍼, 가변인자 처리
	char* buffer;
	int len;

	va_list ap;			// 현재 읽고 있는 번지를 기억
	va_start(ap, pch);	// ap 포인터 변수가 첫번째 가변인수를 가리키도록 초기화

	len = _vscprintf(pch, ap) + 1;
	buffer = new char[len];

	vsprintf_s(buffer, len, pch, ap);
	va_end(ap);


	//int nLen = lstrlenA(pch);
	int nLen = MultiByteToWideChar(CP_ACP, 0, buffer, lstrlen(buffer), NULL, NULL);

	BSTR unicodestr = SysAllocStringLen(NULL, nLen);

	// 당연히 멀티바이트와 유니코드의 문자열 길이 차이가 있다. 2바이트 문자 등
	::MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), unicodestr, nLen);

	if (unicodestr != NULL)
	{
		// 폰트별 차지하는 영역 계산 필요
		m_pRenderTarget->DrawText(unicodestr, wcslen(unicodestr), m_pTextFormat,
			//D2D1::RectF(x, y, m_RenderTargetSize.width, m_RenderTargetSize.height),
			D2D1::RectF((float)x, (float)y, (float)(x + 500), (float)(y + 40)),
			m_pNowBrush);
	}

	//... when done, free the BSTR
	::SysFreeString(unicodestr);

	// 포맷관련 버퍼 삭제
	delete[] buffer;

	return true;
}

bool SREngine::DrawText(int x, int y, COLORREF crBrush, const char* pch, ...)
{
	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(crBrush)), &m_pTempBrush);

	// 포맷 스트링 관련 버퍼, 가변인자 처리
	char* buffer;
	int len;

	va_list ap;			// 현재 읽고 있는 번지를 기억
	va_start(ap, pch);	// ap 포인터 변수가 첫번째 가변인수를 가리키도록 초기화

	len = _vscprintf(pch, ap) + 1;
	buffer = new char[len];

	vsprintf_s(buffer, len, pch, ap);
	va_end(ap);


	//int nLen = lstrlenA(pch);
	int nLen = MultiByteToWideChar(CP_ACP, 0, buffer, lstrlen(buffer), NULL, NULL);

	BSTR unicodestr = SysAllocStringLen(NULL, nLen);

	// 당연히 멀티바이트와 유니코드의 문자열 길이 차이가 있다. 2바이트 문자 등
	::MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), unicodestr, nLen);

	if (unicodestr != NULL)
	{
		// 폰트별 차지하는 영역 계산 필요
		m_pRenderTarget->DrawText(unicodestr, wcslen(unicodestr), m_pTextFormat,
			//D2D1::RectF(x, y, m_RenderTargetSize.width, m_RenderTargetSize.height),
			D2D1::RectF((float)x, (float)y, (float)(x + 500), (float)(y + 40)),
			m_pTempBrush);
	}

	//... when done, free the BSTR
	::SysFreeString(unicodestr);

	// 포맷관련 버퍼 삭제
	delete[] buffer;
	SafeRelease(&m_pTempBrush);

	return true;
}

bool SREngine::DrawText(D2D1_RECT_F d2d1Rect, int fontSize, COLORREF crBrush, const char* pch, ...)
{
	m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(crBrush)), &m_pTempBrush);

	m_pDWriteFactory->CreateTextFormat(
		L"맑은 고딕",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"", //locale
		&m_pTempTextFormat
		);
	m_pTempTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pTempTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	// 포맷 스트링 관련 버퍼, 가변인자 처리
	char* buffer;
	int len;

	va_list ap;			// 현재 읽고 있는 번지를 기억
	va_start(ap, pch);	// ap 포인터 변수가 첫번째 가변인수를 가리키도록 초기화

	len = _vscprintf(pch, ap) + 1;
	buffer = new char[len];

	vsprintf_s(buffer, len, pch, ap);
	va_end(ap);


	//int nLen = lstrlenA(pch);
	int nLen = MultiByteToWideChar(CP_ACP, 0, buffer, lstrlen(buffer), NULL, NULL);

	BSTR unicodestr = SysAllocStringLen(NULL, nLen);

	// 당연히 멀티바이트와 유니코드의 문자열 길이 차이가 있다. 2바이트 문자 등
	::MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), unicodestr, nLen);

	if (unicodestr != NULL && m_pTempBrush != NULL && m_pTempTextFormat != NULL)
	{
		// 폰트별 차지하는 영역 계산 필요
		//영역을 넘어가면 클립 해주는 옵션
		m_pRenderTarget->DrawText(unicodestr, wcslen(unicodestr), m_pTempTextFormat,
			//D2D1::RectF(x, y, m_RenderTargetSize.width, m_RenderTargetSize.height),
		d2d1Rect, m_pTempBrush, D2D1_DRAW_TEXT_OPTIONS_CLIP);
	}

	//... when done, free the BSTR
	::SysFreeString(unicodestr);

	// 포맷관련 버퍼 삭제
	delete[] buffer;
	SafeRelease(&m_pTempBrush);
	SafeRelease(&m_pTempTextFormat);
	return true;
}

bool SREngine::DrawText_Unicode(int x, int y, const wchar_t* unicodestr, ...)
{
	// 포맷 스트링 관련 버퍼, 가변인자 처리
	wchar_t* buffer;
	int len;

	va_list ap;			// 현재 읽고 있는 번지를 기억
	va_start(ap, unicodestr);	// ap 포인터 변수가 첫번째 가변인수를 가리키도록 초기화

	len = _vscwprintf(unicodestr, ap) + 1;
	buffer = new wchar_t[len];

	vswprintf_s(buffer, len, unicodestr, ap);
	va_end(ap);

	// 폰트별 차지하는 영역 계산 필요
	m_pRenderTarget->DrawText(buffer, wcslen(buffer), m_pTextFormat,
		//D2D1::RectF(x, y, m_RenderTargetSize.width, m_RenderTargetSize.height),
		D2D1::RectF((float)x, (float)y, (float)(x + 500), (float)(y + 40)),
		m_pLightGreenBrush);

	delete[] buffer;

	return true;
}

bool SREngine::DrawText_Unicode(D2D1_RECT_F d2d1Rect, const wchar_t* unicodestr, ...)
{
	// 포맷 스트링 관련 버퍼, 가변인자 처리
	wchar_t* buffer;
	int len;

	va_list ap;			// 현재 읽고 있는 번지를 기억
	va_start(ap, unicodestr);	// ap 포인터 변수가 첫번째 가변인수를 가리키도록 초기화

	len = _vscwprintf(unicodestr, ap) + 1;
	buffer = new wchar_t[len];

	vswprintf_s(buffer, len, unicodestr, ap);
	va_end(ap);

	// 폰트별 차지하는 영역 계산 필요
	m_pRenderTarget->DrawText(buffer, wcslen(buffer), m_pTextFormat,
		//D2D1::RectF(x, y, m_RenderTargetSize.width, m_RenderTargetSize.height),
		d2d1Rect,
		m_pYellowBrush);

	delete[] buffer;

	return true;
}

HRESULT SREngine::LoadMyBitMap(PCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap)
{
	IWICBitmapDecoder* pDecoder = NULL;
	IWICBitmapFrameDecode* pSource = NULL;
	IWICStream* pStream = NULL;
	IWICFormatConverter* pConverter = NULL;
	IWICBitmapScaler* pScaler = NULL;

	HRESULT hr = m_pWICFactory->CreateDecoderFromFilename(
		uri,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if (SUCCEEDED(hr))
	{
		hr = pDecoder->GetFrame(0, &pSource);
		hr = m_pWICFactory->CreateFormatConverter(&pConverter);
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
		hr = m_pRenderTarget->CreateBitmapFromWicBitmap(
			pConverter,
			NULL,
			ppBitmap
		);
	}

	SafeRelease(&pDecoder);
	SafeRelease(&pSource);
	SafeRelease(&pStream);
	SafeRelease(&pConverter);
	SafeRelease(&pScaler);

	return hr;
}

void SREngine::DrawImage(float x, float y, float width, float height, ID2D1Bitmap* bitmap)
{
	D2D1_SIZE_F _Size = bitmap->GetSize();
	D2D1_RECT_F _Rt = D2D1::RectF(x, y, x + width, y + height);
	m_pRenderTarget->DrawBitmap(bitmap, _Rt, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}

void SREngine::SetTransform(const D2D1_MATRIX_3X2_F& transformMatrix)
{
	m_pRenderTarget->SetTransform(transformMatrix);

}
