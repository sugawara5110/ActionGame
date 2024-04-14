///////////////////////////////////////////////Enemy/////////////////////////////////////////////////////////

#ifndef Class_Enemy_Header
#define Class_Enemy_Header

#include "SkinObj.h"
#include "../DelayPosition/DelayPosition.h"

class Enemy :public SkinObj {

private:
	DelayPosition dpos = {};
	DelayTheta dtheta = {};
	float downTime = 0.0f;
	bool isDown = false;
	float AnimThreshold = 0.0f;
	const float thetaRange[2] = { 20.0f, 30.0f };
	int thetaRangeIndex = 0;
	DxAudioSourceVoice* source[2] = {};
	float footPosZ1Max[2] = { -7.0f, -10.0f };
	bool footOn[2] = { false,false };

public:
	~Enemy();
	void create();
	void update(CoordTf::VECTOR3 target);
};

#endif