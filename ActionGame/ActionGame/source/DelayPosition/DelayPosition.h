///////////////////////////////////////////////DelayPosition/////////////////////////////////////////////////////////

#ifndef Class_DelayPosition_Header
#define Class_DelayPosition_Header

#include "../../../../CoordTf/CoordTf.h"

class DelayPosition {

private:
	CoordTf::VECTOR3 Current = {};

public:
	void init(CoordTf::VECTOR3 current);
	bool update(CoordTf::VECTOR3 target, float delay, float range);
	CoordTf::VECTOR3 getCurrent();
};

class DelayTheta {

private:
	float Current = 0.0f;

public:
	void init(float current);
	bool update(float target, float delay, float range);
	float getCurrent();
};

#endif
