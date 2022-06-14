///////////////////////////////////////////////Vector/////////////////////////////////////////////////////////

#define _USE_MATH_DEFINES
#include "Vector.h"
#include <math.h>

void Vector::thetaCalc(float* theta, float add) {
	*theta += add;
	if (*theta < 0)*theta = 360.0f + *theta;
	if (*theta > 360.0f)*theta = *theta - 360.0f;
}

float Vector::getThetaXY(CoordTf::VECTOR2 target, CoordTf::VECTOR2 self) {
	CoordTf::VECTOR2 diff = {};
	diff.as(
		target.x - self.x,
		target.y - self.y
	);
	float the = (float)(atan2(diff.y, diff.x) * 360.0f / (M_PI * 2.0f) + 90.0f);
	if (the < 0.0f)the += 360.0f;
	return the;
}