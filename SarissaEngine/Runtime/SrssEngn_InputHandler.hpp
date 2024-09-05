#pragma once

#include "SrssEngn_Component.hpp"
#include "SrssEngn_Vector3.h"
#include "DxLib.h"
#include "string"
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
	Tab, Shift, Ctrl, Alt, Space,
};

// ���̓n���h���[�A���̓f�o�C�X����̓��͏��������s����B
class InputHandler final : public Component
{
private:
#pragma region -�L�[�{�[�h���͂܂��-
	// ���͗����オ��
	bool started_[41]{ false };
	// ���͂��P�̂Ƃ�
	bool performed_[41]{ false };
	// ���͗�����
	bool canceled_[41]{ false };
	// ���͂������Ă���t���[����
	int performedFrames_[41]{ 0 };

	// �L�[�{�[�h���͎�t����
	void const CheckKeyboardInput();
	// ���͂� �����オ��A������̓��͂��X�V����
	void const CheckInputStarted();
	// ���̗͂�����𒲂ׂď�Ԃ̍X�V������	
	void const CheckInputCanceled();
#pragma endregion

public:
	Vector3 moveVec_{ 0 };
	const bool const GetInput(int inputAction, InputDeviceType inputType);
	const bool const GetInputStarted(const KeyboardKey key);
	const bool const GetInputPerformed(const KeyboardKey key);
	const bool const GetInputCanceled(const KeyboardKey key);
	void Begin_() override;
	void Tick_(float deltaTime)  override;
	void End_()override;
};

