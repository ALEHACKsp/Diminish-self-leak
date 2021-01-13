#include "Overlay.h"
#include <thread>
#include "../Game/Game.h"
#include "../Hacks/Visuals.h"
//#include "data.h"

void CheckboxColPicker(const std::string& name, bool* enable, float* color)
{
    ImGui::Checkbox(("##" + name).c_str(), enable);
    ImGui::SameLine(0.0f, 10.5f);
    ImGui::PushID(0);
    bool openPopup = ImGui::ColorButton(("##" + name).c_str(), ImColor{ color[0], color[1], color[2] }, ImGuiColorEditFlags_NoTooltip);
    ImGui::PopID();
    ImGui::SameLine(0.0f, 10.0f);
    ImGui::TextUnformatted(name.c_str());
    ImGui::PushID(1);
    if (openPopup)
        ImGui::OpenPopup(("##" + name).c_str());
    if (ImGui::BeginPopup(("##" + name).c_str())) {
        ImGui::PushID(2);
        ImGui::ColorPicker3(("##" + name).c_str(), color, ImGuiColorEditFlags_NoOptions);
        ImGui::PopID();
        ImGui::EndPopup();
    }
    ImGui::PopID();
}

void ELMenu() {

	ImGui::Begin(x("Diminsh.VIP"), &whatever, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	{
		if (ImGui::Button(x("Aimbot"), ImVec2(120.0f, 33.0f)))
			SwitchTabs = 0;
		ImGui::SameLine();
		if (ImGui::Button(x("Visuals"), ImVec2(120.0f, 33.0f)))
			SwitchTabs = 1;
		ImGui::SameLine();
		if (ImGui::Button(x("Misc"), ImVec2(120.0f, 33.0f)))
			SwitchTabs = 2;
		ImGui::SameLine();
		if (ImGui::Button(x("AntiAim"), ImVec2(120.0f, 33.0f)))
			SwitchTabs = 3;
		ImGui::SameLine();
		if (ImGui::Button(x("Configs"), ImVec2(120.0f, 33.0f)))
			SwitchTabs = 4;

		ImGui::Separator();
	}

	if (SwitchTabs == 0) {

		ImGui::Checkbox(x("Aimb0t"), &Config::AimFunctions::Aimbot);
		ImGui::Checkbox(x("pSilent"), &Config::AimFunctions::pSilent);
		ImGui::Checkbox(x("EntitySwap"), &Config::AimFunctions::EntitySwap);
		ImGui::Checkbox(x("Gun Rotation"), &Config::AimFunctions::GunRotation);
		 
		ImGui::Checkbox(x("FOV Check"), &Config::AimFunctions::FovCheck);
		ImGui::SliderInt(x("Aimbot FOV"), &Config::AimFunctions::FOV, 0, 1000);

		ImGui::Spacing();
		ImGui::Text("Key Binds");
		ImGui::Separator();

		ImGui::Combo(x("Aim Button"), &Config::AimFunctions::AimBotKey, KeyNames, IM_ARRAYSIZE(KeyNames));
		ImGui::Combo(x("pSilent Button"), &Config::AimFunctions::pSilentKey, KeyNames, IM_ARRAYSIZE(KeyNames));
		ImGui::Combo(x("Entity Swap Button"), &Config::AimFunctions::EntitySwapKey, KeyNames, IM_ARRAYSIZE(KeyNames));
		ImGui::Combo(x("Gun Rotation Button"), &Config::AimFunctions::GunRotationKey, KeyNames, IM_ARRAYSIZE(KeyNames));
	}
	if (SwitchTabs == 1) {
		ImGui::Checkbox(x("ESP"), &Config::ESPFunctions::bESP);

		ImGui::Checkbox(x("Outlines"), &Config::ESPFunctions::Outlines);
		if (Config::ESPFunctions::Outlines) {
			ImGui::SliderInt(x("Thickness"), &Config::ESPFunctions::OutlinesThickness, 0, 10);
		} 
		ImGui::Checkbox(x("Glow"), &Config::ESPFunctions::Glow);
		ImGui::Checkbox(x("Box2D"), &Config::ESPFunctions::Box2D);
		ImGui::Checkbox(x("Box3D"), &Config::ESPFunctions::Box3D);
		ImGui::Checkbox(x("CornerBox"), &Config::ESPFunctions::CornerBox);
		ImGui::Checkbox(x("HealthBar"), &Config::ESPFunctions::HealthBars);
		ImGui::Checkbox(x("SnapLines"), &Config::ESPFunctions::SnapLines);
		if (Config::ESPFunctions::SnapLines) {
			ImGui::Checkbox(x("On Target"), &Config::ESPFunctions::OnTarget);
			ImGui::Checkbox(x("On Silent Key"), &Config::ESPFunctions::OnSilentKey);
			ImGui::SliderInt(x("Thickness"), &Config::ESPFunctions::SnapLineThickness, 1, 10);
			ImGui::Combo("SnapFrom", &Config::ESPFunctions::SnapFromType, Config::ESPFunctions::SnapFrom, IM_ARRAYSIZE(Config::ESPFunctions::SnapFrom));
			ImGui::Combo("SnapTo", &Config::ESPFunctions::SnapToType, Config::ESPFunctions::SnapTo, IM_ARRAYSIZE(Config::ESPFunctions::SnapTo));
		}

		ImGui::Spacing();
		ImGui::Text("Colors (Checkbox = Rainbow)");
		ImGui::Separator();

		CheckboxColPicker("Fov", &Config::ESPFunctions::FovColorRainbow, Config::ESPFunctions::FovColor);
		CheckboxColPicker("Box", &Config::ESPFunctions::BoxColorRainbow, Config::ESPFunctions::BoxColor);
		CheckboxColPicker("Glow", &Config::ESPFunctions::GlowColorRainbow, Config::ESPFunctions::GlowColor);
		CheckboxColPicker("Outline", &Config::ESPFunctions::OutlineColorRainbow, Config::ESPFunctions::OutlineColor);
		CheckboxColPicker("Snap Line", &Config::ESPFunctions::SnapLineColorRainbow, Config::ESPFunctions::SnapLineColor);
		ImGui::SliderInt(x("Rainbow Speed"), &Config::ESPFunctions::RainbowSpeed, 0, 500);

		if (Config::ESPFunctions::FovColorRainbow) {
			Config::ESPFunctions::FovColor[0] = RainbowColor[0] / 255;
			Config::ESPFunctions::FovColor[1] = RainbowColor[1] / 255;
			Config::ESPFunctions::FovColor[2] = RainbowColor[2] / 255;
		}
		if (Config::ESPFunctions::BoxColorRainbow) {
			Config::ESPFunctions::BoxColor[0] = RainbowColor[0] / 255;
			Config::ESPFunctions::BoxColor[1] = RainbowColor[1] / 255;
			Config::ESPFunctions::BoxColor[2] = RainbowColor[2] / 255;
		}
		if (Config::ESPFunctions::GlowColorRainbow) {
			Config::ESPFunctions::GlowColor[0] = RainbowColor[0] / 255;
			Config::ESPFunctions::GlowColor[1] = RainbowColor[1] / 255;
			Config::ESPFunctions::GlowColor[2] = RainbowColor[2] / 255;
		}
		if (Config::ESPFunctions::OutlineColorRainbow) {
			Config::ESPFunctions::OutlineColor[0] = RainbowColor[0] / 255;
			Config::ESPFunctions::OutlineColor[1] = RainbowColor[1] / 255;
			Config::ESPFunctions::OutlineColor[2] = RainbowColor[2] / 255;
		}
		if (Config::ESPFunctions::SnapLineColorRainbow) {
			Config::ESPFunctions::SnapLineColor[0] = RainbowColor[0] / 255;
			Config::ESPFunctions::SnapLineColor[1] = RainbowColor[1] / 255;
			Config::ESPFunctions::SnapLineColor[2] = RainbowColor[2] / 255;
		}
	}
	if (SwitchTabs == 2) {
		ImGui::Checkbox(x("Miscellaneous"), &Config::MiscFucntions::Misc);
		ImGui::Checkbox(x("Done Mods"), &Config::MiscFucntions::DoneMods);
		if (Config::MiscFucntions::DoneMods) {
			ImGui::Indent();
			ImGui::SliderInt(x("Drone Jump Height"), &Config::MiscFucntions::DroneJumpHeight, 0, 1500);
			ImGui::SliderInt(x("Drone Gravity"), &Config::MiscFucntions::DroneGravity, 0, 1000);
			ImGui::SliderInt(x("Drone Speed"), &Config::MiscFucntions::DroneSpeed, 0, 1000);
			ImGui::Unindent();
		}

		ImGui::Checkbox(x("Speed"), &Config::MiscFucntions::Speed);
		if (Config::MiscFucntions::Speed) { ImGui::Indent(); ImGui::SliderInt(x("Speed"), &Config::MiscFucntions::SpeedVal, 0, 200); ImGui::Unindent(); }

		ImGui::Checkbox(x("Gun Mods"), &Config::MiscFucntions::GunMods);
		if (Config::MiscFucntions::GunMods) {
			ImGui::Indent();
			ImGui::SliderInt(x("Recoil"), &Config::MiscFucntions::Recoil, 0, 75);
			ImGui::SliderInt(x("Spread"), &Config::MiscFucntions::Spread, 0, 75);
			ImGui::Unindent();
		}

		ImGui::Checkbox(x("Knife Mods"), &Config::MiscFucntions::KnifeMods);
		if (Config::MiscFucntions::KnifeMods) {
			ImGui::Indent();
			ImGui::Checkbox(x("Knife Through Walls"), &Config::MiscFucntions::KnifeThroughWalls);
			ImGui::SliderInt(x("Knife Distance"), &Config::MiscFucntions::KnifeDistance, 0, 750);
			ImGui::Unindent();
		}

		ImGui::Checkbox(x("Fire Mode"), &Config::MiscFucntions::FireMode);
		ImGui::Indent(); ImGui::Combo(x(""), &Config::MiscFucntions::FinalFireMode, Config::MiscFucntions::FireModes, IM_ARRAYSIZE(Config::MiscFucntions::FireModes));  ImGui::Unindent();

		ImGui::Checkbox(x("No Animation"), &Config::MiscFucntions::NoAnimation);
		ImGui::Checkbox(x("Fov Changer"), &Config::MiscFucntions::Fov);
		if (Config::MiscFucntions::Fov) {
			ImGui::Indent();
			ImGui::SliderInt(x("Player GOV"), &Config::MiscFucntions::PlayerFov, 0, 360);
			ImGui::SliderInt(x("Gun GOV"), &Config::MiscFucntions::ViewModleFov, 0, 360);
			ImGui::Unindent();
		}

	}
	if (SwitchTabs == 3) {
		ImGui::Checkbox(x("AntiAim Status"), &Config::AntiAim::Status);
		ImGui::Spacing();
		ImGui::Checkbox(x("Local Test"), &Config::AntiAim::LocalTest);
		ImGui::Spacing();
		ImGui::Checkbox(x("Spin"), &Config::AntiAim::Spin);

		ImGui::Spacing();

		if (Config::AntiAim::Spin) {
			ImGui::Indent();
			ImGui::Checkbox(x("Spin Yaw"), &Config::AntiAim::SpinYaw);
			if (Config::AntiAim::SpinYaw) { ImGui::Indent(); ImGui::Checkbox(x("Random Yaw Speed"), &Config::AntiAim::RandomYawSpeed); ImGui::Unindent(); }

			ImGui::Checkbox(x("Spin Pitch"), &Config::AntiAim::SpinPitch);
			if (Config::AntiAim::SpinPitch) { ImGui::Indent(); ImGui::Checkbox(x("Random Pitch Speed"), &Config::AntiAim::RandomPitchSpeed); ImGui::Unindent(); }

			ImGui::Checkbox(x("Spin Roll"), &Config::AntiAim::SpinRoll);
			if (Config::AntiAim::SpinRoll) { ImGui::Indent(); ImGui::Checkbox(x("Random Roll Speed"), &Config::AntiAim::RandomRollSpeed); ImGui::Unindent(); }

			ImGui::Spacing();
			ImGui::Unindent();
		}

		ImGui::Checkbox(x("Random Angle"), &Config::AntiAim::Rand);

		if (Config::AntiAim::Rand) {
			ImGui::Indent();  ImGui::Checkbox(x("Random Yaw"), &Config::AntiAim::RandYaw); ImGui::Unindent();

			if (Config::AntiAim::RandYaw) {
				ImGui::Indent();
				ImGui::SliderInt(x("Random Start Yaw"), &Config::AntiAim::MinRandYaw, -180, 180);
				ImGui::SliderInt(x("Random End Yaw"), &Config::AntiAim::MaxRandYaw, -180, 180);
				ImGui::Unindent();
			}

			ImGui::Indent(); ImGui::Checkbox(x("Random Pitch"), &Config::AntiAim::RandPitch); ImGui::Unindent();

			if (Config::AntiAim::RandPitch) {
				ImGui::Indent();
				ImGui::SliderInt(x("Random Start Pitch"), &Config::AntiAim::MinRandPitch, -180, 180);
				ImGui::SliderInt(x("Random End Pitch"), &Config::AntiAim::MaxRandPitch, -180, 180);
				ImGui::Unindent();
			}

			ImGui::Indent(); ImGui::Checkbox(x("Random Roll"), &Config::AntiAim::RandRoll); ImGui::Unindent();

			if (Config::AntiAim::RandRoll) {
				ImGui::Indent();
				ImGui::SliderInt(x("Random Start Roll"), &Config::AntiAim::MinRandRoll, -180, 180);
				ImGui::SliderInt(x("Random End Roll"), &Config::AntiAim::MaxRandRoll, -180, 180);
				ImGui::Unindent();
			}
		}

		ImGui::Spacing();

		ImGui::Checkbox(x("Manual"), &Config::AntiAim::ManualAim);

		if (Config::AntiAim::ManualAim) {
			int Size = 3 + Configs;

			ImGui::Indent();

			for (int i = 0; i < Configs; i++) {
				ImGui::Combo(("Default Manual Button " + std::to_string(i + 1)).c_str(), &Config::AntiAim::ManualAimButton[i], KeyNames, IM_ARRAYSIZE(KeyNames));
				ImGui::Combo(("Default Manual Config " + std::to_string(i + 1)).c_str(), &Config::AntiAim::ManualAimConfigs[i], Config::AntiAim::ManualConfigsNames[i], IM_ARRAYSIZE(Config::AntiAim::ManualConfigsNames));
			}

			ImGui::Separator();

			if (ImGui::Button(x("Add New Config"), ImVec2(180, 20))) {
				Configs++;
			}

			ImGui::Unindent();
		}

		ImGui::Spacing();

		ImGui::SliderInt(x("Yaw (Left/Right)"), &Config::AntiAim::Yaw, -180, 180);
		ImGui::SliderInt(x("Pitch (Up/Down)"), &Config::AntiAim::Pitch, -180, 180);
		ImGui::SliderInt(x("Roll (FerrisWheel)"), &Config::AntiAim::Roll, -180, 180);

	}

}

DWORD Menuthread(LPVOID in)
{
	while (1)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			ShowMenu = !ShowMenu;
		}
		Sleep(2);
	}
}

