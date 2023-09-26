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

//�e�U���̃_���[�W�Q�Ɨp�ԍ��ݒ�
#define jakuNum 0
#define kyouNum 1
#define counterNum 2
#define debugNum 3
#define kirinukeNum 4
#define charge1Num 5
#define charge2Num 6
#define charge3Num 7
#define finishNum 8

//�e�U���̍U������̎w��
#define defaultAttackUnderPos VGet(0.f, 0.f, 0.f)
#define defaultAttackOverPos VGet(0.f, 0.f, 300.f)
#define skillAttackOverPos VGet(0.f, 0.f, 200.f)

//����Ǐ]�t���[���ԍ��ݒ�
#define rWeponParentFrame 192
#define lWeponParentFrame 167

typedef ExclusiveState _estate;
using namespace model;

bool player::Initialize()
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

	//�J�n���_�̎��@�̈ʒu�ƕ����ݒ�
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
	for (std::string seNameList : _valData->playerSeList)
	{
		std::string insName = "game/res/SE/player/" + seNameList;
		soundHandle.emplace_back(LoadSoundMem(insName.c_str()));
	}
	voiceStartNum = soundHandle.size();
	for (std::string voiceNameList : _valData->playerVoiceList)
	{
		std::string insName = "game/res/voice/player/" + voiceNameList;
		soundHandle.emplace_back(LoadSoundMem(insName.c_str()));
	}

	return true;
}

void player::loadEfekseer()
{
	//�G�t�F�N�g�ǂݍ���
	guardEfcHandle = LoadEffekseerEffect("game/res/effect/�K�[�h/guard_effect_01.efkefc", 20.f);
	chargeEfcHandle = LoadEffekseerEffect("game/res/effect/�`���[�W�G�t�F�N�g/charge.efkefc", 20.f);
	healEfcHandle = LoadEffekseerEffect("game/res/effect/��1/heal_01.efkefc", 60.f);
	impactEfcHandle = LoadEffekseerEffect("game/res/effect/�Ռ��g1/slash_shot.efkefc", 80.f);

	return;
}

bool	player::Terminate()
{
	CharBase::Terminate();
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
	MI->wepons[0]->isActive = isIO;
	MI->wepons[1]->isActive = isIO;
	MI->wepons[2]->isActive = !isIO;

	return true;
}

