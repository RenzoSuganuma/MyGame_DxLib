#pragma once
#include <stack>
#include <functional>

namespace tnl {

	//--------------------------------------------------------------------------------------------------------------------------
	// Sequence
	// tips... 1.�f���Q�[�g�ɂ�郁�\�b�h�̌Ăѕ����@�\��񋟂��܂�
	// �@�@�@�@2.�^���R���[�`���}�N���ɂ��ȈՃR���[�`���L�q��񋟂��܂�
	//         3.invoke �ɂ�鎞�Ԍo�߂Ɋ�Â��ď��������s���邽�߂̋@�\��񋟂��܂�
	// 
	//			���ꂼ��̋@�\�͑g�ݍ��킹�Ďg�p���邱�Ƃ��ł��܂�
	// 
	// 
	// ==================================================================================================================
	// �� �g�p�@�T���v�� 1.�f���Q�[�g�ɂ�郁�\�b�h�̌Ăѕ����@�\
	// ==================================================================================================================
	// 
	//  --------------- Example.h ----------------
	// 
	// class Example
	// {
	//	public:
	//		// �V�[�P���X�@�\�̐錾
	//		TNL_CO_SEQUENCE(Example, &Example::seqCallFunctionA); // ������ seqCallFunctionA �����s����܂�
	// 
	//		// ���t���[�����s�����X�V����
	//		void update(float delta_time) ;
	// 
	//		// �V�[�P���X�@�\�ŌĂѕ����郁�\�b�h
	//		bool seqCallFunctionA(float delta_time);
	//		bool seqCallFunctionB(float delta_time);
	//		bool seqCallFunctionC(float delta_time);
	//
	//		����
	// } ;
	// 
	//  --------------- Example.cpp ----------------
	// 
	// void Example::update(float delta_time)
	// {
	//		tnl_sequence_.update(delta_time) ;
	// }
	// 
	// bool Example::seqCallFunctionA(float delta_time)
	// {
	//		// ����if���̓��\�b�h���R�[�����ꂽ�ŏ��̂P�t���[���̂ݐ^�ɂȂ�
	//		if( tnl_sequence_.isStart() ){
	//			// seqCallFunctionA �̔C�ӂ̏���������
	//		}
	// 
	//		// �T�b�o�߂Ŏ��̃t���[������ seqCallFunctionB �Ɉڍs
	//		if( tnl_sequence_.getProgressTime() > 5.0f ){
	//			tnl_sequence_.change( &Example::seqCallFunctionB ) ;
	//		}
	// 
	//		return true ;
	// }
	// 
	// bool Example::seqCallFunctionB(float delta_time)
	// {
	//		// ����if���̓��\�b�h���R�[�����ꂽ�ŏ��̂P�t���[���̂ݐ^�ɂȂ�
	//		if( tnl_sequence_.isStart() ){
	//			// seqCallFunctionA �̔C�ӂ̏���������
	//		}
	// 
	//		// �R�b�o�߂Ŏ��̃t���[�����璼�O�ɌĂяo����Ă������\�b�h�ɖ߂�
	//		if( tnl_sequence_.getProgressTime() > 3.0f ){
	//			tnl_sequence_.undo() ;
	//		}
	// 
	//		return true ;
	// }
	// 
	// 
	// ==================================================================================================================
	// �� �g�p�@�T���v�� 2.�^���R���[�`���}�N���ɂ��ȈՃR���[�`���L�q
	// ==================================================================================================================
	// 
	//  --------------- Example.h ----------------
	// 
	//				�@�@���T���v���P
	// 
	// 
	//  --------------- Example.cpp ----------------
	// 
	// bool Example::seqCallFunctionA(float delta_time)
	// {
	// 
	//		// ---------- �t���[���w��ł̃R���[�`�� ----------
	// 
	//		// 10 �t���[���̊ԃ����_���w��̏��������s�����
	//		TNL_SEQ_CO_FRM_YIELD_RETURN(10, delta_time, [&] {
	// 
	//			// �ŏ��̂P�t���[���̂ݐ^�ɂȂ�
	//			if ( TNL_SEQ_CO_YIELD_FRM_IS_START ) {
	//			}
	// 
	//		});
	// 
	//		// ---------- �b���w��ł̃R���[�`�� ----------
	// 
	//		// 1 �b�Ԃ̊ԃ����_���w��̏��������s�����
	//		TNL_SEQ_CO_TIM_YIELD_RETURN(1.0f, delta_time, [&] {
	// 
	//			// �ŏ��̂P�t���[���̂ݐ^�ɂȂ�
	//			if ( TNL_SEQ_CO_YIELD_FRM_IS_START ) {
	//			}
	// 
	//		});
	// 
	//		// ��L�Q�̃R���[�`���������I�������� seqCallFunctionB �Ɉڍs
	//		tnl_sequence_.change(&Example::seqCallFunctionB) ;
	// 
	// 
	//		// �R���[�`���}�N�����g�p���郁�\�b�h�̍Ō�ɕK���L�q
	//		// ���̃}�N���g�p���� return false �s�v
	//		TNL_SEQ_CO_END ;
	// }
	// 
	// 
	// 
	// bool Example::seqCallFunctionB(float delta_time){
	// 
	// 
	//		// ---------- �R���[�`�������̓r���j�� ----------
	// 
	//		// 60 �t���[���̊ԃ����_���w��̏��������s�����
	//		TNL_SEQ_CO_FRM_YIELD_RETURN(60, delta_time, [&] {
	// 
	//			// ���̃R���[�`���̌o�߃t���[������ 30 �ɂȂ�����^
	//			if( TNL_SEQ_CO_PROG_FRAME == 30 ){
	//				// ���̃R���[�`����j������
	//				TNL_SEQ_CO_BREAK ;
	//			}
	// 
	//		});
	// 
	//		// �R���[�`���������I�������� seqCallFunctionC �Ɉڍs
	//		tnl_sequence_.change( &Example::seqCallFunctionC ) ;
	// 
	//		// �R���[�`���}�N�����g�p���郁�\�b�h�̍Ō�ɕK���L�q
	//		TNL_SEQ_CO_END ;
	// 
	// }
	// 
	// 
	// bool Example::seqCallFunctionC( float delta_time ){
	// 
	//		// ---------- �R���[�`���������J��Ԃ�( �������[�v�Œ�~�͂��Ȃ� ) ----------
	// 
	//		while( 1 ){
	//			// 10 �t���[���̊ԃ����_���w��̏��������s�����
	//			TNL_SEQ_CO_FRM_YIELD_RETURN(10, delta_time, [&] {
	// 
	//			});
	// 
	//			// 10 �t���[���̊ԃ����_���w��̏��������s�����
	//			TNL_SEQ_CO_FRM_YIELD_RETURN(10, delta_time, [&] {
	// 
	//			});
	// 
	//		}
	// 
	//		// �R���[�`���}�N�����g�p���郁�\�b�h�̍Ō�ɕK���L�q
	//		TNL_SEQ_CO_END ;
	// 
	// }
	// 
	// 
	// ==================================================================================================================
	// �� �g�p�@�T���v�� 3.invoke �ɂ�鎞�Ԍo�߂Ɋ�Â��ď��������s���邽�߂̋@�\
	// ==================================================================================================================
	// 
	//  --------------- Example.h ----------------
	// 
	// class Example 
	// {
	//	public:
	// 
	//		Example() ;
	// 
	//		TNL_CO_SEQUENCE(Example, &Example::seqCallFunctionA); // ������ seqCallFunctionA �����s����܂�
	// 
	//		// �V�[�P���X�@�\�ŌĂѕ����郁�\�b�h
	//		bool seqCallFunctionA(float delta_time);
	// 
	//		// Invoke �ŌĂяo�����\�b�h
	//		void calledFromInvokeA(float delta_time);
	//		void calledFromInvokeB(float delta_time);
	//
	//		����
	// } ;
	// 
	//  --------------- Example.cpp ----------------
	// 
	// void Example::Example()
	// {
	// 
	//		// seqCallFunctionB �� 1�b��� 1�񂾂����s
	//		tnl_sequence_.invoke( &Example::calledFromInvokeA, 1.0f);
	// 
	//		// seqCallFunctionC �� 3�b��� 0.5�b�Ԋu�Ŏ��s
	//		tnl_sequence_.invokeRepeating( &Example::calledFromInvokeB, 3.0f, 0.5f);
	// 
	//		// �w�肵�� invoke ��S�ăL�����Z��
	//		tnl_sequence_.cancelInvoke( &Example::calledFromInvokeB ) ;
	// }
	// 
	// bool Example::seqCallFunctionA(float DeltaSeconds)
	// {
	//		// �C�ӂ̏���
	// }
	// bool Example::calledFromInvokeA(float DeltaSeconds)
	// {
	//		// �C�ӂ̏���
	// }
	// bool Example::calledFromInvokeB(float DeltaSeconds)
	// {
	//		// �C�ӂ̏���
	// }
	// 
	// 
	//--------------------------------------------------------------------------------------------------------------------------