void Overlay::setup_keybinds()
{
	CreateThread(NULL, NULL, Menuthread, NULL, NULL, NULL); 
}

void Overlay::ChangeClickAbility(bool canclick, HWND passhwnd)
{
	long style = GetWindowLong(passhwnd, GWL_EXSTYLE);
	if (canclick)
	{
		style &= ~WS_EX_LAYERED;
		SetWindowLong(passhwnd, GWL_EXSTYLE, style);
		SetForegroundWindow(passhwnd);
	}
	else
	{
		style |= WS_EX_LAYERED;
		SetWindowLong(passhwnd, GWL_EXSTYLE, style);
	}
}

HRESULT Overlay::DirectXInit(HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	D3DPRESENT_PARAMETERS p_Params = { 0 };
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = hWnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
	{
		p_Object->Release();
		exit(4);
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;


	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	ImGuiStyle* style = &ImGui::GetStyle();

	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.65f, 0.00f, 0.00f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.98f, 0.26f, 0.33f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(1.00f, 0.00f, 0.00f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.06f, 0.12f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(1.00f, 0.00f, 0.00f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.00f, 0.00f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(1.00f, 0.00f, 0.00f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.00f, 0.00f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.00f, 0.00f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.00f, 0.00f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.00f, 0.13f, 0.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.58f, 0.18f, 0.18f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.98f, 0.26f, 0.45f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.68f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(p_Device);

	p_Object->Release();

	return S_OK;
}

void SetupWindow()
{
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, 0, 0, 0);

	WNDCLASSEX wClass =
	{
		sizeof(WNDCLASSEX),
		0,
		WinProc,
		0,
		0,
		nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		TEXT("R6Stats"),
		LoadIcon(nullptr, IDI_APPLICATION)
	};

	if (!RegisterClassEx(&wClass))
		exit(1);

	GameWnd = FindWindowW(TEXT(L"R6Game"), NULL);

	if (GameWnd)
	{
		GetClientRect(GameWnd, &GameRect);
		POINT xy;
		ClientToScreen(GameWnd, &xy);
		GameRect.left = xy.x;
		GameRect.top = xy.y;

		Width = GameRect.right;
		Height = GameRect.bottom;
	}
	else
		exit(2);

	MyWnd = CreateWindowExA(NULL, ("R6Stats"), ("R6Stats"), WS_POPUP | WS_VISIBLE, GameRect.left, GameRect.top, Width, Height, NULL, NULL, 0, NULL);

	DwmExtendFrameIntoClientArea(MyWnd, &Margin);
	SetWindowLong(MyWnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);

	ShowWindow(MyWnd, SW_SHOW);
	UpdateWindow(MyWnd);

}

bool initt = false;

void Overlay::Draw()
{

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if(ShowMenu)
		ELMenu();

	Render.DrawString(20.f, 10, 10, &Colors.red, false, true, "Diminsh.VIP - BETA 1.0.0");

	if (Config::ESPFunctions::bESP) {
		if (Game.GetGameState() == 2 || Game.GetGameState() == 3)
		{
			ESP(Config::ESPFunctions::bESP);
		}
	}

	ImGui::EndFrame();
	p_Device->SetRenderState(D3DRS_ZENABLE, false);
	p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	p_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	p_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (p_Device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		p_Device->EndScene();
	}
	HRESULT result = p_Device->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && p_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		p_Device->Reset(&p_Params);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

WPARAM Overlay::MainLoop()
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, MyWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();

		if (GetAsyncKeyState(VK_END) & 1)
			exit(8);

		if (hwnd_active == GameWnd)
		{
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(MyWnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(GameWnd, &rc);
		ClientToScreen(GameWnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameWnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(VK_LBUTTON))
		{
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{
			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			p_Params.BackBufferWidth = Width;
			p_Params.BackBufferHeight = Height;
			SetWindowPos(MyWnd, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			p_Device->Reset(&p_Params);
		}
		Draw();

		//Sleep(1);
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanuoD3D();
	DestroyWindow(MyWnd);

	return Message.wParam;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message)
	{
	case WM_DESTROY:
		CleanuoD3D();
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (p_Device != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_Params.BackBufferWidth = LOWORD(lParam);
			p_Params.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = p_Device->Reset(&p_Params);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void CleanuoD3D()
{
	if (p_Device != NULL)
	{
		p_Device->EndScene();
		p_Device->Release();
	}
	if (p_Object != NULL)
	{
		p_Object->Release();
	}
}

int Overlay::isTopwin()
{
	HWND hWnd = GetForegroundWindow();

	if (hWnd == GameWnd)
		return TopWindowGame;

	if (hWnd == MyWnd)
		return TopWindowMvoe;

	return 0;
}

void SetWindowToTarget()
{
	while (true)
	{
		if (GameWnd)
		{
			ZeroMemory(&GameRect, sizeof(GameRect));
			GetWindowRect(GameWnd, &GameRect);
			Width = GameRect.right - GameRect.left;
			Height = GameRect.bottom - GameRect.top;
			DWORD dwStyle = GetWindowLong(GameWnd, GWL_STYLE);

			if (dwStyle & WS_BORDER)
			{
				GameRect.top += 32;
				Height -= 39;
			}
			ScreenCenterX = Width / 2;
			ScreenCenterY = Height / 2;
			MoveWindow(MyWnd, GameRect.left, GameRect.top, Width, Height, true);
		}
	}
}

void Overlay::setup_overlay()
{
	printf("[>] Setting up window\n");

	SetupWindow();

	printf("[>] Initializing DX (overlay %d)\n", (int)MyWnd);

	this->DirectXInit(MyWnd);
	this->MainLoop();
}