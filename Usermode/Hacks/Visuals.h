#pragma once
#include <array>
#include <thread>
#include <vector>
#include "../Game/Game.h"
#include "../Render/Renderer.h"

inline float rotation = 0.0f;

inline Vector3 RainbowESP()
{
	static uint32_t cnt = 0;
	float freq = 0.0005f;

	if (cnt++ >= (uint32_t)-1)
		cnt = 0;

	Vector3 ret = { std::sin(freq * cnt + 0) * 0.5f + 0.5f, std::sin(freq * cnt + 2) * 0.5f + 0.5f , std::sin(freq * cnt + 4) * 0.5f + 0.5f };
	return ret;
}

uint64_t GetClosestEntityToCrosshair2()
{
	float oDist = FLT_MAX;
	float nDist = 0;
	uint64_t cur = NULL;
	uint64_t target = NULL;
	Vector2 screenPos;
	Vector2 screenCenter = { (float)(GetSystemMetrics(SM_CXSCREEN) / 2), (float)(GetSystemMetrics(SM_CYSCREEN) / 2) };
	float fov = Config::AimFunctions::FovCheck ? Config::AimFunctions::FOV : 100000.f; //Here you can change the fov

	for (int i = 0; i < Game.GetEntityCount(); i++)
	{
		cur = Game.GetEntityByID(i);

		if (!Game.IsEnemy(cur)) continue;

		if (Game.WorldToScreen(Game.GetEntityHead(cur), screenPos))
		{
			nDist = DistanceVec2(screenCenter, screenPos);

			if (nDist < fov && nDist < oDist)
			{
				oDist = nDist;
				target = cur;
			}
		}
	}
	return target;
}

inline void SpottedEsp(bool enable)
{
	Write<uint8_t>(GameBase + OFFSET_PATCH_SPOTTED_ESP + 0x6, enable ? 0x7 : 0x0);
}

inline void SpoofSpectate(bool enable)
{
	uint64_t spoof = Read<uint64_t>(GameBase + OFFSET_SPOOF_SPECTATE);
	Write<BYTE>(spoof + 0x5D, enable ? 0x1 : 0x0);
}

inline float BOG_TO_GRD(float BOG)
{
	return (float)(180 / M_PI) * BOG;
}

inline float GRD_TO_BOG(float GRD)
{
	return (float)(M_PI / 180) * GRD;
}

void Outline(bool Stuats, int Thickness) {
	WriteReadOnly<uintptr_t>(GameBase + 0x312345F, Stuats ? 0x75 : 0x74 , PAGE_EXECUTE_READWRITE);

	Write<float>(Read<uintptr_t>(GameBase + 0x5DC3A80) + 0x4, Thickness);
}

inline void Crosshair(int size, RGBA* color)
{

	POINT Screen;
	Screen.x = GetSystemMetrics(SM_CXSCREEN); Screen.y = GetSystemMetrics(SM_CYSCREEN);
	//Middle POINT
	POINT Middle; Middle.x = (int)(Screen.x / 2); Middle.y = (int)(Screen.y / 2);
	float gamma = atanf(1);

	int rot = (int)rotation;
	int i = 0;

	while (i < 4)
	{
		std::vector <int> p;
		p.push_back(size * sin(GRD_TO_BOG(rot + (i * 90))));									
		p.push_back(size * cos(GRD_TO_BOG(rot + (i * 90))));									
		p.push_back((size / cos(gamma)) * sin(GRD_TO_BOG(rot + (i * 90) + BOG_TO_GRD(gamma))));	
		p.push_back((size / cos(gamma)) * cos(GRD_TO_BOG(rot + (i * 90) + BOG_TO_GRD(gamma))));

		Render.DrawLine(Middle.x, Middle.y, Middle.x + p[0], Middle.y - p[1], color, 1.f);
		Render.DrawLine(Middle.x + p[0], Middle.y - p[1], Middle.x + p[2], Middle.y - p[3], color, 1.f);

		i++;
	}
}

inline void CrosshairCordinates()
{
	while (true)
	{
		rotation++;
		if ((int)rotation > 89) { rotation = (float)0; }
		rotation++;
		std::this_thread::sleep_for(std::chrono::nanoseconds(10000000));
	}
}

