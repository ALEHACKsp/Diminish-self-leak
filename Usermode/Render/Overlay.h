#include <dwmapi.h>
#include <comdef.h> 
#include <d3d9.h>
#include "Renderer.h"

#include "..\imgui\imgui.h"
#include "..\imgui\imgui_impl_dx9.h"
#include "..\imgui\imgui_impl_win32.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")

inline int Width = GetSystemMetrics(SM_CXSCREEN);
inline int Height = GetSystemMetrics(SM_CYSCREEN);

inline const MARGINS Margin = { -1 };
inline DWORD ScreenCenterX;
inline DWORD ScreenCenterY;

inline RECT GameRect = { NULL };
inline HWND GameWnd = NULL;

inline IDirect3D9Ex* p_Object = NULL;
inline IDirect3DDevice9Ex* p_Device = NULL;
inline D3DPRESENT_PARAMETERS p_Params = { NULL };

inline HWND MyWnd = NULL;
inline MSG Message = { NULL };

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void SetupWindow();
void SetWindowToTarget();
void CleanuoD3D();
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
inline HWND hwnd = NULL;

inline bool ShowMenu = false;

class Overlay
{
public:
	void setup_overlay();
	void setup_keybinds();

private:
	HRESULT DirectXInit(HWND hWnd);
	WPARAM MainLoop();

	int isTopwin();
	void ChangeClickAbility(bool canclick, HWND passhwnd);
	void Draw();
	ImFont* Mark;

#define TopWindowGame 11
#define TopWindowMvoe 22
};

inline Overlay overlay;