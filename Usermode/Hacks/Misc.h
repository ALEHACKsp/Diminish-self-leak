#pragma once
#include <array>
#include "../Game/Game.h"
//Unlock All

void UnlockAll()
{
	//4C 8B 38 48 85 F6
	//Patternscan and then break and trace in cheat engine for R15 or dereference RAX
	//Then pattern scan for one of the above addresses
	//Dont read the last pointer but instead add it
	//Then adds 0xC to get to the owned bool
	//After every season update the loop number to the number of operators

	uint64_t contentManager = Read<uint64_t>(GameBase + 0x5DE5C70);
	contentManager = Read<uint64_t>(contentManager + 0x218);
	contentManager = Read<uint64_t>(contentManager);
	contentManager += 0x30C;

	for (int i = 0; i < 57; i++)
	{
		Write<uint32_t>(contentManager + ((uint64_t)i * 0x10), true);
	}
}

void InfiniteHealth(bool enable)
{
	if (enable)
	{
		uint64_t healthAddr = Game.DecryptEntityInfo(Game.GetLocalPlayer());
		healthAddr = Read<uint64_t>(healthAddr + 0x18);
		healthAddr = Read<uint64_t>(healthAddr + 0xD8);
		healthAddr = Read<uint64_t>(healthAddr + 0x8);
		Write<int>(healthAddr + 0x170, 42000);
	}
}

void InfiniteAmmo(bool enable)
{
	if (enable)
	{
		uint64_t ammoAddr = Game.DecryptCurrentWeapon(Game.GetLocalPlayer());

		uint32_t tempAmmo = 1337;
		tempAmmo -= 0x9DA3B829;
		tempAmmo = __ROR4__(tempAmmo, 0xF);
		tempAmmo -= 0x2E582D32;
		Write<uint32_t>(ammoAddr + 0xE0, tempAmmo); //Ammo

		uint32_t tempClip = 1337;
		tempClip ^= 0x12CC7B16;
		tempClip -= 0x414E942B;
		tempClip ^= 0x45;
		Write<uint32_t>(ammoAddr + 0xE8, tempClip); //Clip
	}
}

void RunAndGun(bool enable)
{
	//Go to R&S1 patch offset and pattern scan for the register address that is added with 0x178 and use that

	static bool getOrigRCX = true;
	static uint64_t origRCX = 0x0;
	static bool getOrigRCX40 = true;
	static uint64_t origRCX40 = 0x0;

	if (enable)
	{
		uint64_t rcx = Read<uint64_t>(GameBase + 0x68E5F80);
		rcx = Read<uint64_t>(rcx + 0x18);
		rcx = Read<uint64_t>(rcx);
		rcx = Read<uint64_t>(rcx + 0x30);
		rcx = Read<uint64_t>(rcx + 0x30);
		rcx = Read<uint64_t>(rcx + 0x50);
		rcx = Read<uint64_t>(rcx + 0x8);
		rcx = Read<uint64_t>(rcx + 0x148);
		uint64_t rcxW = Read<uint64_t>(rcx + 0x30);
		rcx = Read<uint64_t>(rcxW + 0x178);
		if (getOrigRCX)
		{
			origRCX = rcx;
			getOrigRCX = false;
		}
		if (getOrigRCX40)
		{
			origRCX40 = Read<uint64_t>(rcx + 0x40);
			getOrigRCX40 = false;
		}
		Write<uint64_t>(rcx + 0x40, 0x0);
		Write<uint8_t>(rcx + 0x80, 0x1);
		Write<uint64_t>(rcxW + 0x178, 0x0);
	}
	else
	{
		uint64_t rcx = Read<uint64_t>(GameBase + 0x68CEC90);
		rcx = Read<uint64_t>(rcx + 0x18);
		rcx = Read<uint64_t>(rcx);
		rcx = Read<uint64_t>(rcx + 0x30);
		rcx = Read<uint64_t>(rcx + 0x30);
		rcx = Read<uint64_t>(rcx + 0x50);
		rcx = Read<uint64_t>(rcx + 0x8);
		rcx = Read<uint64_t>(rcx + 0x148);
		rcx = Read<uint64_t>(rcx + 0x30);
		Write<uint64_t>(rcx + 0x178, origRCX);
		getOrigRCX = true;
		rcx = Read<uint64_t>(rcx + 0x178);
		Write<uint64_t>(rcx + 0x40, origRCX40);
		getOrigRCX40 = true;
	}
}