	template <class T>
	class Sequence final {
	private:

		class Invoke {
		public:
			Invoke(T* obj, float time_to_trigger, void (T::* p_func)())
				: object_(obj)
				, time_to_trigger_(time_to_trigger)
				, function_(p_func)
				, p_function_(p_func)
			{}
			Invoke(T* obj, float time_to_trigger, float repeat_time, void (T::* p_func)())
				: object_(obj)
				, time_to_trigger_(time_to_trigger)
				, repeat_time_(repeat_time)
				, is_repeating_(true)
				, function_(p_func)
				, p_function_(p_func)
			{}
			bool update(const float delta_time) {
				sum_time_ += delta_time;
				if (time_to_trigger_ > sum_time_) return false;
				if (!is_repeating_) {
					function_(object_);
					return true;
				}
				sum_time_ = 0;
				time_to_trigger_ = repeat_time_;
				function_(object_);
				return false;
			}
			inline bool isComparable(void (T::*func)()) const { return p_function_ == func; }
		private:
			T* object_;
			float sum_time_ = 0;
			float time_to_trigger_ = 0;
			float repeat_time_ = 0;
			bool is_repeating_ = false;
			std::function<void(T*)> function_;
			void(T::* p_function_)();
		};

		T* object_;
		std::function<bool(T*, const float)> now_;
		std::function<bool(T*, const float)> next_;
		std::stack<std::function<bool(T*, const float)>> prevs_;
		bool (T::*p_now_)(const float);
		bool (T::*p_next_)(const float);
		typedef bool (T::*p_prev_)(const float) ;
		std::stack<p_prev_> p_prevs_;
		bool is_start_ = true;
		bool is_change_ = false;
		float sum_time_ = 0;
		uint32_t sum_frame_ = 0;

