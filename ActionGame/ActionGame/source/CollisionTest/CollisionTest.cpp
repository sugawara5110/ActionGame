///////////////////////////////////////////////CollisionTest/////////////////////////////////////////////////////

#include "CollisionTest.h"
#include "../Util/Util.h"

void CollisionTest::init(CollisionParameter** cArr, int cNum, AttackParameter** aArr, int aNum) {
	cnum = cNum;
	anum = aNum;
	carr = std::make_unique<CollisionParameter* []>(cnum);
	addpos = std::make_unique<CoordTf::VECTOR3[]>(cnum);
	aarr = std::make_unique<AttackParameter* []>(anum);

	memcpy(carr.get(), cArr, cnum * sizeof(CollisionParameter*));
	memcpy(aarr.get(), aArr, anum * sizeof(AttackParameter*));
}

void CollisionTest::update() {

	for (int i = 0; i < cnum; i++) {
		for (int j = 0; j < cnum; j++) {
			if (j == i || carr[i]->meshNo == carr[j]->meshNo)continue;
			float dist = Util::dist3d(carr[i]->nextPos, carr[j]->nextPos);
			if (carr[i]->Range + carr[j]->Range > dist) {
				carr[i]->hit = true;
				addpos[i] = Util::getWallRubbingVector(carr[j]->Pos, carr[i]->Pos, carr[i]->nextPos);
			}
		}
	}

	for (int i = 0; i < cnum; i++) {
		if (carr[i]->hit) {
			CoordTf::VECTOR3 v = addpos[i];
			CoordTf::VECTOR3 v1 = carr[i]->Pos;
			carr[i]->Pos.as(v1.x + v.x, v1.y + v.y, v1.z + v.z);
		}
		else {
			carr[i]->Pos = carr[i]->nextPos;
		}
	}
}