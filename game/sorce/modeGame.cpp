#include"allMode.h"
#include <sstream>

bool makeChar(modeG* insMG, std::shared_ptr<CB> charPoint, const char* nameA)
{
	//SetUseASyncLoadFlag(true);
	charPoint->Initialize();
	charPoint->setCB(&insMG->charBox);
	charPoint->setGroundInf(&insMG->stage);
	charPoint->allColl = &insMG->mAllColl;
	charPoint->_valData = &insMG->_valData;
	charPoint->getInputKey(&insMG->_imputInf, &insMG->cameraDir);
	insMG->charBox.emplace(nameA, std::move(charPoint));
	return true;
}

bool loadAnimTs(bool *endSignal)
{
	int i = 0;
	while (!*endSignal && ProcessMessage() == 0)
	{
		ProcessMessage();
		ClearDrawScreen();/*
		if (i < 20) { DrawString(640, 360, "loading.", GetColor(255, 255, 255)); }
		else if (i < 40) { DrawString(640, 360, "loading..", GetColor(255, 255, 255)); }
		else if (i < 60) { DrawString(640, 360, "loading...", GetColor(255, 255, 255)); }
		else { i = 0; }
		i++;*/
		i++;
		DrawBox(0, 0, i, 20, GetColor(255, 255, 255), true);
		
		ScreenFlip();
	}
	*endSignal = true;
	for (int i = 0; i < 100; i++)
	{
		OutputDebugString(std::to_string(i).c_str());
	}
	return true;
}

bool	modeG::ASyncLoadAnim()
{
	SetDrawScreen(DX_SCREEN_BACK);
	OutputDebugString("a");
	return GetASyncLoadNum();
}

bool	modeG::Initialize()
{
	bool _endSignal = false;
	std::future<bool> f = std::async(std::launch::async, std::bind(loadAnimTs, &_endSignal));
	SetUseASyncLoadFlag(true);
	//_modelManager.modelImport("game/res/mapkari2/Heliport.mv1", 20.f, &stage);
	//_modelManager.modelImport("game/res/Bitch Slap Scene/BitchSlapHeliPort.mv1", 10.f, &stage);
	_modelManager.modelImport("game/res/karimap/Haikei demo2.mv1", 20.f, &stage);
	SetUseLighting(true);
	//ChangeLightTypePoint(VGet(0.f, 200.f, 0.f), 700.f, 0.0002f, 0.f, 0.f);
	SetUseZBuffer3D(TRUE);// Ｚバッファを有効にする
	SetWriteZBuffer3D(TRUE);// Ｚバッファへの書き込みを有効にする
	SetUseBackCulling(true);
	countTime = GetNowCount();
	MV1SetupCollInfo(stage.modelHandle, -1, 32, 6, 32);

	testAttackCap.underPos = VGet(0.f, 30.f, 0.f);
	testAttackCap.overPos = VGet(0.f, 170.f, 0.f);
	testAttackCap.r = 30.f;

	BGM = LoadSoundMem("game/res/BGM/DEATH TRIGGER.mp3");
	ChangeVolumeSoundMem(255 * (0.01 * 50), BGM);
	UIkari = LoadGraph("game/res/A.png");
	lockOnMarkerHandle = LoadGraph("game/res/lockOnMarker.png");
	//ASyncLoadAnim();
	makeChar(this, std::make_unique<PL>(), Char_PL);
	makeChar(this, std::make_unique<Boss>(), Char_BOSS1);
	// シャドウマップハンドルの作成
	ShadowMapHandle = MakeShadowMap(16384, 16384);
	// シャドウマップが想定するライトの方向もセット
	VECTOR lightDir = VGet(-3.0f, -4.0f, 0.0f);
	SetShadowMapLightDirection(ShadowMapHandle, lightDir);
	ChangeLightTypeDir(lightDir);
	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(ShadowMapHandle, VGet(-5000.0f, -1.0f, -5000.0f), VGet(5000.0f, 1000.0f, 5000.0f));
	_endSignal = true;
	f.get();
	SetUseASyncLoadFlag(false);
	insEfcHandle = LoadEffekseerEffect("game/res/Laser01.efkefc", 20.f);
	return true;
}

