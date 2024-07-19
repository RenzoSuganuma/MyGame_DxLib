#pragma once

constexpr bool comp_debug = true;

class Actor;

class Component
{
protected:
	const Actor* attachedActor_ = nullptr;

public:
	bool enabled_ = true;

	Component();
	~Component();

	virtual void Begin();
	virtual void Tick();
	virtual void End();

	const Actor* const GetActor() const ;
	void const SetActor(const Actor* actor);
};

