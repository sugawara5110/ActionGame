///////////////////////////////////////////////Enemy/////////////////////////////////////////////////////////

#include "Enemy.h"
#include "../Util/Util.h"

void Enemy::create() {
	SetState(true, true);
	GetFbx("mesh/golem/ICE2 all.fbx");
	float end[2] = { 1200,2000 };
	GetBuffer(1, 2, end);
	setMaterialType(EMISSIVE, 0, 0);
	createBloomParameter();

	SetVertex(true);
	SetConnectStep(0, 2000);

	CreateFromFBX();

	dpos.init({ 0.0f, -150.0f, -15.0f });
	lastPos.as(0.0f, -160.0f);
	dtheta.init(180.0f);
	AnimThreshold = 50.0f;
	scale = 0.11f;

	numCp = 1;
	cp = new CollisionParameter[numCp];
	for (int i = 0; i < numCp; i++) {
		cp[i].Pos.as(0.0f, -150.0f, -15.0f);
		cp[i].nextPos.as(0.0f, -150.0f, -15.0f);
		cp[i].meshNo = 0;
		cp[i].Range = 30.0f;
		cp[i].Weight = 10.0f;
		cp[i].HP = 2000;
	}
	numAp = 1;
	ap = new AttackParameter[numAp];
	for (int i = 0; i < numAp; i++) {
		ap[i].meshNo = 0;
		ap[i].Range = 20.0f;
		ap[i].att = 20.0f;
	}
}

static T_float tfloat;

void Enemy::update(CoordTf::VECTOR3 target) {

	if (cp[0].Chit) {
		dpos.ImmediatelyUpdate(cp[0].Pos);
		cp[0].Chit = false;
	}

	if (cp[0].Ahit) {
		cp[0].Ahit = false;
		cp[0].down = true;
	}

	float m = tfloat.Add(0.5f);
	if (dtheta.update(Util::getThetaXY({ dpos.getCurrent().x, dpos.getCurrent().y }, lastPos), 0.02f, thetaRange[thetaRangeIndex])) {
		thetaRangeIndex = 1 - thetaRangeIndex;
	}

	if (Update(meshIndex, m, cp[0].Pos, { 0,0,0,0 }, { 0,0, dtheta.getCurrent() }, { scale,scale,scale }, internalIndex)) {
		if (cp[0].down) {
			cp[0].down = false;
		}
	}
	lastPos.as(dpos.getCurrent().x, dpos.getCurrent().y);

	if (dpos.update(target, 0.003f, AnimThreshold)) {
		internalIndex = 0;
		AnimThreshold = 80.0f;
	}
	else {
		internalIndex = 1;
		AnimThreshold = 50.0f;
	}

	cp[0].nextPos = dpos.getCurrent();
	theta = dtheta.getCurrent();

	ap[0].Pos = GetVertexPosition(1, 1320);//‰E˜r1320
}
