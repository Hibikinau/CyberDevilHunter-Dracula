#pragma once
#include"ComponentBase.h"

class ComponentAI :public ComponentBase
{
public:
	ComponentAI();
	~ComponentAI();
   bool  Initialize();
   void Process();
   void Render(float timeSpeed);
};
