#include"allMode.h"
#include <sstream>

bool	modeG::Initialize()
{
	_modelManager.modelImport("game/res/ZENRYOKUstage/tsStage.mv1", 7.0f, &stage);
	SetUseLighting(true);
	SetUseZBuffer3D(TRUE);// �y�o�b�t�@��L���ɂ���
	SetWriteZBuffer3D(TRUE);// �y�o�b�t�@�ւ̏������݂�L���ɂ���
	countTime = GetNowCount();

	/*shadowMapHandle = MakeShadowMap(2048, 2048);
	SetShadowMapLightDirection(shadowMapHandle, VGet(-1.f, -1.f, 0.5f));
	SetShadowMapDrawArea(2048, VGet(-1024, -1024, -1024), VGet(1024, 1024, 1024));*/
	//SetGlobalAmbientLight(GetColorF(0.5f, 0.5f, 0.5f, 0.7f));

	auto insPL = std::make_unique<PL>();
	insPL->Initialize();
	insPL->setCB(&charBox);
	insPL->_valData = &_valData;
	insPL->getInputKey(&_imputInf._gKeyp, &_imputInf._gTrgp, _imputInf._gKeyb, _imputInf._gTrgb, &cameraDir);
	charBox.emplace(Char_PL, std::move(insPL));

	auto boss = std::make_unique<Boss>();
	boss->Initialize();
	boss->setCB(&charBox);
	charBox.emplace(Char_BOSS1, std::move(boss));
	return true;
}

bool	modeG::Process()
{
	for (auto i = charBox.begin(); i != charBox.end(); i++)
	{
		if (i->second->getType() == 1)
		{
			i->second->Process();
			i->second->gravity();
			plMI = i->second->getInf();
		}
		else { i->second->Process(); }
	}

	//if (_imputInf._gKeyb[KEY_INPUT_UP]) { cameraHigh -= 7.f; }
	//if (_imputInf._gKeyb[KEY_INPUT_DOWN]) { cameraHigh += 7.f; }
	//if (_imputInf._gKeyb[KEY_INPUT_RIGHT]) { cameraDir += 5.f; }
	//if (_imputInf._gKeyb[KEY_INPUT_LEFT]) { cameraDir -= 5.f; }
	cameraDir += _imputInf.rStickX / 200;
	cameraHigh += _imputInf.rStickY / 200;

	useCommand();

	cameraMove();
	//cameraFor = VAdd(plMI.pos, VGet(0.f, 20.f, 0.f));
	SetCameraPositionAndTarget_UpVecY(cameraPos, cameraFor);
	//SetLightPositionHandle(LightHandle02, plMI.pos);

	debugWardBox.emplace_back("���݂�FPS�l/" + std::to_string(FPS));
	debugWardBox.emplace_back("��U��1�̃t���[����/" + std::to_string(_valData.plAtkSpd1));
	debugWardBox.emplace_back("��U��2�̃t���[����/" + std::to_string(_valData.plAtkSpd2));
	debugWardBox.emplace_back("��U��3�̃t���[����/" + std::to_string(_valData.plAtkSpd3));
	debugWardBox.emplace_back("��U��4�̃t���[����/" + std::to_string(_valData.plAtkSpd4));
	debugWardBox.emplace_back("x." + std::to_string(static_cast<int>(plMI->pos.x))
		+ "/y." + std::to_string(static_cast<int>(plMI->pos.y))
		+ "/z." + std::to_string(static_cast<int>(plMI->pos.z)));

	// PC�����擾���܂�
	getPcInf();

	return true;
}

bool	modeG::Render()
{
	for (auto i = charBox.begin(); i != charBox.end(); i++) { i->second->Render(); }

	//SetLightPositionHandle(LightHandle02, PLpos);
	//ShadowMap_DrawSetup(shadowMapHandle);
	MV1DrawModel(stage.modelHandle);
	//ShadowMap_DrawEnd();

	//for (int i = 0; i < 6; i++)
	//{
	//	DrawSphere3D(VGet(-575 + (230 * i), 60.f, 0.f), 50.f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), true);
	//}
	debugWardBox.emplace_back(std::to_string( plMI->playTime));
	debugWardBox.emplace_back(std::to_string( plMI->playTimeOld));
	debugWardBox.emplace_back("-------����Z�b�g�ꗗ-------");
	debugWardBox.emplace_back("No.0 ����:SwordBreaker �E��:RabbitBunker");
	debugWardBox.emplace_back("No.1 ����:�����@        �E��:GunBlade");
	debugWardBox.emplace_back("-------�R�}���h�ꗗ-------");
	debugWardBox.emplace_back("/debug(�f�o�b�O���[�h�̐؂�ւ�)");
	debugWardBox.emplace_back("/menu(���j���[��ʕ\��)");
	debugWardBox.emplace_back("/atkF1 ~ 4^�t���[����^(���@��1 ~ 4�Ԗڂ̍U�����[�V�����̑��t���[�����ύX)");
	debugWardBox.emplace_back("/atkFall^�t���[����^(���@�̂��ׂĂ̍U�����[�V�����̑��t���[�����ύX)");
	debugWardBox.emplace_back("/weponset^����Z�b�g�̔ԍ�^(����Z�b�g�̔ԍ�)");
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
	return true;
}

void modeG::cameraMove()
{
	float radian = cameraDir * DX_PI_F / 180.0f;
	auto InsV = VScale(VGet(sin(radian) * 100.f, 100, cos(radian) * 100.f), 3.f);
	auto _pos = cameraPos = VAdd(plMI->pos, InsV);
	cameraFor = VSub(plMI->pos, VSub(InsV, VGet(0.f, 300.f, 0.f)));
	cameraPos.y += cameraHigh;
	cameraFor.y -= cameraHigh;
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