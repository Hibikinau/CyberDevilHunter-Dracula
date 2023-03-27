/*****************************************************************//**
 * \file   player.cpp
 * \brief  �v���C���N���X
 *
 * \author �R�{介v
 * \date   March 2023
 *********************************************************************/
#include"player.h"

 //�v���C����run��Ԃł̈ړ����x
#define runSpd 40.f

//�e�U���̃_���[�W�ݒ�
#define jakuATK 50.f
#define kyouATK 120.f
#define counterATK 200.f
#define debugATK 99999.f
#define kirinukeATK 80.f
#define charge1ATK 100.f
#define charge2ATK 160.f
#define charge3ATK 160.f
#define finishATK 200.f

//����Ǐ]�t���[���ԍ��ݒ�
#define rWeponParentFrame 192
#define lWeponParentFrame 167

typedef ExclusiveState _estate;
using namespace model;

VECTOR getDirVecP(float dir, int powar)
{
	float _Dir = dir - 180.f;
	float radian = _Dir * DX_PI_F / 180.0f;
	VECTOR dirZ = VGet(0, 0, 0);
	dirZ.x = sin(radian) * powar;
	dirZ.z = cos(radian) * powar;
	dirZ.y = 0;
	return dirZ;
}

bool PL::Initialize()
{
	//�e�ϐ��̏�����
	spd = runSpd;
	type = 1;
	g = 1.f;
	isCharge = 0;
	Estate = _estate::NORMAL;
	_statusInf.maxHitPoint = _statusInf.hitPoint = 200;
	_statusInf.maxBloodPoint = 1500;
	_statusInf.bloodPoint = 0;

	_modelInf.pos = VGet(210.0f, 1100.0f, 3100.f);
	_modelInf.dir = VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);

	//���f���̓ǂݍ���
	modelImport("game/res/Player01/Player1.mv1", 1.5f, &_modelInf, RS);
	weponAttach("game/res/Weapon_Katana/Weapon_katana.mv1", &_modelInf, rWeponParentFrame, 2.f, true, "katana", RS);
	weponAttach("game/res/Weapon_Saya/Weapon_saya.mv1", &_modelInf, lWeponParentFrame, 2.f, true, "saya", RS);
	weponAttach("game/res/Weapon_noutou/Weapon_noutou.mv1", &_modelInf, lWeponParentFrame, 2.f, false, "noutou", RS);

	//����ւ��Z�ݒ�
	CA_change(_valData->plChangeAttackX, "X");
	CA_change(_valData->plChangeAttackY, "Y");

	//�����f�[�^�̓ǂݍ���
	soundHandle.emplace_back(LoadSoundMem("game/res/SE/�v���C���[�@�U���q�b�g��/SE_Damage_01.mp3"));
	soundHandle.emplace_back(LoadSoundMem("game/res/SE/�v���C���[�@��U��4�i��/SE_Player_nATK4_Finish.mp3"));
	soundHandle.emplace_back(LoadSoundMem("game/res/SE/�v���C���[�@��U���O�i��SE/SE_Player_ATK_01.mp3"));
	soundHandle.emplace_back(LoadSoundMem("game/res/SE/�v���C���[�@��U���O�i��SE/SE_Player_ATK_02.mp3"));
	voiceStartNum = soundHandle.size();
	for (std::string voiceNameList : _valData->playerVoiceList)
	{
		std::string insName = "game/res/Player01/voice/" + voiceNameList;
		soundHandle.emplace_back(LoadSoundMem(insName.c_str()));
	}

	guardEfcHandle = LoadEffekseerEffect("game/res/effect/�K�[�h/guard_effect_01.efkefc", 20.f);
	chargeEfcHandle = LoadEffekseerEffect("game/res/effect/�`���[�W�G�t�F�N�g/charge.efkefc", 20.f);
	healEfcHandle = LoadEffekseerEffect("game/res/effect/��1/heal_01.efkefc", 60.f);
	impactEfcHandle = LoadEffekseerEffect("game/res/effect/�Ռ��g1/slash_shot.efkefc", 80.f);

	return true;
}


bool	PL::Terminate()
{
	CB::Terminate();
	//�����f�[�^�̃��������
	for (auto handle : soundHandle) { DeleteSoundMem(handle); }
	return true;
}

/**
 * @brief ����̔[���A�������^�C�̐؂�ւ�
 * \param MI ���f���f�[�^
 * \param isIO true�Ŕ���false�Ŕ[��
 * \return true
 */
bool KATANAIO(modelInf* MI, bool isIO)
{
	MI->wepons[0].isActive = isIO;
	MI->wepons[1].isActive = isIO;
	MI->wepons[2].isActive = !isIO;

	return true;
}

