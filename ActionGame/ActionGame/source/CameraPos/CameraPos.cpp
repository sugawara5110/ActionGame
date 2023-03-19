///////////////////////////////////////////////CameraPos///////////////////////////////////////////////////////

#include "CameraPos.h"

void CameraPos::init(CoordTf::VECTOR3 subjectPos, float subjectTheta,
	CoordTf::VECTOR3 camDistancePos, float camDistanceTheta) {

	using namespace CoordTf;
	GazePoint.init(subjectPos);
	VECTOR3 pAdd = camDistancePos;
	MATRIX t = {};
	MatrixRotationZ(&t, subjectTheta + camDistanceTheta);
	VectorMatrixMultiply(&pAdd, &t);

	VECTOR3 current = {};
	current.as(
		subjectPos.x + pAdd.x,
		subjectPos.y + pAdd.y,
		subjectPos.z + pAdd.z
	);
	Pos.init(current);
}

void CameraPos::update(CoordTf::VECTOR3 subjectPos, float subjectTheta,
	float delay, float range,
	CoordTf::VECTOR3 camDistancePos, float camDistanceTheta) {

	using namespace CoordTf;
	GazePoint.update(subjectPos, delay, range);
	VECTOR3 pAdd = camDistancePos;
	MATRIX t = {};
	MatrixRotationZ(&t, subjectTheta + camDistanceTheta);
	VectorMatrixMultiply(&pAdd, &t);

	VECTOR3 TargetPos = {};
	TargetPos.as(
		subjectPos.x + pAdd.x,
		subjectPos.y + pAdd.y,
		subjectPos.z + pAdd.z
	);

	Pos.update(TargetPos, delay, range);

	Dx_SwapChain* sw = Dx_SwapChain::GetInstance();
	sw->Cameraset(Pos.getCurrent(), GazePoint.getCurrent());
}