#include"ComponentBase.h"

ComponentBase::ComponentBase() :_parent{ nullptr }
{
}

ComponentBase::~ComponentBase()
{
}

bool ComponentBase::Initialize()
{
	return false;
}

void ComponentBase::Process()
{
}

void ComponentBase::Render(float timeSpeed)
{
}
