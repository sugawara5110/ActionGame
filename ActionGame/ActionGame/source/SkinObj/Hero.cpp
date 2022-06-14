///////////////////////////////////////////////Hero/////////////////////////////////////////////////////////

#include "Hero.h"

void Hero::create() {
	sk = std::make_unique<SkinMeshBloom>();
	sk->SetState(true, true);
	sk->GetFbx("mesh/p1/player1_all.fbx");//立ち, ダウン
	float end[2] = { 4000,1000 };
	sk->GetBuffer(1, 2, end);
	sk->createBloomParameter();

	sk->GetFbxSub("mesh/p1/player1_Run.fbx", 1);
	sk->GetBuffer_Sub(1, 400);

	sk->GetFbxSub("mesh/p1/player1_Hook.fbx", 2);
	sk->GetBuffer_Sub(2, 1000);

	sk->GetFbxSub("mesh/p1/player1_kick.fbx", 3);//キック3種類
	float end1[3] = { 3000,3000,3000 };
	sk->GetBuffer_Sub(3, 3, end1);

	sk->GetFbxSub("mesh/p1/player1_backWalk.fbx", 4);//後退
	sk->GetBuffer_Sub(4, 800);

	sk->SetVertex(true);
	sk->SetConnectStep(0, 10000);
	sk->SetConnectStep(1, 1000);
	sk->SetConnectStep(2, 10000);
	sk->SetConnectStep(3, 10000);
	sk->SetConnectStep(4, 1000);

	sk->CreateFromFBX();
	sk->CreateFromFBX_SubAnimation(1);
	sk->CreateFromFBX_SubAnimation(2);
	sk->CreateFromFBX_SubAnimation(3);
	sk->CreateFromFBX_SubAnimation(4);

	pos.as(0.0f, 0.0f, -15.0f);
}

static T_float tfloat;

void Hero::update() {

	float addMov = 0.0f;

	switch (DxInput::GetInstance()->checkKeyDownNo()) {
	case DIK_NUMPAD0:
		break;
	case DIK_NUMPAD1:
		meshIndex = 0;
		internalIndex = 0;
		Vector::thetaCalc(&theta, -tfloat.Add(0.3f));
		break;
	case DIK_NUMPAD2:
		meshIndex = 4;
		internalIndex = 0;
		addMov = tfloat.Add(0.05f);
		break;
	case DIK_NUMPAD3:
		meshIndex = 0;
		internalIndex = 0;
		Vector::thetaCalc(&theta, tfloat.Add(0.3f));
		break;
	case DIK_NUMPAD4:
		meshIndex = 1;
		internalIndex = 0;
		addMov = -tfloat.Add(0.1f);
		Vector::thetaCalc(&theta, -tfloat.Add(0.2f));
		break;
	case DIK_NUMPAD5:
		break;
	case DIK_NUMPAD6:
		meshIndex = 1;
		internalIndex = 0;
		addMov = -tfloat.Add(0.1f);
		Vector::thetaCalc(&theta, tfloat.Add(0.2f));
		break;
	case DIK_NUMPAD7:
		meshIndex = 1;
		internalIndex = 0;
		addMov = -tfloat.Add(0.1f);
		Vector::thetaCalc(&theta, -tfloat.Add(0.1f));
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
		Vector::thetaCalc(&theta, tfloat.Add(0.1f));
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
	pos.x += pAdd.x;
	pos.y += pAdd.y;
	pos.z += pAdd.z;
	float m = tfloat.Add(1.0f);
	if (sk->Update(meshIndex, m, pos, { 0,0,0,0 }, { 0,0,theta }, { 0.02f,0.02f,0.02f }, internalIndex)) {
		meshIndex = 0;
		internalIndex = 0;
	}
}
