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

	//�����蔻��1
	for (int i = 0; i < cnum; i++) {
		for (int j = 0; j < cnum; j++) {
			if (carr[i]->meshNo == carr[j]->meshNo)continue;
			//�ړ���̕��̓��m�̋���
			float dist = Util::dist3d(carr[i]->nextPos, carr[j]->nextPos);
			//�݂��̃����W�̉��Z�l�����������Z���ꍇ�ڐG�����Ɣ��f
			if (carr[i]->Range + carr[j]->Range > dist) {
				CoordTf::VECTOR3 v = Util::getWallRubbingVector(carr[j]->Pos, carr[i]->Pos, carr[i]->nextPos);
				CoordTf::VECTOR3 v1 = carr[i]->Pos;
				carr[i]->nextPos.as(v1.x + v.x, v1.y + v.y, v1.z + v.z);
			}
		}
	}
	//�����蔻��2
	//�����蔻��1�ōŌ�ɔ��肵�����葤obj�ƐڐG���������ꍇ�A��obj�ɐڐG������ԂɂȂ�ꍇ������ׁA
	//�����蔻��2�ōŏI�I�Ȉړ���𔻒�X�V������
	for (int i = 0; i < cnum; i++) {
		for (int j = 0; j < cnum; j++) {
			if (carr[i]->meshNo == carr[j]->meshNo)continue;
			//�ړ���̕��̓��m�̋���
			float dist = Util::dist3d(carr[i]->nextPos, carr[j]->nextPos);
			//�ŏI�I�Ȉړ��悪�A�ǂ�obj�Ƃ��ڐG���Ă��Ȃ��ꍇ�A�ʒu�X�V, �ڐG�L��̏ꍇ�͍X�V����
			if (carr[i]->Range + carr[j]->Range <= dist) {
				carr[i]->Pos = carr[i]->nextPos;
			}
		}
	}

	//�U������
	for (int i = 0; i < cnum; i++) {
		for (int j = 0; j < anum; j++) {
			if (carr[i]->meshNo == aarr[j]->meshNo)continue;
			float dist = Util::dist3d(carr[i]->nextPos, aarr[j]->Pos);
			if (!carr[i]->down && aarr[j]->effect && carr[i]->Range + aarr[j]->Range > dist) {
				carr[i]->down = true;
				carr[i]->HP -= aarr[j]->att;
			}
		}
	}
}