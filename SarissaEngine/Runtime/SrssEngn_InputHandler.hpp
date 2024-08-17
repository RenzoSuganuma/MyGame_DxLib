#pragma once

#include "SrssEngn_RuntimeClasses.hpp"
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

class InputHandler final : public SarissaEngine::Runtime::Framework::Component
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

#pragma region -�Ȃ��[�[�[�[�[�������̊֐�-
	// �L�[�{�[�h���͎�t����
	void const CheckKeyboardInput()
	{
		performed_[KeyboardKey::A] = CheckHitKey(KEY_INPUT_A);
		performed_[KeyboardKey::B] = CheckHitKey(KEY_INPUT_B);
		performed_[KeyboardKey::C] = CheckHitKey(KEY_INPUT_C);
		performed_[KeyboardKey::D] = CheckHitKey(KEY_INPUT_D);
		performed_[KeyboardKey::E] = CheckHitKey(KEY_INPUT_E);
		performed_[KeyboardKey::F] = CheckHitKey(KEY_INPUT_F);
		performed_[KeyboardKey::G] = CheckHitKey(KEY_INPUT_G);
		performed_[KeyboardKey::H] = CheckHitKey(KEY_INPUT_H);
		performed_[KeyboardKey::I] = CheckHitKey(KEY_INPUT_I);
		performed_[KeyboardKey::J] = CheckHitKey(KEY_INPUT_J);
		performed_[KeyboardKey::K] = CheckHitKey(KEY_INPUT_K);
		performed_[KeyboardKey::L] = CheckHitKey(KEY_INPUT_L);
		performed_[KeyboardKey::M] = CheckHitKey(KEY_INPUT_M);
		performed_[KeyboardKey::N] = CheckHitKey(KEY_INPUT_N);
		performed_[KeyboardKey::O] = CheckHitKey(KEY_INPUT_O);
		performed_[KeyboardKey::P] = CheckHitKey(KEY_INPUT_P);
		performed_[KeyboardKey::Q] = CheckHitKey(KEY_INPUT_Q);
		performed_[KeyboardKey::R] = CheckHitKey(KEY_INPUT_R);
		performed_[KeyboardKey::S] = CheckHitKey(KEY_INPUT_S);
		performed_[KeyboardKey::T] = CheckHitKey(KEY_INPUT_T);
		performed_[KeyboardKey::U] = CheckHitKey(KEY_INPUT_U);
		performed_[KeyboardKey::V] = CheckHitKey(KEY_INPUT_V);
		performed_[KeyboardKey::W] = CheckHitKey(KEY_INPUT_W);
		performed_[KeyboardKey::X] = CheckHitKey(KEY_INPUT_X);
		performed_[KeyboardKey::Y] = CheckHitKey(KEY_INPUT_Y);
		performed_[KeyboardKey::Z] = CheckHitKey(KEY_INPUT_Z);

		performed_[KeyboardKey::Num_1] = CheckHitKey(KEY_INPUT_1);
		performed_[KeyboardKey::Num_2] = CheckHitKey(KEY_INPUT_2);
		performed_[KeyboardKey::Num_3] = CheckHitKey(KEY_INPUT_3);
		performed_[KeyboardKey::Num_4] = CheckHitKey(KEY_INPUT_4);
		performed_[KeyboardKey::Num_5] = CheckHitKey(KEY_INPUT_5);
		performed_[KeyboardKey::Num_6] = CheckHitKey(KEY_INPUT_6);
		performed_[KeyboardKey::Num_7] = CheckHitKey(KEY_INPUT_7);
		performed_[KeyboardKey::Num_8] = CheckHitKey(KEY_INPUT_8);
		performed_[KeyboardKey::Num_9] = CheckHitKey(KEY_INPUT_9);
		performed_[KeyboardKey::Num_0] = CheckHitKey(KEY_INPUT_0);

		performed_[KeyboardKey::Shift] = CheckHitKey(KEY_INPUT_LSHIFT) || CheckHitKey(KEY_INPUT_RSHIFT);
		performed_[KeyboardKey::Tab] = CheckHitKey(KEY_INPUT_TAB);
		performed_[KeyboardKey::Ctrl] = CheckHitKey(KEY_INPUT_LCONTROL) || CheckHitKey(KEY_INPUT_RCONTROL);
		performed_[KeyboardKey::Alt] = CheckHitKey(KEY_INPUT_LALT) || CheckHitKey(KEY_INPUT_RALT);
		performed_[KeyboardKey::Space] = CheckHitKey(KEY_INPUT_SPACE);
	}
	// ���͂� �����オ��A������̓��͂��X�V����
	void const CheckInputStarted() {
		for (auto item : performed_) {
			if (item.second) {
				performedFrames_[item.first]++;

				if (performedFrames_[item.first] < 2) {
					started_[item.first] = true;
				}	// ���̓t���[�����𒲂ׂ�
				else {
					started_[item.first] = false;
				}
			}
		}	// ���͒��̏������ׂĂȂ߂�
	}
	// ���̗͂�����𒲂ׂď�Ԃ̍X�V������	
	void const CheckInputCanceled() {
		auto past_it = performedConditionPastFrame_.begin(); // �O�t���[��
		auto current_it = performed_.begin();				 // ���݂̃t���[��
		while (current_it != performed_.end()) {
			if ((*current_it).second == false && (*past_it).second == true) {
				canceled_[(*current_it).first] = true;

				performedFrames_[(*current_it).first] = 0;
			}
			else {
				canceled_[(*current_it).first] = false;
			}

			past_it++;
			current_it++;
		}
	}
	// �O�t���[���̓��͒l���X�V
	void const UpdatePastInputValues() {
		performedConditionPastFrame_ = performed_;	// �l���R�s�[
	}
#pragma endregion
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