bool	modeG::Process()
{
	if (BGMdelay == 300)
	{
		PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);
		BGMdelay++;
	}
	else
	{
		BGMdelay++;
	}
	statusInf plStatus = { 0.f, 0.f, 0.f };
	for (auto i = charBox.begin(); i != charBox.end(); i++)
	{
		if (i->second->getType() == 1)
		{
			i->second->Process();
			i->second->gravity();
			plMI = i->second->getInf();
			plStatus = i->second->getStatus();
		}
		else
		{
			i->second->Process();
			bossMI = i->second->getInf();
			i->second->gravity();/*
			if (i->second->_modelInf.pos.y < -10)
			{
				i->second->_modelInf.pos = VGet(0.f, 0.f, 0.f);
			}*/
		}
	}

	if (_imputInf._gTrgp[XINPUT_BUTTON_RIGHT_THUMB] == 1)
	{
		isLockon ^= true;
	}


	cameraNtDir += _imputInf.rStickX / 5000;
	cameraHigh -= _imputInf.rStickY / 5000;

	useCommand();

	cameraMove();
	//cameraFor = VAdd(plMI.pos, VGet(0.f, 20.f, 0.f));
	SetCameraPositionAndTarget_UpVecY(cameraPos, cameraFor);
	//SetLightPositionHandle(LightHandle02, plMI.pos);
	Effekseer_Sync3DSetting();

	SetGlobalAmbientLight(GetColorF(0.3f, 0.3f, 0.3f, 0.0f));

	debugWardBox.emplace_back("cameraHigh  = " + std::to_string(cameraHigh));

	debugWardBox.emplace_back("自機のHP = " + std::to_string(plStatus.hitPoint));
	debugWardBox.emplace_back("自機のBP = " + std::to_string(plStatus.bloodPoint));
	debugWardBox.emplace_back("自機のVL = " + std::to_string(plStatus.vampireLevel));
	debugWardBox.emplace_back(std::to_string(
		(std::atan2(-_imputInf.lStickX, _imputInf.lStickY) * 180.f) / DX_PI_F));
	debugWardBox.emplace_back("現在のFPS値/" + std::to_string(FPS));
	debugWardBox.emplace_back("弱攻撃1のフレーム数/" + std::to_string(_valData.plAtkSpd1));
	debugWardBox.emplace_back("弱攻撃2のフレーム数/" + std::to_string(_valData.plAtkSpd2));
	debugWardBox.emplace_back("弱攻撃3のフレーム数/" + std::to_string(_valData.plAtkSpd3));
	debugWardBox.emplace_back("弱攻撃4のフレーム数/" + std::to_string(_valData.plAtkSpd4));
	debugWardBox.emplace_back("x." + std::to_string(static_cast<int>(plMI->pos.x))
		+ "/y." + std::to_string(static_cast<int>(plMI->pos.y))
		+ "/z." + std::to_string(static_cast<int>(plMI->pos.z)));

	// PC情報を取得します
	//getPcInf();

	collHitCheck();


	for (auto i = charBox.begin(); i != charBox.end(); ++i)
	{
		if (i->second->isDead == 2 || i->second->_modelInf.pos.y < -500)
		{
			if (i->second->type == 1)
			{//自機の死
				_modeServer->Add(std::make_unique<modeR>(_modeServer), 1, MODE_RESULT);
				Terminate();
				return false;
			}
			else
			{//それ以外の死
				i->second->Terminate();
				i = charBox.erase(i);
			}
		}
	}

	if (_imputInf._gTrgb[KEY_INPUT_M])
	{
		_modeServer->Add(std::make_unique<modeM>(_modeServer), 1, MODE_MENU);
	}

	if (_imputInf._gTrgb[KEY_INPUT_E])
	{
		PlayEffekseer3DEffect(insEfcHandle);
	}
	auto EFK2 = SetPosPlayingEffekseer3DEffect(insEfcHandle, 0, 20, 0);

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();
	return true;
}