bool	PL::Process()
{
	//�}�X�^�[���ʂ̓K��
	if (!isSetSoundValume) { setMasterVolume(_valData->soundMasterValume); isSetSoundValume = true; }
	//���S������
	if (_statusInf.hitPoint <= 0)
	{
		isDead = 1;//���S��ԕϐ������S���[�V�����Đ���Ԃ�
		animSpd = .7f;//�A�j���[�V�����Đ����x�ݒ�
		_modelInf.animSpdBuff = .0f;//�A�j���[�V�����Đ����x�o�t�ݒ�

		animChange(PL_death, &_modelInf, false, true, false);//�A�j���[�V���������S���[�V�����ɕύX
		//���S�����̍Đ�
		if (!deadVoice) { deadVoice = true; PlaySoundMem(soundHandle[voiceStartNum + 31 + rand() % 4], DX_PLAYTYPE_BACK); }
		//�A�j���[�V�����̍Đ����I������玀�S��Ԃ��L�����폜��Ԃ�
		if (_modelInf.isAnimEnd) { isDead = 2; }
		return true;
	}

	//�u���b�h�|�C���g��1500�ȏ�ɂȂ�Ȃ��悤��
	if (_statusInf.bloodPoint > 1500.f)
	{
		_statusInf.bloodPoint = 1500.f;
	}

	//��o������L2/R2�{�^���������Ɗo����ԂɈڍs
	if (_imputInf->lTriggerX > 100 && _imputInf->rTriggerX > 100 && isAwakening == 0 && _statusInf.bloodPoint >= _statusInf.maxBloodPoint)
	{
		//�o���p�����f�[�^�̍Đ�
		PlaySoundMem(soundHandle[voiceStartNum + 39 + rand() % 2], DX_PLAYTYPE_BACK);
		isAwakening = 1, atkBuff = 100.f, _modelInf.animSpdBuff = .5f;
	}

	//L2/R2�{�^���̓��͒l����A���ŏ���������Ȃ��悤�ɒ���
	if (_imputInf->lTriggerX < 20 && _imputInf->rTriggerX < 20)
	{
		if (isAwakening == 1) { isAwakening = 2; }
		if (isAwakening == -1) { isAwakening = 0; }
	}

	//�o�����Ƀu���b�h�|�C���g�̌����A�s�����ۂ̊o����������
	if (isAwakening > 0) { _statusInf.bloodPoint > 0 ? _statusInf.bloodPoint-- : (_statusInf.bloodPoint = 0, isAwakening = 0, atkBuff = 0.f, _modelInf.animSpdBuff = 0.f); }

	//�L�[�{�[�h�ł̎��@�ړ�����(�f�o�b�O�p)
	if (CheckHitKey(KEY_INPUT_W)) { spd = runSpd; charMove(spd, *_cameraDir, false); }
	if (CheckHitKey(KEY_INPUT_A)) { spd = runSpd; charMove(spd, *_cameraDir + 180.f + 90.f, false); }
	if (CheckHitKey(KEY_INPUT_S)) { spd = runSpd; charMove(spd, *_cameraDir + 180.f, false); }
	if (CheckHitKey(KEY_INPUT_D)) { spd = runSpd; charMove(spd, *_cameraDir + 180.f + -90.f, false); }

	float addDir = 0.f, insDir, a;
	bool moveCheck = true;
	switch (setAction())
	{
	case pushButton::Damage://��e
		if (_modelInf.playTime <= animSpd)
		{
			dodgeTime = 0, chargeLevel = 0, waitCAChargeTime = 0, CAChargeTime = 0, isGhost = false, _modelInf.animHandleNext = -1, attackNumOld = 0, waitNextAttack = 0, CAChargeAttackNum = 0;
			isCharge = 0, isFastGuard = true, isGuard = true;
			nextKey = pushButton::Neutral;
			recastSet();
		}
		if (isBlow)
		{
			animSpd = 2.f;
			animChange(PL_huttobi1, &_modelInf, false, false, false);
			setNextAnim(PL_huttobi2, &_modelInf, false, false);
			if (_modelInf.playTime > 180.f) { Estate = _estate::NORMAL; }
			auto animName = _modelInf.animHandleOld;
			if (animName == PL_huttobi1)
			{
				if (_modelInf.playTime <= animSpd)
				{
					_modelInf.dir.y = getMoveDir(true);
					waitCAChargeTime = 20.f;
					CAChargeTime = 100.f;
					CAChargeSpd = -30.f;
				}
				immortalTime = getAnimPlayTotalTime() - _modelInf.playTime;
				if (_modelInf.playTime > 140.f) { Estate = _estate::NORMAL; }
			}
		}//�A�j���[�V�������e���[�V�����ɕύX}
		else
		{
			animChange(PL_hirumi, &_modelInf, false, false, false);
			if (_modelInf.playTime > 25.f) { Estate = _estate::NORMAL; }
		}//�A�j���[�V�������e���[�V�����ɕύX}

		break;
	case pushButton::B://���
		dodgeTime = 0, chargeLevel = 0, waitCAChargeTime = 0, CAChargeTime = 0, isGhost = false, _modelInf.animHandleNext = -1, attackNumOld = 0, waitNextAttack = 0, CAChargeAttackNum = 0;
		animSpd = 4.f;
		spd = 70.f;
		isCharge = 0;
		recastSet();

		insDir = getMoveDir(false);
		if (insDir == 0) { insDir = _modelInf.dir.y; }
		dodgeDir = _modelInf.dir.y - insDir;
		if (dodgeDir > 360) { dodgeDir -= 360; }
		else if (dodgeDir < 0) { dodgeDir += 360; }

		if (dodgeDir >= 45 && 135 > dodgeDir) { animChange(PL_dodge_L, &_modelInf, false, false, false); }//�A�j���[�V��������������[�V�����ɕύX
		else if (dodgeDir >= 135 && 225 > dodgeDir) { animChange(PL_dodge_B, &_modelInf, false, false, false); }//�A�j���[�V��������������[�V�����ɕύX
		else if (dodgeDir >= 225 && 315 > dodgeDir) { animChange(PL_dodge_R, &_modelInf, false, false, false); }//�A�j���[�V�������E������[�V�����ɕύX
		else { animChange(PL_dodge_F, &_modelInf, false, false, false); }//�A�j���[�V������O������[�V�����ɕύX

		dodgeTime = getAnimPlayTotalTime();
		immortalTime = dodgeTime;

		insDir != 0 ? dodgeDir = insDir : dodgeDir = _modelInf.dir.y;
		break;
	case pushButton::X://��U��
		insDir = getMoveDir(true);
		if (insDir != 0) { _modelInf.dir.y = insDir; }
		Estate = _estate::quickATTACK;
		lastAttackState = _estate::quickATTACK;
		waitNextAttack = 40;
		animSpd = 2.f;
		if (attackNumOld < 3) { PlaySoundMem(soundHandle[voiceStartNum + 0 + rand() % 4], DX_PLAYTYPE_BACK); }
		else { PlaySoundMem(soundHandle[voiceStartNum + 4 + rand() % 2], DX_PLAYTYPE_BACK); }
		if (attackNumOld == 0)
		{
			animChange(PL_jaku_1, &_modelInf, false, false, true);//�A�j���[�V��������U���P�i�ڃ��[�V�����ɕύX
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, jakuATK + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
		}
		else if (attackNumOld == 1)
		{
			animChange(PL_jaku_2, &_modelInf, false, false, true);//�A�j���[�V��������U���Q�i�ڃ��[�V�����ɕύX
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, jakuATK + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
		}
		else if (attackNumOld == 2)
		{
			animChange(PL_jaku_3, &_modelInf, false, false, true);//�A�j���[�V��������U���R�i�ڃ��[�V�����ɕύX
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, jakuATK + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
		}
		else if (attackNumOld == 3)
		{
			animChange(PL_jaku_4_1, &_modelInf, false, false, true);//�A�j���[�V��������U���S�i�ڃ��[�V�����P�ɕύX
			setNextAnim(PL_jaku_4_2, &_modelInf, true, true);//���A�j���[�V�����ɔ[�����Z�b�g
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
		}
		else if (attackNumOld == 4 && isPushButtonAct)
		{
			_modelInf.animHandleNext = -1;
			animChange(PL_jaku_4_3, &_modelInf, false, false, true);//�A�j���[�V��������U���S�i�ڃ��[�V�����Q�ɕύX
			setNextAnim(PL_jaku_4_2, &_modelInf, true, false);//���A�j���[�V�����ɔ[�����Z�b�g
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld = 4;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, 12.f, animSpd, true, jakuATK + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 17.f, 8.f, animSpd, true, jakuATK + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 32.f, 10.f, animSpd, true, jakuATK + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 47.f, 13.f, animSpd, true, jakuATK + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
		}
		else if (attackNumOld == 4 && !isPushButtonAct)
		{
			_modelInf.animHandleNext = -1;
			animChange(PL_jaku_4_4, &_modelInf, false, false, true);//�A�j���[�V��������U���S�i�ڃ��[�V�����R�ɕύX
			waitNextAttack = 0;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, jakuATK + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
		}

		break;
	case pushButton::Y://���U��
		insDir = getMoveDir(true);
		if (insDir != 0) { _modelInf.dir.y = insDir; }
		Estate = _estate::slowATTACK;
		lastAttackState = _estate::slowATTACK;
		waitNextAttack = 40;
		animSpd = 2.f;
		if (attackNumOld < 3) { PlaySoundMem(soundHandle[voiceStartNum + 6 + rand() % 5], DX_PLAYTYPE_BACK); }
		else { PlaySoundMem(soundHandle[voiceStartNum + 11 + rand() % 3], DX_PLAYTYPE_BACK); }
		if (attackNumOld == 0)
		{
			animChange(PL_kyou_1, &_modelInf, false, false, true);//�A�j���[�V���������U���P�i�ڃ��[�V�����ɕύX
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, kyouATK + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
		}
		else if (attackNumOld == 1)
		{
			animChange(PL_kyou_2, &_modelInf, false, false, true);//�A�j���[�V���������U���Q�i�ڃ��[�V�����ɕύX
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, kyouATK + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
		}
		else if (attackNumOld == 2)
		{
			animChange(PL_kyou_3, &_modelInf, false, false, true);//�A�j���[�V���������U��3�i�ڃ��[�V�����ɕύX
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, kyouATK + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
		}
		else if (attackNumOld == 3)
		{
			animChange(PL_kyou_4_1, &_modelInf, false, false, true);//�A�j���[�V���������U��4�i�ڃ��[�V�����P�ɕύX
			setNextAnim(PL_kyou_4_2, &_modelInf, true, true);//���A�j���[�V�����ɔ[�����Z�b�g
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
		}
		else if (attackNumOld == 4)
		{
			animChange(PL_kyou_4_3, &_modelInf, false, false, true);//�A�j���[�V���������U��4�i�ڃ��[�V����2�ɕύX
			_modelInf.animHandleNext = -1;
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld = 0;
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, kyouATK + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
		}

		break;
	case pushButton::LBX://����ւ�X
		Estate = _estate::changeATTACKX;
		lastAttackState = _estate::changeATTACKX;
		attackNumOld = 0;
		changeAttackX(this);
		break;
	case pushButton::LBY://����ւ�Y
		Estate = _estate::changeATTACKY;
		lastAttackState = _estate::changeATTACKY;
		attackNumOld = 0;
		changeAttackY(this);
		break;
	case pushButton::finishAttack://�K�E�Z
		Estate = _estate::finishAttack;
		lastAttackState = _estate::finishAttack;
		attackNumOld = 0;
		insDir = getMoveDir(true);
		if (insDir != 0) { _modelInf.dir.y = insDir; }
		PlaySoundMem(soundHandle[voiceStartNum + 39 + rand() % 2], DX_PLAYTYPE_BACK);
		animChange(PL_motion_hissatsu, &_modelInf, false, false, true);//�A�j���[�V�������o�����K�E�Z���[�V�����ɕύX
		makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 200.f), 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, finishATK + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
		isAwakening = -1;
		immortalTime = _modelInf.totalTime;
		_statusInf.bloodPoint = 0;
		break;
	case pushButton::Lstick://�_�b�V��
		Estate = _estate::NORMAL;
		attackNumOld = 0;
		//isDash = true;//------------------------------------------------------------------------------------
		//�ړ���̊p�x���x�N�g���ɂ��Ĉړ��x�N�g���ɉ��Z
		addDir = getMoveDir(false);
		spd = runSpd;//walkTime
		if (addDir != 0) { charMove(spd * (walkTime / 10), addDir, true); }
		moveCheck = false;
		break;
	case pushButton::R1://�K�[�h
		dodgeTime = 0, chargeLevel = 0, waitCAChargeTime = 0, CAChargeTime = 0, isGhost = false, _modelInf.animHandleNext = -1, attackNumOld = 0, waitNextAttack = 0, CAChargeAttackNum = 0;
		recastSet();
		if (Estate != _estate::GUARD && isFastGuard)
		{
			//if()
			Estate = _estate::GUARD;
			insGuardEfcHandle = PlayEffekseer3DEffect(guardEfcHandle);
			SetPosPlayingEffekseer3DEffect(insGuardEfcHandle, _modelInf.pos.x, _modelInf.pos.y + 100.f, _modelInf.pos.z);

			counterTime = _valData->_counterTime;
		}
		nextKey = pushButton::Neutral;
		if (isCounter == 1)
		{
			insDir = getMoveDir(true);
			if (insDir != 0) { _modelInf.dir.y = insDir; }
			animSpd = 2.f;
			KATANAIO(&_modelInf, true);
			PlaySoundMem(soundHandle[voiceStartNum + 37 + rand() % 2], DX_PLAYTYPE_BACK);
			animChange(PL_counter, &_modelInf, false, false, true);//�A�j���[�V�������J�E���^�[���[�V�����ɕύX
			makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 150.f), 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, counterATK + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
			isFastGuard = true, isCounter = 2, counterTime = 0;
			immortalTime = getAnimPlayTotalTime();
			auto a = VAdd(_modelInf.pos, getDirVecP(_modelInf.dir.y - 90, 300));
			auto b = VAdd(_modelInf.pos, getDirVecP(_modelInf.dir.y + 90, 300));
			auto bz = getDirVecP(_modelInf.dir.y, 30);
			a.y = b.y = _modelInf.pos.y + 200;

			makeAttackCap(a, b, 40.f, 10.f, getAnimPlayTotalTime() - 10.f, animSpd, false, 75.f, 20, -1, bz, 0);

			int a2 = PlayEffekseer3DEffect(impactEfcHandle);
			SetPosPlayingEffekseer3DEffect(a2, _modelInf.pos.x, _modelInf.pos.y + 120.f, _modelInf.pos.z);
			SetRotationPlayingEffekseer3DEffect(a2, _modelInf.dir.x * (DX_PI_F / 180), _modelInf.dir.y * (DX_PI_F / 180), _modelInf.dir.z * (DX_PI_F / 180));
		}
		else if (isFastGuard && isCounter == 0)
		{
			animSpd = 10.f;
			animSpd = _valData->counterSpd;
			animChange(PL_guard_1, &_modelInf, false, false, false);//�A�j���[�V�������K�[�h���[�V�����P�ɕύX
		}
		else if (isCounter != 2) {
			animSpd = 1.f;
			animChange(PL_guard_2, &_modelInf, true, false, false);//�A�j���[�V�������K�[�h���[�V�����Q�ɕύX
			Estate = _estate::NORMAL;
			BPmath(-1);
		}
		break;
	case pushButton::Neutral://���͂Ȃ�
		if (_modelInf.animHandleOld == PL_huttobi1 || _modelInf.animHandleOld == PL_huttobi2) { break; }
		if (attackNumOld > 0) { break; }
		Estate = _estate::NORMAL;
		animChange(PL_idel, &_modelInf, true, true, false);//�A�j���[�V������ҋ@���[�V�����ɕύX
		spd = 0.f;
		break;
	default:
		break;
	}

	//����ւ��Z�����Ȃǂ̈ړ�����
	if (waitCAChargeTime > 0) { waitCAChargeTime -= animSpd + _modelInf.animSpdBuff; }
	else if (CAChargeTime > 0)
	{
		waitCAChargeTime = 0;
		CAChargeTime -= animSpd + _modelInf.animSpdBuff;
		charMove(CAChargeSpd, _modelInf.dir.y, false);
	}
	else { CAChargeTime = 0; }

	if (dodgeTime > 0)
	{
		if (_modelInf.playTime < _modelInf.totalTime - (_modelInf.totalTime / 6.f) && _modelInf.playTime > _modelInf.totalTime / 6.f)
		{
			animSpd = 2.f;
			charMove(spd, dodgeDir, false);
		}
		else { animSpd = 3.f; }
		dodgeTime -= animSpd;
	}

	//�{�X�Əd�Ȃ�Ȃ��悤��
	for (auto i = charBox->begin(); i != charBox->end() && !isGhost; ++i)
	{
		if (i->second->type == 1) { continue; }
		auto  bossDisV = VSub(i->second->getInf()->pos, _modelInf.pos);
		float bossDisF = sqrt(bossDisV.x * bossDisV.x + bossDisV.y * bossDisV.y + bossDisV.z * bossDisV.z);
		float plzDis = i->second->collCap.r + collCap.r;
		if (bossDisF < plzDis)
		{
			_modelInf.pos = VAdd(VScale(VNorm(bossDisV), -plzDis), i->second->getInf()->pos);
			CAChargeTime = 0;
		}
	}

	//�x�N�g���v�Z�Ȃǂ̕��t���[���K�v�ȏ���
	immortalTime > 0 ? immortalTime-- : immortalTime = 0;
	waitNextAttack > 0 ? waitNextAttack-- : attackNumOld = 0;
	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	_modelInf.vec.x = 0.f, _modelInf.vec.z = 0.f;
	isImmortal = immortalTime > 0;
	collCap.r = 30.f;
	collCap.underPos = VAdd(_modelInf.pos, VGet(0, 30, 0));
	collCap.overPos = VAdd(_modelInf.pos, VGet(0, 190, 0));
	int insNum;
	isAwakening > 0 ? insNum = 10 : insNum = 1;
	caRecastX > 0 ? caRecastX -= insNum : caRecastX = 0;
	caRecastY > 0 ? caRecastY -= insNum : caRecastY = 0;
	waitBlowTime > 0 ? waitBlowTime-- : waitBlowTime = 0;

	//��U��-------------
	//if (CheckHitKey(KEY_INPUT_RIGHT)) { neckDir += 0.01f; }
	//if (CheckHitKey(KEY_INPUT_LEFT)) { neckDir -= 0.01f; }

	//MV1ResetFrameUserLocalMatrix(_modelInf.modelHandle, 103);
	//MATRIX neckDirMAT = MGetRotY(neckDir);
	//MATRIX neckPosMAT = MV1GetFrameLocalMatrix(_modelInf.modelHandle, 103);
	//MATRIX neckMAT = MMult(neckDirMAT, neckPosMAT);
	//MV1SetFrameUserLocalMatrix(_modelInf.modelHandle, 103, neckMAT);
	//------------------

	//�U���q�b�g����SE�Đ�
	if (isHit)
	{
		BPmath(100);
		if (lastAttackState == _estate::quickATTACK)
		{
			int soundNum = 0;
			attackNumOld >= 5 ? soundNum = 1 : soundNum = 2 + rand() % 3;
			PlaySoundMem(soundHandle[soundNum], DX_PLAYTYPE_BACK);
		}
		isHit = false;
	}

	//modeGame�ł̃J�E���^�[��t���ԍĐ��p
	_valData->plCTimeN = counterTime;

	return true;
}

