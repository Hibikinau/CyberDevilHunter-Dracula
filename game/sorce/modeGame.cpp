#include"allMode.h"

bool	modeG::Initialize()
{
	_modelManager.modelImport("game/res/ZENRYOKUstage/tsStage.mv1", 7.0f, &stage);
	SetUseLighting(true);
	SetUseZBuffer3D(TRUE);// Ｚバッファを有効にする
	SetWriteZBuffer3D(TRUE);// Ｚバッファへの書き込みを有効にする

	/*shadowMapHandle = MakeShadowMap(2048, 2048);
	SetShadowMapLightDirection(shadowMapHandle, VGet(-1.f, -1.f, 0.5f));
	SetShadowMapDrawArea(2048, VGet(-1024, -1024, -1024), VGet(1024, 1024, 1024));*/
	//SetGlobalAmbientLight(GetColorF(0.5f, 0.5f, 0.5f, 0.7f));

	auto insPL = std::make_unique<PL>();
	insPL->Initialize();
	insPL->getInputKey(&_imputInf._gKeyp, &_imputInf._gTrgp, _imputInf._gKeyb, _imputInf._gTrgb, &cameraDir);
	charBox.emplace_back(std::move(insPL));

	auto boss = std::make_unique<Boss>();
	boss->Initialize();
	charBox.emplace_back(std::move(boss));
	return true;
}

bool	modeG::Process()
{
	for (int i = 0; i < charBox.size(); i++)
	{
		if (charBox[i]->getType() == 1)
		{
			charBox[i]->gravity();
			charBox[i]->Process();
			plMI = charBox[i]->getInf();
		}
		else { charBox[i]->Process(); }
	}

	if (_imputInf._gKeyb[KEY_INPUT_UP]) { cameraHigh -= 7.f; }
	if (_imputInf._gKeyb[KEY_INPUT_DOWN]) { cameraHigh += 7.f; }
	if (_imputInf._gKeyb[KEY_INPUT_RIGHT]) { cameraDir += 7.f; }
	if (_imputInf._gKeyb[KEY_INPUT_LEFT]) { cameraDir -= 7.f; }

	if (_imputInf._gTrgp & PAD_INPUT_8)
	{
		auto insPL = std::make_unique<PL>();
		insPL->Initialize();
		insPL->getInputKey(&_imputInf._gKeyp, &_imputInf._gTrgp, _imputInf._gKeyb, _imputInf._gTrgb, &cameraDir);
		charBox.emplace_back(std::move(insPL));
	}

	useCommand();

	cameraMove();
	//cameraFor = VAdd(plMI.pos, VGet(0.f, 20.f, 0.f));
	SetCameraPositionAndTarget_UpVecY(cameraPos, cameraFor);
	//SetLightPositionHandle(LightHandle02, plMI.pos);

	debugWardBox.emplace_back("x." + std::to_string(static_cast<int>(plMI.pos.x))
		+ "/y." + std::to_string(static_cast<int>(plMI.pos.y))
		+ "/z." + std::to_string(static_cast<int>(plMI.pos.z)));

	// PC情報を取得します
	getPcInf();

	return true;
}

bool	modeG::Render()
{
	for (int i = 0; i < charBox.size(); i++) { charBox[i]->Render(); }

	//SetLightPositionHandle(LightHandle02, PLpos);
	//ShadowMap_DrawSetup(shadowMapHandle);
	MV1DrawModel(stage.modelHandle);
	//ShadowMap_DrawEnd();

	//for (int i = 0; i < 6; i++)
	//{
	//	DrawSphere3D(VGet(-575 + (230 * i), 60.f, 0.f), 50.f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), true);
	//}

	for (int i = 0; i < debugWardBox.size() && debugMode; i++)
	{
		DrawString(10, 10 + 20 * i, debugWardBox[i].c_str(), GetColor(255, 255, 255));
	}
	debugWardBox.clear();

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
	auto _pos =
		cameraPos = VAdd(plMI.pos, InsV);
	cameraFor = VSub(plMI.pos, VSub(InsV, VGet(0.f, 300.f, 0.f)));
	cameraPos.y += cameraHigh;
	cameraFor.y -= cameraHigh;
}

int modeG::useCommand()
{
	if (!_imputInf._gTrgb[KEY_INPUT_RETURN]) { return 1; }
	DrawBox(10, 700, 1270, 730, GetColor(0, 0, 0), true);
	auto a = KeyInputString(10, 700, 141, _imputInf.wardBox, true);
	std::string _wardBox = _imputInf.wardBox;
	if (_wardBox.size() == 0) { return 1; }

	if (std::equal(_wardBox.begin(), _wardBox.end(), "/debug")) { debugMode ? debugMode = false : debugMode = true;	return 2; }
	if (std::equal(_wardBox.begin(), _wardBox.end(), "/menu")){ _modeServer->Add(std::make_unique<modeM>(_modeServer), 1, MODE_MENU); }
	if (std::equal(_wardBox.begin(), _wardBox.end(), "/test"))
	{
		OutputDebugString("succes");
		return 2;
	}
	//debugWardBox.emplace_back(_wardBox);

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