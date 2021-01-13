#pragma once
#include "..\Memory\Memory.h"
#include "Offsets.h"
#include "..\Misc\Vector.h"
#include "..\Misc\Defs.h"
#include <thread>

//Ignore for now we will make a better one
namespace Config
{
	namespace AntiAim
	{
		static bool Status = true, LocalTest;

		static int Yaw, Pitch, Roll;

		static bool Spin;
		static bool SpinYaw, SpinPitch, SpinRoll;
		static int YawSpeed, PitchSpeed, RollSpeed;
		static bool RandomYawSpeed, RandomPitchSpeed, RandomRollSpeed;
		static int RandomYawMin, RandomPitchMin, RandomRollMin, RandomYawMax, RandomPitchMax, RandomRollMax;

		static bool ManualAim;
		int ManualAimButton[4];
		int ManualAimConfigs[4];
		const char* ManualConfigsNames[4][4] = {
			{ { "North down" }, { "West down" }, { "South down" }, { "East down" } },
			{ { "North down" }, { "West down" }, { "South down" }, { "East down" } },
			{ { "North down" }, { "West down" }, { "South down" }, { "East down" } },
			{ { "North down" }, { "West down" }, { "South down" }, { "East down" } },
		};
		float ManualConfigs[4][4] = {
			{ -0.6087581,     0.0019610252, -0.002555633,  0.79334927  }, // north down
			{ -0.43209144,   -0.42881674,    0.55884933,   0.5631168   }, // west down
			{  0.0050501544, -0.60860455,    0.79343027,  -0.006584216 }, // south down
			{ -0.43401083,    0.42686567,   -0.55631423,   0.5656277   }, // east down
		};

		static bool Rand;
		static int MinRandYaw, MinRandPitch, MinRandRoll, MaxRandYaw, MaxRandPitch, MaxRandRoll;
		static bool RandYaw, RandRoll, RandPitch;
	}

	namespace AimFunctions {
		bool Aimbot, EntitySwap, pSilent = true, GunRotation, AlwaysSilent, GadgetAimbot;
		bool ShowFov, FovCheck;
		int AimBotKey, EntitySwapKey, pSilentKey = 5, GunRotationKey, GadgetKey;
		int Smoothing, FOV = 100;
	}

	namespace ESPFunctions {
		bool bESP = true, Outlines, Box2D, Box3D, CornerBox, SnapLines, Glow, Dull, HeadDot, bCrosshair, HealthBars, OnTarget, OnSilentKey;
		int SnapFromType, SnapToType, SnapLineThickness, OutlinesThickness;
		const char* SnapFrom[3] = { "Top Middle", "Bottom Middle", "Crosshair" };
		const char* SnapTo[3] = { "Head", "Feet", "Torso" };
		bool FovColorRainbow, BoxColorRainbow, SnapLineColorRainbow, OutlineColorRainbow, GlowColorRainbow;
		float FovColor[4]{ 1,1,1,1 }, BoxColor[4]{ 1,1,1,1 }, SnapLineColor[4]{ 1,1,1,1 }, OutlineColor[4]{ 1,1,1,1 }, GlowColor[4]{ 1,1,1,1 }, CrossHairColor[4]{ 1,1,1,1 };
		int RainbowSpeed = 500;
	}

	namespace MiscFucntions {
		bool Misc;

		bool DoneMods;
		int DroneJumpHeight, DroneGravity, DroneSpeed;

		bool Speed;
		int SpeedVal;

		bool GunMods;
		int Recoil, Spread;

		bool KnifeMods, KnifeThroughWalls;
		int KnifeDistance;

		const char* FireModes[4] = { "Full Auto", "3 Round Burst", "2 Round Burst", "Single Fire" };
		int FinalFireMode;
		bool FireMode, NoAnimation, Fov;
		int PlayerFov, ViewModleFov;
	}
}

int SwitchTabs = 0;
bool whatever;
int Configs = 1;
bool NewAngle = false;
char ConfigName[64];

