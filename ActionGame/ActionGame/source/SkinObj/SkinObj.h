///////////////////////////////////////////////SkinObj/////////////////////////////////////////////////////

#ifndef Class_SkinObj_Header
#define Class_SkinObj_Header

#include "../../../../Common/Direct3DWrapperOption/Dx_PostEffect.h"
#include "../../../../Common/DirectInputWrapper/DxInput.h"
#include "../Util/Util.h"
#include "../CollisionTest/CollisionTest.h"

class SkinObj :public SkinMeshBloom {

protected:
	float theta = 0.0f;
	int meshIndex = 0;
	int internalIndex = 0;
	float scale = 0.0f;

	CollisionParameter* cp = nullptr;
	int numCp = 0;
	AttackParameter* ap = nullptr;
	int numAp = 0;

public:
	~SkinObj();
	void setBloomParameter(int index, float bloomStrength, float thresholdLuminance);
	void StreamOutput(int comNo);
	int getNumBloomParameter();
	BloomParameter* getBloomParameter(int index);
	ParameterDXR* getParameterDXR(int index);
	int getNumParameterDXR();
	CoordTf::VECTOR3 getPos();
	float getTheta();

	CoordTf::VECTOR3 GetVertexPosition(int meshIndex, int verNum);
	CollisionParameter* getCollisionParameter(int index);
	int getNumCollisionParameter();
	AttackParameter* getAttackParameter(int index);
	int getNumAttackParameter();
};

#endif