bool	player::Process()
{
	//�}�X�^�[���ʂ̓K��
	if (!isSetSoundValume) { setMasterVolume(_valData->soundMasterValume); isSetSoundValume = true; }
	//���S������
	if (_statusInf.hitPoint <= 0)
	{
		isDead = 1;//���S��ԕϐ������S���[�V�����Đ���Ԃ�
		animSpd = .7f;//�A�j���[�V�����Đ����x�ݒ�
		_modelInf.animSpdBuff = .0f;//�A�j���[�V�����Đ����x�o�t�ݒ�

		animChange(PL_DEATH, &_modelInf, false, true, false);//�A�j���[�V���������S���[�V�����ɕύX
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
	if (_imputInf->lTriggerX < 20 && _imputInf->rTriggerX < 20 && isAwakening == 1) { isAwakening = 2; }

	//�o�����Ƀu���b�h�|�C���g�̌����A�s�����ۂ̊o����������
	if (isAwakening > 0)
	{
		if (_statusInf.bloodPoint > 0) { _statusInf.bloodPoint--; }
		else
		{
			_statusInf.bloodPoint = 0;
			isAwakening = 0;
			atkBuff = 0.f;
			_modelInf.animSpdBuff = 0.f;
			PlaySoundMem(soundHandle[19], DX_PLAYTYPE_BACK);
		}
	}

	//�L�[�{�[�h�ł̎��@�ړ�����(�f�o�b�O�p)
	if (CheckHitKey(KEY_INPUT_W)) { spd = runSpd; charMove(spd, *_cameraDir, false); }
	if (CheckHitKey(KEY_INPUT_A)) { spd = runSpd; charMove(spd, *_cameraDir + 270.f, false); }
	if (CheckHitKey(KEY_INPUT_S)) { spd = runSpd; charMove(spd, *_cameraDir + 180.f, false); }
	if (CheckHitKey(KEY_INPUT_D)) { spd = runSpd; charMove(spd, *_cameraDir + 90.f, false); }

	//���̍s�����菈��
	float addDir = 0.f, insDir, a;
	bool moveCheck = true;
	switch (setAction())
	{
	case pushButton::Damage://��e
		//��e���[�V�����Đ����񂾂����鏈��
		if (_modelInf.playTime <= animSpd)
		{
			dodgeTime = 0, chargeLevel = 0, waitCAChargeTime = 0, CAChargeTime = 0, isGhost = false, _modelInf.animHandleNext = -1, attackNumOld = 0, waitNextAttack = 0, CAChargeAttackNum = 0;
			isCharge = 0, isFastGuard = true, isGuard = true;
			nextKey = pushButton::Neutral;
			recastSet();
		}

		//������΂��ꒆ�̏���
		//������є��肪�o�Ă��邩
		if (isBlow)
		{
			animSpd = 2.f;
			animChange(PL_huttobi1, &_modelInf, false, false, false);
			setNextAnim(PL_huttobi2, &_modelInf, false, false);
			if (_modelInf.playTime > 180.f) { Estate = _estate::NORMAL; }
			//������у��[�V�����Đ�����
			auto animName = _modelInf.animHandleOld;
			if (animName == PL_huttobi1)
			{
				//������у��[�V�����Đ����񂾂����鏈��
				if (_modelInf.playTime <= animSpd)
				{
					_modelInf.dir.y = getMoveDir(true);
					waitCAChargeTime = 20.f;
					CAChargeTime = 100.f;
					CAChargeSpd = -30.f;
				}
				//������у��[�V���������G���Ԃ̐ݒ�
				immortalTime = getAnimPlayTotalTime() - _modelInf.playTime;
				if (_modelInf.playTime > 140.f) { Estate = _estate::NORMAL; }
			}
		}
		else
		{
			//��e����͏o�Ă��邪������є��肪�o�Ă��Ȃ��̂Ŕ�e���[�V�������Đ�
			animChange(PL_hirumi, &_modelInf, false, false, false);
			if (_modelInf.playTime > 25.f) { Estate = _estate::NORMAL; }
		}

		break;
	case pushButton::B://���
		//�ݒ菈��
		dodgeTime = 0, chargeLevel = 0, waitCAChargeTime = 0, CAChargeTime = 0, isGhost = false, _modelInf.animHandleNext = -1, attackNumOld = 0, waitNextAttack = 0, CAChargeAttackNum = 0;
		animSpd = 4.f;
		spd = 70.f;
		isCharge = 0;
		recastSet();

		//�������̐ݒ�
		insDir = getMoveDir(false);
		if (insDir == 0) { insDir = _modelInf.dir.y; }
		dodgeDir = _modelInf.dir.y - insDir;
		if (dodgeDir > 360) { dodgeDir -= 360; }
		else if (dodgeDir < 0) { dodgeDir += 360; }

		//����������ɂ������[�V�����̑I���ƍĐ�
		if (dodgeDir >= 45 && 135 > dodgeDir) { animChange(PL_DODGE_L, &_modelInf, false, false, false); }//�A�j���[�V��������������[�V�����ɕύX
		else if (dodgeDir >= 135 && 225 > dodgeDir) { animChange(PL_DODGE_B, &_modelInf, false, false, false); }//�A�j���[�V��������������[�V�����ɕύX
		else if (dodgeDir >= 225 && 315 > dodgeDir) { animChange(PL_DODGE_R, &_modelInf, false, false, false); }//�A�j���[�V�������E������[�V�����ɕύX
		else { animChange(PL_DODGE_F, &_modelInf, false, false, false); }//�A�j���[�V������O������[�V�����ɕύX

		//���G����̐ݒ�
		dodgeTime = getAnimPlayTotalTime();
		immortalTime = dodgeTime;

		insDir != 0 ? dodgeDir = insDir : dodgeDir = _modelInf.dir.y;
		break;
	case pushButton::X://��U��
		//�ݒ菈��
		insDir = getMoveDir(true);
		if (insDir != 0) { _modelInf.dir.y = insDir; }
		Estate = _estate::quickATTACK;
		lastAttackState = _estate::quickATTACK;
		waitNextAttack = 40;
		animSpd = 2.f;

		//�{�C�X�Đ�
		if (attackNumOld < 3) { PlaySoundMem(soundHandle[voiceStartNum + 0 + rand() % 4], DX_PLAYTYPE_BACK); }
		else { PlaySoundMem(soundHandle[voiceStartNum + 4 + rand() % 2], DX_PLAYTYPE_BACK); }

		//�U�������i�ڂ��̔���ƍU������
		if (attackNumOld == 0)
		{
			PlaySoundMem(soundHandle[12], DX_PLAYTYPE_BACK);
			animChange(PL_JAB_1, &_modelInf, false, false, true);//�A�j���[�V��������U���P�i�ڃ��[�V�����ɕύX
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
		}
		else if (attackNumOld == 1)
		{
			PlaySoundMem(soundHandle[13], DX_PLAYTYPE_BACK);
			animChange(PL_JAB_2, &_modelInf, false, false, true);//�A�j���[�V��������U���Q�i�ڃ��[�V�����ɕύX
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
		}
		else if (attackNumOld == 2)
		{
			PlaySoundMem(soundHandle[14], DX_PLAYTYPE_BACK);
			animChange(PL_JAB_3, &_modelInf, false, false, true);//�A�j���[�V��������U���R�i�ڃ��[�V�����ɕύX
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
		}
		else if (attackNumOld == 3)
		{
			PlaySoundMem(soundHandle[14], DX_PLAYTYPE_BACK);
			animChange(PL_JAB_4_1, &_modelInf, false, false, true);//�A�j���[�V��������U���S�i�ڃ��[�V�����P�ɕύX
			setNextAnim(PL_JAB_4_2, &_modelInf, true, true);//���A�j���[�V�����ɔ[�����Z�b�g
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
		}
		else if (attackNumOld == 4 && isPushButtonAct)
		{
			PlaySoundMem(soundHandle[14], DX_PLAYTYPE_BACK);
			_modelInf.animHandleNext = -1;
			animChange(PL_JAB_4_3, &_modelInf, false, false, true);//�A�j���[�V��������U���S�i�ڃ��[�V�����Q�ɕύX
			setNextAnim(PL_JAB_4_2, &_modelInf, true, false);//���A�j���[�V�����ɔ[�����Z�b�g
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld = 4;
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 0.f, 12.f, animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 17.f, 8.f, animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 32.f, 10.f, animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 47.f, 13.f, animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
		}
		else if (attackNumOld == 4 && !isPushButtonAct)
		{
			PlaySoundMem(soundHandle[14], DX_PLAYTYPE_BACK);
			_modelInf.animHandleNext = -1;
			animChange(PL_JAB_4_4, &_modelInf, false, false, true);//�A�j���[�V��������U���S�i�ڃ��[�V�����R�ɕύX
			waitNextAttack = 0;
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, _valData->plAtkNum[jakuNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
		}

		break;
	case pushButton::Y://���U��
		//�ݒ菈��
		insDir = getMoveDir(true);
		if (insDir != 0) { _modelInf.dir.y = insDir; }
		Estate = _estate::slowATTACK;
		lastAttackState = _estate::slowATTACK;
		waitNextAttack = 40;
		animSpd = 2.f;

		//�U�������i�ڂ��̔���ƍU������
		if (attackNumOld == 0)
		{
			animChange(PL_kyou_4_1, &_modelInf, false, false, true);//�A�j���[�V���������U���P�i�ڃ��[�V�����ɕύX
			waitNextAttack += getAnimPlayTotalTime();
			attackNumOld++;
		}
		else if (attackNumOld == 1)
		{
			PlaySoundMem(soundHandle[14], DX_PLAYTYPE_BACK);
			animChange(PL_kyou_4_3, &_modelInf, false, false, true);//�A�j���[�V���������U���Q�i�ڃ��[�V�����ɕύX
			waitNextAttack = 0;
			makeAttackCap(defaultAttackUnderPos, defaultAttackOverPos, 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, _valData->plAtkNum[kyouNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 2);
		}

		break;
	case pushButton::LBX://����ւ�X
		//�ݒ菈��
		Estate = _estate::changeATTACKX;
		lastAttackState = _estate::changeATTACKX;
		attackNumOld = 0;
		changeAttackX(this);//����ւ��ZX�̊֐��|�C���^�Ăяo��
		break;
	case pushButton::LBY://����ւ�Y
		//�ݒ菈��
		Estate = _estate::changeATTACKY;
		lastAttackState = _estate::changeATTACKY;
		attackNumOld = 0;
		changeAttackY(this);//����ւ��ZX�̊֐��|�C���^�Ăяo��
		break;
	case pushButton::finishAttack://�K�E�Z
		//�ݒ菈��
		Estate = _estate::finishAttack;
		lastAttackState = _estate::finishAttack;
		attackNumOld = 0;
		insDir = getMoveDir(true);
		if (insDir != 0) { _modelInf.dir.y = insDir; }
		PlaySoundMem(soundHandle[voiceStartNum + 39 + rand() % 2], DX_PLAYTYPE_BACK);//�{�C�X�Đ�
		animChange(PL_motion_hissatsu, &_modelInf, false, false, true);//�A�j���[�V�������o�����K�E�Z���[�V�����ɕύX
		makeAttackCap(defaultAttackUnderPos, skillAttackOverPos, 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, _valData->plAtkNum[finishNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
		//�o�����̃X�e�[�^�X����
		_statusInf.bloodPoint = 0, immortalTime = _modelInf.totalTime, _modelInf.animSpdBuff = 0.f;
		break;
	case pushButton::Lstick://�_�b�V��
		//�ݒ菈��
		Estate = _estate::NORMAL;
		attackNumOld = 0;

		//�ړ���̊p�x���x�N�g���ɂ��Ĉړ��x�N�g���ɉ��Z
		addDir = getMoveDir(false);
		spd = runSpd;//walkTime
		if (addDir != 0) { charMove(spd * (walkTime / 10), addDir, true); }
		moveCheck = false;
		break;
	case pushButton::R1://�K�[�h
		//�ݒ菈��
		dodgeTime = 0, chargeLevel = 0, waitCAChargeTime = 0, CAChargeTime = 0, isGhost = false, _modelInf.animHandleNext = -1, attackNumOld = 0, waitNextAttack = 0, CAChargeAttackNum = 0;
		recastSet();

		//�K�[�h�g�p����񂾂��Ăԏ���
		if (Estate != _estate::GUARD && isFastGuard)
		{
			Estate = _estate::GUARD;
			insGuardEfcHandle = PlayEffekseer3DEffect(guardEfcHandle);
			SetPosPlayingEffekseer3DEffect(insGuardEfcHandle, _modelInf.pos.x, _modelInf.pos.y + 100.f, _modelInf.pos.z);
			PlaySoundMem(soundHandle[17], DX_PLAYTYPE_BACK);

			counterTime = _valData->_counterTime;
		}
		//�K�[�h���L�����Z�����ē�����悤�ɏ���
		nextKey = pushButton::Neutral;

		//�J�E���^�[���肪�o�Ă��邩����
		if (isCounter == 1)
		{
			//�ݒ菈��
			insDir = getMoveDir(true);
			if (insDir != 0) { _modelInf.dir.y = insDir; }
			animSpd = 2.f;
			isFastGuard = true, isCounter = 2, counterTime = 0;
			immortalTime = getAnimPlayTotalTime();
			_valData->hitstopF = 5;//�q�b�g�X�g�b�v
			//�U��������
			KATANAIO(&_modelInf, true);//�J�^�i�̏���J�^�i�ɂ��邩���ɂ��邩�̐ݒ�ύX
			animChange(PL_COUNTER, &_modelInf, false, false, true);//�A�j���[�V�������J�E���^�[���[�V�����ɕύX
			makeAttackCap(defaultAttackUnderPos, skillAttackOverPos, 20.f, 0.f, getAnimPlayTotalTime(), animSpd, true, _valData->plAtkNum[counterNum] + atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
			//��Ԏa���ւ̐ݒ�
			VECTOR underPos, overPos, modeDir;
			mathFlyingSlashPos(&underPos, &overPos, &modeDir, this);
			makeAttackCap(underPos, overPos, 40.f, 10.f, getAnimPlayTotalTime() - 10.f, animSpd, false, 75.f, 20, -1, modeDir, 0);

			//�����̍Đ�
			PlaySoundMem(soundHandle[15], DX_PLAYTYPE_BACK);
			PlaySoundMem(soundHandle[voiceStartNum + 37 + rand() % 2], DX_PLAYTYPE_BACK);

			//�G�t�F�N�g�Đ�
			int a2 = PlayEffekseer3DEffect(impactEfcHandle);
			SetPosPlayingEffekseer3DEffect(a2, _modelInf.pos.x, _modelInf.pos.y + 120.f, _modelInf.pos.z);
			SetRotationPlayingEffekseer3DEffect(a2, _modelInf.dir.x * (DX_PI_F / 180), _modelInf.dir.y * (DX_PI_F / 180), _modelInf.dir.z * (DX_PI_F / 180));
		}
		//�K�[�h�������J�E���^�[���肪�o�Ă��Ȃ�
		else if (isFastGuard && isCounter == 0)
		{
			animSpd = 10.f;
			animSpd = _valData->counterSpd;
			animChange(PL_GUARD_1, &_modelInf, false, false, false);//�A�j���[�V�������K�[�h���[�V�����P�ɕύX
		}
		else if (isCounter != 2)
		{
			animSpd = 1.f;
			animChange(PL_GUARD_2, &_modelInf, true, false, false);//�A�j���[�V�������K�[�h���[�V�����Q�ɕύX
			Estate = _estate::NORMAL;
			BPmath(-1);
		}
		break;
	case pushButton::Neutral://���͂Ȃ�
		//�ꕔ���蒆�ҋ@���[�V�����Ɉڍs���Ȃ��悤�ݒ�
		if (_modelInf.animHandleOld == PL_huttobi1 || _modelInf.animHandleOld == PL_huttobi2) { break; }
		if (attackNumOld > 0) { break; }

		//�ҋ@���[�V�����Ɉڍs
		Estate = _estate::NORMAL;
		animChange(PL_IDLE, &_modelInf, true, true, false);//�A�j���[�V������ҋ@���[�V�����ɕύX
		spd = 0.f;
		break;
	default://���̑�
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
		auto  bossDisVector = VSub(i->second->getInf()->pos, _modelInf.pos);
		float bossDisFloat = sqrt(bossDisVector.x * bossDisVector.x + bossDisVector.y * bossDisVector.y + bossDisVector.z * bossDisVector.z);
		float plzDis = i->second->collCap.r + collCap.r;
		if (bossDisFloat < plzDis)
		{
			_modelInf.pos = VAdd(VScale(VNorm(bossDisVector), -plzDis), i->second->getInf()->pos);
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
	awakeSeCoolTime > 0 ? awakeSeCoolTime-- : awakeSeCoolTime = 0;

	//�U���q�b�g����SE�Đ�
	if (isHit)
	{
		BPmath(100);
		isHit = false;
	}

	//modeGame�ł̃J�E���^�[��t���ԍĐ��p
	_valData->plCTimeN = counterTime;

	return true;
}

bool	player::Render(float timeSpeed)
{
	int i = 0;
	isAnimEnd = modelRender(&_modelInf, animSpd, timeSpeed);//�A�j���[�V�����Đ�

	//�񕜁A�K�[�h�G�t�F�N�g���L�����ɒǏ]����悤��
	if (IsEffekseer3DEffectPlaying(insGuardEfcHandle) == 0)
	{
		SetPosPlayingEffekseer3DEffect(insGuardEfcHandle, _modelInf.pos.x, _modelInf.pos.y + 100.f, _modelInf.pos.z);
	}
	if (IsEffekseer3DEffectPlaying(insHealEfcHandle) == 0)
	{
		SetPosPlayingEffekseer3DEffect(insHealEfcHandle, _modelInf.pos.x, _modelInf.pos.y + 120.f, _modelInf.pos.z);
	}
	return true;
}

void player::charMove(float Speed, float _Dir, bool isAnimChange)
{
	//�ݒ菈��
	_Dir -= 180.f;
	float radian = _Dir * DX_PI_F / 180.0f;
	_modelInf.vec.x += sin(radian) * Speed;
	_modelInf.vec.z += cos(radian) * Speed;

	//�L�����ړ����A�j���[�V�����ύX����
	if (isAnimChange)
	{
		animChange(PL_RUN, &_modelInf, true, true, false);//�A�j���[�V�����𑖂胂�[�V�����ɕύX
		animSpd = 1.f;
		_modelInf.dir.y = _Dir + 180.f;
	}
}

bool player::HPmath(float math, float Stan)
{
	isBlow = false;
	//�_���[�W���񕜂��̔���
	if (math < 0)
	{
		//�J�E���^�[���菈��
		if (counterTime > 0) { isCounter = 1; }
		else if (immortalTime <= 0)
		{
			//�K�[�h��������
			if (!isGuard || isFastGuard)
			{
				//�o�������G����
				if (isAwakening == 0)
				{
					if (!deadVoice) { PlaySoundMem(soundHandle[voiceStartNum + 27 + rand() % 4], DX_PLAYTYPE_BACK); }
					_statusInf.hitPoint += math; BPmath(std::abs(math) * 6);
				}
				PlaySoundMem(soundHandle[11], DX_PLAYTYPE_BACK);
				Estate = _estate::DAMAGE;
				//������у��[�V�����Ɉڍs���邩�̔���
				if (math < -50 || waitBlowTime > 0) { isBlow = true; }
				else { waitBlowTime = 100; }
			}

			//�m�b�N�o�b�N����
			auto ACDisV = VSub(_modelInf.pos, charBox->find(attackChar)->second->_modelInf.pos);
			ACDisV = VNorm(ACDisV);
			_modelInf.vec = VScale(ACDisV, 50);
		}
	}
	else
	{
		//�񕜏���
		_statusInf.hitPoint += math;
		if (_statusInf.hitPoint > _statusInf.maxHitPoint) { _statusInf.hitPoint = _statusInf.maxHitPoint; }
	}

	//��𐬌����{�C�X�Đ�
	if (dodgeTime > 0) { PlaySoundMem(soundHandle[voiceStartNum + 24 + rand() % 3], DX_PLAYTYPE_BACK); }

	return isBlow;
}

bool player::BPmath(float math)
{
	//�o��������
	if (isAwakening > 0 && math > 0) { return false; }

	//BP��������
	_statusInf.bloodPoint += math;
	if (_statusInf.bloodPoint > _statusInf.maxBloodPoint)
	{
		_statusInf.bloodPoint = _statusInf.maxBloodPoint;
		if (awakeSeCoolTime <= 0) { PlaySoundMem(soundHandle[20], DX_PLAYTYPE_BACK);			awakeSeCoolTime = 240; }
	}
	if (_statusInf.bloodPoint < 0) { _statusInf.bloodPoint = 0; }

	return true;
}

pushButton player::setAction()
{
	//�ݒ菈��
	bool isNext = false;
	isPushButtonAct = false;
	pushButton insEnum = pushButton::Neutral;

	//�ꕔ���[�V����������X�L�b�v
	if (Estate == _estate::DAMAGE) { return pushButton::Damage; }
	if (isCounter == 1) { return pushButton::R1; }

	//�A�j���[�V�����̍Đ��I����̔���
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

	//��s���͏���
	if (nextKey != pushButton::Neutral && !isNext && isCharge != 1 && !isGuard && Estate != _estate::DODGE) { insEnum = nextKey, nextKey = pushButton::Neutral; return insEnum; }

	if (checkTrgImput(KEY_INPUT_SPACE, XINPUT_BUTTON_B))//���
	{
		attackNumOld = 0;
		Estate = _estate::DODGE;
		return insEnum = pushButton::B;
	}
	else if (checkKeyImput(KEY_INPUT_G, XINPUT_BUTTON_RIGHT_SHOULDER) && dodgeTime <= 0)//�K�[�h
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

	//�ړ�
	if (checkKeyImput(KEY_INPUT_LSHIFT, XINPUT_BUTTON_LEFT_THUMB) || getMoveDir(false) != 0)
	{
		if (Estate != _estate::slowATTACK) { insEnum = pushButton::Lstick; }
		walkTime < 10 ? walkTime++ : walkTime = 10;
	}
	else { walkTime = 0; }
	if (isNext) { insEnum = pushButton::Irregular; }

	//����ւ��Z
	if (checkKeyImput(KEY_INPUT_C, XINPUT_BUTTON_LEFT_SHOULDER))
	{
		//X���͓���ւ��Z
		if (checkTrgImput(KEY_INPUT_Z, XINPUT_BUTTON_X) && caRecastX <= 0) {
			if (isNext) { if (Estate != _estate::changeATTACKX) { nextKey = pushButton::LBX; } }
			else { insEnum = pushButton::LBX; }
		}
		//Y���͓���ւ��Z
		if (checkTrgImput(KEY_INPUT_X, XINPUT_BUTTON_Y) && caRecastY <= 0) {
			if (isNext) { if (Estate != _estate::changeATTACKY) { nextKey = pushButton::LBY; } }
			else { insEnum = pushButton::LBY; }
		}
		//��
		if (checkTrgImput(KEY_INPUT_V, XINPUT_BUTTON_A) && _statusInf.bloodPoint > 500.f)
		{
			HPmath(100, 10); BPmath(-500);
			insHealEfcHandle = PlayEffekseer3DEffect(healEfcHandle);
			SetPosPlayingEffekseer3DEffect(insHealEfcHandle, _modelInf.pos.x, _modelInf.pos.y + 120.f, _modelInf.pos.z);
			PlaySoundMem(soundHandle[1], DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		//��U��
		if (checkTrgImput(KEY_INPUT_Z, XINPUT_BUTTON_X))
		{
			if (isNext) { if (attackNumOld != 4) { nextKey = pushButton::X; } }
			else { insEnum = pushButton::X; }
			isPushButtonAct = true;
		}
		//���U��
		if (checkTrgImput(KEY_INPUT_X, XINPUT_BUTTON_Y)) { isNext ? nextKey = pushButton::Y : insEnum = pushButton::Y; }
	}

	//X���͎�����ւ��Z�̃`���[�W����
	if (checkKeyImput(KEY_INPUT_Z, XINPUT_BUTTON_X) && Estate == _estate::changeATTACKX)
	{
		if (isCharge >= 1) { insEnum = pushButton::LBX; }
	}
	else { if (isCharge > 0 && Estate == _estate::changeATTACKX) { insEnum = pushButton::LBX, isCharge = 2, isNext = false, nextKey = pushButton::Neutral; } }
	//Y���͎�����ւ��Z�̃`���[�W����
	if (checkKeyImput(KEY_INPUT_X, XINPUT_BUTTON_Y) && Estate == _estate::changeATTACKY)
	{
		if (isCharge >= 1) { insEnum = pushButton::LBY; }
	}
	else { if (isCharge > 0 && Estate == _estate::changeATTACKY) { insEnum = pushButton::LBY, isCharge = 2, isNext = false, nextKey = pushButton::Neutral; } }

	//�o�����t�B�j�b�V���Z
	if (_imputInf->lTriggerX > 100 && _imputInf->rTriggerX > 100 && isAwakening == 2)
	{
		isNext ? nextKey = pushButton::finishAttack : insEnum = pushButton::finishAttack;
	}
	if (Estate == _estate::finishAttack) { return pushButton::Irregular; }

	//��U���㖳���͂ŏo�锲���U��
	if (waitNextAttack <= 0 && lastAttackState == _estate::quickATTACK && attackNumOld == 4)
	{
		insEnum = pushButton::X;
	}

	return insEnum;
}

float player::getMoveDir(bool checkUseCamDir)
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

bool player::CA_change(std::string name, const char* XorY)
{
	//X���͎�����ւ��Z�ݒ�
	if ("X" == XorY)
	{
		if ("���" == name) { changeAttackX = &CA_charge; }
		if ("�ؔ�" == name) { changeAttackX = &CA_kirinuke; }
		if ("���M" == name) { changeAttackX = &CA_debugAttack; }
		if ("NODATA" == name) { changeAttackX = &CA_noData; }
	}
	//Y���͎�����ւ��Z�ݒ�
	else if ("Y" == XorY)
	{
		if ("���" == name) { changeAttackY = &CA_charge; }
		if ("�ؔ�" == name) { changeAttackY = &CA_kirinuke; }
		if ("���M" == name) { changeAttackY = &CA_debugAttack; }
		if ("NODATA" == name) { changeAttackY = &CA_noData; }
	}

	return true;
}


bool player::CA_debugAttack(player* insPL)
{
	//�ݒ菈��
	insPL->setRecastTime = 60;
	auto insDir = insPL->getMoveDir(true);
	if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
	animChange(PL_motion_hissatsu, &insPL->_modelInf, false, false, true);//�A�j���[�V�������o�����K�E�Z���[�V�����ɕύX
	insPL->animSpd = 1.f;
	insPL->makeAttackCap(defaultAttackUnderPos, skillAttackOverPos, 20.f, 0.f, insPL->getAnimPlayTotalTime(), insPL->animSpd, true, 200.f + insPL->atkBuff, 5.f, rWeponParentFrame, VGet(0, 0, 0), 1);

	//��Ԏa���ւ̐ݒ�
	VECTOR underPos, overPos, modeDir;
	insPL->mathFlyingSlashPos(&underPos, &overPos, &modeDir, insPL);
	insPL->makeAttackCap(underPos, overPos, 40.f, 10.f, 50 - 10.f, insPL->animSpd, false, insPL->_valData->plAtkNum[debugNum] + insPL->atkBuff, 5.f, -1, modeDir, 0);

	//�G�t�F�N�g�ASE�Đ�
	int a2 = PlayEffekseer3DEffect(insPL->impactEfcHandle);
	SetPosPlayingEffekseer3DEffect(a2, insPL->_modelInf.pos.x, insPL->_modelInf.pos.y + 120.f, insPL->_modelInf.pos.z);
	SetRotationPlayingEffekseer3DEffect(a2, insPL->_modelInf.dir.x * (DX_PI_F / 180), insPL->_modelInf.dir.y * (DX_PI_F / 180), insPL->_modelInf.dir.z * (DX_PI_F / 180));
	PlaySoundMem(insPL->soundHandle[6], DX_PLAYTYPE_BACK);
	return true;
}

bool player::CA_charge(player* insPL)
{
	//�`���[�W�U����̔�Ԏa������
	if (insPL->CAChargeAttackNum > 0)
	{
		//�ݒ菈��
		auto insDir = insPL->getMoveDir(true);
		if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
		insPL->animSpd = 3.f;
		PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 14 + rand() % 3], DX_PLAYTYPE_BACK);
		animChange(PL_COUNTER, &insPL->_modelInf, false, false, true);//�A�j���[�V�������J�E���^�[���[�V�����ɕύX
		insPL->makeAttackCap(defaultAttackUnderPos, skillAttackOverPos, 20.f, 16.f, 41.f, insPL->animSpd, true, insPL->_valData->plAtkNum[charge3Num] + insPL->atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
		insPL->waitCAChargeTime = 16.f;
		insPL->CAChargeTime = 41.f;
		insPL->CAChargeSpd = 0.f;
		insPL->immortalTime = insPL->_modelInf.totalTime;
		insPL->CAChargeAttackNum--;

		//��Ԏa���ւ̐ݒ�
		VECTOR underPos, overPos, modeDir;
		insPL->mathFlyingSlashPos(&underPos, &overPos, &modeDir, insPL);
		insPL->makeAttackCap(underPos, overPos, 40.f, 10.f, insPL->getAnimPlayTotalTime() - 10.f, insPL->animSpd, false, insPL->_valData->plAtkNum[debugNum] + insPL->atkBuff, 5.f, -1, modeDir, 0);

		//�G�t�F�N�g�ASE�Đ�
		int a2 = PlayEffekseer3DEffect(insPL->impactEfcHandle);
		SetPosPlayingEffekseer3DEffect(a2, insPL->_modelInf.pos.x, insPL->_modelInf.pos.y + 120.f, insPL->_modelInf.pos.z);
		SetRotationPlayingEffekseer3DEffect(a2, insPL->_modelInf.dir.x * (DX_PI_F / 180), insPL->_modelInf.dir.y * (DX_PI_F / 180), insPL->_modelInf.dir.z * (DX_PI_F / 180));
		PlaySoundMem(insPL->soundHandle[6], DX_PLAYTYPE_BACK);
		return true;
	}

	//�R���g���[���[�U��
	insPL->chargeTime < 1000 ? insPL->chargeTime += 12 : insPL->chargeTime = 100;
	StartJoypadVibration(DX_INPUT_PAD1, insPL->chargeTime, -1);

	//�`���[�W�J�n���x�����̏���
	if (insPL->isCharge == 0)
	{
		insPL->isCharge = 1;
		insPL->chargeLevel = 0;
		insPL->chargeTime = 0;
		animChange(PL_arts_tsuki_1, &insPL->_modelInf, false, false, true);//�A�j���[�V������˂����[�V�����P�ɕύX
		setNextAnim(PL_arts_tsuki_2, &insPL->_modelInf, true, true);//�����[�V�����ɓ˂����[�V�����Q���Z�b�g
	}
	//�`���[�W����㏈��
	if (insPL->isCharge == 2)
	{
		//�R���g���[���[�U����~
		StopJoypadVibration(DX_INPUT_PAD1);

		//�`���[�W���x����
		if (insPL->_modelInf.animHandleNext == -1)
		{
			insPL->chargeLevel = 2;
		}
		else if (insPL->_modelInf.playTime >= insPL->_modelInf.totalTime / 5.f)
		{
			insPL->chargeLevel = 1;
		}
		insPL->_modelInf.animHandleNext = -1;

		//�ݒ菈��
		auto insDir = insPL->getMoveDir(true);
		if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
		insPL->isCharge = 0;

		//�ő�`���[�W������
		if (insPL->chargeLevel == 2)
		{
			insPL->animSpd = 3.f;
			PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 14 + rand() % 3], DX_PLAYTYPE_BACK);
			PlaySoundMem(insPL->soundHandle[4], DX_PLAYTYPE_BACK);
			animChange(PL_arts_tsuki_3, &insPL->_modelInf, false, false, true);//�A�j���[�V������˂����[�V�����R�ɕύX
			insPL->makeAttackCap(defaultAttackUnderPos, skillAttackOverPos, 20.f, 16.f, 41.f, insPL->animSpd, true, insPL->_valData->plAtkNum[charge3Num] + insPL->atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
			insPL->waitCAChargeTime = 16.f;
			insPL->CAChargeTime = 41.f;
			insPL->CAChargeSpd = 200.f;
			insPL->immortalTime = insPL->_modelInf.totalTime;
			insPL->CAChargeAttackNum = 2;
		}
		//���`���[�W������
		else
		{
			insPL->animSpd = 3.f;
			PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 14 + rand() % 3], DX_PLAYTYPE_BACK);
			PlaySoundMem(insPL->soundHandle[4], DX_PLAYTYPE_BACK);
			animChange(PL_arts_tsuki_3, &insPL->_modelInf, false, false, true);//�A�j���[�V������˂����[�V�����R�ɕύX

			//�U�����ݒ�K�p
			float insDamage = insPL->atkBuff;
			insPL->chargeLevel == 1 ? insDamage += insPL->_valData->plAtkNum[charge2Num] : insDamage += insPL->_valData->plAtkNum[charge1Num];
			insPL->makeAttackCap(defaultAttackUnderPos, skillAttackOverPos, 20.f, 16.f, 41.f, insPL->animSpd, true, insDamage, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
			insPL->waitCAChargeTime = 16.f;
			insPL->CAChargeTime = 41.f;
			insPL->CAChargeSpd = 120.f;
		}
		insPL->isCharge = 0;
		insPL->chargeLevel = 0;
		insPL->setRecastTime = 60;
	}

	return true;
}

bool player::CA_kirinuke(player* insPL)
{
	//�ݒ菈��
	insPL->animSpd = 1.f;
	auto insDir = insPL->getMoveDir(true);
	if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
	//SE�A�{�C�X�Đ�
	PlaySoundMem(insPL->soundHandle[insPL->voiceStartNum + 22 + rand() % 2], DX_PLAYTYPE_BACK);
	PlaySoundMem(insPL->soundHandle[4], DX_PLAYTYPE_BACK);
	animChange(PL_arts_kirinuke, &insPL->_modelInf, false, false, true);//�A�j���[�V������؂蔲�����[�V�����ɕύX

	//�U�����ݒ�K�p
	insPL->makeAttackCap(defaultAttackUnderPos, skillAttackOverPos, 20.f, 0.f, insPL->getAnimPlayTotalTime(), insPL->animSpd, true, insPL->_valData->plAtkNum[kirinukeNum] + insPL->atkBuff, 5, rWeponParentFrame, VGet(0, 0, 0), 1);
	insPL->waitCAChargeTime = 12.f;
	insPL->CAChargeTime = 53.f - insPL->waitCAChargeTime;
	insPL->CAChargeSpd = 40.f;
	insPL->isGhost = true;
	insPL->setRecastTime = 60;

	return true;
}

bool player::CA_noData(player* insPL) { return false; }