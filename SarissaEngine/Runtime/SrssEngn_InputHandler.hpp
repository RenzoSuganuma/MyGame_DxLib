#pragma once

#include "SrssEngn_RuntimeClasses.hpp"
#include "DxLib.h"
#include "list"
#include "map"

// ���̓f�o�C�X�̃^�C�v
enum class InputDeviceType
{
	Mouse,
	KeyBoard,
	GamePad,
	XInput,
};

// �L�[�{�[�h�̃L�[�̗񋓌^
enum class KeyboardKey : int
{
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	Num_1, Num_2, Num_3, Num_4, Num_5, Num_6, Num_7, Num_8, Num_9, Num_0,
	Tab, Shift, Ctrl, Alt,
};

class InputHandler final : public SarissaEngine::Runtime::Framework::Component
{
private:
#pragma region -�L�[�{�[�h���͂܂��-
	// ���͗����オ��
	std::map< KeyboardKey, bool > started_;
	// ���͂��P�̂Ƃ�
	std::map< KeyboardKey, bool > performed_;
	// ���͗�����
	std::map< KeyboardKey, bool > canceled_;
	// ���͂������Ă���t���[����
	std::map< KeyboardKey, int > performedFrames_;
#pragma endregion

public:
	VECTOR moveVec_{ 0 };
	const bool const GetInput(int inputAction, InputDeviceType inputType);
	void Begin_() override;
	void Tick_(float deltaTime)  override;
	void End_()override;
};

