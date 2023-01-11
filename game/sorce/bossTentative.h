#pragma once
#include"charBase.h"
#include"player.h"



class Boss : public CB
{
public:
	Boss() {};
	~Boss() {};
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Input();
	virtual bool Process();
	virtual bool Render();
	virtual void charMove(float Speed, float _Dir);
	bool getInputKey(int* padInput, int* padTrg, char* keyInput, char* keyTrg, float* cameraDir)
	{
		_gKeyp = padInput, _gTrgp = padTrg, _gKeyb = keyInput, _gTrgb = keyTrg, _cameraDir = cameraDir; return true;
	};
	bool step();
	bool checkTrgImput(int Key, int Pad) { if (_gTrgb[Key] || *_gTrgp & Pad) { return true; } else { return false; } }
	bool checkKeyImput(int Key, int Pad) { if (_gKeyb[Key] || *_gKeyp & Pad) { return true; } else { return false; } }
	void Walk(VECTOR x);

	enum class STATUS {
		NONE,
		WAIT,
		WALK,
		KICK,
		_EOT_
	};
	STATUS status;

	// 3D���f���`��p
	int _handle;
	int Anim_handle;
	int Anim_handle2;
	int Anim_handle3;
	int _attach_index;
	float _total_time;
	float _play_time;
	VECTOR _vPos;		// �ʒu
	VECTOR _vOldPos;	// �O�t���[���̈ʒu
	VECTOR _vDir;	// ����
	float _colSubY;	// �R���W�������莞��Y�␳(���ʒu�j

protected:
	int _cg, useAnim;
	int _x, _y, stepChargeSec, isStep;
	float spd, animSpd, * _cameraDir;
	int* _gKeyp, * _gTrgp;
	char* _gKeyb, * _gTrgb;
	bool isUseFbx;
	modelManager _modelManager;
	bool attackFlag, isDash, isAnimEnd;
	//modelInf _modelInf;
	//std::map<std::string, std::shared_ptr<CB> > charBox;
	modelInf *plMI, stage;
	//std::vector<std::unique_ptr<PL>> player;
	//std::vector<std::unique_ptr<modelInf>> _modelInf;
    //PL player;
	double dir;
};