float RainbowColor[3] = { 255, 0, 0 };
void rainbow(int speed) {

	if (RainbowColor[0] == 255 && RainbowColor[1] < 255 && RainbowColor[2] == 0) {
		RainbowColor[1]++;
	}

	else if (RainbowColor[0] <= 255 && RainbowColor[0] > 0 && RainbowColor[1] == 255 && RainbowColor[2] == 0) {
		RainbowColor[0]--;
	}

	else if (RainbowColor[0] == 0 && RainbowColor[1] == 255 && RainbowColor[2] >= 0 && RainbowColor[2] < 255) {
		RainbowColor[2]++;
	}
	else if (RainbowColor[0] == 0 && RainbowColor[1] <= 255 && RainbowColor[1] > 0 && RainbowColor[2] == 255) {
		RainbowColor[1]--;
	}
	else if (RainbowColor[0] >= 0 && RainbowColor[0] < 255 && RainbowColor[1] == 0 && RainbowColor[2] == 255) {
		RainbowColor[0]++;
	}

	else if (RainbowColor[0] == 255 && RainbowColor[1] == 0 && RainbowColor[2] > 0 && RainbowColor[2] <= 255) {
		RainbowColor[2]--;
	}

	std::this_thread::sleep_for(std::chrono::nanoseconds(speed));
}
const char* x(const char* text) {
	return text;
}

