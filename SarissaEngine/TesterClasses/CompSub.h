#pragma once

#include "..\Runtime\Component.h"

class CompSub : public Framework::Component
{
public:
	void Begin();
	void Tick();
	void End();
};

