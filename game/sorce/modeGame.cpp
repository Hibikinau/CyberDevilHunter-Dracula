#include"allMode.h"
#include <sstream>

bool modeG::makePL()
{
	auto insPL = std::make_unique<PL>();
	insPL->Initialize();
	insPL->setCB(&charBox);
	insPL->_valData = &_valData;
	insPL->getInputKey(&_imputInf, &cameraDir);
	charBox.emplace(Char_PL, std::move(insPL));

	return true;
}

bool	modeG::Initialize()
{
	DrawString(640, 360, "loading...", GetColor(255, 255, 255));
	ScreenFlip();
	_modelManager.modelImport("game/res/kariStage/Haikei demo.mv1", 20.0f, &stage);
	SetUseLighting(false);
	SetUseZBuffer3D(TRUE);// Ｚバッファを有効にする
	SetWriteZBuffer3D(TRUE);// Ｚバッファへの書き込みを有効にする
	countTime = GetNowCount();
	//LightHandle02 = CreatePointLightHandle(VGet(0.f, 0.f, 0.f), 2000.f, 0.f, 0.f, 0.f);

	makePL();

	auto boss = std::make_unique<Boss>();
	boss->Initialize();
	boss->setCB(&charBox);
	charBox.emplace(Char_BOSS1, std::move(boss));

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
		else { i->second->Process(); bossMI = i->second->getInf(); }
	}

	if (charBox[Char_PL]->_statusInf.hitPoint <= 0)
	{
		charBox[Char_PL]->Terminate();
		charBox.erase(Char_PL);

		makePL();
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
	//SetLightPositionHandle(LightHandle02, plMI->pos);

	debugWardBox.emplace_back("自機のHP = " + std::to_string(plStatus.hitPoint));
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
	getPcInf();

	return true;
}

bool	modeG::Render()
{
	for (auto i = charBox.begin(); i != charBox.end(); i++) { i->second->Render(); }

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
	debugWardBox.emplace_back("/weponset^武器セットの番号^(武器セットの番号)");
	debugWardBox.emplace_back("/changeX^入れ替え技の名前^(charge / senpuu)");
	debugWardBox.emplace_back("/changeY^入れ替え技の名前^(charge / senpuu)");
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
	return true;
}

bool	modeG::Terminate()
{
	int a = InitGraph();
	for (auto i = charBox.begin(); i != charBox.end(); i++) { i->second->Terminate(); }
	charBox.clear();
	debugWardBox.clear();
	return true;
}

void modeG::cameraMove()
{
	if (isLockon)
	{
		auto EtoPdir = VSub(bossMI->pos, plMI->pos);
		cameraFor = bossMI->pos;
		cameraPos = VAdd(VAdd(plMI->pos, VScale(VNorm(EtoPdir), -300.f)), VGet(0.f, 250.f, 0.f));
		cameraLockDir = (std::atan2(-EtoPdir.x, -EtoPdir.z) * 180.f) / DX_PI_F;
		cameraDir = cameraLockDir;
	}
	else
	{
		float radian = cameraNtDir * DX_PI_F / 180.0f;
		auto InsV = VScale(VGet(sin(radian) * 100.f, 100, cos(radian) * 100.f), 3.f);
		auto _pos = cameraPos = VAdd(plMI->pos, InsV);
		cameraFor = VSub(plMI->pos, VSub(InsV, VGet(0.f, 300.f, 0.f)));
		cameraPos.y += cameraHigh;
		cameraFor.y -= cameraHigh;
		cameraDir = cameraNtDir;
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

std::string getName(const char* data)
{
	std::string input;
	std::stringstream b{ data };
	std::getline(b, input, '^');
	std::getline(b, input, '^');

	return input.c_str();
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
			if (data.find("weponset") != std::string::npos)
			{
				auto weponSetNum = getNum(data);
				if (weponSetNum == 0)
				{
					plMI->wepons[0].isActive = true;
					plMI->wepons[1].isActive = true;
					plMI->wepons[2].isActive = false;
				}
				else if (weponSetNum == 1)
				{
					plMI->wepons[0].isActive = false;
					plMI->wepons[1].isActive = false;
					plMI->wepons[2].isActive = true;
				}
			}
			if (data.find("atkFall") != std::string::npos)
			{
				auto a = getNum(data);
				_valData.plAtkSpd1 = a, _valData.plAtkSpd2 = a, _valData.plAtkSpd3 = a, _valData.plAtkSpd4 = a;
			}
			if (data.find("changeX") != std::string::npos)
			{
				charBox.find(Char_PL)->second->CA_change(getName(data.c_str()), "X");
			}
			if (data.find("changeY") != std::string::npos)
			{
				charBox.find(Char_PL)->second->CA_change(getName(data.c_str()), "Y");
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