void RainbowThread() {
	while (true) {
		if (Config::ESPFunctions::FovColorRainbow || Config::ESPFunctions::BoxColorRainbow || Config::ESPFunctions::OutlineColorRainbow || Config::ESPFunctions::SnapLineColorRainbow || Config::ESPFunctions::GlowColorRainbow) {
			rainbow(Config::ESPFunctions::RainbowSpeed);
		}
	}
}
static const char* const KeyNames[] = {
	"NONE",
	"VK_LBUTTON",
	"VK_RBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"Unknown",
	"VK_BACK",
	"VK_TAB",
	"Unknown",
	"Unknown",
	"VK_CLEAR",
	"VK_RETURN",
	"Unknown",
	"Unknown",
	"VK_SHIFT",
	"VK_CONTROL",
	"VK_MENU",
	"VK_PAUSE",
	"VK_CAPITAL",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"VK_ESCAPE",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"VK_SPACE",
	"VK_PRIOR",
	"VK_NEXT",
	"VK_END",
	"VK_HOME",
	"VK_LEFT",
	"VK_UP",
	"VK_RIGHT",
	"VK_DOWN",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"VK_SNAPSHOT",
	"VK_INSERT",
	"VK_DELETE",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"VK_LWIN",
	"VK_RWIN",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"VK_NUMPAD0",
	"VK_NUMPAD1",
	"VK_NUMPAD2",
	"VK_NUMPAD3",
	"VK_NUMPAD4",
	"VK_NUMPAD5",
	"VK_NUMPAD6",
	"VK_NUMPAD7",
	"VK_NUMPAD8",
	"VK_NUMPAD9",
	"VK_MULTIPLY",
	"VK_ADD",
	"VK_SEPARATOR",
	"VK_SUBTRACT",
	"VK_DECIMAL",
	"VK_DIVIDE",
	"VK_F1",
	"VK_F2",
	"VK_F3",
	"VK_F4",
	"VK_F5",
	"VK_F6",
	"VK_F7",
	"VK_F8",
	"VK_F9",
	"VK_F10",
	"VK_F11",
	"VK_F12",
	"VK_F13",
	"VK_F14",
	"VK_F15",
	"VK_F16",
	"VK_F17",
	"VK_F18",
	"VK_F19",
	"VK_F20",
	"VK_F21",
	"VK_F22",
	"VK_F23",
	"VK_F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_NUMLOCK",
	"VK_SCROLL",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_LSHIFT",
	"VK_RSHIFT",
	"VK_LCONTROL",
	"VK_RCONTROL",
	"VK_LMENU",
	"VK_RMENU"
};
static const int KeyCodes[] = {
	0x0,  //Undefined
	0x01,
	0x02,
	0x03,
	0x04,
	0x05,
	0x06,
	0x07, //Undefined
	0x08,
	0x09,
	0x0A, //Reserved
	0x0B, //Reserved
	0x0C,
	0x0D,
	0x0E, //Undefined
	0x0F, //Undefined
	0x10,
	0x11,
	0x12,
	0x13,
	0x14,
	0x15,
	0x16, //IME On
	0x17,
	0x18,
	0x19,
	0x1A, //IME Off
	0x1B,
	0x1C,
	0x1D,
	0x1E,
	0x1F,
	0x20,
	0x21,
	0x22,
	0x23,
	0x24,
	0x25,
	0x26,
	0x27,
	0x28,
	0x29,
	0x2A,
	0x2B,
	0x2C,
	0x2D,
	0x2E,
	0x2F,
	0x30,
	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,
	0x37,
	0x38,
	0x39,
	0x3A, //Undefined
	0x3B, //Undefined
	0x3C, //Undefined
	0x3D, //Undefined
	0x3E, //Undefined
	0x3F, //Undefined
	0x40, //Undefined
	0x41,
	0x42,
	0x43,
	0x44,
	0x45,
	0x46,
	0x47,
	0x48,
	0x49,
	0x4A,
	0x4B,
	0x4C,
	0x4B,
	0x4E,
	0x4F,
	0x50,
	0x51,
	0x52,
	0x53,
	0x54,
	0x55,
	0x56,
	0x57,
	0x58,
	0x59,
	0x5A,
	0x5B,
	0x5C,
	0x5D,
	0x5E, //Rservered
	0x5F,
	0x60, //Numpad1
	0x61, //Numpad2
	0x62, //Numpad3
	0x63, //Numpad4
	0x64, //Numpad5
	0x65, //Numpad6
	0x66, //Numpad7
	0x67, //Numpad8
	0x68, //Numpad8
	0x69, //Numpad9
	0x6A,
	0x6B,
	0x6C,
	0x6D,
	0x6E,
	0x6F,
	0x70, //F1
	0x71, //F2
	0x72, //F3
	0x73, //F4
	0x74, //F5
	0x75, //F6
	0x76, //F7
	0x77, //F8
	0x78, //F9
	0x79, //F10
	0x7A, //F11
	0x7B, //F12
	0x7C, //F13
	0x7D, //F14
	0x7E, //F15
	0x7F, //F16
	0x80, //F17
	0x81, //F18
	0x82, //F19
	0x83, //F20
	0x84, //F21
	0x85, //F22
	0x86, //F23
	0x87, //F24
	0x88, //Unkown
	0x89, //Unkown
	0x8A, //Unkown
	0x8B, //Unkown
	0x8C, //Unkown
	0x8D, //Unkown
	0x8E, //Unkown
	0x8F, //Unkown
	0x90,
	0x91,
	0x92, //OEM Specific
	0x93, //OEM Specific
	0x94, //OEM Specific
	0x95, //OEM Specific
	0x96, //OEM Specific
	0x97, //Unkown
	0x98, //Unkown
	0x99, //Unkown
	0x9A, //Unkown
	0x9B, //Unkown
	0x9C, //Unkown
	0x9D, //Unkown
	0x9E, //Unkown 
	0x9F, //Unkown
	0xA0,
	0xA1,
	0xA2,
	0xA3,
	0xA4,
	0xA5
};
struct Gadget
{
	int index;
	uint64_t address;
	uint64_t gadgetInfo;
	float min;
	float max;
	uint32_t state;
	uint32_t type1;
	uint32_t type2;
	Vector3 position;
	std::string name;
};

inline std::vector<Gadget> gadgets;
inline Gadget closestGadget;

class CGame
{
public:

