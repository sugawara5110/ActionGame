///////////////////////////////////////////////Enemy/////////////////////////////////////////////////////////

#include "Enemy.h"
#include "../Util/Util.h"

#include"../../../../Common/Direct3DWrapperOption/DxText.h"

void Enemy::create() {
	SetState(true, true);
	GetFbx("mesh/golem/ICE2 all.fbx");
	//float end[2] = { (float)getMaxEndframe(0,0),(float)getMaxEndframe(0,1) };
	float end[2] = { 126.0f,200.f };
	GetBuffer(1, 2, end);
	setMaterialType(EMISSIVE, 0, 0);

	SetVertex(true);
	SetConnectStep(0, 2000);
	Vertex_hold();
	CreateFromFBX(0);

	dpos.init({ 0.0f, -150.0f, -15.0f });
	dtheta.init(180.0f);
	AnimThreshold = 50.0f;
	scale = 0.11f;

	numCp = 1;
	cp = new CollisionParameter[numCp];
	for (int i = 0; i < numCp; i++) {
		cp[i].Pos.as(0.0f, -150.0f, -15.0f);
		cp[i].nextPos.as(0.0f, -150.0f, -15.0f);
		cp[i].meshNo = 0;
		cp[i].Range = 40.0f;
		cp[i].HP = 2000;
	}
	numAp = 1;
	ap = new AttackParameter[numAp];
	for (int i = 0; i < numAp; i++) {
		ap[i].meshNo = 0;
		ap[i].Range = 40.0f;
		ap[i].att = 20.0f;
	}
}

static T_float tfloat;

void Enemy::update(CoordTf::VECTOR3 target) {

	float m = tfloat.Add(0.05f);

	if (dtheta.update(Util::getThetaXY({ target.x,target.y }, { dpos.getCurrent().x, dpos.getCurrent().y }), 0.005f, thetaRange[thetaRangeIndex])) {
		thetaRangeIndex = 1 - thetaRangeIndex;
	}

	//DxText::GetInstance()->
		//UpDateValue(AttAnimTime, 10, 200, 30.0f, 10, { 1.0f, 1.0f, 1.0f, 1.0f });

	if (cp[0].down && !isDown) {
		isDown = true;
		downTime = 50.0f;
	}
	if (isDown && 0.0f > (downTime -= tfloat.Add(0.1f))) {
		cp[0].down = false;
		isDown = false;
	}

	CoordTf::VECTOR4 col = {};
	if (cp[0].down)col.as(0, -1, -1, 0);
	if (Update(animIndex, m, cp[0].Pos, col, { 0,0, dtheta.getCurrent() }, { scale,scale,scale }, internalIndex)) {
		AttAnimTime = 0.0f;
	}

	if (dpos.update(target, 0.003f, AnimThreshold)) {
		internalIndex = 0;//UŒ‚
		AnimThreshold = 80.0f;
		AttAnimTime += m;
	}
	else {
		AttAnimTime = 0.0f;
		internalIndex = 1;
		AnimThreshold = 50.0f;
	}

	bool att = AttAnimTime > 70.0f && AttAnimTime < 100.0f;
	if (!ap[0].effect && internalIndex == 0 && att) {
		ap[0].effect = true;
	}
	if (ap[0].effect && !att) {
		ap[0].effect = false;
	}

	cp[0].nextPos = dpos.getCurrent();
	theta = dtheta.getCurrent();

	ap[0].Pos = GetVertexPosition(1, 1320);//‰E˜r1320
}
