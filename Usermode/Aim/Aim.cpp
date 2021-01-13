#include <windows.h>
#include <iostream>

#include "../Entity/Entity.h"
#include "../Vectors/Vectors.h"
#include "../Engine/Engine.h"
#include "../Memory/Memory.h"

#include "aim.h"


#define PI 3.14159265358979323846f
#define RAD2DEG (180.f / PI)
#define DEG2RAD (PI / 180.F)
//
//int32_t Health(uint64_t entity)
//{
//	uint64_t tmp = Read<uint64_t>(entity + 0x18);
//	tmp = Read<uint64_t>(tmp + 0xD8);
//	tmp = Read<uint64_t>(tmp + 0x8);
//
//	return Read<int>(tmp + 0x170);
//}
//
//Vector4 GetViewAngles() {
//	unsigned long long local = local_player();
//
//	if (health(local_player()) > 0) {
//		const auto view_angle = Read<uint64_t>(local + 0x1270);
//		return Read<Vector4>(view_angle + 0xC0);
//	}
//}
//
//void SetViewAngle(Vector4 vec) {
//	unsigned long long local = local_player();
//
//	if (health(local_player()) > 0) {
//		const auto view_angle = Read<uint64_t>(local + 0x1270);
//		write<Vector4>(view_angle + 0xC0, vec);
//	}
//
//}
//
//Vector3 VectorAngle(Vector3 forward) {
//	Vector3 angles;
//	float tmp, yaw, pitch;
//
//	if (forward.y == 0) {
//		yaw = 0;
//		if (forward.y > 0)
//			pitch = 270;
//		else
//			pitch = 90;
//	}
//
//	else {
//		yaw = (float)(atan2(forward.y, forward.x) * 180 / PI);
//		tmp = (float)sqrt(forward.x * forward.x + forward.y * forward.y);
//		pitch = (float)(atan2(forward.z * -1, tmp) * 180 / PI);
//	}
//
//	yaw += 90;
//
//	if (yaw > 180)
//		yaw -= 360;
//	
//
//	if (pitch > 180)
//		pitch -= 360;
//
//	angles.x = pitch;
//	angles.y = yaw;
//	angles.z = 0;
//
//	return angles;
//}
//
//Vector3 ToEuler(Vector4 q) {
//	Vector3 end = Vector3();
//
//	float sinr = (float)(+2.0 * (q.w * q.x + q.y * q.z));
//	float cosr = (float)(+1.0 - 2.0 * (q.x * q.x + q.y * q.y));
//	end.z = (float)atan2(sinr, cosr);
//
//	double sinp = +2.0 * (q.w * q.y - q.z * q.x);
//	if (abs(sinp) >= 1)
//		end.x = (float)copysign(PI / 2, sinp);
//	else
//		end.x = (float)asin(sinp);
//
//	double siny = +2.0 * (q.w * q.z + q.x * q.y);
//	double cosy = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
//	end.y = (float)atan2(siny, cosy);
//
//	return end;
//}
//
//Vector4 QuaternionFromYPR(float yaw, float pitch, float roll) {
//	Vector4 q;
//	double cy = cos(yaw * 0.5);
//	double sy = sin(yaw * 0.5);
//	double cr = cos(pitch * 0.5);
//	double sr = sin(pitch * 0.5);
//	double cp = cos(roll * 0.5);
//	double sp = sin(roll * 0.5);
//
//	q.w = (float)(cy * cr * cp + sy * sr * sp);
//	q.x = (float)(cy * sr * cp - sy * cr * sp);
//	q.y = (float)(cy * cr * sp + sy * sr * cp);
//	q.z = (float)(sy * cr * cp - cy * sr * sp);
//	return q;
//}
//
//float GetDifference(float firstAngle, float secondAngle) {
//	float difference = secondAngle - firstAngle;
//	while (difference < -180.f) difference += 360.f;
//	while (difference > 180.f) difference -= 360.f;
//	return difference;
//}
//
//Vector3 CalculateSilentAngle(Vector3 Position) {
//	Vector3 ViewTranslation = View->Translation();
//	Vector3 _Delta = ViewTranslation - Position;
//	return Vector3(std::atan2(_Delta.y, _Delta.x) * RAD2DEG + 90.f, -(std::asin(_Delta.z / ViewTranslation.Distance(Position))) * RAD2DEG, 0.f);
//}
//
//Vector3 CalculateEulerAngle(Vector4 quaternion) {
//	return Vector3(std::atan2(2.f * (quaternion.w * quaternion.z + quaternion.x * quaternion.y), (1.f - 2.f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z))) * RAD2DEG, std::atan2(2.f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z), (1.f - 2.f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y))) * RAD2DEG, 0.f);
//}
//
//Vector4 GetAngle(Vector3 viewTranslation, Vector3 enemyHead, Vector4 originalAngle) {
//	Vector3 aimAngle = VectorAngle(viewTranslation - enemyHead);
//	Vector3 currentAngle = ToEuler(originalAngle);
//
//	currentAngle *= (180.f / PI);
//
//	Vector3 currentAngleV = Vector3(currentAngle.z, currentAngle.y, currentAngle.x);
//	Vector3 smoothedAngle = Vector3(aimAngle.x, aimAngle.y, aimAngle.z);
//
//	smoothedAngle.x = GetDifference(currentAngleV.x, smoothedAngle.x);
//	smoothedAngle.y = GetDifference(currentAngleV.y, smoothedAngle.y);
//
//	float smoothing = 1;
//
//	smoothedAngle *= smoothing;
//	currentAngleV.x += smoothedAngle.x;
//	currentAngleV.y += smoothedAngle.y;
//
//	if (currentAngleV.x > 89.0f && currentAngleV.x <= 180.0f)
//	{
//		currentAngleV.x = 89.0f;
//	}
//
//	while (currentAngleV.x > 180.f)
//	{
//		currentAngleV.x -= 360.f;
//	}
//
//	while (currentAngleV.x < -89.0f)
//	{
//		currentAngleV.x = -89.0f;
//	}
//
//	while (currentAngleV.y > 180.f)
//	{
//		currentAngleV.y -= 360.f;
//	}
//
//	while (currentAngleV.y < -180.f)
//	{
//		currentAngleV.y += 360.f;
//	}
//
//	aimAngle = aimAngle * (PI / 180.f);
//	currentAngle = currentAngle * (PI / 180.f);
//	currentAngleV = currentAngleV * (PI / 180.f);
//
//	Vector4 newQuaternion = QuaternionFromYPR(currentAngleV.y, currentAngleV.x, 0);
//	return newQuaternion;
//}

