#include"allMode.h"
#include <sstream>

bool modeG::makeDefaultChar(modeG* insMG)
{
	auto insPL = std::make_unique<PL>();
	insPL->Initialize();
	insPL->setCB(&insMG->charBox);
	insPL->_valData = &insMG->_valData;
	insPL->getInputKey(&insMG->_imputInf, &insMG->cameraDir);
	insPL->setGroundInf(&insMG->stage);
	insPL->allColl = &insMG->mAllColl;
	insMG->charBox.emplace(Char_PL, std::move(insPL));

	auto boss = std::make_unique<Boss>();
	boss->Initialize();
	boss->setCB(&insMG->charBox);
	boss->setGroundInf(&insMG->stage);
	boss->allColl = &insMG->mAllColl;
	insMG->charBox.emplace(Char_BOSS1, std::move(boss));

	return true;
}

bool	modeG::ASyncLoad(bool (*loadDataClass)(modeG* insMG))
{
	loadDataClass(this);

	int i = 0;
	while (GetASyncLoadNum() > 0)
	{
		ClearDrawScreen();
		if (i < 20) { DrawString(640, 360, "loading.", GetColor(255, 255, 255)); }
		else if (i < 40) { DrawString(640, 360, "loading..", GetColor(255, 255, 255)); }
		else if (i < 60) { DrawString(640, 360, "loading...", GetColor(255, 255, 255)); }
		else { i = 0; }
		i++;
		ScreenFlip();
	}

	return GetASyncLoadNum();
}

bool	modeG::Initialize()
{
	_modelManager.modelImport("game/res/EL-Studio_Field(Grass)/フィールドスタジオ-草原.pmx", 7.0f, &stage);
	SetUseLighting(true);
	//ChangeLightTypePoint(VGet(0.f, 200.f, 0.f), 700.f, 0.0002f, 0.f, 0.f);
	ChangeLightTypeDir(VGet(-1.0f, -4.0f, 0.0f));
	SetUseZBuffer3D(TRUE);// Ｚバッファを有効にする
	SetWriteZBuffer3D(TRUE);// Ｚバッファへの書き込みを有効にする
	countTime = GetNowCount();
	MV1SetupCollInfo(stage.modelHandle, -1, 32, 8, 32);

	testAttackCap.underPos = VGet(0.f, 30.f, 0.f);
	testAttackCap.overPos = VGet(0.f, 170.f, 0.f);
	testAttackCap.r = 30.f;

	int a = ASyncLoad(makeDefaultChar);
	a += 1;
	return true;
}

bool	modeG::Process()
{
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
			i->second->gravity();
			if (i->second->_modelInf.pos.y < -10)
			{
				i->second->_modelInf.pos = VGet(0.f, 0.f, 0.f);
			}
		}
	}

	if (testAttackF <= 0)
	{
		attackColl Acoll;
		Acoll.isUseMat = false;
		Acoll.nonActiveTimeF = 100;
		Acoll.activeTimeF = 100;
		Acoll.attackChar = Char_BOSS1;
		Acoll.damage = 20.f;
		Acoll.capColl = testAttackCap;
		mAllColl.emplace_back(std::move(Acoll));
		testAttackF = 200;
	}
	else
	{
		testAttackF--;
	}

	//if (charBox[Char_PL]->_modelInf.pos.y <= -2000.f)
	//{
	//	charBox[Char_PL]->Terminate();
	//	charBox.erase(Char_PL);

	//	auto insPL = std::make_unique<PL>();
	//	insPL->Initialize();
	//	insPL->setCB(&charBox);
	//	insPL->_valData = &_valData;
	//	insPL->getInputKey(&_imputInf, &cameraDir);
	//	insPL->setGroundInf(&stage);
	//	insPL->allColl = &mAllColl;
	//	charBox.emplace(Char_PL, std::move(insPL));
	//}

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
	//// ＦＶキーでライトの距離減衰パラメータ０の値を変更
	//if (CheckHitKey(KEY_INPUT_F) == 1)
	//{
	//	Atten0 += 0.001f;
	//}
	//if (CheckHitKey(KEY_INPUT_V) == 1)
	//{
	//	Atten0 -= 0.001f;
	//}

	//// ＧＢキーでライトの距離減衰パラメータ１の値を変更
	//if (CheckHitKey(KEY_INPUT_G) == 1)
	//{
	//	Atten1 += 0.00001f;
	//}
	//if (CheckHitKey(KEY_INPUT_B) == 1)
	//{
	//	Atten1 -= 0.00001f;
	//}

	//// ＨＮキーでライトの距離減衰パラメータ２の値を変更
	//if (CheckHitKey(KEY_INPUT_H) == 1)
	//{
	//	Atten2 += 0.0000001f;
	//}
	//if (CheckHitKey(KEY_INPUT_N) == 1)
	//{
	//	Atten2 -= 0.0000001f;
	//}

	//// 距離減衰パラメータの値を補正
	//if (Atten0 < 0.0f) Atten0 = 0.0f;
	//if (Atten1 < 0.0f) Atten1 = 0.0f;
	//if (Atten2 < 0.0f) Atten2 = 0.0f;

	//// モデルの上空にポイントライトを設定
	//ChangeLightTypePoint(
	//	VGet(320.0f, 1000.0f, 200.0f),
	//	10000,
	//	Atten0,
	//	Atten1,
	//	Atten2);
	// グローバルアンビエントカラーを薄い黒(?)に変更
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

	return true;
}

bool	modeG::Render()
{
	for (auto i = charBox.begin(); i != charBox.end(); ++i) { i->second->Render(); }

	MV1DrawModel(stage.modelHandle);

	debugWardBox.emplace_back(std::to_string(plMI->playTime));
	debugWardBox.emplace_back(std::to_string(plMI->playTimeOld));
	debugWardBox.emplace_back("-------武器セット一覧-------");
	debugWardBox.emplace_back("No.0 左手:SwordBreaker 右手:RabbitBunker");
	debugWardBox.emplace_back("No.1 左手:無し　        右手:GunBlade");
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

	DrawLine3D(plMI->pos, VAdd(plMI->pos, VGet(0.f, 40.f, 0.f)), GetColor(0, 255, 0));

	for (int i = 0; i < mAllColl.size(); i++)
	{
		if (mAllColl[i].nonActiveTimeF <= 0.f)
		{
			MATRIX M = MV1GetFrameLocalWorldMatrix(mAllColl.at(i).capColl.parentModelHandle, mAllColl.at(i).capColl.frameNum);

			DrawCapsule3D(VTransform(mAllColl.at(i).capColl.underPos, M), VTransform(mAllColl.at(i).capColl.overPos, M), mAllColl[i].capColl.r, 8, GetColor(255, 0, 255), GetColor(0, 0, 0), false);
		}
	}

	DrawString(1000, 0, std::to_string(charBox[Char_PL]->getStatus().hitPoint).c_str(), GetColor(0.f, 0.f, 0.f));
	DrawString(1000, 50, std::to_string(charBox[Char_BOSS1]->getStatus().hitPoint).c_str(), GetColor(0.f, 0.f, 0.f));
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
	MV1TerminateCollInfo(stage.modelHandle, -1);
	int a = InitGraph();
	for (auto i = charBox.begin(); i != charBox.end(); ++i) { i->second->Terminate(); }
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