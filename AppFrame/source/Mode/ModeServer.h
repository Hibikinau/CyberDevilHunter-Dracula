#pragma once

#include <map>
#include <string>
#include<vector>
#include <memory>
#include "ModeBase.h"

class	ModeServer
{
	typedef	std::map<std::string, std::shared_ptr<ModeBase> >		lstModeBase;

public:
	ModeServer();
	virtual	~ModeServer();
	static ModeServer		*_lpInstance;
	static ModeServer*GetInstance() { return (ModeServer*)_lpInstance; }

	int Add(std::shared_ptr<ModeBase> mode, int layer, const char *name);		// �o�^�͂��邪�A��x���C�����񂳂Ȃ��Ƃ����Ȃ�
	int Del(const char* name);		// �폜�\��
	int activate(const char* name);
	int disable(const char* name);
	void Clear();
	int	LayerTop() { return INT32_MAX; }

	static bool modeSort(const std::shared_ptr<ModeBase> x, const std::shared_ptr<ModeBase> y) {
		return x->_layer < y->_layer;
	}

	int	Imput(imputInf* iInf);

	int ProcessInit();	// �v���Z�X���񂷂��߂̏�����
	int Process();		// ���C���[�̏�̕����珈��
	int ProcessFinish();		// �v���Z�X���񂵂���̌�n��

	int RenderInit();		// �`����񂷂��߂̏�����
	int Render();		// ���C���[�̉��̕����珈��
	int RenderFinish();		// �`����񂵂���̌�n��

private:
	int Release(const char* name, bool isDel);		// �폜��delete
	bool IsDelRegist(const char* name);	// �폜�\�񂳂�Ă��邩�H
	//bool IsAdd(const char* name);		// ���X�g�ɂ��邩�H


public:
	imputInf* _imputInf;
	lstModeBase		_vMode;			// ���[�h���X�g

private:
	int				_uid_count;		// uid�J�E���^
	lstModeBase		_vModeAdd;		// �ǉ��\��
	std::vector<std::string> _vModeDel;		// �폜�\��

	std::vector<std::string> _nowMode;		// ���݌Ăяo�����̃��[�h

};
