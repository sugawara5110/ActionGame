///////////////////////////////////////////////SkinObj/////////////////////////////////////////////////////

#ifndef Class_SkinObj_Header
#define Class_SkinObj_Header

#include "../../../../Common/Direct3DWrapper/Dx12Process.h"
#include "../../../../Common/DirectInputWrapper/DxInput.h"
#include "../Util/Util.h"
#include "../CollisionTest/CollisionTest.h"
#include "../../../../T_float/T_float.h"

class SkinObj :public SkinMesh {

protected:
	float theta = 0.0f;
	int animIndex = 0;
	float AttAnimTime = 0.0f;
	int internalIndex = 0;
	float scale = 0.0f;

	CollisionParameter* cp = nullptr;
	int numCp = 0;
	AttackParameter* ap = nullptr;
	int numAp = 0;

public:
	~SkinObj();
	CoordTf::VECTOR3 getPos();
	float getTheta();

	CoordTf::VECTOR3 GetVertexPosition(int meshIndex, int verNum);
	CollisionParameter* getCollisionParameter(int index);
	int getNumCollisionParameter();
	AttackParameter* getAttackParameter(int index);
	int getNumAttackParameter();
};

#endif