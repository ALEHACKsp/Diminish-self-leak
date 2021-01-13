#include <Windows.h>
#define NO_OP 0
#define CTU_SIZE 28
#define OP_SIZE 5

class EntityInformation {
public:

	std::uintptr_t GetList();
	std::uintptr_t GetEntityObject(int i);
	std::uintptr_t GetEntityObjectRaw(int i);
	std::uintptr_t GetNearestEntity();
	std::uintptr_t GetControllerEncrypted(std::uintptr_t Entity);

	std::uint8_t GetTeam(std::uintptr_t Entity);

	std::uint16_t EntityCount();

	class LocalPlayer {
	public:

		std::uintptr_t Get();
		std::uintptr_t GetWeaponClass();

		void Spread(float Spread);

	private:
		std::uintptr_t LocalPlayer;
		std::uintptr_t Weapon;
		std::uintptr_t InstanceOne;
		std::uintptr_t InstanceTwo;
		std::uintptr_t InstanceEncrypted;

	};

	static struct PlayerBones {
		std::uint16_t Head = 0x1090;
		std::uint16_t Spine = 0x10b0;
		std::uint16_t Ribs = 0x10d0;
		std::uint16_t Foot = 0x10f0;
		std::uint16_t Neck = 0x1100;
		std::uint16_t Pelvis = 0x1120;
	};

private:

	const char* operator_name[CTU_SIZE][OP_SIZE] = {
		{ "NPC", "Smoke", "Mute", "Sledge", "Thatcher" }, 
		{ "Recruit", "Castle", "Ash", "Pulse", "Thermite" },
		{ NO_OP, "Doc", "Rook", "Twitch", "Montagne" }, 
		{ NO_OP, "Glaz", "Fuze", "Kapkan", "Tachanka (Lord)" }, 
		{ NO_OP, "Blitz", "IQ", "Jager", "Bandit" },
		{ NO_OP, "Buck", "Frost" }, 
		{ NO_OP, "Blackbeard", "Valkrie" },
		{ NO_OP, "Capitao", "Caveria" }, 
		{ NO_OP, "Hibana", "Echo" },
		{ NO_OP, "Jackle", "Mira" },
		{ NO_OP, "Ying", "LESION" }, 
		{ NO_OP, "Ela", "Zofia" },
		{ NO_OP, "Dokkaebi", "Vigil" }, 
		{ NO_OP, NO_OP, "Lion", "Finka" },
		{ NO_OP, "Maestro", "Alibi" }, 
		{ NO_OP, "Maverick", "Clash" }, 
		{ NO_OP, "Nomad", "Kaid" }, 
		{ NO_OP, "Mozzie", "Gridlock" },
		{ NO_OP, "Nokk" }, { NO_OP, "Warden" }, 
		{ NO_OP, "Goyo" }, { NO_OP, "Amaru" }, 
		{ NO_OP, "Kali", "Wamai", "Ace" }, 
		{ NO_OP, "Oryx" }, { NO_OP, "Iana" }, 
		{ NO_OP, "Melusi" }, { "Zero" },
		{ "Aruni" } };

	std::uintptr_t ControllerEncrypted;
	std::uintptr_t EntityListEncrypted;
	std::uintptr_t EntityListDecrypted;
	std::uintptr_t EntityHolder;

	std::uintptr_t TeamClass;
	std::uintptr_t PlayerInfo;
	std::uintptr_t TeamIdEncrypted;
};

//EntityInformation* Entity = new EntityInformation;

EntityInformation* Entity = new EntityInformation;

EntityInformation::LocalPlayer* LocalEntity = new EntityInformation::LocalPlayer;







