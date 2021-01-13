#pragma once
#include "../Game/Game.h"

//Unlock All

void InfiniteHealth(bool enable)
{
	if (enable)
	{
		uint64_t healthAddr = Game.DecryptEntityInfo(Game.GetLocalPlayer());
		healthAddr = Read<uint64_t>(healthAddr + 0x18);
		healthAddr = Read<uint64_t>(healthAddr + 0xD8);
		healthAddr = Read<uint64_t>(healthAddr + 0x8);
		Write<int>(healthAddr + 0x170, 120);
	}
}

void InfiniteAmmo(bool enable)
{
	if (enable)
	{
		uint64_t ammo = Game.DecryptCurrentWeapon(Game.GetLocalPlayer());
		Write<int>(ammo + 0x124, 1337); //Ammo
		Write<int>(ammo + 0x128, 1337); //Clip
	}
}

//Speed

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

void NoRecoil(bool enable)
{
	//Write<uint8_t>(base + OFFSET_PATCH_NO_RECOIL + 0x06, enable ? 0x1 : 0x0);

	uint64_t recoil = Game.DecryptWeaponInfo(Game.GetLocalPlayer());
	Write<BYTE>(recoil + 0x12C, enable ? 0x68 : 0x69);
	Write<Vector3>(recoil + 0x1A0, enable ? Vector3(-0.07f, -0.07f, -0.07f) : Vector3(0.550f, 1.0f, 1.0f));
	Write<Vector3>(recoil + 0x1AC, enable ? Vector3(-0.07f, -0.07f, -0.07f) : Vector3(1.0f, 0.550f, 0.750f));
	//Write<bool>(recoil + 0x198, enable ? false : true); //Overwrite
	//Write<float>(recoil + 0x17C, enable ? 0.f : 0.625f); //Horizontal
	//Write<float>(recoil + 0x18C, enable ? 0.f : 0.700f); //Vertical

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

void NoSpread(bool enable)
{
	uint64_t spread = Game.DecryptWeaponInfo(Game.GetLocalPlayer());
	float spreadVal = enable ? 0.f : 1.f;
	uint32_t spreadHex = FloatToHex(spreadVal);
	spreadHex += 0xDA9ADC35;
	spreadHex = __ROR4__(spreadHex, 2);
	spreadHex ^= 0xCA3981FF;
	Write<uint32_t>(spread + 0x68, spreadHex);
}

void NoAnimations(bool enable)
{
	if (enable)
	{
		Write<bool>(Game.DecryptCurrentWeapon(Game.GetLocalPlayer()) + 0x384, false);
	}
	else
	{
		Write<bool>(Game.DecryptCurrentWeapon(Game.GetLocalPlayer()) + 0x384, true);
	}
}

void FullAuto(bool enable)
{
	uint64_t fireType = Game.DecryptCurrentWeapon(Game.GetLocalPlayer());
	Write<BYTE>(fireType + 0x120, 0x0);
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
			uint64_t drone = Read<uint64_t>(base + 0x6ED27B0);
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
	DroneGravity(gravity, Config::DroneGravityVal);
	TwitchJump(twitchJump);
	DroneSpeed(speed, Config::DroneSpeedVal);
	DroneCooldown(cooldown, Config::DroneCoolDownVal);
	DroneJump(jump, Config::DroneJumpVal);
}