void Speed(bool enable, int speed)
{
	uint64_t speedAddr = Game.DecryptEventManager(Game.GetLocalPlayer());
	speedAddr = Read<uint64_t>(speedAddr + 0x30);
	speedAddr = Read<uint64_t>(speedAddr + 0x38);

	//if (Game.GetAnimation() == 1) //Ladder
	//{
	//	Write<int>(speedAddr + 0x58, enable ? 10000 : 105);
	//}
	//else if (Game.GetAnimation() == 2) //Rappel
	//{
	//	Write<int>(speedAddr + 0x58, enable ? 10000 : 105);
	//}
	//else
	//{
	Write<int>(speedAddr + 0x58, enable ? speed : 105);
	//} 
}

void FarKnife()
{
	uint64_t knife = Read<uint64_t>(GameBase + 0x5DAB958);
	knife = Read<uint64_t>(knife + 0x10);
	knife = Read<uint64_t>(knife + 0x90);
	knife = Read<uint64_t>(knife + 0x110);
	knife = Read<uint64_t>(knife + 0x100);
	knife = Read<uint64_t>(knife + 0x70);
	knife = Read<uint64_t>(knife + 0x58);

	Write<float>(knife + 0x130, 750.f);
	Write<float>(knife + 0x134, 750.f);
}

void ShortKnife()
{
	uint64_t knife = Read<uint64_t>(GameBase + 0x5DAB958);
	knife = Read<uint64_t>(knife + 0x10);
	knife = Read<uint64_t>(knife + 0x90);
	knife = Read<uint64_t>(knife + 0x110);
	knife = Read<uint64_t>(knife + 0x100);
	knife = Read<uint64_t>(knife + 0x70);
	knife = Read<uint64_t>(knife + 0x58);

	Write<float>(knife + 0x130, 1.3f);
	Write<float>(knife + 0x134, 1.3f);
}

void QuickKnife(bool enable)
{
	uint64_t knife = Read<uint64_t>(GameBase + 0x5DAB958);
	knife = Read<uint64_t>(knife + 0x10);
	knife = Read<uint64_t>(knife + 0x90);
	knife = Read<uint64_t>(knife + 0x110);
	knife = Read<uint64_t>(knife + 0x100);
	knife = Read<uint64_t>(knife + 0x70);
	knife = Read<uint64_t>(knife + 0x58);

	Write<float>(knife + 0x120, enable ? 0.f : 0.200f);
}

void Recoil(int val)
{
	//Write<uint8_t>(GameBase + OFFSET_PATCH_NO_RECOIL + 0x06, enable ? 0x1 : 0x0);

	uint64_t recoilAddr = Game.DecryptWeaponInfo(Game.GetLocalPlayer());
	//Write<float>(recoil + 0x50, 10.f);
	//Write<float>(recoil + 0x54, 10.f);

	Write<BYTE>(recoilAddr + 0x84, __ROR1__(0x56, 0x2) + 0x7F); //66 0F 6E C1 66 0F 6F D0
	//printf("0x%X\n", __ROL1__(Read<BYTE>(recoilAddr + 0x84) - 0x7F, 0x2));
	//Write<Vector2>(recoil + 0x50, Vector2(0.f, 0.f));
	Vector3 HoizWantedVals = { (float)(val / 100.f), (float)(val / 100.f), (float)(val / 100.f) };
	std::array<uint32_t, 3> HoizActualVals;
	HoizActualVals[0] = (__ROR4__(((*reinterpret_cast<uint32_t*>(&HoizWantedVals.x) ^ 0x2F7C5E82) - 0x3F0467D) ^ 0x8B9D4C05, 0x9) ^ 0x29B03E05) + 0x16;
	HoizActualVals[1] = (__ROR4__(*reinterpret_cast<uint32_t*>(&HoizWantedVals.y), 0x1A) + 0x71) ^ 0x5A2DF1AD;
	HoizActualVals[2] = (__ROR4__(*reinterpret_cast<uint32_t*>(&HoizWantedVals.z), 0xB) ^ 0xA24EAE99) + 0xE;
	Write<std::array<uint32_t, 3>>(recoilAddr + 0x108, HoizActualVals); //Horizontal

	Vector3 VertWantedVals = { 0.f, 0.f, 0.f };
	std::array<uint32_t, 3> VertActualVals;
	VertActualVals[0] = ((*reinterpret_cast<uint32_t*>(&VertWantedVals.x) ^ 0xBDED1BB0) - 0xB67B5FD7) ^ 0x65;
	VertActualVals[1] = *reinterpret_cast<uint32_t*>(&VertWantedVals.y);
	VertActualVals[2] = __ROR4__((*reinterpret_cast<uint32_t*>(&VertWantedVals.z) - 0xC99745C0) ^ 0x71, 0x15);
	Write<std::array<uint32_t, 3>>(recoilAddr + 0x114, VertActualVals); //vertical

	//Multiplier
	//uint64_t weapon = Read<uint64_t>(Game.GetLocalPlayer() + 0x90);
	//uint64_t curWeapon = Read<uint64_t>(weapon + 0x70);
	//uint64_t arrayToSingle = Read<uint64_t>(curWeapon + 0x258);
	//arrayToSingle = Read<uint64_t>(arrayToSingle);
	//uint64_t curWeaponPreset = Read<uint64_t>(arrayToSingle + 0x140);
	//curWeaponPreset = Read<uint64_t>(curWeaponPreset);
	//Write<float>(curWeaponPreset + 0x70, enable ? 0.f : -0.2f); //Pull
	//Write<float>(curWeaponPreset + 0x74, enable ? 0.f : 0.325f); //Kick
}

