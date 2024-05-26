/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021 Silverlan
 */

#include "stdafx_shared.h"
#include "pragma/lua/classes/lquaternion.h"
#include "pragma/lua/classes/langle.h"
#include <pragma/math/angle/wvangle.h>
#include <mathutil/umat.h>
#include "luasystem.h"
#include "pragma/util/util_approach_rotation.hpp"
#include "pragma/game/game_coordinate_system.hpp"

import glm;

Quat Lua::Quaternion::Copy(lua_State *l, const Quat &rot) { return rot; }

EulerAngles Lua::Quaternion::ToEulerAngles(lua_State *l, Quat &q, uint32_t rotationOrder)
{
	//EulerAngles ang(*q);
	//luabind::object(l,ang).push(l);
	EulerAngles ang {};
	Mat4 m {q};
	switch(static_cast<pragma::RotationOrder>(rotationOrder)) {
	case pragma::RotationOrder::XYZ:
		glm::gtx::extractEulerAngleXYZ(m, ang.p, ang.y, ang.r);
		break;
	case pragma::RotationOrder::YXZ:
		glm::gtx::extractEulerAngleYXZ(m, ang.y, ang.p, ang.r);
		break;
	case pragma::RotationOrder::XZX:
		glm::gtx::extractEulerAngleXZX(m, ang.p, ang.r, ang.p);
		break;
	case pragma::RotationOrder::XYX:
		glm::gtx::extractEulerAngleXYX(m, ang.p, ang.y, ang.p);
		break;
	case pragma::RotationOrder::YXY:
		glm::gtx::extractEulerAngleYXY(m, ang.y, ang.p, ang.y);
		break;
	case pragma::RotationOrder::YZY:
		glm::gtx::extractEulerAngleYZY(m, ang.y, ang.r, ang.y);
		break;
	case pragma::RotationOrder::ZYZ:
		glm::gtx::extractEulerAngleZYZ(m, ang.r, ang.y, ang.r);
		break;
	case pragma::RotationOrder::ZXZ:
		glm::gtx::extractEulerAngleZXZ(m, ang.r, ang.p, ang.r);
		break;
	case pragma::RotationOrder::XZY:
		glm::gtx::extractEulerAngleXZY(m, ang.p, ang.r, ang.y);
		break;
	case pragma::RotationOrder::YZX:
		glm::gtx::extractEulerAngleYZX(m, ang.y, ang.r, ang.p);
		break;
	case pragma::RotationOrder::ZYX:
		glm::gtx::extractEulerAngleZYX(m, ang.r, ang.y, ang.p);
		break;
	case pragma::RotationOrder::ZXY:
		glm::gtx::extractEulerAngleZXY(m, ang.r, ang.p, ang.y);
		break;
	}
	ang.p = umath::rad_to_deg(ang.p);
	ang.y = umath::rad_to_deg(ang.y);
	ang.r = umath::rad_to_deg(ang.r);
	return ang;
}

luabind::mult<Vector3, float> Lua::Quaternion::ToAxisAngle(lua_State *l, const Quat &rot)
{
	Vector3 axis {};
	float angle = 0.f;
	uquat::to_axis_angle(rot, axis, angle);
	return {l, axis, angle};
}

luabind::mult<Vector3, Vector3, Vector3> Lua::Quaternion::GetOrientation(lua_State *l, const Quat &rot)
{
	Vector3 forward, right, up;
	uquat::get_orientation(rot, &forward, &right, &up);
	return {l, forward, right, up};
}

void Lua::Quaternion::Set(lua_State *, Quat &q, float w, float x, float y, float z)
{
	q.w = w;
	q.x = x;
	q.y = y;
	q.z = z;
}
static luabind::mult<Quat, Vector2> approach_direction(lua_State *l, const Quat &rot, const Vector3 &up, const Vector3 &targetDir, const ::Vector2 &rotAmount, const ::Vector2 *pitchLimit = nullptr, const ::Vector2 *yawLimit = nullptr, const Quat *rotPivot = nullptr,
  const EulerAngles *angOffset = nullptr)
{
	Vector2 resAm;
	auto r = uquat::approach_direction(rot, up, targetDir, rotAmount, &resAm, pitchLimit, yawLimit, rotPivot, angOffset);
	return {l, r, resAm};
}
luabind::mult<Quat, Vector2> Lua::Quaternion::ApproachDirection(lua_State *l, const Quat &rot, const Vector3 &up, const Vector3 &targetDir, const ::Vector2 &rotAmount, const ::Vector2 *pitchLimit, const ::Vector2 *yawLimit, const Quat *rotPivot, const EulerAngles *angOffset)
{
	return approach_direction(l, rot, up, targetDir, rotAmount, pitchLimit, yawLimit, rotPivot, angOffset);
}
luabind::mult<Quat, Vector2> Lua::Quaternion::ApproachDirection(lua_State *l, const Quat &rot, const Vector3 &up, const Vector3 &targetDir, const ::Vector2 &rotAmount, const ::Vector2 *pitchLimit, const ::Vector2 *yawLimit, const Quat *rotPivot)
{
	return approach_direction(l, rot, up, targetDir, rotAmount, pitchLimit, yawLimit, rotPivot);
}
luabind::mult<Quat, Vector2> Lua::Quaternion::ApproachDirection(lua_State *l, const Quat &rot, const Vector3 &up, const Vector3 &targetDir, const ::Vector2 &rotAmount, const ::Vector2 *pitchLimit, const ::Vector2 *yawLimit)
{
	return approach_direction(l, rot, up, targetDir, rotAmount, pitchLimit, yawLimit);
}
luabind::mult<Quat, Vector2> Lua::Quaternion::ApproachDirection(lua_State *l, const Quat &rot, const Vector3 &up, const Vector3 &targetDir, const ::Vector2 &rotAmount, const ::Vector2 *pitchLimit) { return approach_direction(l, rot, up, targetDir, rotAmount, pitchLimit); }
luabind::mult<Quat, Vector2> Lua::Quaternion::ApproachDirection(lua_State *l, const Quat &rot, const Vector3 &up, const Vector3 &targetDir, const ::Vector2 &rotAmount) { return approach_direction(l, rot, up, targetDir, rotAmount); }