bool	modeG::Render()
{

	// シャドウマップへの描画の準備
	ShadowMap_DrawSetup(ShadowMapHandle);

	for (auto i = charBox.begin(); i != charBox.end(); ++i)
	{
		i->second->Render(1);
		//i->second->_modelInf.isAnimEnd = _modelManager.modelRender(&i->second->_modelInf, 1, 1);
	}
	MV1DrawModel(stage.modelHandle);

	// シャドウマップへの描画を終了
	ShadowMap_DrawEnd();
	// 描画に使用するシャドウマップを設定
	SetUseShadowMap(0, ShadowMapHandle);

	for (auto i = charBox.begin(); i != charBox.end(); ++i)
	{
		i->second->Render(0);
		//i->second->_modelInf.isAnimEnd = _modelManager.modelRender(&i->second->_modelInf, 1, 0);
	}
	MV1DrawModel(stage.modelHandle);

	// 描画に使用するシャドウマップの設定を解除
	SetUseShadowMap(0, -1);


	if (isLockon)
	{
		SetUseZBuffer3D(FALSE);
		auto a = DrawBillboard3D(VSub(cameraFor, VGet(0, 40, 0)), .5, .5, 120, 0, lockOnMarkerHandle, true);
		SetUseZBuffer3D(TRUE);
	}

	debugWardBox.emplace_back(std::to_string(plMI->playTime));
	debugWardBox.emplace_back(std::to_string(plMI->playTimeOld));
	//debugWardBox.emplace_back("-------武器セット一覧-------");
	debugWardBox.emplace_back("-------コマンド一覧-------");
	debugWardBox.emplace_back("/debug(デバッグモードの切り替え)");
	debugWardBox.emplace_back("/menu(メニュー画面表示)");
	debugWardBox.emplace_back("/atkF1 ~ 4^フレーム数^(自機の1 ~ 4番目の攻撃モーションの総フレーム数変更)");
	debugWardBox.emplace_back("/atkFall^フレーム数^(自機のすべての攻撃モーションの総フレーム数変更)");
	for (int i = 0; i < debugWardBox.size() && debugMode; i++)
	{
		int sizeX, sizeY, lineCount;
		GetDrawStringSize(&sizeX, &sizeY, &lineCount, debugWardBox[i].c_str(), debugWardBox[i].length());
		DrawBox(10, 10 + 20 * i, 10 + sizeX, 10 + 20 * i + sizeY, GetColor(0, 0, 0), true);
		DrawString(10, 10 + 20 * i, debugWardBox[i].c_str(), GetColor(255, 255, 255));
	}
	debugWardBox.clear();

	int nowTime = GetNowCount();
	if (countTime + 1000 <= nowTime) { FPS = FPScount, FPScount = 0, countTime += 1000; }
	else { FPScount++; }

	//DrawLine3D(plMI->pos, VAdd(plMI->pos, VGet(0.f, 140.f, 0.f)), GetColor(0, 255, 0));

	for (int i = 0; i < mAllColl.size(); i++)
	{
		if (mAllColl[i].nonActiveTimeF <= 0.f)
		{
			MATRIX M = MV1GetFrameLocalWorldMatrix(mAllColl.at(i).capColl.parentModelHandle, mAllColl.at(i).capColl.frameNum);

			DrawCapsule3D(VTransform(mAllColl.at(i).capColl.underPos, M), VTransform(mAllColl.at(i).capColl.overPos, M), mAllColl[i].capColl.r, 8, GetColor(255, 0, 255), GetColor(0, 0, 0), false);
			if (mAllColl[i].capCollOld.r != -1)
			{
				DrawCapsule3D(mAllColl[i].capCollOld.underPos, mAllColl[i].capCollOld.overPos, mAllColl[i].capCollOld.r, 8, GetColor(255, 0, 255), GetColor(0, 0, 0), false);
			}
		}
	}

	if (charBox.find(Char_PL) != charBox.end())
	{
		DrawString(1000, 0, "自機のHP", GetColor(255.f, 0.f, 0.f));
		DrawString(1000, 20, std::to_string(charBox[Char_PL]->getStatus().hitPoint).c_str(), GetColor(255.f, 0.f, 0.f));
	}
	if (charBox.find(Char_BOSS1) != charBox.end())
	{
		DrawString(1000, 50, "騎士のHP", GetColor(255.f, 0.f, 0.f));
		DrawString(1000, 70, std::to_string(charBox[Char_BOSS1]->getStatus().hitPoint).c_str(), GetColor(255.f, 0.f, 0.f));
	}

	DrawGraph(0, 0, UIkari, true);
	DrawEffekseer3D();// Effekseerにより再生中のエフェクトを描画する。

	return true;
}

bool	modeG::collHitCheck()
{
	for (int i = 0; i < mAllColl.size(); i++)
	{
		if (mAllColl.at(i).nonActiveTimeF > 0) { mAllColl.at(i).nonActiveTimeF--; }
		else if (mAllColl.at(i).activeTimeF > 0) { mAllColl.at(i).activeTimeF--; }
		else { mAllColl.erase(mAllColl.begin() + i); }
	}

	for (auto i = charBox.begin(); i != charBox.end(); i++)
	{
		i->second->hitCheck(i->first.c_str());
	}

	return true;
}

