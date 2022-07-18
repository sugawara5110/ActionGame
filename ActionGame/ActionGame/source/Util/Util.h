///////////////////////////////////////////////Util/////////////////////////////////////////////////////////

#ifndef Class_Util_Header
#define Class_Util_Header

#include "../../../../CoordTf/CoordTf.h"

namespace Util {
	void thetaCalc(float* theta, float add);
	float getThetaXY(CoordTf::VECTOR2 target, CoordTf::VECTOR2 self);
	float dist3d(CoordTf::VECTOR3 pos1, CoordTf::VECTOR3 pos2);
	CoordTf::VECTOR3 getSphereNormal(CoordTf::VECTOR3 target, CoordTf::VECTOR3 self);
	CoordTf::VECTOR3 getDirectionVector(CoordTf::VECTOR3 next, CoordTf::VECTOR3 Current);
	CoordTf::VECTOR3 getWallRubbingVector(CoordTf::VECTOR3 dirVec, CoordTf::VECTOR3 Nor);
	CoordTf::VECTOR3 getWallRubbingVector(CoordTf::VECTOR3 target, CoordTf::VECTOR3 curpos, CoordTf::VECTOR3 nextpos);
}

#endif
