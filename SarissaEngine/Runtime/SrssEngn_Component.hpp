#pragma once

class Actor;

class Component
{
protected:
	const Actor* attachedActor_ = nullptr;

public:
	bool enabled_ = true;

	Component();
	~Component();

	void Begin();
	void Tick(float deltaTime);
	void End();

	virtual void Begin_();
	virtual void Tick_(float deltaTime);
	virtual void End_();

	const Actor*
		const
		GetActor() const;
	void
		const
		SetActor(const Actor* actor);
};
