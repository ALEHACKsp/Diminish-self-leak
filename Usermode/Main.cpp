#include <Windows.h>
#include <thread>
#include <algorithm> 
#include <iostream>
#include <chrono> 
#include "Render/Overlay.h"
#include "Hacks/Aimbot.h"
#include "Hacks/Visuals.h"
#include "Hacks/Misc.h"
#include "../Common/Console/Console.h"

void RenderThread() 
{
	overlay.setup_keybinds();
	overlay.setup_overlay();
}
void FeatureThread() 
{
	bool status = false;

	while (true)
	{
		rainbow(Config::ESPFunctions::RainbowSpeed);
		if (Game.GetGameState() == 2 || Game.GetGameState() == 3) {
			Outline(true, 3);
			InfiniteHealth(true);
			Recoil(Config::MiscFucntions::Recoil);
			NoSpread(Config::MiscFucntions::Spread);
			NoAnimations(Config::MiscFucntions::NoAnimation);
			FullAuto(Config::MiscFucntions::FinalFireMode);
			//Glow(Config::ESPFunctions::Glow, Config::ESPFunctions::Dull);
		}
		std::this_thread::sleep_for(std::chrono::nanoseconds(100));
	}
}
void AimbotThread() {

	while (true) {
		static const uint8_t pSilentAngle[7] = {
			0x0F, 0x28,				//movaps xmm2
			0x97,					//xmm2
			0xE0, 0x00, 0x00, 0x00  //[rdi + 000000C0]
		};
		static const uint8_t DefaultAngle[7] = {
			0x0F, 0x28,				//movaps xmm2
			0x97,					//xmm2
			0xC0, 0x00, 0x00, 0x00  //[rdi + 000000C0]
		};

		if (Config::AimFunctions::pSilent) {
			for (int idx = 0; idx < sizeof(pSilentAngle); idx++) {
				WriteReadOnly(GameBase + 0x1613F08 + idx, 
					Game.GetGameState() == 3 &&
					(GetAsyncKeyState(KeyCodes[Config::AimFunctions::pSilentKey]) != NULL) ? pSilentAngle[idx] : DefaultAngle[idx], PAGE_EXECUTE_READWRITE);
			}
		}

		EntitySwap(Config::AimFunctions::EntitySwap && (GetAsyncKeyState(KeyCodes[Config::AimFunctions::EntitySwapKey]) != NULL));

		AimbotFunc(Config::AimFunctions::Aimbot && (GetAsyncKeyState(KeyCodes[Config::AimFunctions::AimBotKey]) != NULL),
			Config::AimFunctions::GadgetAimbot && (GetAsyncKeyState(KeyCodes[Config::AimFunctions::GadgetKey]) != NULL),
			Config::AimFunctions::GunRotation && (GetAsyncKeyState(KeyCodes[Config::AimFunctions::GunRotationKey]) != NULL),
			Config::AimFunctions::pSilent && (GetAsyncKeyState(KeyCodes[Config::AimFunctions::pSilentKey]) != NULL));
	}
	std::this_thread::sleep_for(std::chrono::nanoseconds(100));

}

Vector4 YPRToQuaternion(float Yaw, float Pitch, float Roll) {
	Vector4 Quaternion;
	double cy = cos(Yaw * 0.5);
	double sy = sin(Yaw * 0.5);
	double cr = cos(Pitch * 0.5);
	double sr = sin(Pitch * 0.5);
	double cp = cos(Roll * 0.5);
	double sp = sin(Roll * 0.5);

	Quaternion.w = (float)(cy * cr * cp + sy * sr * sp);
	Quaternion.x = (float)(cy * sr * cp - sy * cr * sp);
	Quaternion.y = (float)(cy * cr * sp + sy * sr * cp);
	Quaternion.z = (float)(sy * cr * cp - cy * sr * sp);
	return Quaternion;
}
Vector3 ToEuler(Vector4 Quaternion) {
	Vector3 Euler = Vector3();

	float sinr = (float)(+2.0 * (Quaternion.w * Quaternion.x + Quaternion.y * Quaternion.z));
	float cosr = (float)(+1.0 - 2.0 * (Quaternion.x * Quaternion.x + Quaternion.y * Quaternion.y));
	Euler.z = (float)atan2(sinr, cosr);

	double sinp = +2.0 * (Quaternion.w * Quaternion.y - Quaternion.z * Quaternion.x);
	if (abs(sinp) >= 1) {
		Euler.x = (float)copysign(3.14159265358979323846264338f / 2, sinp);
	}
	else {
		Euler.x = (float)asin(sinp);
	}

	double siny = +2.0 * (Quaternion.w * Quaternion.z + Quaternion.x * Quaternion.y);
	double cosy = +1.0 - 2.0 * (Quaternion.y * Quaternion.y + Quaternion.z * Quaternion.z);
	Euler.y = (float)atan2(siny, cosy);
	return Euler;
}
int Range(int Min, int Max) {
	return (rand() % (Max - Min + 1)) + Min;
}

