#include <iostream>
#include "../Vectors/Vectors.h"

class EngineClass {
public:

	int RoundState();
	Vector3 W2S(Vector3 position);
	float W2SDistance(Vector3 position);

	class ManagersClass {
	public:
		HRESULT Init();

		std::uintptr_t Game();
		std::uintptr_t Round();
		std::uintptr_t Camera();
		std::uintptr_t Profile();
		std::uintptr_t Network();

	private:
		std::uintptr_t CameraManager;
		std::uintptr_t GameManagerBase;
		std::uintptr_t RoundManagerBase;
		std::uintptr_t ProfileManagerBase;
		std::uintptr_t NetworkManagerBase;
	};

	class ViewClass {
	public:
		Vector3 Up();
		Vector3 Right();
		Vector3 Forward();
		Vector3 Translation();
	};
	class FovClass {
	public:
		float X();
		float Y();
	};

private:

};


EngineClass* Engine = new EngineClass;

EngineClass::FovClass* Fov = new EngineClass::FovClass;

EngineClass::ViewClass* View = new EngineClass::ViewClass;

EngineClass::ManagersClass* Managers = new EngineClass::ManagersClass;