		uint32_t co_call_count_ = 0;
		uint32_t co_call_through_ = 0;
		int32_t co_frame_ = 0;
		int32_t co_limit_frame_ = 0;
		float co_time_ = 0;
		float co_limit_time_ = 0;
		bool co_is_break_ = false;
		std::list<Shared<Invoke>> invokes_;

		Sequence() {}
	public:

		//---------------------------------------------------------------------------------------------------------
		// �R���X�g���N�^
		// arg1... ��܃N���X�� this �|�C���^���w��
		// arg2... �R�[���o�b�N�ŌĂяo��������܃N���X�̃����o���\�b�h
		Sequence(T* obj, bool (T::*func)(const float))
			: object_(obj)
			, next_(func)
			, now_(func)
			, p_now_(func)
			, p_next_(func)
		{}

		~Sequence() {}

		//---------------------------------------------------------------------------------------------------------
		// �V�[�P���X�̃A�b�v�f�[�g ( ���t���[���Ăяo����OK )
		// arg1... �t���[���Ԃ̌o�ߎ���( �b�̃f���^�^�C�� )
		inline bool update(const float deltatime) {

			auto it = invokes_.begin();
			while (it != invokes_.end()) {
				bool f = (*it)->update(deltatime);
				if (!f) {
					it++;
					continue;
				}
				it = invokes_.erase(it);
			}

			sum_time_ += deltatime;
			bool ret = now_(object_, deltatime);
			sum_frame_++;
			if (!is_change_) {
				is_start_ = false;
				return ret;
			}
			now_ = next_;
			p_now_ = p_next_;
			is_start_ = true;
			sum_time_ = 0;
			sum_frame_ = 0;
			is_change_ = false;
			_co_reset_();
			return ret;
		}

		//---------------------------------------------------------------------------------------------------------
		// ��r
		inline bool isComparable(bool (T::*func)(const float)) const { return p_now_ == func; }

		//---------------------------------------------------------------------------------------------------------
		// �������p�@�V�[�P���X�̍ŏ��̂P�t���[������ true ���A��
		inline bool isStart() const { return is_start_; }

		//---------------------------------------------------------------------------------------------------------
		// �V�[�P���X�̌o�ߎ��Ԃ��擾 ( �b )
		inline float getProgressTime() const { return sum_time_; }

		//---------------------------------------------------------------------------------------------------------
		// �V�[�P���X�̌o�ߎ��Ԃ��擾 ( �b )
		inline uint32_t getProgressFrame() const { return sum_frame_; }

