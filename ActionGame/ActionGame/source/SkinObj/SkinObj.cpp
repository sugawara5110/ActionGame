///////////////////////////////////////////////SkinObj/////////////////////////////////////////////////////

#include "SkinObj.h"

SkinObj::~SkinObj() {
	ARR_DELETE(cp);
	ARR_DELETE(ap);
}

CoordTf::VECTOR3 SkinObj::getPos() {
	return cp[0].Pos;
}

float SkinObj::getTheta() {
	return theta;
}

CoordTf::VECTOR3 SkinObj::GetVertexPosition(int meshIndex, int verNum) {

	CoordTf::VECTOR3 v = SkinMesh::GetVertexPosition(meshIndex, verNum, 0, 0, 0, theta, 0, 0, scale);
	return { cp[0].Pos.x + v.x,cp[0].Pos.y + v.y,cp[0].Pos.z + v.z };
}

CollisionParameter* SkinObj::getCollisionParameter(int index) {
	return &cp[index];
}

int SkinObj::getNumCollisionParameter() {
	return numCp;
}

AttackParameter* SkinObj::getAttackParameter(int index) {
	return &ap[index];
}

int SkinObj::getNumAttackParameter() {
	return numAp;
}