	uint64_t GetLocalPlayer() ////48 8B 18 48 85 DB 74 2F 48 8B 03 48 89 D9 FF Get profile manager xrefs and then go to 14th entry
	{
		uint64_t localPlayer = Read<uint64_t>(GameBase + OFFSET_PROFILE_MANAGER);
		localPlayer = Read<uint64_t>(localPlayer + 0x30);
		localPlayer -= 0x2AC930CD66630F20;
		localPlayer ^= 0xD7F3CA1161C2BFFF;
		localPlayer = __ROL8__(localPlayer, 0x13);
		localPlayer = Read<uint64_t>(localPlayer);
		localPlayer = Read<uint64_t>(localPlayer + 0x28);
		localPlayer = __ROL8__(localPlayer, 0x38);
		localPlayer += 0x5DBC6F3B71E8B1C0;

		return localPlayer;
	}

	uint64_t DecryptEventManager(uint64_t entity)
	{
		uint64_t eventManager = Read<uint64_t>(entity + 0x38);
		eventManager += 0x91865FBDB69B26C1;
		eventManager = __ROL8__(eventManager, 0x8);
		eventManager += 0x237289FB91642D12;

		return eventManager;
	}

	uint64_t DecryptEntityInfo(uint64_t entity)
	{
		uint64_t info = Read<uint64_t>(entity + 0x40);
		info = __ROL8__(info, 0x4);
		info ^= 0x64;
		info += 0xA82E9E31642CBD5B;

		return info;
	}

	uint64_t DecryptPlayerInfo(uint64_t entity)
	{
		uint64_t playerInfo = Read<uint64_t>(entity + 0x128);
		playerInfo -= 0x3;
		playerInfo ^= 0x61;
		playerInfo -= 0x17;

		return playerInfo;
	}

	uint32_t EntityTeam(uint64_t entity) //%08x-%04x-%04x-%04x-%04x%04x%04x and then go up
	{
		//Attack  3
		//Defense 4

		uint64_t teamInfo = DecryptPlayerInfo(entity);
		teamInfo = Read<uint64_t>(teamInfo + 0xE0);
		teamInfo ^= 0x7A;
		teamInfo -= 0x78BFCF37BE1C89;
		teamInfo = __ROL8__(teamInfo, 0x37);
		uint32_t team = Read<uint32_t>(teamInfo + 0x28);
		team -= 0x4D;
		return team;
	}

	uint64_t DecryptWeapon(uint64_t entity) //48 8B 8B ?? ?? ?? ?? 48 8B 01 FF 50 70 then go down
	{
		uint64_t weapon = Read<uint64_t>(entity + 0x98);
		if (!weapon) return 0;
		weapon -= 0x1470DF075B4FEE92;
		weapon = __ROL8__(weapon, 0x20);

		return weapon;
	}

	uint64_t DecryptCurrentWeapon(uint64_t entity) //48 8B 88 ? ? ? ? 48 85 C9 74 0B 48 8B 01 FF 50 48
	{
		uint64_t curWeapon = DecryptWeapon(entity);
		curWeapon = Read<uint64_t>(curWeapon + 0xB8); //120
		curWeapon ^= 0xAE0677FCE36DAC17;
		curWeapon += 0x75FC43CD703574AF;
		curWeapon = __ROL8__(curWeapon, 0xA);

		return curWeapon;
	}

	uint64_t DecryptWeaponInfo(uint64_t entity) //4C 89 F9 E8 ?? ?? ?? ?? 48 89 F9 48 89 C2 E8 ?? ?? ?? ?? 41 then go up
	{
		uint64_t weaponInfo = DecryptCurrentWeapon(entity);
		weaponInfo = Read<uint64_t>(weaponInfo + 0x298);
		weaponInfo -= 0x34DB1C1E215CC293;
		weaponInfo = __ROL8__(weaponInfo, 0x2B);
		weaponInfo -= 0x5CD44235F42B16C8;

		return weaponInfo;
	}

	uint32_t DecryptGameState()
	{
		uint64_t stateAddr = Read<uint64_t>(GameBase + OFFSET_ROUND_MANAGER);
		uint32_t state = Read<uint32_t>(stateAddr + 0xC0);
		state -= 0x56;
		state = __ROL4__(state, 0x1E);

		return state;
	}

