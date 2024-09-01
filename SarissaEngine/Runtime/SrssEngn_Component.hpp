#pragma once

class Actor;

// コンポーネントクラス
class Component
{
protected:
	const Actor* attachedActor_ = nullptr;

	virtual void Begin_();
	virtual void Tick_(float deltaTime);
	virtual void End_();
	virtual void Draw_();
public:
	bool enabled_ = true;

	Component();
	~Component();

	void Begin();
	void Tick(float deltaTime);
	void End();
	void Draw();

	const Actor* const GetActor() const;

	void const SetActor(const Actor* actor);
};
