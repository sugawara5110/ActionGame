//////////////////////////////////////ActionGame/////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS
#include"../../../Common/Direct3DWrapper/Dx12Process.h"
#include"../../../Common/Direct3DWrapperOption/DxText.h"
#include"../../../Common/Direct3DWrapperOption/Dx_ParticleData.h"
#include"../../../Common/Direct3DWrapperOption/Dx_PostEffect.h"
#include"../../../Common/Direct3DWrapperOption/Dx_Wave.h"
#include"../../../Common/Window/Win.h"
#include"../../../JPGLoader/JPGLoader.h"
#include"../../../PNGLoader/PNGLoader.h"
#include"../../../Common/SearchFile/SearchFile.h"
#include"../../../Common/DirectShowWrapper/Movie.h"
#include"../../../MultiThread/MultiThread.h"
#include"../../../UserInterface/UserInterface.h"
#include"../../../TIFLoader/TIFLoader.h"
#include"../../../CreateGeometry/CreateGeometry.h"
#include"../../../Common/DirectStorageWrapper/DStorage.h"
#include"SkinObj/Hero.h"
#include"SkinObj/Enemy.h"
#include"CameraPos/CameraPos.h"
#include "Vector/Vector.h"

#define CURRWIDTH 1024
#define CURRHEIGHT 768
using namespace CoordTf;

void update();
void draw();
void AS();
void raytrace();
void UpdateDxrDivideBuffer();

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

	const int numPolygon = 4;
	float theta = 0;
	float thetaO = 0;
	T_float tfloat;
	int parCnt = 0;
	bool parSwich = false;
	//ウィンドウハンドル
	HWND hWnd;
	MSG msg;
	Dx12Process* dx;
	DxText* text;
	DXR_Basic* dxr;
	PolygonData* pd;
	PolygonDataBloom* pdbl;
	Hero* sk1;
	Enemy* en;
	Wave* wav;
	ParticleData* bil;
	PolygonData* gr;
	PolygonData* soto;
	Movie* mov;
	Control* con;
	VariableBloom* variableBloom;
	CameraPos cam;
}

#include <vector>
#include <memory>

