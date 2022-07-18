///////////////////////////////////////////////DelayPosition/////////////////////////////////////////////////////////

#include "DelayPosition.h"
#include <cmath>
#include "../Util/Util.h"

void DelayPosition::init(CoordTf::VECTOR3 current) {
	Current = current;
}

bool DelayPosition::update(CoordTf::VECTOR3 target, float delay, float range) {

	CoordTf::VECTOR3 add = {
		Current.x - target.x,
		Current.y - target.y,
		Current.z - target.z
	};

	int cnt = 0;
	if (abs(Current.x - target.x) > range) {
		Current.x -= add.x * delay;
		cnt++;
	}
	if (abs(Current.y - target.y) > range) {
		Current.y -= add.y * delay;
		cnt++;
	}
	if (abs(Current.z - target.z) > range) {
		Current.z -= add.z * delay;
		cnt++;
	}
	if (cnt == 0)return true;

	return false;
}

void DelayPosition::ImmediatelyUpdate(CoordTf::VECTOR3 target) {
	Current = target;
}

CoordTf::VECTOR3 DelayPosition::getCurrent() {
	return Current;
}

void DelayTheta::init(float current) {
	Current = current;
}

bool DelayTheta::update(float target, float delay, float range) {

	float add = Current - target;

	int cnt = 0;
	if (abs(Current - target) <= 180.0f) {
		if (abs(Current - target) > range) {
			Current -= add * delay;
			cnt++;
		}
	}
	else {
		if (abs(abs(Current - target) - 180.0f) > range) {
			Current += add * delay;
			cnt++;
		}
	}

	Util::thetaCalc(&Current, 0.0f);

	if (cnt == 0)return true;

	return false;
}

void DelayTheta::ImmediatelyUpdate(float target) {
	Current = target;
}

float DelayTheta::getCurrent() {
	return Current;
}