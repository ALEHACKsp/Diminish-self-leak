//#include <wrl.h>
//#include <dxgi.h>
//#include <d2d1.h>
//#include <d3d11.h>
//#include <d2d1_2.h>
//#include <d3d11_2.h>
//#include <d2d1_3helper.h>
//#include <dwrite_3.h>
//#include <exception>
//#include <string>
//#include <dwrite.h>
//#include <Windows.h>
//#include <dcomp.h>
//#include "../../../../../../../../Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um/dcomp.h"
//#pragma warning(disable:4700)
//
//
//#pragma comment( lib, "dxgi" )
//#pragma comment( lib, "d2d1" )
//#pragma comment( lib, "d3d11" )
//#pragma comment( lib, "dcomp" )
//#pragma comment( lib, "dwrite" )
//
//#pragma comment( lib, "d2d1.lib" )
//#pragma comment( lib, "dxgi.lib" )
//#pragma comment( lib, "dcomp.lib" )
//#pragma comment( lib, "d3d11.lib" )
//
//HWND GetProcessWindow(DWORD pid)
//{
//	std::cout << pid << std::endl;
//	std::pair<HWND, DWORD> params = { 0, pid };
//
//	BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL
//		{
//			auto pParams = (std::pair<HWND, DWORD>*)(lParam);
//
//			DWORD processId;
//
//			if (GetWindowThreadProcessId(hwnd, &processId) && processId == pParams->second)
//			{
//				SetLastError((DWORD)-1);
//				pParams->first = hwnd;
//				return FALSE;
//			}
//
//			return TRUE;
//
//		}, (LPARAM)&params);
//
//	if (!bResult && GetLastError() == -1 && params.first)
//		return params.first;
//
//	return NULL;
//}
//
//class TemporaryWindow
//{
//	constexpr static auto WindowClassName = "AppWindowClass";
//	constexpr static auto WindowTitle = "";
//	HWND window;
//public:
//	TemporaryWindow()
//	{
//		WNDCLASS wc{};
//
//		wc.lpszClassName = WindowClassName;
//		wc.hInstance = GetModuleHandle(nullptr);
//		wc.lpfnWndProc = [](const HWND window, const UINT message, const WPARAM wparam, const LPARAM lparam) -> LRESULT
//		{
//			return DefWindowProc(window, message, wparam, lparam);
//		};
//
//		RegisterClass(&wc);
//
//		this->window = CreateWindow(WindowClassName, WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
//	}
//
//	~TemporaryWindow()
//	{
//		if (this->window)
//			DestroyWindow(this->window);
//
//		UnregisterClass(WindowClassName, GetModuleHandle(nullptr));
//	}
//
//	HWND Handle() const
//	{
//		return window;
//	}
//};
//
//template <typename T>
//using ComPtr = Microsoft::WRL::ComPtr<T>;
//
//class CompositionRenderer
//{
//public:
//	HWND targetWindow;
//	int screenWidth;
//	int screenHeight;
//
//	ID3D11Device* d3d11Device;
//	IDXGIDevice* dxgiDevice;
//	IDXGIFactory2* dxgiFactory;
//	IDXGISwapChain1* dxgiSwapChain;
//	IDXGISurface2* renderSurface;
//	ID2D1Bitmap1* renderBitmap;
//	ID2D1Factory2* d2dFactory;
//	ID2D1Device1* d2dDevice;
//	ID2D1DeviceContext1* d2dDeviceContext;
//	ID2D1SolidColorBrush* d2dSolidBrush;
//	IDWriteFactory* writeFactory;
//	IDWriteTextFormat* writeTextFormat;
//	IDCompositionDevice* compositionDevice;
//	IDCompositionTarget* compositionTarget;
//	IDCompositionVisual* compositionVisual;
//
//	CompositionRenderer() { }
//	CompositionRenderer(const CompositionRenderer&) = delete;
//	~CompositionRenderer() { }
//
//	bool BuildSwapchain(HWND GameWindow)
//	{
//		RECT window_rect;
//
//		if (!GetClientRect(GameWindow, &window_rect))
//		{
//			printf("[Renderer] Invalid Window.");
//			return false;
//		}
//
//		this->targetWindow = GameWindow;
//
//		HRESULT result =
//			D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0, D3D11_SDK_VERSION, &d3d11Device, nullptr, nullptr);
//
//		if (FAILED(result))
//		{
//			printf("[Renderer] Failed To Create Device.");
//			return false;
//		}
//
//		d3d11Device->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
//
//		result = CreateDXGIFactory2(NULL, __uuidof(dxgiFactory), reinterpret_cast<void**>(&dxgiFactory));
//
//		if (FAILED(result))
//		{
//			printf("[Renderer] Failed to Create DXGI factory.");
//			return false;
//		}
//
//		DXGI_SWAP_CHAIN_DESC1 description = { };
//		description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
//		description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//		description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
//		description.BufferCount = 2;
//		description.SampleDesc.Count = 1;
//		description.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
//		description.Width = screenWidth = window_rect.right - window_rect.left;
//		description.Height = screenHeight = window_rect.bottom - window_rect.top;
//
//		// This call can fail if a previous composition has not been cleaned up for this window
//
//		result = dxgiFactory->CreateSwapChainForComposition(d3d11Device, &description, nullptr, &dxgiSwapChain);
//
//		if (FAILED(result))
//		{
//			printf("[Renderer] Failed to Create SwapChain.");
//			return false;
//		}
//
//		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
//		result = d2dFactory->CreateDevice(dxgiDevice, &d2dDevice);
//
//		if (FAILED(result))
//		{
//			printf("[Renderer] Failed to Create ID2D1Device.");
//			return false;
//		}
//
//		result = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dDeviceContext);
//
//		if (FAILED(result))
//		{
//			printf("[Renderer] Failed to Create ID2DDeviceContext.");
//			return false;
//		}
//
//		dxgiSwapChain->GetBuffer(0, __uuidof(renderSurface), reinterpret_cast<void**>(&renderSurface));
//
//		// Render target
//
//		D2D1_BITMAP_PROPERTIES1 properties = { };
//		properties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
//		properties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
//		properties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
//
//		d2dDeviceContext->CreateBitmapFromDxgiSurface(renderSurface, properties, &renderBitmap);
//		d2dDeviceContext->SetTarget(renderBitmap);
//		d2dDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
//
//		// Objects used to draw
//
//		const auto color = D2D1::ColorF(0.f, 0.f, 0.f);
//		d2dDeviceContext->CreateSolidColorBrush(color, &d2dSolidBrush);
//		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), (IUnknown**)&writeFactory);
//		writeFactory->CreateTextFormat(L"tahoma", nullptr, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10.f, L"en-us", &writeTextFormat);
//
//		return true;
//	}
//
//	bool BuildComposition(HWND hTest)
//	{
//		HRESULT result = DCompositionCreateDevice(dxgiDevice, __uuidof(compositionDevice), reinterpret_cast<void**>(&compositionDevice));
//
//		if (FAILED(result))
//		{
//			printf("[Renderer] Create Device Failed.");
//			return false;
//		}
//
//		result = compositionDevice->CreateTargetForHwnd(hTest, true, &compositionTarget);
//
//		if (FAILED(result))
//		{
//			printf("[Renderer] Create Target Device Failed.");
//			return false;
//		}
//
//		result = compositionDevice->CreateVisual(&compositionVisual); if (FAILED(result)) { printf("er11\n"); };
//		result = compositionVisual->SetContent(dxgiSwapChain); if (FAILED(result)) { printf("er11\n"); };
//		result = compositionTarget->SetRoot(compositionVisual); if (FAILED(result)) { printf("er11\n"); };
//		result = compositionDevice->Commit(); if (FAILED(result)) { printf("er11\n"); };
//		result = compositionDevice->WaitForCommitCompletion(); if (FAILED(result)) { printf("er11\n"); };
//
//		return true;
//	}
//
//	void destroy_composition()
//	{
//		compositionVisual->SetContent(nullptr);
//		compositionTarget->SetRoot(nullptr);
//		compositionDevice->Release();
//		compositionVisual->Release();
//		compositionTarget->Release();
//	}
//
//	void begin_draw()
//	{
//		d2dDeviceContext->BeginDraw();
//		d2dDeviceContext->Clear();
//	}
//
//	void end_draw()
//	{
//		d2dDeviceContext->EndDraw();
//
//		HRESULT hPresentResult = dxgiSwapChain->Present(0, 0);
//
//		if (FAILED(hPresentResult))
//		{
//			printf("> present( ) failed = %X\n", hPresentResult);
//		}
//	}
//
//	void draw_rect(float x, float y, float w, float h, float t, const D2D1_COLOR_F& color)
//	{
//		d2dSolidBrush->SetColor(color);
//		d2dDeviceContext->DrawRectangle(D2D1::RectF(x, y, x + w, y + h), d2dSolidBrush, t);
//	}
//
//	void draw_filled_rect(
//		const float			x,
//		const float			y,
//		const float			w,
//		const float			h,
//		const D2D1_COLOR_F& color
//	) const
//	{
//		d2dSolidBrush->SetColor(color);
//		d2dDeviceContext->FillRectangle(D2D1::RectF(x, y, x + w, y + h), d2dSolidBrush);
//	}
//
//	void draw_cirlce(
//		const float			x,
//		const float			y,
//		const float			r,
//		const D2D1_COLOR_F& color,
//		const float			t
//	) const
//	{
//		d2dSolidBrush->SetColor(color);
//		d2dDeviceContext->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), r, r), d2dSolidBrush, t);
//	}
//
//	void draw_filled_circle(
//		const float			x,
//		const float			y,
//		const float			r,
//		const D2D1_COLOR_F& color
//	) const
//	{
//		d2dSolidBrush->SetColor(color);
//		d2dDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), r, r), d2dSolidBrush);
//	}
//
//	void draw_line(
//		const float			x1,
//		const float			y1,
//		const float			x2,
//		const float			y2,
//		const D2D1_COLOR_F& color,
//		const float			t
//	) const
//	{
//		d2dSolidBrush->SetColor(color);
//		d2dDeviceContext->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), d2dSolidBrush, t);
//	}
//
//	void draw_corner_box(int X, int Y, int W, int H, const D2D1_COLOR_F& Color)
//	{
//		float lineW = (W / 5);
//		float lineH = (H / 6);
//		float lineT = 0.5;
//
//		//outline
//		d2dSolidBrush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f));
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X - lineT, Y - lineT), D2D1::Point2F(X + lineW, Y - lineT), d2dSolidBrush); //top left
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X - lineT, Y - lineT), D2D1::Point2F(X - lineT, Y + lineH), d2dSolidBrush);
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X - lineT, Y + H - lineH), D2D1::Point2F(X - lineT, Y + H + lineT), d2dSolidBrush); //bot left
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X - lineT, Y + H + lineT), D2D1::Point2F(X + lineW, Y + H + lineT), d2dSolidBrush);
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X + W - lineW, Y - lineT), D2D1::Point2F(X + W + lineT, Y - lineT), d2dSolidBrush); // top right
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X + W + lineT, Y - lineT), D2D1::Point2F(X + W + lineT, Y + lineH), d2dSolidBrush);
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X + W + lineT, Y + H - lineH), D2D1::Point2F(X + W + lineT, Y + H + lineT), d2dSolidBrush); // bot right
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X + W - lineW, Y + H + lineT), D2D1::Point2F(X + W + lineT, Y + H + lineT), d2dSolidBrush);
//
//		//inline
//		d2dSolidBrush->SetColor(Color);
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X, Y), D2D1::Point2F(X, Y + lineH), d2dSolidBrush);//top left
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X, Y), D2D1::Point2F(X + lineW, Y), d2dSolidBrush);
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X + W - lineW, Y), D2D1::Point2F(X + W, Y), d2dSolidBrush); //top right
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X + W, Y), D2D1::Point2F(X + W, Y + lineH), d2dSolidBrush);
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X, Y + H - lineH), D2D1::Point2F(X, Y + H), d2dSolidBrush); //bot left
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X, Y + H), D2D1::Point2F(X + lineW, Y + H), d2dSolidBrush);
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X + W - lineW, Y + H), D2D1::Point2F(X + W, Y + H), d2dSolidBrush);//bot right
//		d2dDeviceContext->DrawLine(D2D1::Point2F(X + W, Y + H - lineH), D2D1::Point2F(X + W, Y + H), d2dSolidBrush);
//	}
//
//	bool draw_text(const std::wstring& text, float x, float y, float s, const D2D1_COLOR_F& color)
//	{
//		IDWriteTextLayout* layout;
//		writeFactory->CreateTextLayout(text.c_str(), UINT32(text.length() + 1), writeTextFormat, float(screenWidth), float(screenHeight), &layout);
//
//		const DWRITE_TEXT_RANGE range{ 0, UINT32(text.length()) };
//
//		layout->SetFontSize(s, range);
//
//		// draw black background
//
//		d2dSolidBrush->SetColor(D2D1::ColorF(0.f, 0.f, 0.f));
//		d2dDeviceContext->DrawTextLayout(D2D1::Point2F(x - 1, y), layout, d2dSolidBrush);
//		d2dDeviceContext->DrawTextLayout(D2D1::Point2F(x + 1, y), layout, d2dSolidBrush);
//		d2dDeviceContext->DrawTextLayout(D2D1::Point2F(x, y - 1), layout, d2dSolidBrush);
//		d2dDeviceContext->DrawTextLayout(D2D1::Point2F(x, y + 1), layout, d2dSolidBrush);
//
//		// draw color text
//
//		d2dSolidBrush->SetColor(color);
//		d2dDeviceContext->DrawTextLayout(D2D1::Point2F(x, y), layout, d2dSolidBrush);
//
//		layout->Release();
//
//		return true;
//	}
//
//	bool DrawTextCenter(const std::wstring& text, float x, float y, float s, const D2D1_COLOR_F& color)
//	{
//		IDWriteTextLayout* layout;
//		writeFactory->CreateTextLayout(text.c_str(), UINT32(text.length() + 1), writeTextFormat, float(screenWidth), float(screenHeight), &layout);
//
//		const DWRITE_TEXT_RANGE range{ 0, UINT32(text.length()) };
//
//		layout->SetFontSize(s, range);
//
//		// draw black background
//
//		DWRITE_TEXT_METRICS metrics = { 0 };
//		layout->GetMetrics(&metrics);
//		x -= metrics.width / 2.f;
//
//		d2dSolidBrush->SetColor(D2D1::ColorF(0.f, 0.f, 0.f));
//		d2dDeviceContext->DrawTextLayout(D2D1::Point2F(x - 1, y), layout, d2dSolidBrush);
//		d2dDeviceContext->DrawTextLayout(D2D1::Point2F(x + 1, y), layout, d2dSolidBrush);
//		d2dDeviceContext->DrawTextLayout(D2D1::Point2F(x, y - 1), layout, d2dSolidBrush);
//		d2dDeviceContext->DrawTextLayout(D2D1::Point2F(x, y + 1), layout, d2dSolidBrush);
//
//		// draw color text
//
//		d2dSolidBrush->SetColor(color);
//		d2dDeviceContext->DrawTextLayout(D2D1::Point2F(x, y), layout, d2dSolidBrush);
//
//		layout->Release();
//
//		return true;
//	}
//
//};
