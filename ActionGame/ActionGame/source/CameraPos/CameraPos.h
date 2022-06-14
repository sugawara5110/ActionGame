///////////////////////////////////////////////CameraPos///////////////////////////////////////////////////////

#ifndef Class_CameraPos_Header
#define Class_CameraPos_Header

#include "../../../../Common/Direct3DWrapper/Dx12Process.h"
#include "../DelayPosition/DelayPosition.h"

class CameraPos {

private:
	DelayPosition Pos = {};
	DelayPosition GazePoint = {};

public:
	void init(CoordTf::VECTOR3 subjectPos, float subjectTheta,
		CoordTf::VECTOR3 camDistancePos = { 0.0f,80.0f,20.0f }, float camDistanceTheta = 0.0f);

	void update(CoordTf::VECTOR3 subjectPos, float subjectTheta,
		float delay = 0.03f, float range = 1.0f,
		CoordTf::VECTOR3 camDistancePos = { 0.0f,80.0f,20.0f }, float camDistanceTheta = 0.0f);
};

#endif