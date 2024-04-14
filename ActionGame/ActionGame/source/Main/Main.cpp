///////////////////////////////////////////////Main/////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS
#include"../../../../Common/Direct3DWrapper/Dx12Process.h"
#include"../../../../Common/Direct3DWrapperOption/DxText.h"
#include"../../../../Common/Direct3DWrapperOption/Dx_ParticleData.h"
#include"../../../../Common/Direct3DWrapperOption/Dx_PostEffect.h"
#include"../../../../Common/Direct3DWrapperOption/Dx_Wave.h"
#include"../../../../Common/Direct3DWrapperOption/Dx_Bloom.h"
#include"../../../../Common/Window/Win.h"
#include"../../../../JPGLoader/JPGLoader.h"
#include"../../../../PNGLoader/PNGLoader.h"
#include"../../../../Common/SearchFile/SearchFile.h"
#include"../../../../Common/DirectShowWrapper/Movie.h"
#include"../../../../MultiThread/MultiThread.h"
#include"../../../../UserInterface/UserInterface.h"
#include"../../../../TIFLoader/TIFLoader.h"
#include"../../../../CreateGeometry/CreateGeometry.h"
#include"../../../../Common/DirectStorageWrapper/DStorage.h"
#include"../../../../T_float/T_float.h"
#include"../SkinObj/Hero.h"
#include"../SkinObj/Enemy.h"
#include"../CameraPos/CameraPos.h"
#include"../CollisionTest/CollisionTest.h"
#include"../Util/Util.h"
#include "Main.h"

#define CURRWIDTH 1024
#define CURRHEIGHT 768
using namespace CoordTf;

namespace {

