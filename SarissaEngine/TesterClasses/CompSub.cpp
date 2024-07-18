#include "CompSub.h"

using Framework::Component;

void Component::Begin()
{
	DrawString(0, 60, "Begin", GetColor(255, 255, 255));
}

void Component::Tick() 
{
	DrawString(0, 60, "Tick", GetColor(255, 255, 255));
}

void Component::End()
{
	DrawString(0, 60, "End", GetColor(255, 255, 255));
}