int screenx = GetSystemMetrics(SM_CXSCREEN);
int screeny = GetSystemMetrics(SM_CYSCREEN);

inline void ESP(bool enable)
{
	const int Width = GetSystemMetrics(SM_CXSCREEN);
	const int Height = GetSystemMetrics(SM_CYSCREEN);

	if (enable && (Game.GetGameState() == 2 || Game.GetGameState() == 3))
	{
		Vector3 rainbowVec = RainbowESP();
		RGBA BoxColor = { Config::ESPFunctions::BoxColor[0] * 255 ,Config::ESPFunctions::BoxColor[1] * 255 , Config::ESPFunctions::BoxColor[2] * 255 , Config::ESPFunctions::BoxColor[3] * 255 };

		if(Config::AimFunctions::ShowFov) Render.DrawCircle(Width / 2, Height / 2, (int)Config::AimFunctions::FOV, &Colors.lightblue, 50);

		//Crosshair
		if (Config::ESPFunctions::bCrosshair) {
			RGBA BoxColor = { Config::ESPFunctions::CrossHairColor[0] * 255 ,Config::ESPFunctions::CrossHairColor[1] * 255 , Config::ESPFunctions::CrossHairColor[2] * 255 , Config::ESPFunctions::CrossHairColor[3] * 255 };
			Crosshair(20, &BoxColor);
		}

		for (int i = 0; i < Game.GetEntityCount(); i++)
		{
			uint64_t cur = Game.GetEntityByID(i);
			if (!Game.IsEnemy(cur)) continue;

			//Snaplines
			Vector3 feetPos3D = Game.GetEntityPos(cur);
			Vector2 feetPos2D;

			Vector3 headPos3D = Game.GetEntityHead(cur);
			Vector2 headPos2D;

			Vector3 ribsPos3D = Game.GetEntityRibs(cur);
			Vector2 ribsPos2D;

			if (Game.WorldToScreen(feetPos3D, feetPos2D))

			if (Game.WorldToScreen(ribsPos3D, ribsPos2D))

			RGBA colorgreen = { 0, 255, 0, 255 };
			RGBA colorpurp = { 75, 0, 130, 255 };
			RGBA colorblack = { 0, 0, 0, 255 };

			if (Game.WorldToScreen(headPos3D, headPos2D))
			{
				Render.DrawCircle((int)headPos2D.x, (int)headPos2D.y, 4, &colorblack, 15);
				Render.DrawCircle((int)headPos2D.x, (int)headPos2D.y, 3, &colorpurp, 15);
				Vector2 SnapFrom;
				Vector2 SnapTo;

				if (Config::ESPFunctions::SnapLines) {
					if (Config::ESPFunctions::SnapFromType == 0) { //Top Middle
						SnapFrom.x = screenx / 2;
						SnapFrom.y = 0;
					}
					if (Config::ESPFunctions::SnapFromType == 1) { //Bottom Middle
						SnapFrom.x = screenx / 2;
						SnapFrom.y = screeny;
					}
					if (Config::ESPFunctions::SnapFromType == 2) { //Crosshair
						SnapFrom.x = screenx / 2;
						SnapFrom.y = screeny / 2;
					}
					if (Config::ESPFunctions::SnapToType == 0) { //Head
						SnapTo.x = headPos2D.x;
						SnapTo.y = headPos2D.y;
					}
					if (Config::ESPFunctions::SnapToType == 1) { //Feet
						SnapTo.x = feetPos2D.x;
						SnapTo.y = feetPos2D.y;
					}
					if (Config::ESPFunctions::SnapToType == 2) { //Torso
						SnapTo.x = ribsPos2D.x;
						SnapTo.y = ribsPos2D.y;
					}
					RGBA RGBASnapLineColor = {
						Config::ESPFunctions::SnapLineColor[0] * 255,
						Config::ESPFunctions::SnapLineColor[1] * 255,
						Config::ESPFunctions::SnapLineColor[2] * 255,
						Config::ESPFunctions::SnapLineColor[3] * 255 };
					if (Config::ESPFunctions::OnTarget) {
						if (cur == GetClosestEntityToCrosshair2()) {
							if (Config::ESPFunctions::OnSilentKey) {
								if (GetAsyncKeyState(KeyCodes[Config::AimFunctions::pSilentKey]) != NULL) {
									Render.DrawLine(SnapFrom.x, SnapFrom.y, SnapTo.x, SnapTo.y, &RGBASnapLineColor, Config::ESPFunctions::SnapLineThickness);
									Render.DrawCircleFilled(SnapTo.x, SnapTo.y, 2, &BoxColor, 50);
								}
							}
							else {
								Render.DrawLine(SnapFrom.x, SnapFrom.y, SnapTo.x, SnapTo.y, &RGBASnapLineColor, Config::ESPFunctions::SnapLineThickness);
								Render.DrawCircleFilled(SnapTo.x, SnapTo.y, 2, &BoxColor, 50);
							}
						}
					}
					else {
						Render.DrawLine(SnapFrom.x, SnapFrom.y, SnapTo.x, SnapTo.y, &RGBASnapLineColor, Config::ESPFunctions::SnapLineThickness);
						Render.DrawCircleFilled(SnapTo.x, SnapTo.y, 2, &BoxColor, 50);
					}
				}
			}

			AfterSnap:
			if (Config::ESPFunctions::Box3D) {
				float box_size = 0.3f;
				
				Vector3 top_1 = Game.W2S({ headPos3D.x + box_size, headPos3D.y + box_size, headPos3D.z + box_size });
				Vector3 top_2 = Game.W2S({ headPos3D.x + box_size, headPos3D.y - box_size, headPos3D.z + box_size });
				Vector3 top_3 = Game.W2S({ headPos3D.x - box_size, headPos3D.y - box_size, headPos3D.z + box_size });
				Vector3 top_4 = Game.W2S({ headPos3D.x - box_size, headPos3D.y + box_size, headPos3D.z + box_size });

				Vector3 bottom_1 = Game.W2S({ feetPos3D.x + box_size, feetPos3D.y + box_size, feetPos3D.z });
				Vector3 bottom_2 = Game.W2S({ feetPos3D.x + box_size, feetPos3D.y - box_size, feetPos3D.z });
				Vector3 bottom_3 = Game.W2S({ feetPos3D.x - box_size, feetPos3D.y - box_size, feetPos3D.z });
				Vector3 bottom_4 = Game.W2S({ feetPos3D.x - box_size, feetPos3D.y + box_size, feetPos3D.z });

				// Top lines
				if (top_1.z > 0.0f && top_2.z > 0.0f)
				{
					Render.DrawLine(top_1.x, top_1.y, top_2.x, top_2.y, &BoxColor, 1.0f);
				}

				if (top_2.z > 0.0f && top_3.z > 0.0f)
				{
					Render.DrawLine(top_2.x, top_2.y, top_3.x, top_3.y, &BoxColor, 1.0f);
				}

				if (top_3.z > 0.0f && top_4.z > 0.0f)
				{
					Render.DrawLine(top_3.x, top_3.y, top_4.x, top_4.y, &BoxColor, 1.0f);
				}

				if (top_4.z > 0.0f && top_1.z > 0.0f)
				{
					Render.DrawLine(top_4.x, top_4.y, top_1.x, top_1.y, &BoxColor, 1.0f);
				}

				// Bottom lines
				if (bottom_1.z > 0.0f && bottom_2.z > 0.0f)
				{
					Render.DrawLine(bottom_1.x, bottom_1.y, bottom_2.x, bottom_2.y, &BoxColor, 1.0f);
				}

				if (bottom_2.z > 0.0f && bottom_3.z > 0.0f)
				{
					Render.DrawLine(bottom_2.x, bottom_2.y, bottom_3.x, bottom_3.y, &BoxColor, 1.0f);
				}

				if (bottom_3.z > 0.0f && bottom_4.z > 0.0f)
				{
					Render.DrawLine(bottom_3.x, bottom_3.y, bottom_4.x, bottom_4.y, &BoxColor, 1.0f);
				}

				if (bottom_4.z > 0.0f && bottom_1.z > 0.0f)
				{
					Render.DrawLine(bottom_4.x, bottom_4.y, bottom_1.x, bottom_1.y, &BoxColor, 1.0f);
				}

				// Vertical lines
				if (bottom_1.z > 0.0f && top_1.z > 0.0f)
				{
					Render.DrawLine(bottom_1.x, bottom_1.y, top_1.x, top_1.y, &BoxColor, 1.0f);
				}

				if (bottom_2.z > 0.0f && top_2.z > 0.0f)
				{
					Render.DrawLine(bottom_2.x, bottom_2.y, top_2.x, top_2.y, &BoxColor, 1.0f);
				}

				if (bottom_3.z > 0.0f && top_3.z > 0.0f)
				{
					Render.DrawLine(bottom_3.x, bottom_3.y, top_3.x, top_3.y, &BoxColor, 1.0f);
				}

				if (bottom_4.z > 0.0f && top_4.z > 0.0f)
				{
					Render.DrawLine(bottom_4.x, bottom_4.y, top_4.x, top_4.y, &BoxColor, 1.0f);
				}
			}

			//Box Esp
			headPos3D.z += 0.2f;
			Vector2 boxTop2D;
			if (Game.WorldToScreen(headPos3D, boxTop2D))
			{
	
				Vector3 W2SEntityHead = Game.W2S(headPos3D);
				Vector3 W2SEntityFeet = Game.W2S(feetPos3D);

				float x, y, w, h;
				h = abs(W2SEntityHead.y - W2SEntityFeet.y + 2);
				w = h * .6f;
				x = W2SEntityHead.x - w / 2;
				y = W2SEntityHead.y - (h * .00066666666f);
				float health_height = ((h) * ((float)Game.GetEntityHealth(cur) / 100));

				Render.DrawRect(x, y, w, h, &colorblack, 3.f);
				Render.DrawRect(x, y, w, h, &colorpurp, 0.7f);

				if (Config::ESPFunctions::HealthBars) {
					Render.DrawFilledRect(x - 4, y, 2, h, &colorblack);
					Render.DrawFilledRect(x - 4, y + (h - health_height), 2, health_height, &Colors.green);
				}

			
			}
		}
	}
}

