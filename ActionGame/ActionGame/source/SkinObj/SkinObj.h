///////////////////////////////////////////////SkinObj/////////////////////////////////////////////////////

#ifndef Class_SkinObj_Header
#define Class_SkinObj_Header

#include "../../../../Common/Direct3DWrapperOption/Dx_PostEffect.h"
#include "../../../../Common/DirectInputWrapper/DxInput.h"
#include "../Vector/Vector.h"

class SkinObj {

protected:
	std::unique_ptr<SkinMeshBloom> sk = nullptr;
	CoordTf::VECTOR3 pos = {};
	float theta = 0.0f;
	int meshIndex = 0;
	int internalIndex = 0;

public:
	void setBloomParameter(int index, float bloomStrength, float thresholdLuminance);
	void StreamOutput(int comNo);
	int getNumBloomParameter();
	BloomParameter* getBloomParameter(int index);
	ParameterDXR* getParameterDXR(int index);
	int getNumParameterDXR();
	CoordTf::VECTOR3 getPos();
	float getTheta();
};

#endif