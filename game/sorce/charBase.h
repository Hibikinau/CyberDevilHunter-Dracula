/*****************************************************************//**
 * \file   charBase.h
 * \brief  �L�����N�^�[�̊��N���X
 *
 * \author �R�{介v
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"
#include"motionDefineList.h"

 /**
	* @brief �X�e�[�^�X�����܂Ƃ߂��\����
	*/
struct statusInf
{
	float hitPoint//!�̗͒l
		, stanPoint//!�X�^���l
		, maxHitPoint//!�ő�̗͒l
		, bloodPoint//!�u���b�h�|�C���g�l
		, maxBloodPoint;//!�ő�u���b�h�|�C���g�l
};

/**
 * @brief �����蔻������܂Ƃ߂��\����
 */
struct collCapsule
{
	VECTOR underPos, overPos;//!�����蔻��p�J�v�Z���̍��W���
	float r = -1;//!�����蔻��p�J�v�Z���̔��a���
	int parentModelHandle//!�Ǐ]�����郂�f���̃n���h��
		, frameNum;//!�Ǐ]�����郂�f���̃t���[���ԍ�
};

/**
 * @brief ���̋O�Օ`�掞�Ɏg�������܂Ƃ߂��\����
 */
struct weaponEffect
{
	std::vector<VECTOR> upCornerPos, downCornerPos;//!���̍��������A���敔���̍��W���
	float lifeTime = 10//!�`�掞��
		, maxLifeTime = 10;//!�ő�`�掞��
	int efcPicHandle;//�摜�n���h��
};

/**
 * @brief �U����������܂Ƃ߂��\����
 */
struct attackColl
{
	collCapsule capColl, capCollOld;//!���݂�1F�O�̓����蔻����
	std::string attackChar;//!�U�������L�����N�^�̖��O
	MATRIX Mold;//!1F�O�̒Ǐ]�����郂�f���̃t���[���̕ϊ��s��
	VECTOR Vec;
	float damage;//!�_���[�W
	int nonActiveTimeF//!�`��ҋ@����
		, activeTimeF;//!�`�掞��
	bool isUseMat//!���f���̃t���[���ɒǏ]�����邩
		, isAlive = true;//!�����蔻�肪�L����
	weaponEffect rightingEfc;//!���̋O�Տ��
	float stan;//!�X�^���l
};

/**
 * @class CB
 * @brief �L�����N�^�̊��N���X
 */
class CB
{
public:
	CB() {};
	virtual ~CB() {};

	/**
	 * @brief ����������
	 * @return true
	 */
	virtual bool	Initialize() { return true; }

	/**
	 * @brief �폜����
	 * @return modelDelete������������
	 */
	virtual bool	Terminate() { return model::modelDelete(&_modelInf); };

	/**
	 * @brief �X�V����
	 * @return true
	 */
	virtual bool	Process() { return true; }

	/**
	 * @brief �`�揈��
	 * @param �Đ����x
	 * @return true
	 */
	virtual bool	Render(float timeSpeed) { return true; }

	/**
	 * @brief �d�͏���
	 * @return true
	 */
	virtual bool gravity();

	/**
	 * @brief �L�����N�^�^�C�v�̎擾
	 * @return �L�����N�^�̃^�C�v
	 */
	virtual int	getType() { return type; }

	/**
	 * @brief charBox�R���e�i�̃|�C���^�擾
	 */
	void setCB(std::map<std::string, std::shared_ptr<CB> >* _CB) { charBox = _CB; }

	/**
	 * @brief ���\�[�X�T�[�o�[�̃|�C���^�擾
	 */
	void setRS(Rserver* _RS) { RS = _RS; }

	/**
	 * @brief ���@����ւ��Z�ύX����
	 * @param name ����ւ��Z��
	 * @param XorY 2���邤���ǂ���̓���ւ��Z��ς��邩
	 * @return true
	 */
	virtual bool CA_change(std::string name, const char* XorY) { return false; };

	/**
	 * @brief �̗͌v�Z����
	 * @param ���l
	 * @return true
	 */
	virtual bool HPmath(float Num, float Stan) { return false; };

	/**
	 * @brief �u���b�h�|�C���g�v�Z����
	 * @param ���l
	 * @return true
	 */
	virtual bool BPmath(float Num) { return false; };

	/**
	 * @brief ���C���J�����ʒu���Z�b�g����
	 * @param ���l
	 */
	virtual void setCamDir(float Num) { camDir = Num; };

	/**
	 * @brief ���C���Q�[���I�����{�C�X����
	 */
	virtual void battleEndVoice() { return; }