	BYTE GetGameState()
	{
		uint32_t encryptState = DecryptGameState();
		BYTE state = 0;
		switch (encryptState)
		{
		case 0x3F1D7DAA: //Operator Select
		{
			state = 1;
			break;
		}
		case 0x3F1D7DA9: //Prep
		{
			state = 2;
			break;
		}
		case 0x3F1D7DA8: //Action
		{
			state = 3;
			break;
		}
		case 0x3F1D7DAB: //Loading
		{
			state = 4;
			break;
		}
		case 0x3F1D7DAE: //menu
		{
			state = 5;
			break;
		}
		}

		return state;
	}

	int GetAnimation()
	{
		uint64_t animations = Read<uint64_t>(GameBase + 0x5E06DC8); //48 8B 05 ? ? ? ? 48 8B 00 48 8B 00
		animations = Read<uint64_t>(animations + 0x0);
		animations = Read<uint64_t>(animations + 0x28);
		animations = Read<uint64_t>(animations + 0x0);
		animations = Read<uint64_t>(animations + 0xD8);
		animations = Read<uint64_t>(animations + 0x18);
		animations = Read<uint64_t>(animations + 0xC0);

		return Read<int>(animations + 0xB94);
	}

	uint64_t GetEntityList() //4D 8B 46 08 4C 89 F9 4C 89 FA E8 ? ? ? ? 49 8B 8D
	{
		uint64_t gameManager = Read<uint64_t>(GameBase + OFFSET_GAME_MANAGER);
		uint64_t entityList = Read<uint64_t>(gameManager + 0x58);
		entityList ^= 0xF9145217BE6CB56D;
		entityList += 0xFFFFFFFFFFFFFFAE;
		entityList ^= 0xB663AD449A6CBC4E;

		return entityList;
	}

	int GetEntityCount()
	{
		uint64_t gameManager = Read<uint64_t>(GameBase + OFFSET_GAME_MANAGER);
		uint64_t entityCount = Read<uint64_t>(gameManager + 0x60);
		entityCount ^= 0xF9145217BE6CB56D;
		entityCount += 0xFFFFFFFFFFFFFFAE;
		entityCount ^= 0xB663AD449A6CBC4E;
		int count = (int)(entityCount ^ 0x18C0000000);

		return count;
	}

	uint64_t GetGadgetList()
	{
		uint64_t gadgetList = Read<uint64_t>(GameBase + OFFSET_GADGETMANAGER);
		gadgetList = Read<uint64_t>(gadgetList + 0x40);

		return gadgetList;
	}

	int GetGadgetCount()
	{
		uint64_t gadgetCountAddr = Read<uint64_t>(GameBase + OFFSET_GADGETMANAGER);
		gadgetCountAddr = Read<uint64_t>(gadgetCountAddr + 0x48);
		int gadgetCount = (int)(gadgetCountAddr & 0x3FFFFFFF);

		return gadgetCount;
	}

	uint64_t GetGadgetByID(int id)
	{
		uint64_t gadgetList = GetGadgetList();
		if (!gadgetList) return 0;

		uint64_t gadget = Read<uint64_t>(gadgetList + (sizeof(PVOID) * id));
		if (!gadget) return 0;

		return gadget;
	}

