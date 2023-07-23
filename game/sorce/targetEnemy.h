#pragma once
#include"charBase.h"
#include"player.h"

class targetEnemy : public CharBase
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	targetEnemy() {};
	/**
	 * @brief �f�X�g���N�^
	 */
	~targetEnemy() {};
	/**
	 * @brief ����������
	 * @return true
	 */
	bool Initialize() override;
	/**
	 * @brief �폜����
	 * @return true
	 */
	bool Terminate() override;
	/**
	 * @brief �X�V����
	 * @return true
	 */
	bool Process() override;
	/**
	 * @brief �`�揈��
	 * @param �Đ����x
	 * @return true
	 */
	bool Render(float timeSpeed) override;
	/**
	 * @brief �̗͌v�Z����
	 * @param ���l
	 * @return true
	 */
	bool HPmath(float Num, float Stan) override;
	/**
	 * @brief �L�����N�^�[�̈ړ�
	 * @param speed �ړ����x
	 * @param radian �ړ�����
	 * @return true
	 */
	void Move(float speed, float radian);

	/**
	 * @brief �G�t�F�N�g�ǂݍ��ݗp�֐�
	 */
	void loadEfekseer() override;

protected:
	VECTOR siz;
};