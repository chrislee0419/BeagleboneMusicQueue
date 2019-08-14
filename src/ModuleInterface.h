#pragma once

class ModuleInterface
{
public:
	virtual ModuleInterface &GetInstance() = 0;
}
