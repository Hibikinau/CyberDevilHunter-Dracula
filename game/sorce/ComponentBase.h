#pragma once
#include"bossBase.h"

class ComponentBase
{
public:
    ComponentBase();
    ~ComponentBase();
    virtual bool  Initialize();
    virtual void Process();
    virtual void Render(float timeSpeed);
  
    virtual void SetParent(BossBase* parent) { _parent = parent; }
protected:
    BossBase* _parent;
};