void NoSpread(int val)
{
	uint64_t spread = Game.DecryptWeaponInfo(Game.GetLocalPlayer());
	float spreadVal = val / 100.f;
	uint32_t spreadHex = FloatToHex(spreadVal);
	spreadHex = __ROR4__(spreadHex, 0x12);
	spreadHex -= 0x1598F9D0;
	Write<uint32_t>(spread + 0x180, spreadHex);
}

void NoAnimations(bool enable)
{
	if (enable)
	{
		Write<bool>(Game.DecryptCurrentWeapon(Game.GetLocalPlayer()) + 0x394, false);
	}
	else
	{
		Write<bool>(Game.DecryptCurrentWeapon(Game.GetLocalPlayer()) + 0x394, true);
	}
}

void FullAuto(int val)
{
	uint64_t fireType = Game.DecryptCurrentWeapon(Game.GetLocalPlayer());
	uint32_t temp = val;
	temp -= 0x4D02F7BD;
	temp = __ROR4__(temp, 0x6);
	temp ^= 0x7B1897FF;
	Write<uint32_t>(fireType + 0xD0, temp);
}

void ShootThroughWalls(bool enable)
{
	if (enable)
	{
		//Ash 0x04
		uint64_t caliber = Read<uint64_t>(GameBase + 0x5DD9F98);
		caliber = Read<uint64_t>(caliber + 0x918);
		caliber = Read<uint64_t>(caliber + 0xF00);
		caliber = Read<uint64_t>(caliber + 0x140);
		caliber = Read<uint64_t>(caliber + 0x90);
		caliber = Read<uint64_t>(caliber + 0x118);
		Write<BYTE>(caliber + 0x5C, 0x04);
	}
	else
	{

	}
}

