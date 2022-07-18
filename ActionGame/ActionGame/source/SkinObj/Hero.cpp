///////////////////////////////////////////////Hero/////////////////////////////////////////////////////////

#include "Hero.h"

void Hero::create() {
	SetState(true, true);
	GetFbx("mesh/p1/player1_all.fbx");//立ち, ダウン
	float end[2] = { 4000,1000 };
	GetBuffer(1, 2, end);
	createBloomParameter();

	GetFbxSub("mesh/p1/player1_Run.fbx", 1);
	GetBuffer_Sub(1, 400);

	GetFbxSub("mesh/p1/player1_Hook.fbx", 2);
	GetBuffer_Sub(2, 1000);

	GetFbxSub("mesh/p1/player1_kick.fbx", 3);//キック3種類
	float end1[3] = { 3000,3000,3000 };
	GetBuffer_Sub(3, 3, end1);

	GetFbxSub("mesh/p1/player1_backWalk.fbx", 4);//後退
	GetBuffer_Sub(4, 800);

	SetVertex(true);
	SetConnectStep(0, 10000);
	SetConnectStep(1, 1000);
	SetConnectStep(2, 10000);
	SetConnectStep(3, 10000);
	SetConnectStep(4, 1000);

	CreateFromFBX();
	CreateFromFBX_SubAnimation(1);
	CreateFromFBX_SubAnimation(2);
	CreateFromFBX_SubAnimation(3);
	CreateFromFBX_SubAnimation(4);

	scale = 0.02f;

	numCp = 1;
	cp = new CollisionParameter[numCp];
	cp[0].Pos.as(0.0f, 0.0f, -15.0f);
	cp[0].nextPos.as(0.0f, 0.0f, -15.0f);
	cp[0].meshNo = 1;
	cp[0].Range = 10.0f;
	cp[0].Weight = 5.0f;
}

static T_float tfloat;

void Hero::update() {

	if (cp[0].hit)cp[0].hit = false;

	float m = tfloat.Add(1.0f);
	if (Update(meshIndex, m, cp[0].Pos, { 0,0,0,0 }, { 0,0,theta }, { scale,scale,scale }, internalIndex)) {
		meshIndex = 0;
		internalIndex = 0;
	}

	float addMov = 0.0f;

	switch (DxInput::GetInstance()->checkKeyDownNo()) {
	case DIK_NUMPAD0:
		break;
	case DIK_NUMPAD1:
		meshIndex = 0;
		internalIndex = 0;
		Util::thetaCalc(&theta, -tfloat.Add(0.3f));
		break;
	case DIK_NUMPAD2:
		meshIndex = 4;
		internalIndex = 0;
		addMov = tfloat.Add(0.05f);
		break;
	case DIK_NUMPAD3:
		meshIndex = 0;
		internalIndex = 0;
		Util::thetaCalc(&theta, tfloat.Add(0.3f));
		break;
	case DIK_NUMPAD4:
		meshIndex = 1;
		internalIndex = 0;
		addMov = -tfloat.Add(0.1f);
		Util::thetaCalc(&theta, -tfloat.Add(0.2f));
		break;
	case DIK_NUMPAD5:
		break;
	case DIK_NUMPAD6:
		meshIndex = 1;
		internalIndex = 0;
		addMov = -tfloat.Add(0.1f);
		Util::thetaCalc(&theta, tfloat.Add(0.2f));
		break;
	case DIK_NUMPAD7:
		meshIndex = 1;
		internalIndex = 0;
		addMov = -tfloat.Add(0.1f);
		Util::thetaCalc(&theta, -tfloat.Add(0.1f));
		break;
	case DIK_NUMPAD8:
		meshIndex = 1;
		internalIndex = 0;
		addMov = -tfloat.Add(0.1f);
		break;
	case DIK_NUMPAD9:
		meshIndex = 1;
		internalIndex = 0;
		addMov = -tfloat.Add(0.1f);
		Util::thetaCalc(&theta, tfloat.Add(0.1f));
		break;
	}
	switch (DxInput::GetInstance()->checkKeyActionNo()) {
	case DIK_A:
		meshIndex = 0;
		internalIndex = 0;
		break;
	case DIK_S:
		meshIndex = 0;
		internalIndex = 1;
		break;
	case DIK_D:
		break;
	case DIK_F:
		meshIndex = 2;
		internalIndex = 0;
		break;
	case DIK_G:
		meshIndex = 3;
		internalIndex = 0;
		break;
	case DIK_H:
		meshIndex = 3;
		internalIndex = 1;
		break;
	case DIK_J:
		meshIndex = 3;
		internalIndex = 2;
		break;
	}
	using namespace CoordTf;

	VECTOR3 pAdd = { 0.0f,addMov,0.0f };
	MATRIX t = {};
	MatrixRotationZ(&t, theta);
	VectorMatrixMultiply(&pAdd, &t);
	cp[0].nextPos.x = cp[0].Pos.x + pAdd.x;
	cp[0].nextPos.y = cp[0].Pos.y + pAdd.y;
	cp[0].nextPos.z = cp[0].Pos.z + pAdd.z;
}