//inline void GadgetESP(bool enable)
//{
//	bool test = false; /*GetAsyncKeyState(VK_XBUTTON2);*/
//
//	for (int i = 0; i < gadgets.size(); i++)
//	{
//		Gadget gadget = gadgets[i];
//
//		//if (test)
//		//{
//		//	if (DistanceVec3(Game.GetViewTranslation(), gadget.position) <= 2.f)
//		//	{
//		//		Vector2 screenPos;
//		//		if (!Game.WorldToScreen(gadget.position, screenPos)) continue;
//
//		//		DrawString((int)screenPos.x, (int)screenPos.y, Colors::Yellow, true, false, "%d: %s S: 0x%lX, T1: 0x%lX, T2: 0x%lX", gadget.index, gadget.name, gadget.state, gadget.type1, gadget.type2);
//		//	}
//		//}
//		//else
//		//{
//		Vector3 pos = gadget.position;
//		Vector2 screenPos;
//		if (!Game.WorldToScreen(pos, screenPos)) continue;
//		DrawString((int)screenPos.x, (int)screenPos.y, Colors::Yellow, true, false, "%s", gadget.name);
//		//}
//	}
//
//	//for (int i = 9000; i > 5000 /*Game.GetGadgetCount()*/; i--)
//	//{
//	//	uint64_t gadget = Game.GetGadgetByID(i);
//	//	if (!gadget) continue;
//	//
//	//	uint64_t gadgetInfo = Read<uint64_t>(gadget + 0x60);
//	//	if (!gadgetInfo) continue;
//	//
//	//	Vector3 pos = Read<Vector3>(gadget + 0x30);
//	//	float min = Read<float>(gadget + 0x50);
//	//	float max = Read<float>(gadget + 0x40);
//	//
//	//	uint32_t state = Read<uint32_t>(gadget + 0xB0);
//	//	uint32_t type1 = Read<uint32_t>(gadget + 0xB4);
//	//	uint32_t type2 = Read<uint32_t>(gadgetInfo + 0x84);
//	//
//	//	if (test)
//	//	{
//	//		if (DistanceVec3(Game.GetViewTranslation(), pos) <= 5.f)
//	//		{
//	//			Vector2 screenPos;
//	//			if (!Game.WorldToScreen(pos, screenPos)) continue;
//	//
//	//			DrawString(screenPos.x, screenPos.y, Colors::Blue, true, false, "%d -> State: 0x%lX, Type1: 0x%lX, Type2: 0x%lX", i, state, type1, type2);
//	//		}
//	//	}
//	//	else
//	//	{
//	//		std::string name = Game.GetGadgetName(gadget);
//	//		if (name == "") continue;
//	//
//	//		Vector2 screenPos;
//	//		if (!Game.WorldToScreen(pos, screenPos)) continue;
//	//
//	//		DrawString(screenPos.x, screenPos.y, Colors::Yellow, true, false, "%s",name.c_str());
//	//	}
//	//}
//}

