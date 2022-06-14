///////////////////////////////////////////////SkinObj/////////////////////////////////////////////////////

#include "SkinObj.h"

void SkinObj::setBloomParameter(int index, float bloomStrength, float thresholdLuminance) {
	sk->setBloomParameter(index, bloomStrength, thresholdLuminance);
	sk->DrawPreparation();
}

void SkinObj::StreamOutput(int comNo) {
	sk->StreamOutput(comNo);
}

ParameterDXR* SkinObj::getParameterDXR(int index) {
	return sk->getParameter(index);
}

int SkinObj::getNumParameterDXR() {
	return sk->getNumMesh();
}

CoordTf::VECTOR3 SkinObj::getPos() {
	return pos;
}

float SkinObj::getTheta() {
	return theta;
}

int SkinObj::getNumBloomParameter() {
	return sk->getNumBloomParameter();
}

BloomParameter* SkinObj::getBloomParameter(int index) {
	return sk->getBloomParameter(index);
}