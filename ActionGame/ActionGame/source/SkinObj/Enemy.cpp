///////////////////////////////////////////////Enemy/////////////////////////////////////////////////////////

#include "Enemy.h"
#include "../Vector/Vector.h"

void Enemy::create() {
	sk = std::make_unique<SkinMeshBloom>();
	sk->SetState(true, true);
	sk->GetFbx("mesh/golem/ICE2 all.fbx");
	float end[2] = { 1200,2000 };
	sk->GetBuffer(1, 2, end);
	sk->setMaterialType(EMISSIVE, 0, 0);
	sk->createBloomParameter();

	sk->SetVertex(true);
	sk->SetConnectStep(0, 2000);

	sk->CreateFromFBX();

	pos.init({ 0.0f, -150.0f, -15.0f });
	lastPos.as(0.0f, -160.0f);
	theta.init(180.0f);
	AnimThreshold = 50.0f;
}

static T_float tfloat;

void Enemy::update(CoordTf::VECTOR3 target) {

	if (pos.update(target, 0.003f, AnimThreshold)) {
		internalIndex = 0;
		AnimThreshold = 80.0f;
	}
	else {
		internalIndex = 1;
		AnimThreshold = 50.0f;
	}

	float m = tfloat.Add(0.5f);
	if (theta.update(Vector::getThetaXY({ pos.getCurrent().x, pos.getCurrent().y }, lastPos), 0.02f, thetaRange[thetaRangeIndex])) {
		thetaRangeIndex = 1 - thetaRangeIndex;
	}

	sk->Update(meshIndex, m, pos.getCurrent(), { 0,0,0,0 }, { 0,0, theta.getCurrent() }, { 0.13f,0.13f,0.13f }, internalIndex);
	lastPos.as(pos.getCurrent().x, pos.getCurrent().y);
}