//#include <xmmintrin.h>
//#include <Windows.h>
//#include <thread>
//#include <vector>
//#include "../Vectors/Vectors.h"
//#include "../Memory/Memory.h"
//#include "../Entity/Entity.h"
//#include "../Engine/Engine.h"
//#include "../Engine/ida.h"
//
//#using <system.drawing.dll>
//
//std::uintptr_t EntityInformation::GetList() {
//    EntityListEncrypted = Read<std::uintptr_t>(Managers->Game() + 0x150);
//    return ((EntityListEncrypted ^ 0x6cef13d90bedde9) + 0xcaa5e4b09d811db3) ^ 0xfdb06d03ecc0f0e5;
//}
//
//uint64_t EntityInformation::GetEntityObject(int i) {
//    EntityListDecrypted = Read<uint64_t>(GetList() + (i * 0x8));
//    EntityListEncrypted = Read<uint64_t>(EntityListDecrypted + 0x40);
//    EntityHolder += 0x5E2D834092A81BE3;
//    EntityHolder ^= 0x14378CB664BB53DB;
//    return __ROL8__(EntityHolder, 0x31);
//}
//
//uintptr_t EntityInformation::GetEntityObjectRaw(int i) {
//    return Read<uint64_t>(GetList() + (i * 0x8));
//}
//
//std::uintptr_t EntityInformation::GetControllerEncrypted(std::uintptr_t Entity) {
//    ControllerEncrypted = Read<std::uintptr_t>(Entity + 0x28) - 0x20;
//    return ControllerEncrypted;
//}
//
//std::uint8_t EntityInformation::GetTeam(std::uintptr_t Entity) {
//    uint64_t teamInfo = Read<uint64_t>(Entity + 0xB0);
//    teamInfo ^= 0xE6A296C831E11288;
//    teamInfo = Read<uint64_t>(teamInfo + 0x140);
//    teamInfo = __ROL8__(teamInfo, 0x22);
//    teamInfo += 0x9CBB12F3E5A18695;
//    teamInfo = __ROL8__(teamInfo, 0x2);
//    uint32_t team = Read<uint32_t>(teamInfo + 0x68);
//    team ^= 0x990B5DCC;
//    team = __ROL4__(team, 10);
//    return team - 0x63;
//}
//
//uint16_t EntityInformation::EntityCount() {
//    return static_cast<std::uint16_t>(((Read<uintptr_t>((Managers->Game() + 0x150) + 0x8) ^ 0x6CEF13D90BEDDE9) + 0xCAA5E4B09D811DB3) ^ 0xFDB06D03ECC0F0E);
//}
//
//std::uintptr_t EntityInformation::LocalPlayer::Get() {
//    LocalPlayer = Read<uint64_t>(Managers->Profile() + 0x18);
//    LocalPlayer = Read<uint64_t>(__ROL8__(__ROL8__(LocalPlayer, 0x11) ^ 0x37012B49B22968A, 0x1C));
//    return __ROL8__((Read<uint64_t>(LocalPlayer + 0x118) - 0x6F) ^ 0xD6EC8724059C1BCA, 0x2E);
//}
//
//uint64_t EntityInformation::GetNearestEntity() {
//    uint64_t bestentity = NULL;
//    float bestdelta = FLT_MAX;
//
//    for (int i = 0; i < this->EntityCount(); i++) {
//        Vector3 entityHead = Read<Vector3>(GetEntityObject(i) + 0x1090);
//        Vector3 w2shead = Engine->W2S(entityHead);
//
//        if (/*w2shead.z >= 1.f && */GetEntityObject(i) != Local->())
//        {
//            float delta = W2Sdistance(entityHead);
//
//            if (delta < bestdelta)
//            {
//                bestdelta = delta;
//                bestentity = entity_by_id(i, false);
//            }
//        }
//    }
//    return bestentity;
//}
//
//std::uintptr_t EntityInformation::LocalPlayer::GetWeaponClass() {
//    Weapon = Read<std::uintptr_t>(Get() + 0x1c30);
//    InstanceOne = Read<std::uintptr_t>(Weapon + 0xd0);
//    InstanceTwo = Read<uintptr_t>(InstanceOne + 0x0);
//    InstanceEncrypted = Read<uintptr_t>(InstanceTwo + 0x288);
//    return __ROL8__(InstanceEncrypted - 73, 0x28) ^ 0xF7165CCB6554F8A9; 
//}
//
//void EntityInformation::LocalPlayer::Spread(float Spread){
//    Spread = __ROR4__(*(uint32_t*)&Spread - 0x0DA9ADC35, 2) ^ 0X0CA3981FF;
//    Write<uint32_t>(GetWeaponClass() + 0x68, Spread);
//}
//
//
//
//
////
////int Health(std::uintptr_t entity)
////{
////    uintptr_t health_temp = Read<uint64_t>(entity + 0x18);
////    health_temp = Read<uint64_t>(health_temp + 0xd8);
////    health_temp = Read<uint64_t>(health_temp + 0x8);
////    return Read<int>(health_temp + 0x170);
////}
////uint8_t local_team()
////{
////    uint64_t
////        local = Read<uint64_t>(GameBase + 0x5E32C50);
////    local = Read<uint64_t>(local + 0x88);
////    local = Read<uint64_t>(local + 0x0);
////    local = Read<uint64_t>(local + 0x30) - 0x6F;
////
////    uintptr_t Team = Read<uintptr_t>(local + 0xD0);
////    Team = Read<uintptr_t>(Team + 0x98);
////
////    Team = _rotl64(Team, 0x26);
////    Team += 0x60A7E4A6C2B31A41;
////    Team = _rotl64(Team, 0x38);
////
////    return Read<uint8_t>(Team + 0x30);
////}
////uint8_t entity_team(uint64_t entity) {
////
////    uintptr_t Team = Read<uintptr_t>(entity + 0xD0);
////    Team = Read<uintptr_t>(Team + 0x98);
////
////    Team = _rotl64(Team, 0x26);
////    Team += 0x60A7E4A6C2B31A41;
////    Team = _rotl64(Team, 0x38);
////
////    return Read<uint8_t>(Team + 0x30);
////}
////auto entity_base::valid() -> bool
////{
////    Vector3 head = Engine::GetInstance()->W2S(Read<Vector3>(base() + 0x1070));
////    return (health() > 0 && head.z >= 0.1f && base() != local_player());
////}
////auto entity_base::get_team(unsigned long long ent) -> int
////{
////    uintptr_t team = Read<uintptr_t>(ent + 0xD0);
////    team = Read<uintptr_t>(team + 0x98);
////    team = _rotl64(team, 0x26);
////    team += 0x60A7E4A6C2B31A41;
////    team = _rotl64(team, 0x38);
////    return Read<uint8_t>(team + 0x30);
////}
////auto entity_base::bounds(float size) -> System::Drawing::RectangleF
////{
////    Vector3 head = Engine::GetInstance()->W2S(Read<Vector3>(base() + 0x1070));
////    Vector3 feet = Engine::GetInstance()->W2S(Read<Vector3>(base() + 0x1100));
////    float x, y, w, h;
////    h = abs(head.y - feet.y);
////    w = h * size;
////    x = head.x - w / 2;
////    y = head.y - (h * .16666666666f);
////    return System::Drawing::RectangleF(x, y, w, h + (h / 6));
////}
////auto entity_base::operator_name() -> const char*
////{
////    int ctu, op;
////    unsigned long long info =
////        Read<unsigned long long>(base() + 0xD0);
////    info = Read<uintptr_t>(info + 0x98);
////    info = _rotl64(info, 0x26);
////    info += 0x60A7E4A6C2B31A41;
////    info = _rotl64(info, 0x38);
////
////    ctu = Read<BYTE>(info + 0x1B8);
////    op = Read<BYTE>(info + 0x1B9);
////
////    return "negro"; //OperatorName[ctu][op];
////}
////uint64_t entity_by_id(int id, bool decrypt = true)
////{
////    uint64_t entityList = Read<uint64_t>(GameBase + 0x5E019A8);
////    entityList = Read<uint64_t>(entityList + 0xB0);
////    entityList += 0x0FFFFFFFFFFFFFFCA;
////    uint64_t entityList1 = entityList >> 0x11;
////    uint64_t entityList2 = entityList << 0x2F;
////    entityList = entityList1 | entityList2;
////    entityList += 0x0FFFFFFFFFFFFFFA8;
////    entityList = Read<uint64_t>(entityList + (8 * id));
////
////    if (decrypt == true)
////        return decrpyt_ent_info(entityList);
////    else
////        return entityList;
////
////}
////
////float rainbowcolor[3] = { 255, 0, 0 };
////void rainbow(int speed) {
////
////    if (rainbowcolor[0] == 255 && rainbowcolor[1] < 255 && rainbowcolor[2] == 0) {
////        rainbowcolor[1]++;
////    }
////
////    else if (rainbowcolor[0] <= 255 && rainbowcolor[0] > 0 && rainbowcolor[1] == 255 && rainbowcolor[2] == 0) {
////        rainbowcolor[0]--;
////    }
////
////    else if (rainbowcolor[0] == 0 && rainbowcolor[1] == 255 && rainbowcolor[2] >= 0 && rainbowcolor[2] < 255) {
////        rainbowcolor[2]++;
////    }
////    else if (rainbowcolor[0] == 0 && rainbowcolor[1] <= 255 && rainbowcolor[1] > 0 && rainbowcolor[2] == 255) {
////        rainbowcolor[1]--;
////    }
////    else if (rainbowcolor[0] >= 0 && rainbowcolor[0] < 255 && rainbowcolor[1] == 0 && rainbowcolor[2] == 255) {
////        rainbowcolor[0]++;
////    }
////
////    else if (rainbowcolor[0] == 255 && rainbowcolor[1] == 0 && rainbowcolor[2] > 0 && rainbowcolor[2] <= 255) {
////        rainbowcolor[2]--;
////    }
////
////    std::this_thread::sleep_for(std::chrono::milliseconds(speed));
////}
////float W2Sdistance(Vector3 position)
////{
////    Vector3 v = Engine::GetInstance()->W2S(position);
////    float Hypotenuse;
////    Hypotenuse = sqrt(pow(abs(v.x - (1920 / 2)), 2) + pow(abs(v.y - (1080 / 2)), 2));
////    return Hypotenuse;
////}
//
//
//
//void transform_calculation(__int64 pbones, __m128* resaultpos, __m128* boneinfo) {
//    __m128 v5; // xmm2
//    __m128 v6; // xmm3
//    __m128 v7; // xmm0
//    __m128 v8; // xmm4
//    __m128 v9; // xmm1
//
//    __m128 v10 = { 0.500f, 0.500f, 0.500f, 0.500f };
//    __m128 v11 = { 2.000f, 2.000f, 2.000f, 0.000f };
//
//    __m128 v12 = Read<__m128>(pbones);
//    __m128 v13 = Read<__m128>(pbones + 0x10);
//
//    v5 = v13;
//
//    v6 = _mm_mul_ps(*(__m128*)boneinfo, v5);
//    v6.m128_f32[0] = v6.m128_f32[0]
//        + (float)(_mm_cvtss_f32(_mm_shuffle_ps(v6, v6, 0x55)) + _mm_cvtss_f32(_mm_shuffle_ps(v6, v6, 0xAA)));
//    v7 = _mm_shuffle_ps(v13, v5, 0xFF);
//    v8 = _mm_sub_ps(
//        _mm_mul_ps(_mm_shuffle_ps(v5, v5, 0xD2), *(__m128*)boneinfo),
//        _mm_mul_ps(_mm_shuffle_ps(*(__m128*)boneinfo, *(__m128*)boneinfo, 0xD2), v5));
//    v9 = _mm_shuffle_ps(v12, v12, 0x93);
//    v9.m128_f32[0] = 0.0;
//    *(__m128*)resaultpos = _mm_add_ps(
//        _mm_shuffle_ps(v9, v9, 0x39),
//        _mm_mul_ps(
//            _mm_add_ps(
//                _mm_add_ps(
//                    _mm_mul_ps(_mm_shuffle_ps(v8, v8, 0xD2), v7),
//                    _mm_mul_ps(_mm_shuffle_ps(v6, v6, 0), v5)),
//                _mm_mul_ps(
//                    _mm_sub_ps(_mm_mul_ps(v7, v7), (__m128)v10),
//                    *(__m128*)boneinfo)),
//            (__m128)v11));
//}
//Vector3 transform_bone(DWORD_PTR entity, __int64 bone_id)
//{
//    __m128 output;
//    __int64 bonechain1 = Read<__int64>(entity + 0xC30);
//    __int64 bonechain2 = Read<__int64>(bonechain1);
//    __int64 bones = Read<__int64>(bonechain2 + 0x0);
//    __int64 bonesdata = Read<__int64>(bones + 0x260);
//
//    __m128 boneinfo = Read<__m128>((0x20 * bone_id) + bonesdata);
//
//    transform_calculation(bones, &output, &boneinfo);
//
//    return Vector3(output.m128_f32[0], output.m128_f32[1], output.m128_f32[2]);
//}