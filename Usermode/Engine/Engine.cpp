//#include <Windows.h>
//#include "../Vectors/Vectors.h"
//#include "../Memory/Memory.h"
//#include "Engine.h"
//HRESULT EngineClass::ManagersClass::Init() {
//    GameManagerBase = (Read<uintptr_t>(GameBase + 0x5DC6828));
//    RoundManagerBase = (Read<uintptr_t>(GameBase + 0x7096828));
//    ProfileManagerBase = (Read<uintptr_t>(GameBase + 0x5DF5180));
//    NetworkManagerBase = (Read<uintptr_t>(GameBase + 0x5AC0540));
//    return true;
//}
//
//std::uintptr_t EngineClass::ManagersClass::Game() {
//    return GameManagerBase;
//}
//
//std::uintptr_t EngineClass::ManagersClass::Round() {
//    return RoundManagerBase;
//}
//
//std::uintptr_t EngineClass::ManagersClass::Profile() {
//    return ProfileManagerBase;
//}
//
//std::uintptr_t EngineClass::ManagersClass::Network() {
//    return NetworkManagerBase;
//}
//
//std::uintptr_t EngineClass::ManagersClass::Camera() {
//    CameraManager = Read<std::uintptr_t>(Managers->Profile() + 0x18);
//    CameraManager = Read<std::uintptr_t>(_rotl64(_rotl64(CameraManager, 0x11) ^ 0x37012B49B22968A, 0x1C));
//    CameraManager = Read<std::uintptr_t>(CameraManager + 0x260);
//    return Read<std::uintptr_t>(CameraManager + 0x410);
//}
//
//Vector3 EngineClass::ViewClass::Up() {
//    return Read<Vector3>(Managers->Camera() + 0x7B0);
//}
//
//Vector3 EngineClass::ViewClass::Right() {
//    return Read<Vector3>(Managers->Camera() + 0x7A0);
//}
//
//Vector3 EngineClass::ViewClass::Forward() {
//    return Read<Vector3>(Managers->Camera() + 0x7C0);
//}
//
//Vector3 EngineClass::ViewClass::Translation() {
//    return Read<Vector3>(Managers->Camera() + 0x7D0);
//}
//
//float EngineClass::FovClass::X() {
//    return Read<float>(Managers->Camera() + 0x7E0);
//}
//
//float EngineClass::FovClass::Y() {
//    return Read<float>(Managers->Camera() + 0x7F4);
//}
//
//Vector3 EngineClass::W2S(Vector3 position) {
//    Vector3 Position = position - View->Translation();
//    Vector3 MyVector = { Position.Dot(View->Right()), Position.Dot(View->Up()), Position.Dot(View->Forward() * -1) };
//    return Vector3((1920 / 2) * (1 + MyVector.x / Fov->X() / MyVector.z), (1080 / 2) * (1 - MyVector.y / Fov->Y() / MyVector.z), MyVector.z);
//}
//
//float EngineClass::W2SDistance(Vector3 position)
//{
//    Vector3 v = W2S(position);
//    float Hypotenuse;
//    Hypotenuse = sqrt(pow(abs(v.x - (1920 / 2)), 2) + pow(abs(v.y - (1080 / 2)), 2));
//    return Hypotenuse;
//}
//
//int EngineClass::RoundState() {
//    return Read<int>(Managers->Round() + 0x300);
//}
//