void createTexture2(Dx12Process* dx) {
	SearchFile* sf = new SearchFile(1);
	char** strE = new char* [2];
	strE[0] = "jpg";
	strE[1] = "png";
	sf->Search(L"./tex/*", 0, strE, 2);
	UINT numFile1 = sf->GetFileNum(0);
	InputParameter* ip = new InputParameter[numFile1];

	ARR_DELETE(strE);
	int cnt = 0;
	for (int k = 0; k < 1; k++) {
		for (int i = 0; i < sf->GetFileNum(k); i++) {
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

	for (int i = 0; i < numFile1; i++) {
		dx->createTextureArr(numFile1, i, Dx_Util::GetNameFromPass(ip[i].getFileName()),
			R[i].Subresource, ip[i].format,
			ip[i].width, ip[i].width * 4, ip[i].height, R[i].Texture);
	}

	ARR_DELETE(ip);
	S_DELETE(sf);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	srand((unsigned)time(NULL));

	if (Createwindow(&hWnd, hInstance, nCmdShow, CURRWIDTH, CURRHEIGHT, L"ActionGame") == -1)return -1;

	float aaa = Vector::getThetaXY({ 0,-1}, { 0,0});
	// { 1,0}, { 0,0} = 0      
	// { 0,1}, { 0,0} = 90     
	// { -1,0}, { 0,0} = 180   
	// { 0,-1}, { 0,0} = 270   
	//DirectX12ラッパー
	Dx12Process::InstanceCreate();
	dx = Dx12Process::GetInstance();
	con = Control::GetInstance();
	dx->dxrCreateResource();
	dx->setPerspectiveFov(45, 0, 300);
	dx->setNumResourceBarrier(1024);
	//dx->reportLiveDeviceObjectsOn();
	dx->setNumRtv(3);
	dx->Initialize(hWnd, CURRWIDTH, CURRHEIGHT);
	//dx->wireFrameTest(true);
	//dx->NorTestOn();
	dx->setGlobalAmbientLight(0.0600f, 0.0600f, 0.0600f);
	DxInput* di = DxInput::GetInstance();
	di->create(hWnd);
	di->SetWindowMode(true);
	di->setCorrectionX(1.015f);
	di->setCorrectionY(1.055f);

	createTexture2(dx);

	//文字入力
	DxText::InstanceCreate();
	text = DxText::GetInstance();
	mov = new Movie("tex/torch1.avi");

	pd = new PolygonData[numPolygon];
	pdbl = new PolygonDataBloom[2];
	variableBloom = new VariableBloom();
	bil = new ParticleData();
	wav = new Wave();
	soto = new PolygonData();
	gr = new PolygonData();
	sk1 = new Hero();
	en = new Enemy();
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
	bil->SetVertex(0, { 50,0,7 });
	bil->SetVertex(1, { -50,0,18 });
	bil->TextureInit(256, 256);

	pd[0].GetVBarray(CONTROL_POINT, 1);
	pd[1].GetVBarray(SQUARE, 3);
	pdbl[0].GetVBarray(SQUARE, 1);
	pdbl[1].GetVBarray(SQUARE, 1);
	pd[2].GetVBarray(SQUARE, 1);
	pd[3].GetVBarray(CONTROL_POINT, 1);

	VECTOR3 v3[] = { {},{2,0,0},{3,0,0} };
	VECTOR3 v3s[] = { {1,1,1},{1,1,1},{1,1,1} };

	Vertex* sv = (Vertex*)CreateGeometry::createSphere(10, 10, 3, v3, v3s, false);
	unsigned int* svI = CreateGeometry::createSphereIndex(10, 10, 3);

	Vertex* v = (Vertex*)CreateGeometry::createCube(2, v3, v3s, false);
	Vertex* vRev = (Vertex*)CreateGeometry::createCube(1, v3, v3s, true);

	unsigned int* ind = CreateGeometry::createCubeIndex(2);
	pd[0].setVertex(v, 24 * 2, ind, 36 * 2);
	pd[1].setVertex(sv, 11 * 11 * 3, svI, 10 * 10 * 6 * 3);
	pdbl[0].setVertex(v, 24, ind, 36);
	pdbl[1].setVertex(v, 24, ind, 36);
	pd[2].setVertex(v, 24, ind, 36);
	pd[3].setVertex(ver4, 4, index6, 6);

	wav->SetCommandList(0);
	wav->GetVBarray(1);
	//wav->SetVertex(v, 24, &ind[30], 6);
	wav->SetVertex(ver4, 4, index6, 6);

	gr->GetVBarray(CONTROL_POINT, 1);
	gr->setVertex(v, 24, &ind[30], 6);

	soto->GetVBarray(SQUARE, 1);
	soto->setVertex(vRev, 24, ind, 36);

	ARR_DELETE(sv);
	ARR_DELETE(svI);
	ARR_DELETE(v);
	ARR_DELETE(vRev);
	ARR_DELETE(ind);

	dx->Bigin(0);
	sk1->create();
	en->create();
	UINT gaSize[7] = {
				512,256,128,64,32,16,8
	};
	BloomParameter** bp;
	int numskBl = sk1->getNumBloomParameter();
	int numenBl = en->getNumBloomParameter();
	bp = new BloomParameter * [2 + numenBl + numskBl];
	int bpIndex = 0;
	bp[bpIndex++] = pdbl[0].getBloomParameter();
	bp[bpIndex++] = pdbl[1].getBloomParameter();
	for (int i = 0; i < numskBl; i++)
		bp[bpIndex++] = sk1->getBloomParameter(i);

	for (int i = 0; i < numenBl; i++)
		bp[bpIndex++] = en->getBloomParameter(i);

	variableBloom->SetCommandList(0);
	variableBloom->init(bp, bpIndex, true, 7, gaSize);
	bil->setMaterialType(EMISSIVE);
	bil->CreateBillboard(true, true);
	
	wav->SetCol(0.5f, 0.5f, 0.5f, 1, 1, 1);
	wav->setMaterialType((MaterialType)(TRANSLUCENCE | METALLIC));
	wav->Create(-1/*dx->GetTexNumber("maru.png")*//*dx->GetTexNumber("wave.jpg")*/, -1, true, true, 0.05f, 64.0f, true);

	//gr->setMaterialType(METALLIC);
	gr->Create(true, dx->GetTexNumber("ground3.jpg"),
		dx->GetTexNumber("ground3Nor.png"),
		dx->GetTexNumber("ground3.jpg"), true, true, true);
	//gr->getParameter()->updateF = true;

	pd[0].Create(true, dx->GetTexNumber("wall1.jpg"),
		dx->GetTexNumber("wall1Nor.png"),
		dx->GetTexNumber("wall1.jpg"), false, false, false);
	pd[1].setMaterialType(METALLIC);
	pd[1].Create(true, dx->GetTexNumber("ceiling5.jpg"), -1/*dx->GetTexNumber("ceiling5Nor.png")*/, -1, false, true);
	pdbl[0].setMaterialType(EMISSIVE);
	pdbl[0].Create(false, dx->GetTexNumber("siro.png"), -1, -1, false, false);
	pdbl[1].setMaterialType(EMISSIVE);
	pdbl[1].Create(false, dx->GetTexNumber("siro.png"), -1, -1, false, false);
	pd[2].Create(true, dx->GetTexNumber("boss_magic.png"), -1, -1, true, true);
	pd[3].setMaterialType(TRANSLUCENCE);
	pd[3].Create(true, dx->GetTexNumber("siro.png"), dx->GetTexNumber("wall1Nor.png"), -1, false, false, false);

	soto->setMaterialType((MaterialType)(DIRECTIONLIGHT | NONREFLECTION));
	soto->Create(true, dx->GetTexNumber("wall1.jpg"),
		dx->GetTexNumber("wall1Nor.png"),
		dx->GetTexNumber("wall1.jpg"), false, false);

	dx->End(0);
	dx->RunGpu();
	dx->WaitFence();

	int numEnemy = en->getNumParameterDXR();
	int numMesh1 = sk1->getNumParameterDXR();
	UINT numMT = numPolygon + numMesh1 + numEnemy + 4 + 2;

	ParameterDXR** pdx = new ParameterDXR * [numMT];

	int dxrCnt = 0;

	for (int i = 0; i < numEnemy; i++)
		pdx[dxrCnt++] = en->getParameterDXR(i);

	for (int i = 0; i < numPolygon; i++)
		pdx[dxrCnt++] = pd[i].getParameter();

	for (int i = 0; i < numMesh1; i++)
		pdx[dxrCnt++] = sk1->getParameterDXR(i);

	pdx[dxrCnt++] = wav->getParameter();
	pdx[dxrCnt++] = bil->getParameter();
	pdx[dxrCnt++] = gr->getParameter();
	pdx[dxrCnt++] = soto->getParameter();
	pdx[dxrCnt++] = pdbl[0].getParameter();
	pdx[dxrCnt++] = pdbl[1].getParameter();

	dxr = new DXR_Basic();
	dxr->initDXR(numMT, pdx, 10);

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

		dx->Bigin(0);
		bil->SetTextureMPixel(0, mov->GetFrame(256, 256, 50));
		dx->End(0);

		dxr->allSwapIndex();

		//dx->Cameraset({ cam1.x, cam1.y, cam1.z }, { 0, 0, 0 });
		cam.update(sk1->getPos(), sk1->getTheta(), 0.03, 1.0, { 0,150,30 });

		th.wait();

		dx->RunGpuCom();
		dx->RunGpu();
		dx->WaitFenceCom();
		dx->WaitFence();
		UpdateDxrDivideBuffer();

		dx->Bigin(4);
		dxr->copyBackBuffer(4);
		dxr->copyDepthBuffer(4);
		dx->End(4);
		dx->RunGpu();
		dx->WaitFence();

		dx->Bigin(4);
		dx->BiginDraw(4, false);

		variableBloom->ComputeBloom(4, true);

		dx->EndDraw(4);
		dx->End(4);
		dx->RunGpu();
		dx->WaitFence();

		dx->Bigin(4);
		dx->BiginDraw(4, false);
		text->Draw(4);
		dx->EndDraw(4);
		dx->End(4);
		dx->RunGpu();
		dx->WaitFence();
		dx->DrawScreen();
	}
	th.end();

	dx->WaitFence();
	dx->WaitFenceCom();
	S_DELETE(gr);
	S_DELETE(bil);
	S_DELETE(mov);
	S_DELETE(wav);
	S_DELETE(soto);
	S_DELETE(sk1);
	S_DELETE(en);
	ARR_DELETE(bp);
	ARR_DELETE(pd);
	ARR_DELETE(pdbl);
	S_DELETE(variableBloom);
	ARR_DELETE(pdx);
	S_DELETE(dxr);
	DxInput::DeleteInstance();
	Control::DeleteInstance();
	DxText::DeleteInstance();
	Dx12Process::DeleteInstance();
	return 0;
}

void update() {
	dx->SetDirectionLight(true);
	dx->DirectionLight(0.4f, 0.4f, -1.0f, 0.2f, 0.2f, 0.2f);
	float th = tfloat.Add(0.05f);
	theta = theta += th;
	if (theta > 360)theta = 0;
	MATRIX thetaZ;
	VECTOR3 light1 = { 0, -40, 15 };
	VECTOR3 light2 = { 0,40,15 };
	MatrixRotationZ(&thetaZ, 360 - theta);
	VectorMatrixMultiply(&light1, &thetaZ);
	VectorMatrixMultiply(&light2, &thetaZ);

	float th1 = tfloat.Add(0.05f);

	thetaO = thetaO += th1;
	if (thetaO > 360)thetaO = 0;

	pd[0].Instancing({ (float)35, 0, 20 },
		{ 0, 0, thetaO },
		{ 7, 7, 7 }, { 0, 0, 0, 0 });
	pd[0].InstancingUpdate(
		0.2f,
		0.2f,
		4.0f);
	float r = (float)(rand() % 11) * 0.1f;
	float g = (float)(rand() % 11) * 0.1f;
	float b1 = (float)(rand() % 11) * 0.1f;

	for (int b = 0; b < 3; b++) {
		pd[1].Instancing({ (float)-38, b * 10.0f, 25 },
			{ 0, 0, thetaO },
			{ 7, 7, 7 }, { -r, -g, -b1, 0 });
	}
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
	pdbl[0].setPointLight(0, true, 1000, { 0.1f,0.1f,0.000f });
	pdbl[1].setPointLight(0, true, 1000, { 0.1f,0.1f,0.000f });

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
		{ 0,0,thetaO },
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

	float sp1 = tfloat.Add(0.03f);
	parCnt += sp1;
	if (parCnt > 3) {
		parCnt = 0; parSwich = true;
	}
	float sp = tfloat.Add(0.03f);
	if (1) {
		parSwich = false;
		bil->Update({ 5,0,0 }, { 0,0,0,-0.3f }, 50, 10.0f, false, 0);
		bil->setPointLight(0, true, 1000, { 0.5f,0.4f,0.000f });
		bil->setPointLight(1, true, 1000, { 0.5f,0.4f,0.000f });
	}
	else {
		bil->DrawOff();
	}
	text->UpDate();
}

void draw() {
	int com = 1;
	dx->Bigin(com);
	dx->BiginDraw(com);
	sk1->setBloomParameter(0, 0.0f, 0.0f);
	en->setBloomParameter(0, 10.0f, 0.0f);
	pdbl[0].setBloomParameter(20.0f, 0.0f);
	pdbl[0].DrawPreparation();
	pdbl[1].setBloomParameter(5.0f, 0.0f);
	pdbl[1].DrawPreparation();
	soto->Draw(com);
	pd[1].Draw(com);
	pd[2].Draw(com);
	gr->Draw(com);
	wav->Draw(com);
	pd[0].Draw(com);
	pd[3].Draw(com);
	bil->DrawBillboard(com);
	variableBloom->Draw(com);
	dx->EndDraw(com);

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
	dx->End(com);
}

void UpdateDxrDivideBuffer() {
	pd[0].UpdateDxrDivideBuffer();
	wav->UpdateDxrDivideBuffer();
	gr->UpdateDxrDivideBuffer();
}

void AS() {
	int com = 2;
	dx->Bigin(com);
	dxr->update_g(com, 6);
	dx->End(com);
}

void raytrace() {
	int com = 3;
	dx->BiginCom(com);
	dxr->raytrace_c(com);
	dx->EndCom(com);
}