void Fov(bool enable, float playerFov, float weaponFov)
{
	static bool doOnce = false;

	if (enable)
	{
		uint64_t fov = Read<uint64_t>(GameBase + OFFSET_FOV_MANAGER);
		fov = Read<uint64_t>(fov + 0xB8);
		fov = __ROL8__(fov, 0x37);
		fov ^= 0xD2DAAE4B0D8E73DB;
		fov = __ROL8__(fov, 0x35);
		fov = Read<uint64_t>(fov);
		uint32_t playerFovHex = FloatToHex(playerFov);
		playerFovHex -= 0x6A447659;
		playerFovHex = __ROR4__(playerFovHex, 0xE);
		playerFovHex ^= 0xD0B79D2A;
		uint32_t weaponFovHex = FloatToHex(weaponFov);
		weaponFovHex -= 0x3A63B7C9;
		weaponFovHex = __ROR4__(weaponFovHex, 0x10);
		weaponFovHex += 0x41;
		Write<uint32_t>(fov + 0x60, playerFovHex);
		Write<uint32_t>(fov + 0x38, weaponFovHex);
		doOnce = true;
	}
	else
	{
		if (doOnce)
		{
			float disablePlayerFov = 1.047f;
			float disableWeaponFov = 0.873f;
			uint64_t fov = Read<uint64_t>(GameBase + OFFSET_FOV_MANAGER);
			fov = Read<uint64_t>(fov + 0xB8);
			fov = __ROL8__(fov, 0x37);
			fov ^= 0xD2DAAE4B0D8E73DB;
			fov = __ROL8__(fov, 0x35);
			fov = Read<uint64_t>(fov);
			uint32_t playerFovHex = FloatToHex(disablePlayerFov);
			playerFovHex -= 0x6A447659;
			playerFovHex = __ROR4__(playerFovHex, 0xE);
			playerFovHex ^= 0xD0B79D2A;
			uint32_t weaponFovHex = FloatToHex(disableWeaponFov);
			weaponFovHex -= 0x3A63B7C9;
			weaponFovHex = __ROR4__(weaponFovHex, 0x10);
			weaponFovHex += 0x41;
			Write<uint32_t>(fov + 0x60, playerFovHex);
			Write<uint32_t>(fov + 0x38, weaponFovHex);
			doOnce = false;
		}
	}
}

void FreezeLobby(bool enable)
{
	static bool freeze = false;
	static bool freezeOnce = true;
	static bool disableOnce = false;
	//GameBase + 0x5DFB0A0] + 0x6E 48 8B 05 ? ? ? ? 80 40 6E FF
	if (enable)
	{
		if (GetAsyncKeyState(0x4B) & 1)
		{
			freeze = !freeze;
			printf("Freeze -> %s\n", freeze ? "TRUE" : "FALSE");
		}

		if (freeze)
		{
			if (freezeOnce)
			{
				uint64_t freezeAddr = Read<uint64_t>(GameBase + 0x5DFB0A0);
				Write<bool>(freezeAddr + 0x65 /*6E*/, true);
				freezeOnce = false;
				disableOnce = true;
			}
		}
		else
		{
			if (disableOnce)
			{
				uint64_t freezeAddr = Read<uint64_t>(GameBase + 0x5DFB0A0);
				Write<bool>(freezeAddr + 0x65, false);
				freezeOnce = true;
				disableOnce = false;
			}
		}
	}
}

void DroneGravity(bool enable, float gravity)
{
	static bool doOnce = false;
	if (enable)
	{
		uint64_t drone = Read<uint64_t>(GameBase + 0x6ED27B0);
		drone = Read<uint64_t>(drone);
		drone = Read<uint64_t>(drone + 0x110);
		drone = Read<uint64_t>(drone + 0x100);
		drone = Read<uint64_t>(drone + 0x70);
		drone = Read<uint64_t>(drone + 0x60);
		Write<float>(drone + 0xA8, gravity);
		doOnce = true;
	}
	else
	{
		if (doOnce)
		{
			uint64_t drone = Read<uint64_t>(GameBase + 0x6ED27B0);
			drone = Read<uint64_t>(drone);
			drone = Read<uint64_t>(drone + 0x110);
			drone = Read<uint64_t>(drone + 0x100);
			drone = Read<uint64_t>(drone + 0x70);
			drone = Read<uint64_t>(drone + 0x60);
			Write<float>(drone + 0xA8, -9.800000191f);
			doOnce = false;
		}
	}
}

void TwitchJump(bool enable)
{
	static int orig;
	static bool doOnce = false;
	if (enable)
	{
		uint64_t drone = Read<uint64_t>(GameBase + 0x6ED27B0);
		drone = Read<uint64_t>(drone);
		drone = Read<uint64_t>(drone + 0x110);
		drone = Read<uint64_t>(drone + 0x100);
		drone = Read<uint64_t>(drone + 0x70);
		drone = Read<uint64_t>(drone + 0x60);
		drone = Read<uint64_t>(drone + 0xB90);
		orig = Read<int>(drone + 0x1480);
		Write<int>(drone + 0x1480, 2);
		doOnce = true;
	}
	else
	{
		if (doOnce)
		{
			uint64_t drone = Read<uint64_t>(GameBase + 0x6ED27B0);
			drone = Read<uint64_t>(drone);
			drone = Read<uint64_t>(drone + 0x110);
			drone = Read<uint64_t>(drone + 0x100);
			drone = Read<uint64_t>(drone + 0x70);
			drone = Read<uint64_t>(drone + 0x60);
			drone = Read<uint64_t>(drone + 0xB90);
			Write<int>(drone + 0x1480, orig);
			doOnce = false;
		}
	}
}