	Vertex ver4[] =
	{
		{ {-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} ,{0.0f,0.0f}},
		{ {1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} ,{1.0f,0.0f}},
		{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} ,{1.0f,1.0f}},
		{ {-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} ,{0.0f,1.0f}},
	};

	UINT index6[] =
	{
		0,1,2,
		0,2,3
	};

	float fpz0 = 0.0f;
	float fpz2 = 0.0f;
	const int numPolygon = 4;
	int numAddInstancing = 6;
	float theta = 0;
	float thetaO = 0;
	T_float tfloat;
	//ウィンドウハンドル
	HWND hWnd;
	MSG msg;
	DxText* text;
	DxrRenderer* dxr;
	PolygonData* pd;
	PolygonData* pdbl;
	Hero* sk1;
	Enemy* en;
	Wave* wav;
	ParticleData* bil;
	PostEffect* blur;
	PolygonData* gr;
	PolygonData* soto;
	Movie* mov;
	Control* con;
	Dx_Bloom* bl;
	CameraPos cam;
	CollisionTest ctest;

	void update() {
		Dx_Light::SetDirectionLight(true);
		Dx_Light::DirectionLight(0.4f, 0.4f, -1.0f, 0.1f, 0.1f, 0.1f);
		float th = tfloat.Add(0.05f);
		theta = theta += th;
		if (theta > 360)theta = 0;
		MATRIX thetaZ;
		VECTOR3 light1 = { 0, -60, 35 };
		VECTOR3 light2 = { 0,60,35 };
		MatrixRotationZ(&thetaZ, 360 - theta);
		VectorMatrixMultiply(&light1, &thetaZ);
		VectorMatrixMultiply(&light2, &thetaZ);

		float th1 = tfloat.Add(0.05f);

		thetaO = thetaO += th1;
		if (thetaO > 360)thetaO = 0;

#if defined(DEBUG) || defined(_DEBUG) 

		DxText::GetInstance()->
			UpDateText(DxText::GetInstance()->getStr("人間の体力 %d", sk1->getCollisionParameter(0)->HP), 100, 40.0f, 30, { 1.0f, 1.0f, 1.0f, 1.0f });
		DxText::GetInstance()->
			UpDateText(DxText::GetInstance()->getStr("岩男の体力 %d", en->getCollisionParameter(0)->HP), 100, 70.0f, 30, { 1.0f, 1.0f, 1.0f, 1.0f });
		DxText::GetInstance()->
			UpDateText("岩男の当たり判定", 100, 110, 30, { 1, 1, 1, 1.0f });
		DxText::GetInstance()->
			UpDateText("岩男の攻撃判定", 100, 140, 30, { 1.0f, 1.0f, 0.3f, 1.0f });
		DxText::GetInstance()->
			UpDateText("人間の当たり判定", 100, 170, 30, { 0.0f, 1.0f, 0.0f, 1.0f });
		DxText::GetInstance()->
			UpDateText("人間の攻撃判定", 100, 200, 30, { 1.0f, 0.0f, 0.0f, 1.0f });

		CoordTf::VECTOR3 vPos = en->getAttackParameter(0)->Pos;
		float r0 = en->getAttackParameter(0)->Range;
		CoordTf::VECTOR3 vPos1 = en->getCollisionParameter(0)->Pos;
		float r1 = en->getCollisionParameter(0)->Range;

		CoordTf::VECTOR3 footPos = en->GetVertexPosition(1, 1600);//右足
		CoordTf::VECTOR3 footPos2 = en->GetVertexPosition(1, 2570);//左足

		if (footPos.z > fpz0)fpz0 = footPos.z;
		if (footPos2.z > fpz2)fpz2 = footPos2.z;
		
		CoordTf::VECTOR3 vPos2 = sk1->getAttackParameter(0)->Pos;
		CoordTf::VECTOR3 vPos3 = sk1->getAttackParameter(1)->Pos;
		float r2 = sk1->getAttackParameter(0)->Range;
		float r3 = sk1->getAttackParameter(1)->Range;
		CoordTf::VECTOR3 vPos4 = sk1->getCollisionParameter(0)->Pos;
		float r4 = sk1->getCollisionParameter(0)->Range;

		if (en->getAttackParameter(0)->effect)pd[0].Instancing(vPos,
			{ 0, 0, 0 },
			{ r0, r0, r0 }, { 0, 0, -0.7f, -0.6f });
		pd[0].Instancing(vPos1,
			{ 0, 0, 0 },
			{ r1, r1, r1 }, { 0, 0, 0, -0.6f });
		if (sk1->getAttackParameter(0)->effect)pd[0].Instancing(vPos2,
			{ 0, 0, 0 },
			{ r2, r2, r2 }, { 0, -1, -1, -0.6f });
		if (sk1->getAttackParameter(1)->effect)pd[0].Instancing(vPos3,
			{ 0, 0, 0 },
			{ r3, r3, r3 }, { 0, -1, -1, -0.6f });
		pd[0].Instancing(vPos4,
			{ 0, 0, 0 },
			{ r4, r4, r4 }, { -1, 0, -1, -0.6f });
		pd[0].Instancing(footPos,
			{ 0, 0, 0 },
			{ 10, 10, 10 }, { -1, 0, -1, -0.6f });
		pd[0].Instancing(footPos2,
			{ 0, 0, 0 },
			{ 10, 10, 10 }, { -1, 0, -1, -0.6f });
		pd[0].InstancingUpdate(
			0.2f,
			0.2f,
			4.0f);
#endif

		float r = (float)(rand() % 11) * 0.1f;
		float g = (float)(rand() % 11) * 0.1f;
		float b1 = (float)(rand() % 11) * 0.1f;

		pd[1].Instancing({ (float)-38, 10.0f, 25 },
			{ 0, 0, thetaO },
			{ 7, 7, 7 }, { -r, -g, -b1, 0 });

		pd[1].InstancingUpdate(
			0,
			4.0f);
		pd[1].setRefractiveIndex(0.1f);


		pdbl[0].Instancing({ light1.x, light1.y, light1.z },
			{ 0, 0, 0 },
			{ 1, 1, 1 }, { 0, 0, 0, 0 });
		pdbl[0].InstancingUpdate(
			0,
			4.0f);
		pdbl[1].Instancing({ light2.x, light2.y, light2.z },
			{ 0, 0, 0 },
			{ 1, 1, 1 }, { 0, 0, 0, 0 });
		pdbl[1].InstancingUpdate(
			0,
			4.0f);
		pdbl[0].setPointLight(0, true, 1000, { 0.0f, 0.01f, 0.000f });
		pdbl[1].setPointLight(0, true, 1000, { 0.0f, 0.01f, 0.000f });

		pd[2].Instancing({ 0, 0, 15 },
			{ 0, 0, thetaO },
			{ 4, 4, 4 }, { 0, 0, 0, 0 });
		pd[2].InstancingUpdate(
			0,
			4.0f);

		pd[3].Instancing({ 0, -20, 8 },
			{ 90, 0, 0 },
			{ 10, 10, 10 }, { 0, 0, 0, -0.4f });
		pd[3].InstancingUpdate(
			0,
			4.0f);
		pd[3].setRefractiveIndex(0.1f);

		float m = tfloat.Add(1.0f);

		sk1->update();
		en->update(sk1->getPos());

		float m2 = tfloat.Add(0.03f);
		m2 = 1.1f;//波紋
		//m2 = 0.3f;//sin
		wav->Instancing({ 0, 0, -20 },
			{ 0,0,0 },
			{ 140, 140, 15 }, { 0, 0, 0, -0.2f });
		wav->InstancingUpdate(1, m2,
			0.0f, 0.5f, 32, 32, 0.7f);

		wav->setRefractiveIndex(0.1f);

		gr->Instancing({ 0, 0, -28 },
			{ 0, 0, 0 },
			{ 360, 360, 15 }, { 0, 0, 0, 0 });

		gr->InstancingUpdate(
			0.0f, 0.1f, 1.0f, 4.0f);

		soto->Instancing({ 0, 0, 0 },
			{ 0, 0, 0 },
			{ 360, 360, 360 }, { 0, 0, 0, 0 });
		soto->InstancingUpdate(
			0,
			4.0f);

		bil->Update({ 5,0,0 }, { 0,0,0,-0.3f }, 50, 10.0f, false, 0);
		bil->setPointLight(0, true, 1000, { 0.001f, 0.0001f, 0.001f });
		bil->setPointLight(1, true, 1000, { 0.001f, 0.0001f, 0.001f });

		ctest.update();

		text->UpDate();
	}

	void draw() {
		int com = 1;
		Dx_CommandManager* cMa = Dx_CommandManager::GetInstance();
		Dx_CommandListObj* cObj = cMa->getGraphicsComListObj(com);
		cObj->Bigin();
		sk1->StreamOutput(com);//update
		en->StreamOutput(com);
		bil->StreamOutputBillboard(com);//update
		soto->StreamOutput(com);
		pd[0].StreamOutput(com);//update
		pd[1].StreamOutput(com);
		pdbl[0].StreamOutput(com);
		pdbl[1].StreamOutput(com);
		pd[2].StreamOutput(com);
		pd[3].StreamOutput(com);
		gr->StreamOutput(com);//update
		wav->StreamOutput(com);//update
		cObj->End();
	}

	void UpdateDxrDivideBuffer() {
		pd[0].UpdateDxrDivideBuffer();
		wav->UpdateDxrDivideBuffer();
		gr->UpdateDxrDivideBuffer();
	}

	void AS() {
		int com = 2;
		Dx_CommandManager* cMa = Dx_CommandManager::GetInstance();
		Dx_CommandListObj* cObj = cMa->getGraphicsComListObj(com);
		cObj->Bigin();
		dxr->update_g(com, 6);
		cObj->End();
	}

	void raytrace() {
		int com = 3;
		Dx_CommandManager* cMa = Dx_CommandManager::GetInstance();
		Dx_CommandListObj* cObj = cMa->getComputeComListObj(com);
		cObj->Bigin();
		dxr->raytrace_c(com);
		cObj->End();
	}

	void createTexture() {
		SearchFile* sf = NEW SearchFile(1);
		char** strE = NEW char* [2];
		strE[0] = "jpg";
		strE[1] = "png";
		sf->Search(L"./tex/*", 0, strE, 2);
		UINT numFile1 = sf->GetFileNum(0);
		InputParameter* ip = NEW InputParameter[numFile1];

		ARR_DELETE(strE);
		int cnt = 0;
		for (int k = 0; k < 1; k++) {
			for (UINT i = 0; i < sf->GetFileNum(k); i++) {
				char* str = sf->GetFileName(k, i);
				ip[cnt].width = 512;
				ip[cnt].height = 512;
				ip[cnt].format = DXGI_FORMAT_R8G8B8A8_UNORM;
				ip[cnt++].setFileName(str);
			}
		}
		ID3D12Device* device = Dx_Device::GetInstance()->getDevice();
		std::unique_ptr<OutputResource[]> R = DStorage::Load(device, ip, numFile1);
		DStorage::Delete();

		for (UINT i = 0; i < numFile1; i++) {
			Dx_TextureHolder::GetInstance()->createTextureArr(numFile1, i, Dx_Util::GetNameFromPass(ip[i].getFileName()),
				R[i].Subresource, ip[i].format,
				ip[i].width, ip[i].width * 4, ip[i].height, R[i].Texture);
			ARR_DELETE(R[i].Subresource);//struct OutputResourceに後でデストラクタ追加する
		}

		ARR_DELETE(ip);
		S_DELETE(sf);
	}

	int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

		Dx_Util::memory_leak_test();

		srand((unsigned)time(NULL));

		if (Createwindow(&hWnd, hInstance, nCmdShow, CURRWIDTH, CURRHEIGHT, L"ActionGame") == -1)return -1;

		//DirectX12ラッパー
		Dx_Device::InstanceCreate();
		Dx_Device::GetInstance()->createDevice();
		Dx_Device::GetInstance()->reportLiveDeviceObjectsOn();
		Dx_CommandManager::InstanceCreate();
		Dx_SwapChain::InstanceCreate();

		Dx_TextureHolder::InstanceCreate();
		Dx_TextureHolder* dx = Dx_TextureHolder::GetInstance();
		con = Control::GetInstance();
		Dx_Device* dev = Dx_Device::GetInstance();
		dev->dxrCreateResource();
		Dx_SwapChain* sw = Dx_SwapChain::GetInstance();

		Dx_CommandManager::setNumResourceBarrier(1024);

		sw->Initialize(hWnd, CURRWIDTH, CURRHEIGHT);

		sw->setPerspectiveFov(55, 1, 10000);
		Dx_Light::Initialize();
		Dx_ShaderHolder::CreateShaderByteCode();

		Dx_Light::setGlobalAmbientLight(0.01f, 0.01f, 0.01f);
		DxInput* di = DxInput::GetInstance();
		di->create(hWnd);
		di->SetWindowMode(true);
		di->setCorrectionX(1.015f);
		di->setCorrectionY(1.055f);

		createTexture();

		//文字入力
		DxText::InstanceCreate();
		text = DxText::GetInstance();
		mov = NEW Movie("tex/torch1.avi");

		pd = NEW PolygonData[numPolygon];
		pdbl = NEW PolygonData[2];
		bl = NEW Dx_Bloom();
		bil = NEW ParticleData();
		wav = NEW Wave();
		soto = NEW PolygonData();
		gr = NEW PolygonData();
		sk1 = NEW Hero();
		en = NEW Enemy();
		blur = NEW PostEffect();
		cam.init({ 0,0,-15.0f }, 0, { 0,150,30 });

		DivideArr arr[3];
		arr[0].distance = 90.0f;
		arr[0].divide = 16;//頂点数 3 → 3 * 6 = 18
		arr[1].distance = 50.0f;
		arr[1].divide = 32;//頂点数 3 → 3 * 3456 = 10368
		arr[2].distance = 36.0f;
		arr[2].divide = 64;//頂点数 3 → 3 * 13824 = 41472

		DivideArr arrsk[3];
		arrsk[0].distance = 90.0f;
		arrsk[0].divide = 1;//頂点数 3 → 3 * 6 = 18
		arrsk[1].distance = 50.0f;
		arrsk[1].divide = 1;
		arrsk[2].distance = 36.0f;
		arrsk[2].divide = 1;

		DivideArr arrWv[3];
		arrWv[0].distance = 1000.0f;
		arrWv[0].divide = 64;//頂点数 3 → 3 * 6 = 18
		arrWv[1].distance = 50.0f;
		arrWv[1].divide = 64;
		arrWv[2].distance = 36.0f;
		arrWv[2].divide = 64;

		wav->setDivideArr(arrWv, 3);
		gr->setDivideArr(arr, 3);
		pd[0].setDivideArr(arr, 3);
		pd[3].setDivideArr(arr, 3);

		bil->GetBufferBill(2);
		bil->SetVertex(0, { 100,0,7 });
		bil->SetVertex(1, { -100,0,18 });
		bil->TextureInit(256, 256);

		pd[0].GetVBarray(SQUARE, 1 + numAddInstancing);
		pd[1].GetVBarray(SQUARE, 1);
		pd[2].GetVBarray(SQUARE, 1);
		pd[3].GetVBarray(SQUARE, 1);
		pdbl[0].GetVBarray(SQUARE, 1);
		pdbl[1].GetVBarray(SQUARE, 1);

		VECTOR3 v3[] = { {},{2,0,0},{3,0,0} };
		VECTOR3 v3s[] = { {1,1,1},{1,1,1},{1,1,1} };

		Vertex* sv = (Vertex*)CreateGeometry::createSphere(10, 10, 1, v3, v3s, false);
		unsigned int* svI = CreateGeometry::createSphereIndex(10, 10, 1, false);

		Vertex* v = (Vertex*)CreateGeometry::createCube(1, v3, v3s, false);
		Vertex* vRev = (Vertex*)CreateGeometry::createCube(1, v3, v3s, true);

		unsigned int* ind = CreateGeometry::createCubeIndex(1, false);
		unsigned int* indr = CreateGeometry::createCubeIndex(1, true);
		pd[0].setVertex(sv, 11 * 11, svI, 10 * 10 * 6);
		pd[1].setVertex(v, 24, ind, 36);
		pd[2].setVertex(v, 24, ind, 36);
		pd[3].setVertex(ver4, 4, index6, 6);

		pdbl[0].setVertex(v, 24, ind, 36);
		pdbl[1].setVertex(v, 24, ind, 36);

		wav->GetVBarray(1);
		wav->SetVertex(ver4, 4, index6, 6);

		gr->GetVBarray(CONTROL_POINT, 1);
		gr->setVertex(v, 24, &ind[30], 6);

		soto->GetVBarray(SQUARE, 1);
		soto->setVertex(vRev, 24, indr, 36);

		ARR_DELETE(v);
		ARR_DELETE(vRev);
		ARR_DELETE(ind);
		ARR_DELETE(indr);
		ARR_DELETE(sv);
		ARR_DELETE(svI);

		Dx_CommandManager* cMa = Dx_CommandManager::GetInstance();
		Dx_CommandListObj* cObj = cMa->getGraphicsComListObj(0);

		cObj->Bigin();
		sk1->create();
		en->create();

		blur->ComCreateDepthOfField(0);

		int cNum1 = en->getNumCollisionParameter();
		int cNum2 = sk1->getNumCollisionParameter();
		std::unique_ptr<CollisionParameter* []> cPara;
		cPara = std::make_unique<CollisionParameter * []>(cNum1 + cNum2);
		int cCnt = 0;
		for (int i = 0; i < cNum1; i++) {
			cPara[cCnt++] = en->getCollisionParameter(i);
		}
		for (int i = 0; i < cNum2; i++) {
			cPara[cCnt++] = sk1->getCollisionParameter(i);
		}

		int aNum1 = en->getNumAttackParameter();
		int aNum2 = sk1->getNumAttackParameter();
		std::unique_ptr<AttackParameter* []> aPara;
		aPara = std::make_unique<AttackParameter * []>(aNum1 + aNum2);
		int aCnt = 0;
		for (int i = 0; i < aNum1; i++) {
			aPara[aCnt++] = en->getAttackParameter(i);
		}
		for (int i = 0; i < aNum2; i++) {
			aPara[aCnt++] = sk1->getAttackParameter(i);
		}
		ctest.init(cPara.get(), cNum1 + cNum2, aPara.get(), aNum1 + aNum2);

		bil->setMaterialType(EMISSIVE);
		bil->CreateBillboard(0, true, true);

		wav->SetCol(0.5f, 0.5f, 0.5f, 1, 1, 1);
		wav->setMaterialType((MaterialType)(TRANSLUCENCE | METALLIC));
		wav->Create(0, -1/*dx->GetTexNumber("maru.png")*//*dx->GetTexNumber("wave.jpg")*/, -1, true, true, 0.05f, 64, true);

		//gr->setMaterialType(METALLIC);
		gr->Create(0, true, dx->GetTexNumber("ground3.jpg"),
			dx->GetTexNumber("ground3Nor.png"),
			dx->GetTexNumber("ground3.jpg"), true, true, true);
		//gr->getParameter()->updateF = true;

		pd[0].Create(0, true, dx->GetTexNumber("siro.png"),
			-1, -1, true, true, false);
		pd[1].setMaterialType(METALLIC);
		pd[1].Create(0, true, dx->GetTexNumber("ceiling5.jpg"), -1/*dx->GetTexNumber("ceiling5Nor.png")*/, -1, false, true);
		pdbl[0].setMaterialType(EMISSIVE);
		pdbl[0].Create(0, false, dx->GetTexNumber("siro.png"), -1, -1, false, false);
		pdbl[1].setMaterialType(EMISSIVE);
		pdbl[1].Create(0, false, dx->GetTexNumber("siro.png"), -1, -1, false, false);
		pd[2].Create(0, true, dx->GetTexNumber("boss_magic.png"), -1, -1, true, true);
		pd[3].setMaterialType(TRANSLUCENCE);
		pd[3].Create(0, true, dx->GetTexNumber("siro.png"), dx->GetTexNumber("wall1Nor.png"), -1, false, false, false);

		soto->setMaterialType((MaterialType)(DIRECTIONLIGHT | NONREFLECTION));
		soto->Create(0, true, dx->GetTexNumber("wall1.jpg"),
			dx->GetTexNumber("wall1Nor.png"),
			dx->GetTexNumber("wall1.jpg"), false, false);

		cObj->End();
		cMa->RunGpu();
		cMa->WaitFence();

		int numEnemy = en->getNumMesh();
		int numMesh1 = sk1->getNumMesh();
		//UINT numMT = numPolygon + numMesh1 + numEnemy + 4 + 2;

		int blId[4] = {};

		std::vector<ParameterDXR*> pdx;

		for (int i = 0; i < numEnemy; i++)
			pdx.push_back(en->getParameter(i));

		for (int i = 0; i < numPolygon; i++)
			pdx.push_back(pd[i].getParameter());

		for (int i = 0; i < numMesh1; i++)
			pdx.push_back(sk1->getParameter(i));

		blId[0] = 0;
		blId[1] = blId[2] = blId[3] = numEnemy + (numPolygon + numAddInstancing) + numMesh1;

		pdx.push_back(wav->getParameter());
		pdx.push_back(bil->getParameter());
		blId[1]++;
		pdx.push_back(gr->getParameter());
		pdx.push_back(soto->getParameter());
		pdx.push_back(pdbl[0].getParameter());
		pdx.push_back(pdbl[1].getParameter());
		blId[2] += 4;
		blId[3] += 5;
		dxr = NEW DxrRenderer();
		dxr->initDXR(pdx, 10);

		std::vector<std::vector<uint32_t>> gausu = {
			{256, 128},
			{128, 64,32},
			{256, 128},
			{256, 128}
		};
		std::vector<float> sigma = { 10,10,10,10 };
		std::vector<Dx_Bloom::GaussianType> type = { Dx_Bloom::GaussianType::Type1D, Dx_Bloom::GaussianType::Type1D,
			Dx_Bloom::GaussianType::Type1D, Dx_Bloom::GaussianType::Type1D };

		bl->Create(0, 4, dxr->getInstanceIdMap(), &sigma, &gausu, &type);

		UINT gaSize[7] = {
			512,256,128,64,32,16,8
		};



		MultiThread th;
		th.setFunc(update);
		th.setFunc(draw);
		th.setFunc(AS);
		th.setFunc(raytrace);
		th.start();
		while (1) {
			if (!DispatchMSG(&msg)) {
				break;
			}
			T_float::GetTime(hWnd);
			T_float::GetTimeUp(hWnd);

			cObj->Bigin();
			bil->SetTextureMPixel(0, mov->GetFrame(256, 256, 50));
			cObj->End();

			dxr->allSwapIndex();

			cam.update(sk1->getPos(), sk1->getTheta(), 0.03f, 1.0f, { 0.0f,150.0f,30.0f });

			th.wait();

			cMa->RunGpuCom();
			cMa->RunGpu();
			cMa->WaitFenceCom();
			cMa->WaitFence();
			UpdateDxrDivideBuffer();

			Dx_CommandListObj* cObj4 = cMa->getGraphicsComListObj(4);

			cObj4->Bigin();
			dxr->copyBackBuffer(4);
			dxr->copyDepthBuffer(4);
			cObj4->End();
			cMa->RunGpu();
			cMa->WaitFence();

			Dx_Bloom::InstanceParam pa1 = {};
			pa1.bloomStrength = 10.0f;
			pa1.EmissiveInstanceId = blId[0];
			pa1.thresholdLuminance = 0.0f;
			Dx_Bloom::InstanceParam pa2 = {};
			pa2.bloomStrength = 10.0f;
			pa2.EmissiveInstanceId = blId[1];
			pa2.thresholdLuminance = 0.0f;
			Dx_Bloom::InstanceParam pa3 = {};
			pa3.bloomStrength = 10.0f;
			pa3.EmissiveInstanceId = blId[2];
			pa3.thresholdLuminance = 0.0f;
			Dx_Bloom::InstanceParam pa4 = {};
			pa4.bloomStrength = 10.0f;
			pa4.EmissiveInstanceId = blId[3];
			pa4.thresholdLuminance = 0.0f;

			std::vector<Dx_Bloom::InstanceParam>pa = { pa1,pa2,pa3,pa4 };

			bl->Compute(4, pa, sw->GetRtBuffer());

			cObj4->Bigin();
			blur->ComputeDepthOfField(4, true, 500, 0.992f, 0.00f);
			//focusDepth 1.0=iwao, 0.992=ningen 
			sw->BiginDraw(4, false);
			sk1->drawMeter(4);
			en->drawMeter(4);
			text->Draw(4);
			sw->EndDraw(4);
			cObj4->End();
			cMa->RunGpu();
			cMa->WaitFence();
			sw->DrawScreen();
		}
		th.end();

		cMa->WaitFence();
		cMa->WaitFenceCom();
		S_DELETE(blur);
		S_DELETE(gr);
		S_DELETE(bil);
		S_DELETE(mov);
		S_DELETE(wav);
		S_DELETE(soto);
		S_DELETE(sk1);
		S_DELETE(en);
		ARR_DELETE(pd);
		ARR_DELETE(pdbl);
		S_DELETE(bl);
		S_DELETE(dxr);
		DxInput::DeleteInstance();
		Control::DeleteInstance();
		DxText::DeleteInstance();
		Dx_SwapChain::DeleteInstance();
		Dx_TextureHolder::DeleteInstance();
		Dx_CommandManager::DeleteInstance();
		Dx_Device::DeleteInstance();
		return 0;
	}
}

int Main::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}