		//---------------------------------------------------------------------------------------------------------
		// �V�[�P���X�̕ύX
		// arg1... ���̃t���[��������s�������܃N���X�̃��\�b�h���w��
		inline void change(bool (T::*func)(const float)) {
			prevs_.push(now_);
			p_prevs_.push(p_now_);
			next_ = func;
			p_next_ = func;
			is_change_ = true;
		}

		//---------------------------------------------------------------------------------------------------------
		// 1�O�̃V�[�P���X�ɖ߂�
		// tips... �O�̃V�[�P���X�����݂��Ȃ���Ή������Ȃ�
		inline void undo() {
			if (prevs_.empty()) return;			
			next_ = prevs_.top();
			p_next_ = p_prevs_.top();
			prevs_.pop();
			p_prevs_.pop();
			is_change_ = true;
		}


		//---------------------------------------------------------------------------------------------------------
		// �V�[�P���X�𑦍��ɕύX
		// arg1... ���s�������܃N���X�̃��\�b�h���w��
		// tisp... ���t���[����҂��������ɃV�[�P���X��ύX����
		inline void immediatelyChange(bool (T::*func)(const float)) {
			prevs_.push(now_);
			p_prevs_.push(p_now_);
			now_ = func;
			p_now_ = func;
			is_start_ = true;
			sum_time_ = 0;
			_co_reset_();
			is_change_ = false;
		}

		//---------------------------------------------------------------------------------------------------------
		// �w��b���o�ߌ�Ɏ��s����郁�\�b�h
		// arg1... ���s���\�b�h
		// arg2... �����܂ł̎���
		void invoke(void (T::* func)(), float invoke_time) {
			invokes_.emplace_back( std::make_shared<Invoke>(object_, invoke_time, func) );
		}

		//---------------------------------------------------------------------------------------------------------
		// �w��b���o�ߌ�ɌJ��Ԃ����s����郁�\�b�h
		// arg1... ���s���\�b�h
		// arg2... �����܂ł̎���
		// arg3... ���b���Ƃɂ��肩������
		void invokeRepeating(void (T::* func)(), float invoke_time, float repeat_time) {
			invokes_.emplace_back(std::make_shared<Invoke>(object_, invoke_time, repeat_time, func));
		}

		//---------------------------------------------------------------------------------------------------------
		// invoke �̎�����
		// arg1... invoke or invokeRepeating �Ŏw�肵�����\�b�h
		// tips... �����o�^����Ă��Ă��S�ăL�����Z������܂�
		void cancelInvoke(void (T::* func)()) {
			auto it = invokes_.begin();
			while (it != invokes_.end()) {
				if ((*it)->isComparable(func)) {
					it = invokes_.erase(it);
					continue;
				}
				it++;
			}
		}



		//------------------------------------------------------------------------------------------------------------------------
		//
		//
		// �^���R���[�`��
		// 
		//

		// �R���[�`���@�\�t���V�[�P���X�錾�}�N��
		// arg1.. ��܃N���X��
		// arg2.. �����V�[�P���X
		#define TNL_CO_SEQUENCE( class_name, start_func )	tnl::Sequence<class_name> tnl_sequence_ = tnl::Sequence<class_name>(this, start_func);

		// �R���[�`���V�[�P���X�I������
		// tips.. �R���[�`���g�p���\�b�h�̍Ō�ɕK���R�[�����邱��
		#define TNL_SEQ_CO_END								tnl_sequence_._co_reset_call_count_(); return true ;

		// �R���[�`�����s�t���[�����̎擾
		#define TNL_SEQ_CO_PROG_FRAME						tnl_sequence_._co_get_prog_frame_()

		// �R���[�`�����s���Ԃ̎擾
		#define TNL_SEQ_CO_PROG_TIME						tnl_sequence_._co_get_prog_time_()

		// ���Y�R���[�`���̔j��
		#define TNL_SEQ_CO_BREAK							{ tnl_sequence_._co_break_() ; return ; }


		// �R���[�`���������������}�N��( �t���[�����w�� )
		// arg1... ���s�t���[���� (�}�C�i�X�̒l�Ŗ������[�v)
		// arg2... �f���^�^�C��
		// arg3... �R���[�`���Ŏ��s���郆�[�U��`����( void() �����_�� )
		// tips... �������[�v���w�肵���ꍇ�ł������Ōo�߃t���[�������J�E���g���Ă��܂�
		// ....... �t���[�����J�E���g�� int32_t �𒴂���� 0 �Ƀ��Z�b�g����邱�Ƃɒ���
		#define TNL_SEQ_CO_FRM_YIELD_RETURN( lim_frame, delta_time, logic )		if (tnl_sequence_._co_yield_by_frame_( lim_frame, delta_time, logic ) ) return true ;

