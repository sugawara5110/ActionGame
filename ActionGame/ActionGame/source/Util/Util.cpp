///////////////////////////////////////////////Util/////////////////////////////////////////////////////////

#define _USE_MATH_DEFINES
#include "Util.h"
#include <math.h>

void Util::thetaCalc(float* theta, float add) {
	*theta += add;
	if (*theta < 0)*theta = 360.0f + *theta;
	if (*theta > 360.0f)*theta = *theta - 360.0f;
}

float Util::getThetaXY(CoordTf::VECTOR2 target, CoordTf::VECTOR2 self) {
	CoordTf::VECTOR2 diff = {};
	diff.as(
		target.x - self.x,
		target.y - self.y
	);
	float the = (float)(atan2(diff.y, diff.x) * 360.0f / (M_PI * 2.0f) + 90.0f);
	if (the < 0.0f)the += 360.0f;
	return the;
}

float Util::dist3d(CoordTf::VECTOR3 pos1, CoordTf::VECTOR3 pos2) {
	float dx = pos2.x - pos1.x;
	float dy = pos2.y - pos1.y;
	float dz = pos2.z - pos1.z;

	return sqrt(dx * dx + dy * dy + dz * dz);
}

CoordTf::VECTOR3 Util::getSphereNormal(CoordTf::VECTOR3 target, CoordTf::VECTOR3 self) {
	CoordTf::VECTOR3 vec = {};
	CoordTf::VECTOR3 vecout = {};
	vec.as(self.x - target.x, self.y - target.y, self.z - target.z);
	CoordTf::VectorNormalize(&vecout, &vec);
	return vecout;
}

CoordTf::VECTOR3 Util::getDirectionVector(CoordTf::VECTOR3 next, CoordTf::VECTOR3 Current) {
	return getSphereNormal(Current, next);
}

CoordTf::VECTOR3 Util::getWallRubbingVector(CoordTf::VECTOR3 dirVec, CoordTf::VECTOR3 Nor) {
	float a = CoordTf::VectorDot(&dirVec, &Nor);
	CoordTf::VECTOR3 out1 = {};
	out1.as(Nor.x * a, Nor.y * a, Nor.z * a);
	CoordTf::VECTOR3 out2 = {};
	out2.as(dirVec.x - out1.x, dirVec.y - out1.y, dirVec.z - out1.z);
	CoordTf::VECTOR3 out3 = {};
	CoordTf::VectorNormalize(&out3, &out2);
	return out3;
}

CoordTf::VECTOR3 Util::getWallRubbingVector(CoordTf::VECTOR3 target, CoordTf::VECTOR3 curpos, CoordTf::VECTOR3 nextpos) {
	return getWallRubbingVector(
		getDirectionVector(nextpos, curpos),
		getSphereNormal(target, curpos)
	);
}