inline void Glow(bool enable, bool dull) //44 0F 58 C5 44 0F 58 C4 Then go up
{
	float d = 100.f;
	float s = 2.0f;
	float a = 4.0f; //Doesnt do much
	float o;

	if (dull)
	{
		o = 0.0001f; //Dull
	}
	else
	{
		o = 5.f; //Bright
	}

	if (enable && Game.GetGameState() == 3 && Game.GetEntityHealth(Game.GetLocalPlayer()) > 0)
	{
		Vector3 rainbowVec = RainbowESP();
		if (dull)
		{
			rainbowVec.x *= 2.55f; //Dull
			rainbowVec.y *= 2.55f;
			rainbowVec.z *= 2.55f;
		}
		else
		{
			rainbowVec.x *= 255.f; //Bright
			rainbowVec.y *= 255.f;
			rainbowVec.z *= 255.f;
		}

		uint64_t glow = Read<uint64_t>(GameBase + OFFSET_GLOW_MANAGER);
		glow = Read<uint64_t>(glow + 0xB8);
		std::array<uint32_t, 2> colorRG = { *(uint32_t*)(&rainbowVec.x), *(uint32_t*)(&rainbowVec.y) };
		*(uint64_t*)&colorRG[0] ^= 0xFB93A2A527F68810;
		*(uint64_t*)&colorRG[0] -= 0xFA0D705A37961A7F;
		*(uint64_t*)&colorRG[0] ^= 0x903839D4222A6336;

		Write<std::array<uint32_t, 2>>(glow + 0x90, colorRG);
		std::array<uint32_t, 2> colorBA = { *(uint32_t*)(&rainbowVec.z), 0x3F800000 };
		*(uint64_t*)&colorBA[0] ^= 0xFB93A2A527F68810;
		*(uint64_t*)&colorBA[0] -= 0xFA0D705A37961A7F;
		*(uint64_t*)&colorBA[0] ^= 0x903839D4222A6336;
		Write<std::array<uint32_t, 2>>(glow + 0x98, colorBA);
		//printf("0x%llX\n", ((Read<uint64_t>(glow + 0x90) ^ 0x903839D4222A6336) + 0xFA0D705A37961A7F) ^ 0xFB93A2A527F68810);

		//Distance
		//_mm_cvtsi32_si128(v51 ^ 0xB134F5F5)
		uint32_t distance = FloatToHex(d);
		distance ^= 0xB134F5F5;
		Write<uint32_t>(glow + 0x34, distance);

		//Start
		//_mm_cvtsi32_si128((((*(_DWORD *)(v25 + 0x48) >> 31) | (2 * *(_DWORD *)(v25 + 0x48))) ^ 0xFFC087DA) - 0x5FEC4F8F))
		uint32_t start = FloatToHex(s);
		start += 0x5FEC4F8F;
		start ^= 0xFFC087DA;
		uint32_t start1 = start << 31;
		uint32_t start2 = start / 2;
		start = start1 | start2;
		Write<uint32_t>(glow + 0x48, start);
		//printf("Start: 0x%lX\n", start);
		//printf("Start: %f\n", HexToFloat(((((Read<uint32_t>(glow + 0x48) >> 31) | (2 * Read<uint32_t>(glow + 0x48))) ^ 0xFFC087DA) - 0x5FEC4F8F)));

		//Alpha
		//_mm_cvtsi32_si128((((*(_DWORD *)(v25 + 0x80) >> 0x1F) + 2 * (*(_DWORD *)(v25 + 0x80)) - 0xA) >> 7) | (((*(_DWORD *)(v25 + 0x80) >> 0x1F) + 2 * (*(_DWORD *)(v25 + 0x80)) - 0xA) << 25))
		uint32_t alpha = FloatToHex(a);
		uint32_t alpha1 = alpha << 7;
		alpha1 += 0xA;
		alpha1 = ((alpha1 >> 0x1F) + (alpha1 / 2));
		uint32_t alpha2 = alpha >> 25;
		alpha2 += 0xA;
		alpha2 = ((alpha2 >> 0x1F) + (alpha2 / 2));
		alpha = alpha1 | alpha2;
		Write<uint32_t>(glow + 0x20, alpha);
		//printf("Alpha: 0x%lX\n", alpha);
		//printf("Alpha: %f\n", HexToFloat((((Read<uint32_t>(glow + 0x80) >> 0x1F) + 2 * (Read<uint32_t>(glow + 0x80)) - 0xA) >> 7) | (((Read<uint32_t>(glow + 0x80) >> 0x1F) + 2 * (Read<uint32_t>(glow + 0x80)) - 0xA) << 25)));

		//Opacity
		//_mm_cvtsi32_si128((((unsigned int)(*(_DWORD *)(v25 + 0x120) + 0x120840E7) >> 4) | ((*(_DWORD *)(v25 + 0x120) + 0x120840E7) << 28)) - 0x2D))))
		uint32_t opacity = FloatToHex(o);
		opacity += 0x2D;
		opacity = __ROL4__(opacity, 4);
		opacity -= 0x120840E7;
		Write<uint32_t>(glow + 0x120, opacity);
		//printf("Opacity: 0x%lX\n", opacity);
		//printf("Opacity: %f\n", HexToFloat((((Read<uint32_t>(glow + 0x120) + 0x120840E7) >> 4) | ((Read<uint32_t>(glow + 0x120) + 0x120840E7) << 28)) - 0x2D));
	}
}

