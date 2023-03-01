#include"allMode.h"
#include <sstream>

bool makeChar(modeG* insMG, std::shared_ptr<CB> charPoint, const char* nameA)
{
	charPoint->_valData = insMG->_valData;
	charPoint->Initialize();
	charPoint->setCB(&insMG->charBox);
	charPoint->setGroundInf(&insMG->stage);
	charPoint->allColl = &insMG->mAllColl;
	charPoint->getInputKey(&insMG->_imputInf, &insMG->cameraDir);
	insMG->charBox.emplace(nameA, std::move(charPoint));
	return true;
}
bool	modeG::popBoss(int bossType, const char* _nameA)
{
	if (bossType == 1) { makeChar(this, std::shared_ptr<BossKnight>(), _nameA); }

	return true;
}

bool	modeG::ASyncLoadAnim()
{
	SetUseASyncLoadFlag(false);
	SetDrawScreen(DX_SCREEN_BACK);
	int i = 0;
	while (GetASyncLoadNum() > 0)
	{
		ProcessMessage();
		ClearDrawScreen();
		i++;
		DrawBox(0, 0, i, 20, GetColor(255, 255, 255), true);

		ScreenFlip();
	}

	return GetASyncLoadNum();
}

bool	modeG::Initialize()
{
	SetUseLighting(true);
	SetUseZBuffer3D(true);// �y�o�b�t�@��L���ɂ���
	SetWriteZBuffer3D(true);// �y�o�b�t�@�ւ̏������݂�L���ɂ���
	//SetUseBackCulling(false);
	SetUseASyncLoadFlag(true);
	SetAlwaysRunFlag(true);
	Effekseer_StartNetwork(60000);// �l�b�g���[�N�@�\��L���ɂ���
	_valData = &_modeServer->_valData;
	_modelManager.modelImport("game/res/Stage1/Stage1.mv1", 10.f, &stage);
	_modelManager.modelImport("game/res/skyDoom/incskies_029_16k.x", 20.f, &skyDoom);
	makeChar(this, std::make_unique<PL>(), Char_PL);

	if (_valData->popBossNum == 1) { makeChar(this, std::make_shared<BossKnight>(), Char_BOSS1); }
	if (_valData->popBossNum == 2) { makeChar(this, std::make_shared<BossLion>(), Char_BOSS2); }

	countTime = GetNowCount();

	testAttackCap.underPos = VGet(0.f, 30.f, 0.f);
	testAttackCap.overPos = VGet(0.f, 170.f, 0.f);
	testAttackCap.r = 30.f;

	UIkari = LoadGraph("game/res/A.png");
	HPgaugeHandle = LoadGraph("game/res/GameUI_HP.png");
	HPgaugeHandle2 = LoadGraph("game/res/GameUI_HPB.png");
	BPgaugeHandle = LoadGraph("game/res/c.png");
	LoadDivGraph("game/res/lockon/lockon_ui01_sheet.png", 30, 14, 3, 72, 72, lockOnMarkerHandle);

	LoadDivGraph("game/res/keepout.png", 180, 1, 180, 2400, 120, keepout);
	insEfcHamdle = LoadGraph("game/res/kari2.png");

	//�����܂Ŕ񓯊����[�h-------------------------------------------------------------------
	ASyncLoadAnim();

	// �V���h�E�}�b�v�n���h���̍쐬
	ShadowMapHandle = MakeShadowMap(16384, 16384);
	// �V���h�E�}�b�v���z�肷�郉�C�g�̕������Z�b�g
	VECTOR lightDir = VGet(-3.0f, -4.0f, 0.0f);
	SetShadowMapLightDirection(ShadowMapHandle, lightDir);
	ChangeLightTypeDir(lightDir);
	// �V���h�E�}�b�v�ɕ`�悷��͈͂�ݒ�
	SetShadowMapDrawArea(ShadowMapHandle, VGet(-5000.0f, -1.0f, -5000.0f), VGet(5000.0f, 1000.0f, 5000.0f));

	_valData->efcHandle = LoadEffekseerEffect("game/res/test.efkefc", 20.f);
	BGM = LoadSoundMem("game/res/BGM/DEATH TRIGGER.mp3");
	ChangeVolumeSoundMem(255 * (0.01 * 50), BGM);

	//�ǂݍ���3d���f���̃T�C�Y����
	for (auto i = charBox.begin(); i != charBox.end(); i++)
	{
		_modelManager.changeScale(&i->second->_modelInf);
	}
	_modelManager.changeScale(&stage);
	_modelManager.changeScale(&skyDoom);
	MV1SetFrameVisible(stage.modelHandle, 84, false);
	MV1SetFrameVisible(stage.modelHandle, 85, false);

	//�X�e�[�W�̓����蔻��쐬
	//MV1SetupCollInfo(stage.modelHandle, -1, 32, 6, 32);

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
	plStatus = { 0.f };
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
			bossStatus = i->second->getStatus();
		}
	}

	if (_imputInf._gTrgp[XINPUT_BUTTON_RIGHT_THUMB] || _imputInf._gTrgb[KEY_INPUT_L])
	{//���b�N�I��
		if(charBox.size() >= 2){ isLockon ^= true; }
	}

	//�R�}���h�Ăяo������
	useCommand();

	//�J��������---------------------------------------------
	if (CheckHitKey(KEY_INPUT_RIGHT)) { cameraNtDir += 1.f; }
	if (CheckHitKey(KEY_INPUT_LEFT)) { cameraNtDir -= 1.f; }
	cameraNtDir += _imputInf.rStickX / 5000;
	cameraHigh -= _imputInf.rStickY / 5000;
	cameraMove();
	SetCameraPositionAndTarget_UpVecY(cameraPos, cameraFor);
	Effekseer_Sync3DSetting();

	//�e�̖��邳����-----------------------------------------
	if (CheckHitKey(KEY_INPUT_UP)) { bright += .01f; }
	if (CheckHitKey(KEY_INPUT_DOWN)) { bright -= .01f; }
	if (bright < 0) { bright = 0; }
	if (bright > 1) { bright = 1; }
	SetGlobalAmbientLight(GetColorF(bright, bright, bright, 0.0f));
	debugWardBox.emplace_back("�e�̖��邳  = " + std::to_string(bright));

	debugWardBox.emplace_back("���@��HP = " + std::to_string(plStatus.hitPoint));
	debugWardBox.emplace_back("���@��BP = " + std::to_string(plStatus.bloodPoint));
	debugWardBox.emplace_back(std::to_string(
		(std::atan2(-_imputInf.lStickX, _imputInf.lStickY) * 180.f) / DX_PI_F));
	debugWardBox.emplace_back("���݂�FPS�l/" + std::to_string(FPS));
	debugWardBox.emplace_back("��U��1�̃t���[����/" + std::to_string(_valData->plAtkSpd1));
	debugWardBox.emplace_back("��U��2�̃t���[����/" + std::to_string(_valData->plAtkSpd2));
	debugWardBox.emplace_back("��U��3�̃t���[����/" + std::to_string(_valData->plAtkSpd3));
	debugWardBox.emplace_back("��U��4�̃t���[����/" + std::to_string(_valData->plAtkSpd4));
	debugWardBox.emplace_back("�K�[�h�o�����̃��[�V�����X�s�[�h/" + std::to_string(_valData->counterSpd));
	debugWardBox.emplace_back("�J�E���^�[�̑���t����/" + std::to_string(_valData->_counterTime));
	debugWardBox.emplace_back("�c��̃J�E���^�[��t����/" + std::to_string(_valData->plCTimeN));
	debugWardBox.emplace_back("x." + std::to_string(static_cast<int>(plMI->pos.x))
		+ "/y." + std::to_string(static_cast<int>(plMI->pos.y))
		+ "/z." + std::to_string(static_cast<int>(plMI->pos.z)));

	//�����蔻��v�Z�Ăяo��
	collHitCheck();

	for (auto i = charBox.begin(); i != charBox.end(); ++i)
	{
		if (i->second->isDead == 2 || i->second->_modelInf.pos.y < -500)
		{
			if (i->second->type == 1)
			{//���@�̎�
				_modeServer->Add(std::make_unique<modeGO>(_modeServer), 1, MODE_GO);
				Terminate();
				return false;
			}
			else
			{//����ȊO�̎�
				_modeServer->Add(std::make_unique<modeR>(_modeServer), 1, MODE_RESULT);
				Terminate();
				return false;
			}
		}
	}

	//���j���[��ʌĂяo��
	if (_imputInf._gTrgb[KEY_INPUT_M])
	{
		_modeServer->Add(std::make_unique<modeM>(_modeServer), 1, MODE_MENU);
	}

	if (_imputInf._gTrgb[KEY_INPUT_E])
	{
		int a = PlayEffekseer3DEffect(_valData->efcHandle);
		SetPosPlayingEffekseer3DEffect(a, 0, 1300, 0);
	}
	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();
	return true;
}