bool	modeG::Terminate()
{
	StopSoundMem(BGM);
	MV1TerminateCollInfo(stage.modelHandle, -1);
	int a = InitGraph();
	for (auto i = charBox.begin(); i != charBox.end(); ++i) { i->second->Terminate(); }
	mAllColl.clear();
	charBox.clear();
	debugWardBox.clear();
	return true;
}

void modeG::cameraMove()
{
	if (isLockon)
	{
		auto EtoPdir = VSub(bossMI->pos, plMI->pos);
		cameraFor = VAdd(bossMI->pos, VGet(0.f, 250.f, 0.f));
		cameraPos = VAdd(VAdd(plMI->pos, VScale(VNorm(EtoPdir), -300.f)), VGet(0.f, 250.f, 0.f));
		cameraLockDir = (std::atan2(-EtoPdir.x, -EtoPdir.z) * 180.f) / DX_PI_F;
		cameraDir = cameraLockDir;
		charBox[Char_PL]->setCamDir(cameraLockDir);
	}
	else
	{
		float radian = cameraNtDir * DX_PI_F / 180.0f;
		auto InsV = VScale(VGet(sin(radian) * 100.f, 100, cos(radian) * 100.f), 3.f);
		auto _pos = cameraPos = VAdd(plMI->pos, InsV);
		cameraFor = VSub(plMI->pos, VSub(InsV, VGet(0.f, 300.f, 0.f)));
		if (cameraHigh < -260.f) { cameraHigh = -260.f; }
		if (cameraHigh > 60.f) { cameraHigh = 60.f; }
		cameraPos.y += cameraHigh;
		cameraFor.y -= cameraHigh;
		cameraDir = cameraNtDir;
		charBox[Char_PL]->setCamDir(-1.f);
	}
}

float getNum(std::string data)
{
	std::string input;
	std::stringstream b{ data };
	std::getline(b, input, '^');
	std::getline(b, input, '^');

	return std::stof(input);
}

int modeG::useCommand()
{
	if (!_imputInf._gTrgb[KEY_INPUT_RETURN]) { return 1; }
	DrawBox(10, 700, 1270, 730, GetColor(0, 0, 0), true);
	KeyInputString(10, 700, 141, _imputInf.wardBox, true);
	std::string _wardBox = _imputInf.wardBox;
	if (_wardBox.size() == 0) { return 1; }
	std::stringstream a{ _wardBox };
	std::string data, input;
	auto commandNum = std::count(_wardBox.cbegin(), _wardBox.cend(), '/');

	try
	{
		std::getline(a, data, '/');
		for (int i = 0; i < commandNum; i++)
		{
			std::getline(a, data, '/');

			if (data == "debug") { debugMode ? debugMode = false : debugMode = true;	return 2; }
			if (data == "menu") { _modeServer->Add(std::make_unique<modeM>(_modeServer), 1, MODE_MENU); }
			if (data.find("atkF1") != std::string::npos) { _valData.plAtkSpd1 = getNum(data); }
			if (data.find("atkF2") != std::string::npos) { _valData.plAtkSpd2 = getNum(data); }
			if (data.find("atkF3") != std::string::npos) { _valData.plAtkSpd3 = getNum(data); }
			if (data.find("atkF4") != std::string::npos) { _valData.plAtkSpd4 = getNum(data); }
			if (data.find("atkFall") != std::string::npos)
			{
				auto a = getNum(data);
				_valData.plAtkSpd1 = a, _valData.plAtkSpd2 = a, _valData.plAtkSpd3 = a, _valData.plAtkSpd4 = a;
			}
			if (data == "test")
			{
				OutputDebugString("succes");
				return 2;
			}
		}
	}
	catch (std::exception)
	{
		return -1;
	}
	return -1;
}

bool modeG::getPcInf() {
	char s[5][256];
	int i;
	double f[4];
	int A = GetPcInfo(s[0], s[1], s[2], &i, f, f + 1, s[3], s[4], f + 2, f + 3);
	for (int i = 0; i < 5; i++)
	{
		std::string a = s[i];
		debugWardBox.emplace(debugWardBox.begin() + i, a);
	}
	debugWardBox.emplace(debugWardBox.begin() + 5, "CPUspeed " + std::to_string(i));
	debugWardBox.emplace(debugWardBox.begin() + 6, "freeMemorySize " + std::to_string(f[0]));
	debugWardBox.emplace(debugWardBox.begin() + 7, "totalMemorySize " + std::to_string(f[1]));
	return true;
}