bool	PL::Render(float timeSpeed)
{
	int i = 0;
	isAnimEnd = modelRender(&_modelInf, animSpd, timeSpeed);
	//DrawCapsule3D(collCap.underPos, collCap.overPos, collCap.r, 8, GetColor(255, 0, 255), GetColor(0, 0, 0), false);

	if (IsEffekseer3DEffectPlaying(insGuardEfcHandle) == 0)
	{
		SetPosPlayingEffekseer3DEffect(insGuardEfcHandle, _modelInf.pos.x, _modelInf.pos.y + 100.f, _modelInf.pos.z);
	}
	return true;
}

void PL::charMove(float Speed, float _Dir, bool isAnimChange)
{
	_Dir -= 180.f;
	float radian = _Dir * DX_PI_F / 180.0f;
	_modelInf.vec.x += sin(radian) * Speed;
	_modelInf.vec.z += cos(radian) * Speed;

	if (isAnimChange)
	{
		animChange(PL_run, &_modelInf, true, true, false);//�A�j���[�V�����𑖂胂�[�V�����ɕύX
		animSpd = 1.f;
		_modelInf.dir.y = _Dir + 180.f;
	}
}

bool PL::HPmath(float math, float Stan)
{
	if (math < 0)
	{
		if (counterTime > 0) { isCounter = 1; }
		else if (immortalTime <= 0)
		{
			if (!isGuard || isFastGuard)
			{
				if (isAwakening == 0)
				{
					if (!deadVoice) { PlaySoundMem(soundHandle[voiceStartNum + 27 + rand() % 4], DX_PLAYTYPE_BACK); }
					_statusInf.hitPoint += math; BPmath(std::abs(math) * 6);
				}
				PlaySoundMem(soundHandle[0], DX_PLAYTYPE_BACK);
				Estate = _estate::DAMAGE;
				if (math < -30 || waitBlowTime > 0) { isBlow = true; }
				else { waitBlowTime = 100; }
			}

			auto ACDisV = VSub(_modelInf.pos, charBox->find(attackChar)->second->_modelInf.pos);
			ACDisV = VNorm(ACDisV);
			_modelInf.vec = VScale(ACDisV, 50);
		}
	}
	else
	{
		_statusInf.hitPoint += math;
		if (_statusInf.hitPoint > _statusInf.maxHitPoint) { _statusInf.hitPoint = _statusInf.maxHitPoint; }
	}
	if (dodgeTime > 0) { PlaySoundMem(soundHandle[voiceStartNum + 24 + rand() % 3], DX_PLAYTYPE_BACK); }

	return true;
}
bool PL::BPmath(float math)
{
	if (isAwakening > 0 && math > 0)
	{
		return false;
	}
	_statusInf.bloodPoint += math;
	if (_statusInf.bloodPoint > _statusInf.maxBloodPoint) { _statusInf.bloodPoint = _statusInf.maxBloodPoint; }
	if (_statusInf.bloodPoint < 0) { _statusInf.bloodPoint = 0; }

	return true;
}

