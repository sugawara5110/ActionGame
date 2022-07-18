///////////////////////////////////////////////SkinObj/////////////////////////////////////////////////////

#include "SkinObj.h"

SkinObj::~SkinObj() {
	ARR_DELETE(cp);
	ARR_DELETE(ap);
}

void SkinObj::setBloomParameter(int index, float bloomStrength, float thresholdLuminance) {
	SkinMeshBloom::setBloomParameter(index, bloomStrength, thresholdLuminance);
	SkinMeshBloom::DrawPreparation();
}

void SkinObj::StreamOutput(int comNo) {
	SkinMeshBloom::StreamOutput(comNo);
}

ParameterDXR* SkinObj::getParameterDXR(int index) {
	return SkinMeshBloom::getParameter(index);
}

int SkinObj::getNumParameterDXR() {
	return SkinMeshBloom::getNumMesh();
}

CoordTf::VECTOR3 SkinObj::getPos() {
	return cp[0].Pos;
}

float SkinObj::getTheta() {
	return theta;
}

int SkinObj::getNumBloomParameter() {
	return SkinMeshBloom::getNumBloomParameter();
}

BloomParameter* SkinObj::getBloomParameter(int index) {
	return SkinMeshBloom::getBloomParameter(index);
}

CoordTf::VECTOR3 SkinObj::GetVertexPosition(int meshIndex, int verNum) {

	CoordTf::VECTOR3 v = SkinMeshBloom::GetVertexPosition(meshIndex, verNum, 0, 0, 0, theta, 0, 0, scale);
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