//void AimClass::Aimbot(bool Status) {
//	if (!Status) return;														          // If not enabled return
//
//	std::uintptr_t Local = LocalEntity->Get();										      // Get an instance of local player
//
//	if (Health(Local) > 0) {														      // Local player health check
//		
//		std::uintptr_t BestEntity = Entity->GetNearestEntity();						      // Get an instance of nearest entity
//		INT8 LocalTeam = Entity->GetTeam(Local);                                          // Get an instance of local player team
//		INT8 BestEntityTeam = Entity->GetTeam(BestEntity);                                // Get an instance of entity player team
//
//		if (BestEntityTeam != LocalTeam && Health(BestEntity) > 0) {                      // If entity is not my team, and the entitys health i above 0
//			Vector3 TargetPos = Read<Vector3>(BestEntity + 0x1090);						  // Entity player world head pos x, y, z (0x1090 is player head offset)
//			float Distance =  Engine->W2SDistance(TargetPos);                             // Entity player screen head pos x, y
//
//			Vector4 qCAngles = GetAngle(View->Translation(), TargetPos, GetViewAngles()); // Get angle to be set, includes smoothing
//			SetViewAngle(qCAngles); //Set the new angle
//		}
//	}
//}

//void AimClass::GunRotation(bool status) {
//	if (!status) return;
//
//	uint64_t lentity = local_player();
//
//	if (health(lentity) > 0) {
//		uint64_t best_entity = get_nearest_ent();
//
//		INT8 lteam = local_team();
//		INT8 pteam = entity_team(best_entity);
//
//		int phealth = health(decrpyt_ent_info(best_entity));
//
//		if (pteam != lteam && phealth > 0) {
//
//			uint64_t
//			local = Read<uint64_t>(GameBase + 0x5E32C50);
//			local = Read<uint64_t>(local + 0x88);
//			local = Read<uint64_t>(local + 0x0);
//			local = Read<uint64_t>(local + 0x30) - 0x6F; if (!local) return;
//
//			uint64_t
//			weapon_manager = Read<uint64_t>(local + 0x90);
//			weapon_manager = Read<uint64_t>(weapon_manager + 0x70);
//			weapon_manager = Read<uint64_t>(weapon_manager + 0x288);
//
//			weapon_manager -= 0x34; weapon_manager ^= 0x4F; weapon_manager -= 0x76; if (!weapon_manager) return;
//
//			Vector3 target_position = Read<Vector3>(decrpyt_ent_info(best_entity) + 0x1070);
//
//			Vector3 angle = CalculateSilentAngle(target_position);
//
//			Vector3 new_angle = (angle - CalculateEulerAngle(get_view_angle())) * DEG2RAD;
//
//			write<bool>(weapon_manager + 0xC8, false);
//
//			write<Vector3>(weapon_manager + 0x148, new_angle);
//
//		}
//	}
//}
//
//
//int spinbot_target = 0;
//
//void AimClass::EntitySwap(bool enable) {
//	if (!enable) return;
//
//	uint64_t lentity = local_player();
//	INT8 lteam = local_team();
//
//	if (health(lentity) > 0) {
//		Vector4 q_angles = get_view_angle();
//		Vector3 view_translation_ = view_translation();
//
//		if (spinbot_target >= entity_count()) spinbot_target = 0;
//
//		uint64_t targetentity = entity_by_id(spinbot_target);
//		INT8 pteam = entity_team(entity_by_id(spinbot_target, false));
//		int pHealth = health(targetentity);
//
//		Vector3 head_pos = Read<Vector3>(targetentity + 0x1070);
//
//		if (!targetentity || pHealth < 1 || pHealth > 700 || targetentity == lentity || pteam == lteam) { spinbot_target++; return; }
//		
//		Vector4 qc_angles = get_angle(view_translation_, head_pos, q_angles);
//		set_view_angle(qc_angles);
//
//		spinbot_target++;
//	}
//}