//inline void CaveiraESP(bool enable)
//{
//	static bool doOnce = false;
//	if (enable)
//	{
//		for (int i = 0; i < Game.GetEntityCount(); i++)
//		{
//			auto entityObject = Game.GetEntityByID(i);
//			entityObject = Game.DecryptEntityInfo(entityObject);
//
//			auto entityInfo = Read<uint64_t>(entityObject + 0x18);
//			entityInfo = Read<uint64_t>(entityInfo + 0xD8);
//
//			for (uint32_t curCo = 0x80; curCo < 0xF0; curCo += 4)
//			{
//				auto marker = Read<uint64_t>(entityInfo + curCo);
//				if (marker == 0)
//					continue;
//
//				auto markerCheck = Read<uint64_t>(marker);
//				if (markerCheck != (GameGameBase + 0x52C71F0))
//					continue;
//
//
//				Write<uint8_t>(marker + 0x220, 0x85);
//			}
//		}
//		doOnce = true;
//	}
//	else
//	{
//		if (doOnce)
//		{
//			for (int i = 0; i < Game.GetEntityCount(); i++)
//			{
//				auto entityObject = Game.GetEntityByID(i);
//				entityObject = Game.DecryptEntityInfo(entityObject);
//
//				auto entityInfo = Read<uint64_t>(entityObject + 0x18);
//				entityInfo = Read<uint64_t>(entityInfo + 0xD8);
//
//				for (uint32_t curCo = 0x80; curCo < 0xF0; curCo += 4)
//				{
//					auto marker = Read<uint64_t>(entityInfo + curCo);
//					if (marker == 0)
//						continue;
//
//					auto markerCheck = Read<uint64_t>(marker);
//					if (markerCheck != (GameGameBase + 0x52C71F0))
//						continue;
//
//
//					Write<uint8_t>(marker + 0x220, 0x84);
//				}
//			}
//			doOnce = false;
//		}
//	}
//}