bool	modeG::Render()
{
	MV1DrawModel(skyDoom.modelHandle);
	//�V���h�E�}�b�v�R�R�J��-----------------------------------------
	ShadowMap_DrawSetup(ShadowMapHandle);
	//3d���f���̕`��
	for (auto i = charBox.begin(); i != charBox.end(); ++i)
	{
		i->second->Render(1);
	}
	MV1DrawModel(stage.modelHandle);

	ShadowMap_DrawEnd();
	SetUseShadowMap(0, ShadowMapHandle);
	//�e�p��3d���f���̕`��
	for (auto i = charBox.begin(); i != charBox.end(); ++i)
	{
		i->second->Render(0);
	}
	MV1DrawModel(stage.modelHandle);

	SetUseShadowMap(0, -1);
	//�V���h�E�}�b�v�����܂�-----------------------------------------

	SetUseLighting(false);
	for (int i = 0; i < mAllColl.size(); i++)
	{

		MATRIX M = MV1GetFrameLocalWorldMatrix(mAllColl.at(i).capColl.parentModelHandle, mAllColl.at(i).capColl.frameNum);
		auto insUnderPos = VTransform(mAllColl.at(i).capColl.underPos, M);
		auto insOverPos = VTransform(mAllColl.at(i).capColl.overPos, M);
		auto insUnderPosOld = mAllColl.at(i).capCollOld.underPos;
		auto insOverPosOld = mAllColl.at(i).capCollOld.overPos;

		auto a = _modelManager.drawBPolygon(insUnderPosOld, insOverPosOld, insUnderPos, insOverPos, insEfcHamdle);

	}
	for (int i = 0; i < mAllColl.size(); i++)
	{
		if (mAllColl[i].nonActiveTimeF <= 0.f)
		{
			MATRIX M = MV1GetFrameLocalWorldMatrix(mAllColl.at(i).capColl.parentModelHandle, mAllColl.at(i).capColl.frameNum);

			mAllColl[i].rightingEfc.downCornerPos.push_back(VTransform(mAllColl.at(i).capColl.underPos, M));
			mAllColl[i].rightingEfc.upCornerPos.push_back(VTransform(mAllColl.at(i).capColl.overPos, M));

			for (int j = 1; j < mAllColl[i].rightingEfc.downCornerPos.size(); ++j)
			{
				_modelManager.drawBPolygon(mAllColl[i].rightingEfc.downCornerPos[j], mAllColl[i].rightingEfc.upCornerPos[j], mAllColl[i].rightingEfc.downCornerPos[j - 1], mAllColl[i].rightingEfc.upCornerPos[j - 1], insEfcHamdle);
			}
		}
	}

	for (int i = 0; i < atkEfc.size(); i++)
	{
		for (int j = 1; j < atkEfc[i].downCornerPos.size(); j++)
		{
			if (_valData->isAtkEfcArufa) { SetDrawBlendMode(DX_BLENDMODE_ALPHA, (255 / atkEfc[i].maxLifeTime) * atkEfc[i].lifeTime); }
			_modelManager.drawBPolygon(atkEfc[i].downCornerPos[j], atkEfc[i].upCornerPos[j], atkEfc[i].downCornerPos[j - 1], atkEfc[i].upCornerPos[j - 1], insEfcHamdle);
		}
		if (_valData->isAtkEfcArufa) { SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); }
		if (atkEfc[i].lifeTime > 0) { atkEfc[i].lifeTime--; }
		else { atkEfc.erase(atkEfc.begin() + i); }
	}
	SetUseLighting(true);

	if (isLockon)
	{
		LOMarkerNum < 29 ? LOMarkerNum++ : LOMarkerNum = 0;
		SetUseZBuffer3D(FALSE);
		auto a = DrawBillboard3D(VAdd(cameraFor, VGet(0, 170, 0)), .5, .5, 300, 0, lockOnMarkerHandle[LOMarkerNum], true);
		SetUseZBuffer3D(TRUE);
	}

	int nowTime = GetNowCount();
	if (countTime + 1000 <= nowTime) { FPS = FPScount, FPScount = 0, countTime += 1000; }
	else { FPScount++; }

	//if (charBox.find(Char_PL) != charBox.end())
	//{
	//	DrawString(1000, 0, "���@��HP", GetColor(255.f, 0.f, 0.f));
	//	DrawString(1000, 20, std::to_string(charBox[Char_PL]->getStatus().hitPoint).c_str(), GetColor(255.f, 0.f, 0.f));
	//}
	//if (charBox.find(Char_BOSS1) != charBox.end())
	//{
	//	DrawString(1000, 50, "�R�m��HP", GetColor(255.f, 0.f, 0.f));
	//	DrawString(1000, 70, std::to_string(charBox[Char_BOSS1]->getStatus().hitPoint).c_str(), GetColor(255.f, 0.f, 0.f));
	//}

	DrawEffekseer3D();// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	drawUI();

	debugWardBox.emplace_back(std::to_string(plMI->playTime));
	debugWardBox.emplace_back(std::to_string(plMI->playTimeOld));
	float insDirY = charBox[Char_PL]->_modelInf.dir.y;
	if (insDirY > 360) { insDirY -= 360; }
	else if (insDirY < 0) { insDirY += 360; }
	//debugWardBox.emplace_back("-------����Z�b�g�ꗗ-------");
	debugWardBox.emplace_back("-------�R�}���h�ꗗ-------");
	debugWardBox.emplace_back("/debug(�f�o�b�O���[�h�̐؂�ւ�)");
	debugWardBox.emplace_back("/menu(���j���[��ʕ\��)");
	debugWardBox.emplace_back("/atkF1 ~ 4^�t���[����^(���@��1 ~ 4�Ԗڂ̍U�����[�V�����̑��t���[�����ύX)");
	debugWardBox.emplace_back("/atkFall^�t���[����^(���@�̂��ׂĂ̍U�����[�V�����̑��t���[�����ύX)");
	debugWardBox.emplace_back("/GSpd^�t���[����^(�K�[�h�o�����̃��[�V�����̑���)");
	debugWardBox.emplace_back("/CTime^�t���[����^(�J�E���^�[�̎�t���ԁA�W����40)");
	debugWardBox.emplace_back("/effectChange^�t�@�C����^^�X�P�[��^(E�L�[�ōĐ������G�t�F�N�g�̕ύX�A�g���q�s�v/res����̑��΃p�X�K�v)");
	for (int i = 0; i < debugWardBox.size() && debugMode; i++)
	{
		int sizeX, sizeY, lineCount;
		GetDrawStringSize(&sizeX, &sizeY, &lineCount, debugWardBox[i].c_str(), debugWardBox[i].length());
		DrawBox(10, 10 + 20 * i, 10 + sizeX, 10 + 20 * i + sizeY, GetColor(0, 0, 0), true);
		DrawString(10, 10 + 20 * i, debugWardBox[i].c_str(), GetColor(255, 255, 255));
	}
	debugWardBox.clear();

	return true;
}