	std::string GetGadgetName(uint64_t gadget)
	{
		/*
		Drone:
			State: 0x744000
			Type1: 0x10B0120
			Type2: 0xBE04CFBE

		Breach Charge:
			State: 0x744000
			Type1: 0x1070300
			Type2: 0xBE7C74FD

		Shield:
			State: 0x2740800
			Type1: 0x1068100
			Type2: 0xBD52C06C

		Barbed Wire:
			State: 0x744000
			Type1: 0x10B0100
			Type2: 0xBFDA1440

		Reinforcement:
			State: 0x1741000
			Type1: 0x1060100
			Type2: 0xBF734352

		Resupply:
			State: 0x740806
			Type1: 0x1068300
			Type2: 0xC0900270

		Ladder:
			State: 0x740807
			Type1: 0x1068700
			Type2: 0xBF800000

		C4:
			State: 0x744000
			Type1: 0x10F0120
			Type2: 0xBDA55196
		*/

		uint64_t gadgetInfo = Read<uint64_t>(gadget + 0x60);
		uint32_t state = Read<uint32_t>(gadget + 0xB0);
		uint32_t type1 = Read<uint32_t>(gadget + 0xB4);
		uint32_t type2 = Read<uint32_t>(gadgetInfo + 0x84);

		if (state == 0x744000 && type1 == 0x10b0120 && type2 == 0xBE04CFBE) return "Drone";
		else if (state == 0x1744000 && type1 == 0x1078100 && type2 == 0xBCDA3EB4) return "Flash";
		else if (type1 == 0x83C100 && type2 == 0xBD17BD90) return "Frag";
		else if (state == 0x3744000 && type1 == 0x1078100 && type2 == 0xBD0192A9) return "Smoke";
		else if (state == 0x741000 && type1 == 0x10E8320 && type2 == 0xBE1D9359) return "Camera";
		else if (state == 0x741000 && type1 == 0x10E8B20 && type2 == 0xBE1D9359) return "Camera";
		else if (type1 == 0x874320) return "Camera";
		else if (state == 0x744000 && type1 == 0x858120 && type2 == 0xBD75C28F) return "Valk Cam";
		else if (type1 == 0x858120 && type2 == 0xBE6147AE) return "Jammer";
		else if (state == 0x744000 && type1 == 0x10F0120 && type2 == 0xBDA55196) return "C4";
		else if (type1 == 0x834320 && type2 == 0xBDA55196) return "C4";
		else if (state == 0x744000 && type1 == 0x854300 && type2 == 0xBDE60584) return "Cluster Charge";
		else if (state == 0x744000 && type1 == 0x854300 && type2 == 0xBF2A1AB6) return "Welcome Mat";
		else if (state == 0x43744000 && type1 == 0x1170122 && type2 == 0xBDCCCCCD) return "Gu";
		else if (type1 == 0x898122 && type2 == 0xBE051EB8) return "Echo Drone";
		else if (state == 0x4744000 && type1 == 0x10B8320 && type2 == 0xC0243063) return "Entry Denial";
		else if (state == 0x744000 && type1 == 0x1070300 && type2 == 0xBE99B404) return "Thermite";
		else if (state == 0x744000 && type1 == 0x10B0120 && type2 == 0xBE03C799) return "Twitch Drone";
		else if (state == 0x744000 && type1 == 0x1078300 && type2 == 0xBE08380E) return "Claymore";
		//else if (state == 0x2744000 && type1 == 0x1078100 /*&& type2 == 0xBDA55196*/) return "Bomb"; //Broken
		else if (state == 0x740800 && type1 == 0x1068700 && type2 == 0xBF800000) return "Ladder"; //Broken
		else if (state == 0x740807 && type1 == 0x1068700 && type2 == 0xBF800000) return "Ladder"; //Broken
		else if (state == 0x744000 && type1 == 0x1070300 && type2 == 0xBE7C74FD) return "Breach Charge";
		else if (state == 0x744000 && type1 == 0x1078100 && type2 == 0xBF3FFFA4) return "Shield";
		else if (state == 0x2744000 && type1 == 0x10B8100 && type2 == 0xBE566E00) return "Resupply";
		else if (state == 0x2744000 && type1 == 0x10B8100 && type2 == 0xBE566D80) return "Resupply";
		//else if (state == 0x744000 && type1 == 0x10B0100 && type2 == 0xBFB06A98) return "Barbed Wire";
		//else if (state == 0x744000 && type1 == 0x10B0100 && type2 == 0xBF95E608) return "Barbed Wire";
		//else if (state == 0x744000 && type1 == 0x10B0100 && type2 == 0xBFE89140) return "Barbed Wire";
		//else if (state == 0x1741000 && type1 == 0x1060100 && type2 == 0xBF80080F) return "Reinforcement";

		return "";
	}

