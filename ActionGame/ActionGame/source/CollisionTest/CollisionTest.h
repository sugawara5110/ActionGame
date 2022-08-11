///////////////////////////////////////////////CollisionTest/////////////////////////////////////////////////////

#ifndef Class_CollisionTest_Header
#define Class_CollisionTest_Header

#include "../../../../CoordTf/CoordTf.h"
#include <memory>

struct CollisionParameter {
	CoordTf::VECTOR3 Pos = {};
	CoordTf::VECTOR3 nextPos = {};
	float Range = 0.0f;
	float Weight = 0.0f;
	int meshNo = -1;
	bool Chit = false;
	bool Ahit = false;
	int HP = 0;
	bool down = false;
};

struct AttackParameter {
	CoordTf::VECTOR3 Pos = {};
	float Range = 0.0f;
	int att = 0;
	int meshNo = -1;
	bool effect[2] = {};
};

class CollisionTest {

private:
	std::unique_ptr<CollisionParameter* []> carr = nullptr;
	std::unique_ptr<CoordTf::VECTOR3[]> addpos = nullptr;
	int cnum = 0;
	std::unique_ptr<AttackParameter* []> aarr = nullptr;
	int anum = 0;

public:
	void init(CollisionParameter** cArr, int cNum, AttackParameter** aArr, int aNum);
	void update();
};

#endif