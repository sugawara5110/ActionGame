///////////////////////////////////////////////Enemy/////////////////////////////////////////////////////////

#ifndef Class_Enemy_Header
#define Class_Enemy_Header

#include "SkinObj.h"
#include "../DelayPosition/DelayPosition.h"

class Enemy :public SkinObj {

private:
	DelayPosition pos = {};
	DelayTheta theta = {};
	CoordTf::VECTOR2 lastPos = {};
	float AnimThreshold = 0.0f;
	const float thetaRange[2] = { 20.0f, 30.0f };
	int thetaRangeIndex = 0;

public:
	void create();
	void update(CoordTf::VECTOR3 target);
};

#endif