	uint64_t GetEntityByID(int id)
	{
		uint64_t entityList = GetEntityList();
		if (!entityList) return 0;

		uint64_t entity = Read<uint64_t>(entityList + (sizeof(PVOID) * id));
		if (!entity) return 0;

		return entity;
	}

	int GetEntityHealth(uint64_t entity)
	{
		uint64_t healthAddr = DecryptEntityInfo(entity);
		healthAddr = Read<uint64_t>(healthAddr + 0x18);
		healthAddr = Read<uint64_t>(healthAddr + 0xD8);
		healthAddr = Read<uint64_t>(healthAddr + 0x8);
		int health = Read<int>(healthAddr + 0x170);
		return health;
	}

	Vector3 GetEntityPos(uint64_t entity)
	{
		uint64_t posAddr = DecryptEntityInfo(entity);
		posAddr = Read<uint64_t>(posAddr + 0x18);
		Vector3 pos = Read<Vector3>(posAddr + 0x50);
		return pos;
	}

	/*
	#define OFFSET_BONE_HEAD 0x1090
	#define OFFSET_BONE_SPINE 0X10b0
	#define OFFSET_BONE_RIBS 0x10D0
	#define OFFSET_BONE_FOOT 0x10F0
	#define OFFSET_BONE_NECK 0x1100
	#define OFFSET_BONE_PELVIS 0X1120
	*/

	Vector3 GetEntityHead(uint64_t entity)
	{
		uint64_t skeleton = DecryptEntityInfo(entity);
		Vector3 head = Read<Vector3>(skeleton + 0x10C0);
		return head;
	}

	Vector3 GetEntityRibs(uint64_t entity)
	{
		uint64_t skeleton = DecryptEntityInfo(entity);
		Vector3 head = Read<Vector3>(skeleton + 0X10b0);
		return head;
	}

	bool IsEnemy(uint64_t entity)
	{
		if (entity == 0) return false;

		if (GetLocalPlayer() == entity) return false;

		//if (entity_team(get_localplayer()) == entity_team(entity)) return false; //Comment out if you want this to work for teamates/NPC's

		return true;
	}

	uint64_t GetCamera()
	{
		/*
		uint64_t localPlayer = Read<uint64_t>(GameBase + OFFSET_PROFILE_MANAGER);
		localPlayer = Read<uint64_t>(localPlayer + 0x18);
		if (!localPlayer) return 0;
		localPlayer = __ROL8__(localPlayer, 0x11);
		localPlayer ^= 0x37012B49B22968A;
		localPlayer = __ROL8__(localPlayer, 0x1C);
		if (!localPlayer) return 0;
		localPlayer = Read<uint64_t>(localPlayer);
		if (!localPlayer) return 0;
		*/
		uint64_t camera = Read<uint64_t>(GameBase + OFFSET_PROFILE_MANAGER);
		camera = Read<uint64_t>(camera + 0x30);
		camera -= 0x2AC930CD66630F20;
		camera ^= 0xD7F3CA1161C2BFFF;
		camera = __ROL8__(camera, 0x13);
		camera = Read<uint64_t>(camera);
		camera = Read<uint64_t>(camera + 0x268);
		camera = Read<uint64_t>(camera + 0x410);
		return camera;
	}

	Vector3 GetViewRight()
	{
		Vector3 right = Read<Vector3>(GetCamera() + 0x7A0);
		return right;
	}

	Vector3 GetViewUp()
	{
		Vector3 up = Read<Vector3>(GetCamera() + 0x7B0);
		return up;
	}

	Vector3 GetViewForward()
	{
		Vector3 forward = Read<Vector3>(GetCamera() + 0x7C0);
		return forward;
	}