pushButton PL::setAction()
{
	bool isNext = false;
	isPushButtonAct = false;
	pushButton insEnum = pushButton::Neutral;

	if (Estate == _estate::DAMAGE) { return pushButton::Damage; }
	if (isCounter == 1) { return pushButton::R1; }
	if (_modelInf.isAnimEnd)
	{
		isAnimEnd = false, isGhost = false, isBlow = false;
		StopJoypadVibration(DX_INPUT_PAD1);
		if (Estate != _estate::NORMAL && isCharge == 0 && !isGuard) { Estate = _estate::NORMAL; }
		if (isFastGuard) { isFastGuard = false; }
		if (isCounter == 2) { isCounter = 0; }
		if (CAChargeAttackNum > 0) { if (lastAttackState == _estate::changeATTACKX) { return pushButton::LBX; } else { return pushButton::LBY; } }
		else { CAChargeAttackNum = 0; }
		recastSet();
	}
	else if (Estate != _estate::NORMAL) { isNext = true; }

	if (nextKey != pushButton::Neutral && !isNext && isCharge != 1 && !isGuard && Estate != _estate::DODGE) { insEnum = nextKey, nextKey = pushButton::Neutral; return insEnum; }

	if (checkTrgImput(KEY_INPUT_SPACE, XINPUT_BUTTON_B))//B
	{
		attackNumOld = 0;
		Estate = _estate::DODGE;
		return insEnum = pushButton::B;
	}
	else if (checkKeyImput(KEY_INPUT_G, XINPUT_BUTTON_RIGHT_SHOULDER) && dodgeTime <= 0)
	{
		if (isFastGuard && _modelInf.playTime > 17.5f) { KATANAIO(&_modelInf, false); }
		if (checkTrgImput(KEY_INPUT_G, XINPUT_BUTTON_RIGHT_SHOULDER)) { isFastGuard = true, isGuard = true; }
		counterTime > 0 ? counterTime-- : counterTime = 0;
		return insEnum = pushButton::R1;
	}
	else
	{
		KATANAIO(&_modelInf, true);
		isGuard = false, counterTime = 0;
	}

	if (checkKeyImput(KEY_INPUT_LSHIFT, XINPUT_BUTTON_LEFT_THUMB) || getMoveDir(false) != 0) {
		if (Estate != _estate::slowATTACK) { insEnum = pushButton::Lstick; }
		walkTime < 10 ? walkTime++ : walkTime = 10;
	}//Lstick
	else { walkTime = 0; }
	if (isNext) { insEnum = pushButton::Irregular; }

	if (checkKeyImput(KEY_INPUT_C, XINPUT_BUTTON_LEFT_SHOULDER))
	{//����ւ��Z
		if (checkTrgImput(KEY_INPUT_Z, XINPUT_BUTTON_X) && caRecastX <= 0) {
			if (isNext) { if (Estate != _estate::changeATTACKX) { nextKey = pushButton::LBX; } }
			else { insEnum = pushButton::LBX; }
		}//LBX
		if (checkTrgImput(KEY_INPUT_X, XINPUT_BUTTON_Y) && caRecastY <= 0) {
			if (isNext) { if (Estate != _estate::changeATTACKY) { nextKey = pushButton::LBY; } }
			else { insEnum = pushButton::LBY; }
		}//LBY
		if (checkTrgImput(KEY_INPUT_V, XINPUT_BUTTON_A) && _statusInf.bloodPoint > 500.f)
		{
			HPmath(100, 10); BPmath(-500);
			int a = PlayEffekseer3DEffect(healEfcHandle);
			SetPosPlayingEffekseer3DEffect(a, _modelInf.pos.x, _modelInf.pos.y + 120.f, _modelInf.pos.z);
		}//LBA
	}
	else
	{//�ʏ�Z
		if (checkTrgImput(KEY_INPUT_Z, XINPUT_BUTTON_X))
		{
			if (isNext) { if (attackNumOld != 4) { nextKey = pushButton::X; } }
			else { insEnum = pushButton::X; }
			isPushButtonAct = true;
		}//X
		if (checkTrgImput(KEY_INPUT_X, XINPUT_BUTTON_Y)) { isNext ? nextKey = pushButton::Y : insEnum = pushButton::Y; }//Y
	}

	if (checkKeyImput(KEY_INPUT_Z, XINPUT_BUTTON_X) && Estate == _estate::changeATTACKX)//LBX�`���[�W
	{
		if (isCharge >= 1) { insEnum = pushButton::LBX; }
	}
	else { if (isCharge > 0 && Estate == _estate::changeATTACKX) { insEnum = pushButton::LBX, isCharge = 2, isNext = false, nextKey = pushButton::Neutral; } }//LB,X�������Ƃ�
	if (checkKeyImput(KEY_INPUT_X, XINPUT_BUTTON_Y) && Estate == _estate::changeATTACKY)//LBY�`���[�W
	{
		if (isCharge >= 1) { insEnum = pushButton::LBY; }
	}
	else { if (isCharge > 0 && Estate == _estate::changeATTACKY) { insEnum = pushButton::LBY, isCharge = 2, isNext = false, nextKey = pushButton::Neutral; } }//LB,Y�������Ƃ�

	if (_imputInf->lTriggerX > 100 && _imputInf->rTriggerX > 100 && isAwakening == 2)
	{
		isNext ? nextKey = pushButton::finishAttack : insEnum = pushButton::finishAttack;
	}

	if (Estate == _estate::finishAttack) { return pushButton::Irregular; }

	if (waitNextAttack <= 0 && lastAttackState == _estate::quickATTACK && attackNumOld == 4)
	{
		insEnum = pushButton::X;
	}


	return insEnum;
}