bool	modeG::collHitCheck()
{
	for (int i = 0; i < mAllColl.size(); i++)
	{
		if (mAllColl.at(i).nonActiveTimeF > 0) { mAllColl.at(i).nonActiveTimeF--; }
		else if (mAllColl.at(i).activeTimeF > 0) { mAllColl.at(i).activeTimeF--; }
		else
		{
			atkEfc.emplace_back(mAllColl.at(i).rightingEfc);
			mAllColl.erase(mAllColl.begin() + i);
		}
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
	//MV1TerminateCollInfo(stage.modelHandle, -1);
	MV1DeleteModel(stage.modelHandle);
	MV1DeleteModel(skyDoom.modelHandle);
	int a = InitGraph();

	for (auto i = charBox.begin(); i != charBox.end(); ++i) { i->second->Terminate(); i->second.reset(); }
	mAllColl.clear();
	charBox.clear();
	debugWardBox.clear();
	InitGraph();
	InitSoundMem();
	DeleteLightHandleAll();
	return true;
}

void modeG::cameraMove()
{
	if (isLockon)
	{
		auto EtoPdir = VSub(bossMI->pos, plMI->pos);
		cameraFor = VAdd(bossMI->pos, VGet(0.f, 100.f, 0.f));
		cameraPos = VAdd(VAdd(plMI->pos, VScale(VNorm(EtoPdir), -350.f)), VGet(0.f, 250.f, 0.f));
		if (cameraPos.y < 0) { cameraPos.y = 0; }
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

float getNum(std::string data, int Num)
{
	std::string input;
	std::stringstream b{ data };
	for (int i = 0; i < Num; i++)
	{
		std::getline(b, input, '^');
		std::getline(b, input, '^');
	}

	return std::stof(input);
}

std::string getChar(std::string data, int Num)
{
	std::string input;
	std::stringstream b{ data };
	for (int i = 0; i < Num; i++)
	{
		std::getline(b, input, '^');
		std::getline(b, input, '^');
	}

	return input;
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
			if (data.find("atkF1") != std::string::npos) { _valData->plAtkSpd1 = getNum(data, 1); }
			if (data.find("atkF2") != std::string::npos) { _valData->plAtkSpd2 = getNum(data, 1); }
			if (data.find("atkF3") != std::string::npos) { _valData->plAtkSpd3 = getNum(data, 1); }
			if (data.find("atkF4") != std::string::npos) { _valData->plAtkSpd4 = getNum(data, 1); }
			if (data.find("GSpd") != std::string::npos) { _valData->counterSpd = getNum(data, 1); }
			if (data.find("CTime") != std::string::npos) { _valData->_counterTime = getNum(data, 1); }
			if (data.find("atkFall") != std::string::npos)
			{
				auto a = getNum(data, 1);
				_valData->plAtkSpd1 = a, _valData->plAtkSpd2 = a, _valData->plAtkSpd3 = a, _valData->plAtkSpd4 = a;
			}
			if (data.find("effectChange") != std::string::npos)
			{
				auto comEfcDir = "game/res/" + getChar(data, 1) + ".efkefc";
				auto efcScale = getNum(data, 2);
				_valData->efcHandle = LoadEffekseerEffect(comEfcDir.c_str(), efcScale);
			}
			if (data == "kill")
			{
				charBox[Char_PL]->_statusInf.hitPoint = 0;
			}
			if (data == "test")
			{
				OutputDebugString("succes");
				return 2;
			}
		}
	}
	catch (std::exception) { return -1; }
	return -1;
}

bool modeG::drawUI()
{
	//HP�o�[
	int barPposX = 10, barPosY = 10, barLength = 660;
	int gauge = barLength - static_cast<int>((barLength / static_cast<float>(plStatus.maxHitPoint)) * static_cast<float>(plStatus.hitPoint));
	DrawRectGraph(barPposX, barPosY, 0, 0, barLength - gauge, 30, HPgaugeHandle, true, false);

	//BP�o�[
	barLength = 600, barPposX = 640 - barLength / 2, barPosY = 600;
	gauge = barLength - static_cast<int>((barLength / static_cast<float>(plStatus.maxBloodPoint)) * static_cast<float>(plStatus.bloodPoint));
	DrawRectGraph(barPposX, barPosY, 0, 0, barLength - gauge, 23, BPgaugeHandle, true, false);

	//bossHP�o�[
	barPposX = 600, barPosY = 10, barLength = 660;
	gauge = barLength - static_cast<int>((barLength / static_cast<float>(bossStatus.maxHitPoint)) * static_cast<float>(bossStatus.hitPoint));
	auto J = DrawRectGraph(barPposX + gauge, barPosY, gauge, 0, barLength, 30, HPgaugeHandle2, true, false);
	return true;
}
