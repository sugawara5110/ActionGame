///////////////////////////////////////////////CollisionTest/////////////////////////////////////////////////////

#include "CollisionTest.h"
#include "../Util/Util.h"

void CollisionTest::init(CollisionParameter** cArr, int cNum, AttackParameter** aArr, int aNum) {
	cnum = cNum;
	anum = aNum;
	carr = std::make_unique<CollisionParameter * []>(cnum);
	aarr = std::make_unique<AttackParameter * []>(anum);

	memcpy(carr.get(), cArr, cnum * sizeof(CollisionParameter*));
	memcpy(aarr.get(), aArr, anum * sizeof(AttackParameter*));
}

void CollisionTest::update() {

	for (int i = 0; i < cnum; i++) {
		for (int j = 0; j < cnum; j++) {
			if (carr[i]->meshNo == carr[j]->meshNo)continue;
			//移動後の物体同士の距離
			float dist = Util::dist3d(carr[i]->nextPos, carr[j]->nextPos);
			//互いのレンジの加算値よりも距離が短い場合接触したと判断
			if (carr[i]->Range + carr[j]->Range > dist) {
				carr[i]->Chit = true;
				CoordTf::VECTOR3 v = Util::getWallRubbingVector(carr[j]->Pos, carr[i]->Pos, carr[i]->nextPos);
				CoordTf::VECTOR3 v1 = carr[i]->Pos;
				carr[i]->Pos.as(v1.x + v.x, v1.y + v.y, v1.z + v.z);
			}
			else {
				carr[i]->Pos = carr[i]->nextPos;
			}
		}
	}

	for (int i = 0; i < cnum; i++) {
		for (int j = 0; j < anum; j++) {
			if (carr[i]->meshNo == aarr[j]->meshNo)continue;
			float dist = Util::dist3d(carr[i]->nextPos, aarr[j]->Pos);
			if (!carr[i]->Ahit && !carr[i]->down && aarr[j]->effect && carr[i]->Range + aarr[j]->Range > dist) {
				carr[i]->Ahit = true;
				carr[i]->HP -= aarr[j]->att;
			}
		}
	}
}