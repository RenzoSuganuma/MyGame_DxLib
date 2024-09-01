#pragma once

#include "SrssEngn_Component.hpp"
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

// ���̓n���h���[
class InputHandler final : public Component
{
private:
#pragma region -�L�[�{�[�h���͂܂��-
	// ���͗����オ��
	std::map< const KeyboardKey, bool > started_;
	// ���͂��P�̂Ƃ�
	std::map< const KeyboardKey, bool > performed_;
	// ���͗�����
	std::map< const KeyboardKey, bool > canceled_;
	// �O�t���[���̓��͂̏�Ԃ�ێ�����
	std::map< const KeyboardKey, bool > performedConditionPastFrame_;
	// ���͂������Ă���t���[����
	std::map< const KeyboardKey, int > performedFrames_;

	// �L�[�{�[�h���͎�t����
	void const CheckKeyboardInput();
	// ���͂� �����オ��A������̓��͂��X�V����
	void const CheckInputStarted();
	// ���̗͂�����𒲂ׂď�Ԃ̍X�V������	
	void const CheckInputCanceled();
	// �O�t���[���̓��͒l���X�V
	void const UpdatePastInputValues();
#pragma endregion

public:
	VECTOR moveVec_{ 0 };
	const bool const GetInput(int inputAction, InputDeviceType inputType);
	const bool const GetInputStarted(const KeyboardKey key);
	const bool const GetInputPerformed(const KeyboardKey key);
	const bool const GetInputCanceled(const KeyboardKey key);
	void Begin_() override;
	void Tick_(float deltaTime)  override;
	void End_()override;
};

