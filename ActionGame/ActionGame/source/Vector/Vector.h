///////////////////////////////////////////////Vector/////////////////////////////////////////////////////////

#ifndef Class_Vector_Header
#define Class_Vector_Header

#include "../../../../CoordTf/CoordTf.h"

namespace Vector {
	void thetaCalc(float* theta, float add);
	float getThetaXY(CoordTf::VECTOR2 target, CoordTf::VECTOR2 self);
}

#endif