void DroneSpeed(bool enable, float speed)
{
	static bool doOnce = false;
	if (enable)
	{
		uint64_t drone = Read<uint64_t>(GameBase + 0x6ED27B0);
		drone = Read<uint64_t>(drone);
		drone = Read<uint64_t>(drone + 0x110);
		drone = Read<uint64_t>(drone + 0x100);
		drone = Read<uint64_t>(drone + 0x70);
		drone = Read<uint64_t>(drone + 0x60);
		drone = Read<uint64_t>(drone + 0xB90);
		Write<float>(drone + 0x14CC, speed);
		doOnce = true;
	}
	else
	{
		if (doOnce)
		{
			uint64_t drone = Read<uint64_t>(GameBase + 0x6ED27B0);
			drone = Read<uint64_t>(drone);
			drone = Read<uint64_t>(drone + 0x110);
			drone = Read<uint64_t>(drone + 0x100);
			drone = Read<uint64_t>(drone + 0x70);
			drone = Read<uint64_t>(drone + 0x60);
			drone = Read<uint64_t>(drone + 0xB90);
			Write<float>(drone + 0x14CC, 4.f);
			doOnce = false;
		}
	}
}

void DroneCooldown(bool enable, float cooldown)
{
	static bool doOnce = false;
	if (enable)
	{
		uint64_t drone = Read<uint64_t>(GameBase + 0x6ED27B0);
		drone = Read<uint64_t>(drone);
		drone = Read<uint64_t>(drone + 0x110);
		drone = Read<uint64_t>(drone + 0x100);
		drone = Read<uint64_t>(drone + 0x70);
		drone = Read<uint64_t>(drone + 0x60);
		drone = Read<uint64_t>(drone + 0xB90);
		Write<float>(drone + 0x150C, cooldown);
		doOnce = true;
	}
	else
	{
		if (doOnce)
		{
			uint64_t drone = Read<uint64_t>(GameBase + 0x6ED27B0);
			drone = Read<uint64_t>(drone);
			drone = Read<uint64_t>(drone + 0x110);
			drone = Read<uint64_t>(drone + 0x100);
			drone = Read<uint64_t>(drone + 0x70);
			drone = Read<uint64_t>(drone + 0x60);
			drone = Read<uint64_t>(drone + 0xB90);
			Write<float>(drone + 0x150C, 2.f);
			doOnce = false;
		}
	}
}

void DroneJump(bool enable, float jump)
{
	static bool doOnce = false;
	if (enable)
	{
		uint64_t drone = Read<uint64_t>(GameBase + 0x6ED27B0);
		drone = Read<uint64_t>(drone);
		drone = Read<uint64_t>(drone + 0x110);
		drone = Read<uint64_t>(drone + 0x100);
		drone = Read<uint64_t>(drone + 0x70);
		drone = Read<uint64_t>(drone + 0x60);
		drone = Read<uint64_t>(drone + 0xB90);
		Write<float>(drone + 0x1524, jump);
		doOnce = true;
	}
	else
	{
		if (doOnce)
		{
			uint64_t drone = Read<uint64_t>(GameBase + 0x6ED27B0);
			drone = Read<uint64_t>(drone);
			drone = Read<uint64_t>(drone + 0x110);
			drone = Read<uint64_t>(drone + 0x100);
			drone = Read<uint64_t>(drone + 0x70);
			drone = Read<uint64_t>(drone + 0x60);
			drone = Read<uint64_t>(drone + 0xB90);
			Write<float>(drone + 0x1524, 5.5f);
			doOnce = false;
		}
	}
}

void DroneMods(bool gravity, bool twitchJump, bool speed, bool cooldown, bool jump)
{
	DroneGravity(gravity, Config::MiscFucntions::DroneGravity);
	TwitchJump(twitchJump);
	DroneSpeed(speed, Config::MiscFucntions::DroneSpeed);
	//DroneCooldown(cooldown, Config::DroneCoolDownVal);
	DroneJump(jump, Config::MiscFucntions::DroneJumpHeight);
}