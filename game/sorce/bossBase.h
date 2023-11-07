#pragma once
#include"charBase.h"
#include"ComponentBase.h"
#include "picojson/picojson.h"
#include <map>
#include <memory>
#include <typeindex>


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

	bool HPmath(float Num, float Stan) override;

	//���^�R���|�[�l���g��ǉ�����ۂ͈قȂ�ID��ݒ肷��
	virtual void AddComponent(std::unique_ptr<ComponentBase> component, int id = 0);

	/**
	 * \brief �w��̃R���|�[�l���g���擾����@��FGetComponent<MV1Component>()
	 * \return �R���|�[�l���g�̃|�C���^��Ԃ��B�������Ă��Ȃ����nullptr��Ԃ��B
	 */
	template <typename C>
	C* GetComponent(int id = 0) {
		C* result = nullptr;

		if (_components.count({ typeid(C), id }) != 0) {
			result = static_cast<C*>(_components[{typeid(C), id}].get());
		}
		return result;
	}

	/**
	 * \brief �w��̃N���X���m�F����@��FCheckType<Player>()�ACheckType<Enemy>()
	 * \return �w��̃N���X�ł����true��Ԃ�
	 */
	template <typename T>
	bool CheckType()const {
		if (typeid(T&) == typeid(*this)) {
			return true;
		}

		return false;
	}


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
	std::map<std::pair<std::type_index, int>, std::unique_ptr<ComponentBase>> _components;
};