float PL::getMoveDir(bool checkUseCamDir)
{
	float _addDir = 0.f;
	//�ړ���̊p�x�w��
	_addDir = (std::atan2(-_imputInf->lStickX, -_imputInf->lStickY) * 180.f) / DX_PI_F;
	if (_imputInf->lStickY != 0 && _addDir == 0.f) { _addDir = 360.f; }
	if (_addDir != 0) { _addDir += *_cameraDir + 180.f; }
	if (camDir != -1.f && checkUseCamDir)
	{
		_addDir = camDir;
	}
	return _addDir;
}

bool PL::CA_change(std::string name, const char* XorY)
{//changeAttackY = &CA_charge;
	if ("X" == XorY)
	{
		if ("charge" == name) { changeAttackX = &CA_charge; }
		if ("kirinuke" == name) { changeAttackX = &CA_kirinuke; }
		if ("debug" == name) { changeAttackX = &CA_debugAttack; }
		if ("NODATA" == name) { changeAttackX = &CA_noData; }
	}
	else if ("Y" == XorY)
	{
		if ("charge" == name) { changeAttackY = &CA_charge; }
		if ("kirinuke" == name) { changeAttackY = &CA_kirinuke; }
		if ("debug" == name) { changeAttackY = &CA_debugAttack; }
		if ("NODATA" == name) { changeAttackY = &CA_noData; }
	}

	return true;
}


