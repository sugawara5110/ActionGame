///////////////////////////////////////////////Hero/////////////////////////////////////////////////////////

#include "Hero.h"

void Hero::create() {
	SetState(true, true);
	GetFbx("mesh/p1/player1_all.fbx");//立ち, ダウン
	float end[2] = { (float)getMaxEndframe(0,0),(float)getMaxEndframe(0,1) };
	GetBuffer(1, 2, end);

	GetFbxSub("mesh/p1/player1_Run.fbx", 1);
	GetBuffer_Sub(1, 40);

	GetFbxSub("mesh/p1/player1_Hook.fbx", 2);
	GetBuffer_Sub(2, (float)getMaxEndframe(2, 0));

	GetFbxSub("mesh/p1/player1_kick.fbx", 3);//キック3種類
	float end1[3] = {
		(float)getMaxEndframe(3,0),
		(float)getMaxEndframe(3,1),
		(float)getMaxEndframe(3,2) };
	GetBuffer_Sub(3, 3, end1);

	GetFbxSub("mesh/p1/player1_backWalk.fbx", 4);//後退
	GetBuffer_Sub(4, (float)getMaxEndframe(4, 0));

	SetVertex(true);
	SetConnectStep(0, 1000);
	SetConnectStep(1, 100);
	SetConnectStep(2, 1000);
	SetConnectStep(3, 1000);
	SetConnectStep(4, 100);

	Vertex_hold();

	CreateFromFBX(0);
	CreateFromFBX_SubAnimation(1);
	CreateFromFBX_SubAnimation(2);
	CreateFromFBX_SubAnimation(3);
	CreateFromFBX_SubAnimation(4);

	scale = 0.02f;

	numCp = 1;
	cp = NEW CollisionParameter[numCp];
	cp[0].Pos.as(0.0f, 0.0f, -15.0f);
	cp[0].nextPos.as(0.0f, 0.0f, -15.0f);
	cp[0].meshNo = 1;
	cp[0].Range = 20.0f;
	cp[0].HP = maxHP = 1000;

	numAp = 2;
	ap = NEW AttackParameter[numAp];
	for (int i = 0; i < numAp; i++) {
		ap[i].meshNo = 1;
		ap[i].Range = 20.0f;
		ap[i].att = 90;
	}

	createMeter();
}

static T_float tfloat;

void Hero::update() {

	float m = tfloat.Add(0.1f);
	if (Update(animIndex, m, cp[0].Pos, { 0,0,0,0 }, { 0,0,theta }, { scale,scale,scale }, internalIndex)) {
		animIndex = 0;
		internalIndex = 0;
		if (cp[0].down) {
			cp[0].down = false;
		}
	}

	float addMov = 0.0f;

	if (!cp[0].down) {
		switch (DxInput::GetInstance()->checkKeyDownNo()) {
		case DIK_NUMPAD0:
			break;
		case DIK_NUMPAD1:
			animIndex = 0;
			internalIndex = 0;
			Util::thetaCalc(&theta, -tfloat.Add(0.3f));
			break;
		case DIK_NUMPAD2:
			animIndex = 4;
			internalIndex = 0;
			addMov = tfloat.Add(0.05f);
			break;
		case DIK_NUMPAD3:
			animIndex = 0;
			internalIndex = 0;
			Util::thetaCalc(&theta, tfloat.Add(0.3f));
			break;
		case DIK_NUMPAD4:
			animIndex = 1;
			internalIndex = 0;
			addMov = -tfloat.Add(0.1f);
			Util::thetaCalc(&theta, -tfloat.Add(0.2f));
			break;
		case DIK_NUMPAD5:
			break;
		case DIK_NUMPAD6:
			animIndex = 1;
			internalIndex = 0;
			addMov = -tfloat.Add(0.1f);
			Util::thetaCalc(&theta, tfloat.Add(0.2f));
			break;
		case DIK_NUMPAD7:
			animIndex = 1;
			internalIndex = 0;
			addMov = -tfloat.Add(0.1f);
			Util::thetaCalc(&theta, -tfloat.Add(0.1f));
			break;
		case DIK_NUMPAD8:
			animIndex = 1;
			internalIndex = 0;
			addMov = -tfloat.Add(0.1f);
			break;
		case DIK_NUMPAD9:
			animIndex = 1;
			internalIndex = 0;
			addMov = -tfloat.Add(0.1f);
			Util::thetaCalc(&theta, tfloat.Add(0.1f));
			break;
		}
		switch (DxInput::GetInstance()->checkKeyActionNo()) {
		case DIK_A:
			animIndex = 0;
			internalIndex = 0;
			break;
		case DIK_S:

			break;
		case DIK_D:
			break;
		case DIK_F:
			animIndex = 2;
			internalIndex = 0;
			break;
		case DIK_G:
			animIndex = 3;
			internalIndex = 0;
			break;
		case DIK_H:
			animIndex = 3;
			internalIndex = 1;
			break;
		case DIK_J:
			animIndex = 3;
			internalIndex = 2;
			break;
		}
	}
	else {
		animIndex = 0;
		internalIndex = 1;
	}

	if (!ap[0].effect && (animIndex == 2 || animIndex == 3)) {
		ap[0].effect = true;
	}
	if (ap[0].effect && animIndex != 2 && animIndex != 3) {
		ap[0].effect = false;
	}

	using namespace CoordTf;

	VECTOR3 pAdd = { 0.0f,addMov,0.0f };
	MATRIX t = {};
	MatrixRotationZ(&t, theta);
	VectorMatrixMultiply(&pAdd, &t);
	cp[0].nextPos.x = cp[0].Pos.x + pAdd.x;
	cp[0].nextPos.y = cp[0].Pos.y + pAdd.y;
	cp[0].nextPos.z = cp[0].Pos.z + pAdd.z;

	ap[0].Pos = GetVertexPosition(1, 1515);//左手 mesh1, 1515
	ap[1].Pos = GetVertexPosition(6, 382);//左足 mesh6, 382

	updateDamage({ -20,-90 }, { 0, 1, 0, 1 });
	updateMeter({ -20,-180 });
}
