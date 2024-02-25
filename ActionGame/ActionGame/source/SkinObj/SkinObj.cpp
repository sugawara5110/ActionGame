///////////////////////////////////////////////SkinObj/////////////////////////////////////////////////////

#include "SkinObj.h"

SkinObj::~SkinObj() {
	ARR_DELETE(cp);
	ARR_DELETE(ap);
	S_DELETE(Meter[0]);
	S_DELETE(Meter[1]);
}

CoordTf::VECTOR3 SkinObj::getPos() {
	return cp[0].Pos;
}

float SkinObj::getTheta() {
	return theta;
}

CoordTf::VECTOR3 SkinObj::GetVertexPosition(int meshIndex, int verNum) {

	CoordTf::VECTOR3 v = SkinMesh::GetVertexPosition(meshIndex, verNum, 0, 0, 0, theta, 0, 0, scale);
	return { cp[0].Pos.x + v.x,cp[0].Pos.y + v.y,cp[0].Pos.z + v.z };
}

CollisionParameter* SkinObj::getCollisionParameter(int index) {
	return &cp[index];
}

int SkinObj::getNumCollisionParameter() {
	return numCp;
}

AttackParameter* SkinObj::getAttackParameter(int index) {
	return &ap[index];
}

int SkinObj::getNumAttackParameter() {
	return numAp;
}

void SkinObj::updateDamage(CoordTf::VECTOR2 adjustPos, CoordTf::VECTOR4 col) {

	if (!cp[0].down)return;
	CoordTf::VECTOR3 out = cp[0].Pos;
	PolygonData2D::Pos2DCompute(&out);

	CoordTf::VECTOR2 pos = { out.x + adjustPos.x, out.y + adjustPos.y };

	DxText::GetInstance()->
		UpDateValue(cp[0].damage, pos.x, pos.y, 30.0f, 3, col);
}

void SkinObj::createMeter() {
	for (int i = 0; i < 2; i++) {
		Meter[i] = NEW PolygonData2D();
		Meter[i]->GetVBarray2D(1);
		Meter[i]->CreateBox(0, 0.0f, 0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, true, true);
	}
}

void SkinObj::updateMeter(CoordTf::VECTOR2 adjustPos) {

	float h0 = 15.0f;
	float w0 = (float)(maxHP) / 10.0f + 2.0f;
	float h = 13.0f;
	float w = (float)(cp[0].HP) / 10.0f;

	CoordTf::VECTOR3 out = cp[0].Pos;
	PolygonData2D::Pos2DCompute(&out);//3D ¨ 2D ‚Å•ÏŠ·

	CoordTf::VECTOR2 pos = { out.x + adjustPos.x, out.y + adjustPos.y };

	Meter[0]->Update(pos.x, pos.y, 0.02f,
		0.0f, 0.3f, 1.0f, 1.0f, w0, h0);

	Meter[1]->Update(pos.x + 2.0f, pos.y + 1.0f, 0.01f,
		1.0f, 0.0f, 0.0f, 1.0f, w, h);
}

void SkinObj::drawMeter(int com) {
	Meter[0]->Draw(com);
	Meter[1]->Draw(com);
}