bool PL::CA_debugAttack(PL* insPL)
{
	insPL->setRecastTime = 60;
	auto insDir = insPL->getMoveDir(true);
	if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
	animChange(PL_motion_hissatsu, &insPL->_modelInf, false, false, true);//�A�j���[�V�������o�����K�E�Z���[�V�����ɕύX
	insPL->animSpd = 1.f;
	insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, insPL->getAnimPlayTotalTime(), insPL->animSpd, true, 99999.f, 100000, rWeponParentFrame, VGet(0, 0, 0), 1);

	auto a = VAdd(insPL->_modelInf.pos, getDirVecP(insPL->_modelInf.dir.y - 90, 300));
	auto b = VAdd(insPL->_modelInf.pos, getDirVecP(insPL->_modelInf.dir.y + 90, 300));
	auto bz = getDirVecP(insPL->_modelInf.dir.y, 30);
	a.y = b.y = insPL->_modelInf.pos.y + 200;

	insPL->makeAttackCap(a, b, 40.f, 10.f, insPL->getAnimPlayTotalTime() - 10.f, insPL->animSpd, false, 300.f, 100000, -1, bz, 1);

	int a2 = PlayEffekseer3DEffect(insPL->impactEfcHandle);
	SetPosPlayingEffekseer3DEffect(a2, insPL->_modelInf.pos.x, insPL->_modelInf.pos.y + 120.f, insPL->_modelInf.pos.z);
	SetRotationPlayingEffekseer3DEffect(a2, insPL->_modelInf.dir.x * (DX_PI_F / 180), insPL->_modelInf.dir.y * (DX_PI_F / 180), insPL->_modelInf.dir.z * (DX_PI_F / 180));
	return true;
}

