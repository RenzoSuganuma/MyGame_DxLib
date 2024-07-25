#pragma once

class Actor;

// メインループ : Unityでいうシーン
class Level final
{
private:
	std::list< SarissaEngine::Runtime::Framework::Actor* > objects_;

public:
	Level();
	~Level();

	void
		const
		MainLoopEntry();

	void
		const
		MainLoopUpdate(float deltaTime);

	void
		const
		MainLoopExit();

	const std::list< SarissaEngine::Runtime::Framework::Actor* >::iterator
		const
		AddObject(SarissaEngine::Runtime::Framework::Actor* newObject);

	void
		const
		RemoveObject(SarissaEngine::Runtime::Framework::Actor* obj);

	void
		const
		RemoveObject(const std::list< SarissaEngine::Runtime::Framework::Actor* >::iterator place);
};
