#include"bossBase.h"


float BossBase::RangeJ(){
	for (auto i = charBox->begin(); i != charBox->end(); i++)
	{
		if (i->second->getType() == 1)
		{
			plMI = i->second->getInf();
		}
	}
	Pvector = VSub(plMI->pos, this->_modelInf.pos);
	playerDir = (std::atan2(-Pvector.x, -Pvector.z) * 180.f) / DX_PI_F;
	playerRange = sqrt(Pvector.x * Pvector.x + Pvector.y * Pvector.y + Pvector.z * Pvector.z);
	if (playerRange < 300)
	{
		this->range = RANGE::CrossRange;
	}
	if (300 <= playerRange && playerRange <= 2000)
	{
		this->range = RANGE::MidRange;
	}
	if (playerRange > 2000)
	{
		this->range = RANGE::LongRange;
	}
	return playerDir;
};

/**
 * @brief キャラクターの移動
 * @param speed 移動速度
 * @param radian 移動方向
 * @return true
 */
void BossBase::Move(float speed, float radian) 
{
	float Speed = speed;
	float Radian = (this->_modelInf.dir.y + radian) * DX_PI_F / 180.0f;
	this->_modelInf.pos.x -= sin(Radian) * Speed;
	this->_modelInf.pos.z -= cos(Radian) * Speed;
};


bool BossBase::HPmath(float Num, float Stan)
{
	this->_statusInf.hitPoint += Num;
	if (this->_statusInf.redHitPoint <= 0) {this-> _statusInf.redHitPointDelayTime = 100; }
	this->_statusInf.redHitPoint += std::abs(Num);
	this->_statusInf.stanPoint += Stan;
	
	int a = PlayEffekseer3DEffect(_valData->efcHandle);
	SetPosPlayingEffekseer3DEffect(a, this->_modelInf.pos.x, this->_modelInf.pos.y, this->_modelInf.pos.z);

	return true;
}

void BossBase::AddComponent(std::unique_ptr<ComponentBase> component, int id)
{
	component->SetParent(this);

	if (component->Initialize()) {
		_components[{typeid(*component), id}] = (std::move(component));
	}
}