	Vector3 GetViewTranslation()
	{
		Vector3 translation = Read<Vector3>(GetCamera() + 0x7D0);
		return translation;
	}

	float GetViewFovX()
	{
		float x = Read<float>(GetCamera() + 0x7E0);
		return x;
	}

	float GetViewFovY()
	{
		float y = Read<float>(GetCamera() + 0x7F4);
		return y;
	}
	Vector3 W2S(Vector3 position)
	{
		Vector3 temp = position - GetViewTranslation();

		float x = temp.Dot(GetViewRight());
		float y = temp.Dot(GetViewUp());
		float z = temp.Dot(GetViewForward() * -1);

		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		return Vector3((width / 2) * (1 + x / GetViewFovX() / z), (height / 2) * (1 - y / GetViewFovY() / z), z);
	}

	bool WorldToScreen(Vector3 position, Vector2& Screen)
	{
		Vector3 temp = position - GetViewTranslation();
		float x = temp.Dot(GetViewRight());
		float y = temp.Dot(GetViewUp());
		float z = temp.Dot(GetViewForward() * -1.f);

		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		Screen.x = (width / 2.f) * (1.f + x / GetViewFovX() / z);
		Screen.y = (height / 2.f) * (1.f - y / GetViewFovY() / z);

		return z >= 1.0f ? true : false;
	}
};

inline CGame Game;

//inline void GadgetCache()
//{
//	while (true)
//	{
//		bool shouldGetClosest = false; //GetAsyncKeyState(VK_XBUTTON2);
//		float oDist = FLT_MAX;
//		float nDist = 0;
//		Gadget target = {};
//		Vector2 screenPos;
//		Vector2 screenCenter = { (float)(GetSystemMetrics(SM_CXSCREEN) / 2), (float)(GetSystemMetrics(SM_CYSCREEN) / 2) };
//		float fov = 1000.f; //Here you can change the fov
//		bool isTesting = false;
//		//isTesting = GetAsyncKeyState(VK_XBUTTON2);
//		std::vector<Gadget> tempGadgets;
//		if (!Config::GadgetEsp) return;
//
//		for (int i = 0; i < Game.GetGadgetCount(); i++)
//		{
//			uint64_t gadgetAddr = Game.GetGadgetByID(i);
//			if (!gadgetAddr) continue;
//
//			Gadget gadget;
//			gadget.index = i;
//			gadget.address = gadgetAddr;
//
//			uint64_t gadgetInfo = Read<uint64_t>(gadgetAddr + 0x60);
//			if (!gadgetInfo) continue;
//			gadget.gadgetInfo = gadgetInfo;
//
//			gadget.position = Read<Vector3>(gadgetAddr + 0x30);
//			gadget.min = Read<float>(gadgetAddr + 0x50);
//			gadget.max = Read<float>(gadgetAddr + 0x40);
//
//			gadget.state = Read<uint32_t>(gadgetAddr + 0xB0);
//			gadget.type1 = Read<uint32_t>(gadgetAddr + 0xB4);
//			gadget.type2 = Read<uint32_t>(gadgetInfo + 0x84);
//			//if (gadget.type2 == 0xBE669500 || gadget.type2 == 0xB8800000 || gadget.type2 == 0xBE666B00 || gadget.type2 == 0xBFA66662) continue;
//			std::string name = Game.GetGadgetName(gadgetAddr);
//			if (name == "" && !isTesting) continue;
//			gadget.name = name;
//
//			if (Config::GadgetAimbot && shouldGetClosest)
//			{
//				if (Game.WorldToScreen(gadget.position, screenPos))
//				{
//					nDist = DistanceVec2(screenCenter, screenPos);
//
//					if (nDist < fov && nDist < oDist)
//					{
//						oDist = nDist;
//						target = gadget;
//					}
//				}
//			}
//
//			tempGadgets.push_back(gadget);
//		}
//
//		gadgets = tempGadgets;
//		if (Config::GadgetAimbot && shouldGetClosest) closestGadget = target;
//
//		Sleep(1);
//	}
//}