bool PL::CA_charge(PL* insPL)
{
	if (insPL->CAChargeAttackNum > 0)
	{
		auto insDir = insPL->getMoveDir(true);
		if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }

		insPL->animSpd = 3.f;
		PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 14 + rand() % 3], DX_PLAYTYPE_BACK);
		animChange(PL_counter, &insPL->_modelInf, false, false, true);//�A�j���[�V�������J�E���^�[���[�V�����ɕύX
		insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 16.f, 41.f, insPL->animSpd, true, charge3ATK + insPL->atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 0);
		insPL->waitCAChargeTime = 16.f;
		insPL->CAChargeTime = 41.f;
		insPL->CAChargeSpd = 0.f;
		insPL->immortalTime = insPL->_modelInf.totalTime;
		insPL->CAChargeAttackNum--;

		auto a = VAdd(insPL->_modelInf.pos, getDirVecP(insPL->_modelInf.dir.y - 90, 300));
		auto b = VAdd(insPL->_modelInf.pos, getDirVecP(insPL->_modelInf.dir.y + 90, 300));
		auto bz = getDirVecP(insPL->_modelInf.dir.y, 30);
		a.y = b.y = insPL->_modelInf.pos.y + 200;

		insPL->makeAttackCap(a, b, 40.f, 10.f, insPL->getAnimPlayTotalTime() - 10.f, insPL->animSpd, false, 1.f, 100000, -1, bz, 1);

		int a2 = PlayEffekseer3DEffect(insPL->impactEfcHandle);
		SetPosPlayingEffekseer3DEffect(a2, insPL->_modelInf.pos.x, insPL->_modelInf.pos.y + 120.f, insPL->_modelInf.pos.z);
		SetRotationPlayingEffekseer3DEffect(a2, insPL->_modelInf.dir.x * (DX_PI_F / 180), insPL->_modelInf.dir.y * (DX_PI_F / 180), insPL->_modelInf.dir.z * (DX_PI_F / 180));

		return true;
	}

	insPL->chargeTime < 1000 ? insPL->chargeTime += 12 : insPL->chargeTime = 100;

	StartJoypadVibration(DX_INPUT_PAD1, insPL->chargeTime, -1);
	if (insPL->isCharge == 0)
	{
		insPL->isCharge = 1;
		insPL->chargeLevel = 0;
		insPL->chargeTime = 0;
		animChange(PL_arts_tsuki_1, &insPL->_modelInf, false, false, true);//�A�j���[�V������˂����[�V�����P�ɕύX
		setNextAnim(PL_arts_tsuki_2, &insPL->_modelInf, true, true);//�����[�V�����ɓ˂����[�V�����Q���Z�b�g
	}
	if (insPL->isCharge == 2)
	{
		StopJoypadVibration(DX_INPUT_PAD1);
		if (insPL->_modelInf.animHandleNext == -1)
		{
			insPL->chargeLevel = 2;
		}
		else if (insPL->_modelInf.playTime >= insPL->_modelInf.totalTime / 5.f)
		{
			insPL->chargeLevel = 1;
		}
		insPL->_modelInf.animHandleNext = -1;

		auto insDir = insPL->getMoveDir(true);
		if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
		insPL->isCharge = 0;
		if (insPL->chargeLevel == 2)
		{//�t���`���[�W
			insPL->animSpd = 3.f;
			PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 14 + rand() % 3], DX_PLAYTYPE_BACK);
			animChange(PL_arts_tsuki_3, &insPL->_modelInf, false, false, true);//�A�j���[�V������˂����[�V�����R�ɕύX
			insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 16.f, 41.f, insPL->animSpd, true, charge3ATK + insPL->atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
			insPL->waitCAChargeTime = 16.f;
			insPL->CAChargeTime = 41.f;
			insPL->CAChargeSpd = 200.f;
			insPL->immortalTime = insPL->_modelInf.totalTime;
			insPL->CAChargeAttackNum = 2;
		}
		else
		{//�m���`��
			insPL->animSpd = 3.f;
			PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 14 + rand() % 3], DX_PLAYTYPE_BACK);
			animChange(PL_arts_tsuki_3, &insPL->_modelInf, false, false, true);//�A�j���[�V������˂����[�V�����R�ɕύX
			float insDamage = insPL->atkBuff;
			insPL->chargeLevel == 1 ? insDamage += charge2ATK : insDamage += charge1ATK;
			insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 16.f, 41.f, insPL->animSpd, true, insDamage, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
			insPL->waitCAChargeTime = 16.f;
			insPL->CAChargeTime = 41.f;
			insPL->CAChargeSpd = 120.f;
		}
		insPL->isCharge = 0;
		insPL->chargeLevel = 0;
		insPL->setRecastTime = 60;
		//insPL->isGhost = true;
	}

	return true;
}

bool PL::CA_kirinuke(PL* insPL)
{
	insPL->animSpd = 1.f;
	auto insDir = insPL->getMoveDir(true);
	if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
	PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 22 + rand() % 2], DX_PLAYTYPE_BACK);
	animChange(PL_arts_kirinuke, &insPL->_modelInf, false, false, true);//�A�j���[�V������؂蔲�����[�V�����ɕύX
	insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 100.f), 20.f, 0.f, insPL->getAnimPlayTotalTime(), insPL->animSpd, true, kirinukeATK + insPL->atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
	insPL->waitCAChargeTime = 12.f;
	insPL->CAChargeTime = 53.f - insPL->waitCAChargeTime;
	insPL->CAChargeSpd = 40.f;
	insPL->isGhost = true;
	insPL->setRecastTime = 60;

	return true;
}


bool PL::CA_noData(PL* insPL) { return false; }