#define SERVER_ANGLE 0x200
void AntiAimThread() {
	ULONGLONG ViewAngle = Read<ULONGLONG>(Game.DecryptPlayerInfo(Game.GetLocalPlayer()) + 0x12B8);

	if (Config::AntiAim::Spin) {
		if (Config::AntiAim::SpinYaw) {
			if (Config::AntiAim::RandomYawSpeed) {
				for (int idx = 0; idx < 180; idx = idx + Range(Config::AntiAim::MinRandYaw, Config::AntiAim::MaxRandYaw)) {
					Write<float>(ViewAngle + SERVER_ANGLE, idx);
				}
			}
			else {
				for (int idx = 0; idx < 3; idx++) {
					Write<float>(ViewAngle + SERVER_ANGLE, idx);
				}
			}
		}
		if (Config::AntiAim::SpinPitch) {
			if (Config::AntiAim::RandomPitchSpeed) {
				for (int idx = 0; idx < 180; idx = idx + Range(Config::AntiAim::MinRandPitch, Config::AntiAim::MaxRandPitch)) {
					Write<float>(ViewAngle + SERVER_ANGLE + 4, idx);
				}
			}
			else
			{
				for (int idx = 0; idx < 180; idx++) {
					Write<float>(ViewAngle + SERVER_ANGLE + 4, idx);
				}
			}
		}
		if (Config::AntiAim::SpinRoll) {
			if (Config::AntiAim::RandomRollSpeed) {
				for (int idx = 0; idx < 180; idx = idx + Range(Config::AntiAim::MinRandRoll, Config::AntiAim::MaxRandRoll)) {
					Write<float>(ViewAngle + SERVER_ANGLE + 4 + 4, idx);
				}
			}
			else
			{
				for (int idx = 0; idx < 180; idx++) {
					Write<float>(ViewAngle + SERVER_ANGLE + 4 + 4, idx);
				}
			}
		}
	}

	else if (Config::AntiAim::ManualAim) {
		for (int idx = 0; idx < 3; idx++) {
			if (GetAsyncKeyState(Config::AntiAim::ManualAimButton[idx])) {
				Write<Vector4>(ViewAngle + SERVER_ANGLE, Vector4(Config::AntiAim::ManualConfigs[idx][0], Config::AntiAim::ManualConfigs[idx][1],
					Config::AntiAim::ManualConfigs[idx][2], Config::AntiAim::ManualConfigs[idx][3]));
			}
		}
	}

	else if (Config::AntiAim::Rand) {
		if (Config::AntiAim::RandYaw) {
			Write<float>(ViewAngle + SERVER_ANGLE, Range(Config::AntiAim::MinRandYaw, Config::AntiAim::MaxRandYaw));
		}
		if (Config::AntiAim::RandPitch) {
			Write<float>(ViewAngle + SERVER_ANGLE + 4, Range(Config::AntiAim::MinRandPitch, Config::AntiAim::MaxRandPitch));
		}
		if (Config::AntiAim::RandRoll) {
			Write<float>(ViewAngle + SERVER_ANGLE + 4 + 4, Range(Config::AntiAim::MinRandRoll, Config::AntiAim::MaxRandRoll));
		}
	}

	else {
		Write<Vector4>(ViewAngle + SERVER_ANGLE, YPRToQuaternion(Config::AntiAim::Yaw, Config::AntiAim::Pitch, Config::AntiAim::Roll));
	}
}

int main() {

    SetPriorityClass(GetCurrentProcess(), 0x00000100);

	Console::PrintCategory("Diminsh.VIP");

	Console::PrintInfo(" Press Enter To Start Bypass \n");
    std::cin.get();

    NTSTATUS Status = Setup();
	const char* StringToPrint = (Status) ? "Status -> Good :)" : "Status -> Bad :(";
	!Status ? Console::PrintOk("{} \n", StringToPrint) : Console::PrintFail("{} \n", StringToPrint);
    if (!NT_SUCCESS(Status))
        Exit(Status);

	Console::PrintInfo("Press Enter to Start UserMode \n");
    std::cin.get();

	std::thread Aimbot( AimbotThread );
	std::thread Render( RenderThread );
	std::thread Crosshair( CrosshairCordinates );
	std::thread Main( FeatureThread ); 
	std::thread AntiAim( AntiAimThread );

	Aimbot.detach();
	Render.detach();
	Crosshair.detach();
	Main.detach();
	AntiAim.detach();

	while (!(GetAsyncKeyState(VK_END) & 1)) { std::this_thread::sleep_for(std::chrono::seconds(10)); };

}