		// �R���[�`���������������}�N��( ���Ԏw�� )
		// arg1... ���s����
		// arg2... �f���^�^�C��
		// arg3... �R���[�`���Ŏ��s���郆�[�U��`����( void() �����_�� )
		// tips... ���Ԏw�肵���ꍇ�ł������Ōo�߃t���[�������J�E���g���Ă��܂�
		// ....... �t���[�����J�E���g�� int32_t �𒴂���� 0 �Ƀ��Z�b�g����邱�Ƃɒ���
		#define TNL_SEQ_CO_TIM_YIELD_RETURN( lim_time, delta_time, logic )		if (tnl_sequence_._co_yield_by_time_( lim_time, delta_time, logic ) ) return true ;


		// �R���[�`������(TNL_SEQ_CO_FRM_YIELD_RETURN) �̍ŏ��̃t���[���Ȃ� true
		#define TNL_SEQ_CO_YIELD_FRM_IS_START		( 0 == tnl_sequence_._co_get_prog_frame_() )
		// �R���[�`������(TNL_SEQ_CO_FRM_YIELD_RETURN) �̍Ō�̃t���[���Ȃ� true
		#define TNL_SEQ_CO_YIELD_FRM_IS_END			( tnl_sequence_._co_get_limit_frame_() == (tnl_sequence_._co_get_prog_frame_()+1) )

		// �R���[�`������(TNL_SEQ_CO_TIM_YIELD_RETURN) �̍ŏ��̃t���[���Ȃ� true
		#define TNL_SEQ_CO_YIELD_TIM_IS_START		( 0 == tnl_sequence_._co_get_prog_frame_() )
		// �R���[�`������(TNL_SEQ_CO_TIM_YIELD_RETURN) �̍ŏ��̃t���[���Ȃ� true
		#define TNL_SEQ_CO_YIELD_TIM_IS_END			( tnl_sequence_._co_get_limit_time_() <= tnl_sequence_._co_get_prog_time_() )


		//------------------------------------------------------------------------------------------------------------------------
		// 
		// ���[�U�ɂ�钼�ڃR�[���͋֎~
		//
		inline void		_co_break_() { co_is_break_ = true; }
		inline int32_t	_co_get_prog_frame_() { return co_frame_; }
		inline int32_t  _co_get_limit_frame_() { return co_limit_frame_; }
		inline float	_co_get_prog_time_() { return co_time_; }
		inline float	_co_get_limit_time_() { return co_limit_time_; }
		inline void		_co_reset_call_count_() { co_call_count_ = 0; }
		inline bool		_co_yield_by_frame_(int32_t limit_frame, float delta_time, const std::function<void()>& logic) {
			co_limit_frame_ = limit_frame;
			if (co_call_count_++ == co_call_through_) {
				int32_t limit = (0 > limit_frame) ? INT32_MAX : limit_frame;
				if (co_frame_ >= limit) return true;
				co_call_count_ = 0;
				co_time_ += delta_time;
				logic();
				co_frame_++ ;
				if (limit_frame < 0 && INT32_MAX == co_frame_) co_frame_ = 0;
				if (limit_frame < 0 && !co_is_break_) return true;
				if (co_frame_ < limit && !co_is_break_) return true;
				co_call_through_++;
				co_frame_ = 0;
				co_time_ = 0;
				co_is_break_ = false;
				return true;
			}
			return false;
		}
		inline bool		_co_yield_by_time_(float limit_time, float delta_time, const std::function<void()>& logic) {
			co_limit_time_ = limit_time;
			if (co_call_count_++ == co_call_through_) {
				if (co_time_ >= limit_time) return true;
				co_call_count_ = 0;
				co_time_ += delta_time;
				logic();
				co_frame_++;
				if(INT32_MAX == co_frame_) co_frame_ = 0;
				if (co_time_ < limit_time && !co_is_break_) return true;
				co_call_through_++;
				co_frame_ = 0;
				co_time_ = 0;
				co_is_break_ = false;
				return true;
			}
			return false;
		}
		inline void		_co_reset_() {
			co_call_count_ = 0;
			co_call_through_ = 0;
			co_frame_ = 0;
			co_time_ = 0;
			co_is_break_ = false;
		}


	};

}