	/**
	 * @brief ���C���Q�[���J�n���{�C�X����
	 */
	virtual void battleStartVoice() { return; }

	/**
	 * @brief ���O���Z�b�g����
	 * @param ���O
	 * @return true
	 */
	void setName(const char* _name) { name = _name; }

	/**
	 * @brief �����蔻�菈��
	 * @param name �U���L�������
	 * @param hitPos �U���q�b�g���W�擾�p�|�C���^
	 * @param damage �_���[�W�擾�p�|�C���^
	 * @return true
	 */
	bool hitCheck(const char* name, VECTOR* hitPos, VECTOR* hitDir, float* damage);

	/**
	 * @brief �L�������f�����擾
	 * @return �L�������f�����
	 */
	modelInf* getInf() { return &_modelInf; }

	/**
	 * @brief �X�e�[�W���f�����擾
	 * @return �X�e�[�W���f�����
	 */
	void setGroundInf(modelInf* GE) { _GrounfInf = GE; }

	/**
	 * @brief �X�e�[�^�X���擾
	 * @return �X�e�[�^�X���
	 */
	statusInf	getStatus() { return _statusInf; }

	/**
	 * @brief ������͏��̃Z�b�g
	 * @param iInf ������͏��Z�b�g�p�|�C���^
	 * @param cameraDir �J�����ʒu���Z�b�g�p�|�C���^
	 */
	void getInputKey(imputInf* iInf, float* cameraDir) { _imputInf = iInf, _cameraDir = cameraDir; };

	/**
	 * @brief �����蔻����쐬����
	 * @param _underPos �J�v�Z���������W
	 * @param _overPos �J�v�Z���㕔���W
	 * @param r �J�v�Z�����a
	 * @param nonActiveTimeF �ҋ@����
	 * @param activeTimeF ��������
	 * @param isUseMat ���f���t���[���ɒǏ]�����邩
	 * @param damage �U����
	 * @param frameNum �Ǐ]�����郂�f���t���[���̔ԍ�
	 * @param charName �L�����N�^��
	 * @param cameraDir �J�����ʒu���Z�b�g�p�|�C���^
	 */
	bool makeAttackCap(VECTOR _underPos, VECTOR _overPos, float r, int nonActiveTimeF, int activeTimeF, int timeSpeed, bool isUseMat, float damage, float stan, int frameNum, VECTOR _dir);

	/**
	 * @brief �}�X�^�[���ʃZ�b�g����
	 * @param masterValume �}�X�^�[����
	 */
	void setMasterVolume(int masterValume) { for (int handle : soundHandle) { ChangeVolumeSoundMem(255 * (0.01 * _valData->soundMasterValume), handle); } }

	int type = 0//!�L�����N�^�^�C�v(player = 1, boss = 2)
		, caRecastX = 0, caRecastY = 0, setRecastTime = 0;
	bool isGround//!�ݒu���Ă��邩
		, isHit//!�U�����q�b�g������
		, isSetSoundValume;//!�}�X�^�[���ʓK�p�����邩
	float g//!�d�͒l
		, camDir//!���b�N�I���J�����ʒu���
		, * _cameraDir//!�J�����ʒu���
		, animSpd;//!�A�j���[�V����
	int isDead;//!�L�������u�]��Ԃ�(0.�����Ă� 1.���S���[�V������ 2.�C���X�^���X���)
	int isImmortal = false//!���G��Ԃ�
		, dodgeTime;//!�������
	std::string name//!���L�����̖��O
		, attackChar;//!�U�����Ă����L�����̖��O
	modelInf _modelInf//!���f�����C���X�^���X
		, * _GrounfInf;//!�X�e�[�W���C���X�^���X
	statusInf _statusInf;//�X�e�[�^�X���C���X�^���X
	std::map<std::string, std::shared_ptr<CB> >* charBox;//!�S�L�����R���e�i�p�|�C���^
	valData* _valData;//!���L�f�[�^�p�|�C���^
	MV1_COLL_RESULT_POLY hitCheckGround;//�X�e�[�W�̓����蔻����
	collCapsule collCap;//!�����蔻��p�J�v�Z���̃C���X�^���X
	std::vector<int> soundHandle;//!�����f�[�^�p�R���e�i
	std::vector<attackColl>* allColl;//!�����蔻��J�v�Z���p�R���e�i�p�|�C���^
	imputInf* _imputInf;//!���̓f�[�^�p�|�C���^
	Rserver* RS;//!���\�[�X�T�[�o�[�N���X�p�|�C���^
};