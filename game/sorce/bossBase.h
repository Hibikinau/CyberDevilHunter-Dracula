#pragma once
#include"charBase.h"
#include "picojson/picojson.h"

namespace {
	constexpr auto BossUtility = "res/ResourcePathData.json";
}

class BossBase : public CharBase 
{
public:
	enum class TYPE
	{
		KNIGHT,
		LION,
		LAST
	};
	TYPE type;

	/**
	 * @brief �v���C���[�Ƃ̋������
	 */
	enum class RANGE
	{
		NONE,
		CrossRange,
		MidRange,
		LongRange
	};
	RANGE range;

	/**
	 * @brief �L�����N�^�[�̌��݂̏��
	 */
	enum class K_STATUS 
	{
		NONE,
		WAIT,
		DAMAGE,
		STAN,
		DEAD,
		RUN,
		FSTEP,
		BSTEP,
		RSTEP,
		LSTEP,
		SRASH,
		SLAM,
		STAB,
		ROBES,
		JUMPACT,
		ONESLASH,
	};
	
	BossBase() :CharBase() {};
	~BossBase() {};

	bool loadJson(TYPE _type);
	/**
	 * @brief �v���C���[�Ƃ̋�������
	 * @return true
	 */
	float RangeJ();
	/**
 * @brief �L�����N�^�[�̈ړ�
 * @param speed �ړ����x
 * @param radian �ړ�����
 * @return true
 */
	void Move(float speed, float radian);

	//STATUS UtilityJudge() {}
protected:
	float animSpd,     //!�A�j���[�V�����̃X�s�[�h
		stanTime;   //!�X�^������
	int  time,        //!�ҋ@����
		attackStep;   //!�s���ԍ�
	int  swingSE,     //!�a��SE�n���h��
		newSomenHandle; //�G�t�F�N�g�n���h��
	float playerDir,       //!�v���C���[�̕���
		playerRange,       //!�v���C���[�Ƃ̋���
		playerDistance;      //!�v���C���[�Ƃ̋������̂Q
	VECTOR Pvector;               //!�v���C���[�Ƃ̋����̐�
	bool awake;       //!�o���������ǂ���
	float awakeSpd,   //!�o�����X�s�[�h
		awakeDmg,     //!�o�����ǉ��_���[�W
		awakeMove,    //!�o�����ړ���
		awakeTime,       //!�o�����ҋ@����
		awakeAddDistance;      //!�o�����U�����[�V�����ړ��ʒǉ�
	modelInf* plMI; //!�v���C���[���̃C���X�^���X
	bool  isAnimEnd;       //�A�j���[�V�����I��������ǂ���
};