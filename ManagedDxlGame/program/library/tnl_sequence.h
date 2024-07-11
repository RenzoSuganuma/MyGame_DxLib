#pragma once
#include <stack>
#include <functional>

namespace tnl {

	//--------------------------------------------------------------------------------------------------------------------------
	// Sequence
	// tips... 1.デリゲートによるメソッドの呼び分け機能を提供します
	// 　　　　2.疑似コルーチンマクロによる簡易コルーチン記述を提供します
	//         3.invoke による時間経過に基づいて処理を実行するための機能を提供します
	// 
	//			それぞれの機能は組み合わせて使用することもできます
	// 
	// 
	// ==================================================================================================================
	// ◇ 使用法サンプル 1.デリゲートによるメソッドの呼び分け機能
	// ==================================================================================================================
	// 
	//  --------------- Example.h ----------------
	// 
	// class Example
	// {
	//	public:
	//		// シーケンス機能の宣言
	//		TNL_CO_SEQUENCE(Example, &Example::seqCallFunctionA); // 初動で seqCallFunctionA が実行されます
	// 
	//		// 毎フレーム実行される更新処理
	//		void update(float delta_time) ;
	// 
	//		// シーケンス機能で呼び分けるメソッド
	//		bool seqCallFunctionA(float delta_time);
	//		bool seqCallFunctionB(float delta_time);
	//		bool seqCallFunctionC(float delta_time);
	//
	//		中略
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
	//		// このif文はメソッドがコールされた最初の１フレームのみ真になる
	//		if( tnl_sequence_.isStart() ){
	//			// seqCallFunctionA の任意の初期化処理
	//		}
	// 
	//		// ５秒経過で次のフレームから seqCallFunctionB に移行
	//		if( tnl_sequence_.getProgressTime() > 5.0f ){
	//			tnl_sequence_.change( &Example::seqCallFunctionB ) ;
	//		}
	// 
	//		return true ;
	// }
	// 
	// bool Example::seqCallFunctionB(float delta_time)
	// {
	//		// このif文はメソッドがコールされた最初の１フレームのみ真になる
	//		if( tnl_sequence_.isStart() ){
	//			// seqCallFunctionA の任意の初期化処理
	//		}
	// 
	//		// ３秒経過で次のフレームから直前に呼び出されていたメソッドに戻る
	//		if( tnl_sequence_.getProgressTime() > 3.0f ){
	//			tnl_sequence_.undo() ;
	//		}
	// 
	//		return true ;
	// }
	// 
	// 
	// ==================================================================================================================
	// ◇ 使用法サンプル 2.疑似コルーチンマクロによる簡易コルーチン記述
	// ==================================================================================================================
	// 
	//  --------------- Example.h ----------------
	// 
	//				　　同サンプル１
	// 
	// 
	//  --------------- Example.cpp ----------------
	// 
	// bool Example::seqCallFunctionA(float delta_time)
	// {
	// 
	//		// ---------- フレーム指定でのコルーチン ----------
	// 
	//		// 10 フレームの間ラムダ式指定の処理が実行される
	//		TNL_SEQ_CO_FRM_YIELD_RETURN(10, delta_time, [&] {
	// 
	//			// 最初の１フレームのみ真になる
	//			if ( TNL_SEQ_CO_YIELD_FRM_IS_START ) {
	//			}
	// 
	//		});
	// 
	//		// ---------- 秒数指定でのコルーチン ----------
	// 
	//		// 1 秒間の間ラムダ式指定の処理が実行される
	//		TNL_SEQ_CO_TIM_YIELD_RETURN(1.0f, delta_time, [&] {
	// 
	//			// 最初の１フレームのみ真になる
	//			if ( TNL_SEQ_CO_YIELD_FRM_IS_START ) {
	//			}
	// 
	//		});
	// 
	//		// 上記２つのコルーチン処理が終了したら seqCallFunctionB に移行
	//		tnl_sequence_.change(&Example::seqCallFunctionB) ;
	// 
	// 
	//		// コルーチンマクロを使用するメソッドの最後に必ず記述
	//		// このマクロ使用時は return false 不要
	//		TNL_SEQ_CO_END ;
	// }
	// 
	// 
	// 
	// bool Example::seqCallFunctionB(float delta_time){
	// 
	// 
	//		// ---------- コルーチン処理の途中破棄 ----------
	// 
	//		// 60 フレームの間ラムダ式指定の処理が実行される
	//		TNL_SEQ_CO_FRM_YIELD_RETURN(60, delta_time, [&] {
	// 
	//			// このコルーチンの経過フレーム数が 30 になったら真
	//			if( TNL_SEQ_CO_PROG_FRAME == 30 ){
	//				// このコルーチンを破棄する
	//				TNL_SEQ_CO_BREAK ;
	//			}
	// 
	//		});
	// 
	//		// コルーチン処理が終了したら seqCallFunctionC に移行
	//		tnl_sequence_.change( &Example::seqCallFunctionC ) ;
	// 
	//		// コルーチンマクロを使用するメソッドの最後に必ず記述
	//		TNL_SEQ_CO_END ;
	// 
	// }
	// 
	// 
	// bool Example::seqCallFunctionC( float delta_time ){
	// 
	//		// ---------- コルーチン処理を繰り返す( 無限ループで停止はしない ) ----------
	// 
	//		while( 1 ){
	//			// 10 フレームの間ラムダ式指定の処理が実行される
	//			TNL_SEQ_CO_FRM_YIELD_RETURN(10, delta_time, [&] {
	// 
	//			});
	// 
	//			// 10 フレームの間ラムダ式指定の処理が実行される
	//			TNL_SEQ_CO_FRM_YIELD_RETURN(10, delta_time, [&] {
	// 
	//			});
	// 
	//		}
	// 
	//		// コルーチンマクロを使用するメソッドの最後に必ず記述
	//		TNL_SEQ_CO_END ;
	// 
	// }
	// 
	// 
	// ==================================================================================================================
	// ◇ 使用法サンプル 3.invoke による時間経過に基づいて処理を実行するための機能
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
	//		TNL_CO_SEQUENCE(Example, &Example::seqCallFunctionA); // 初動で seqCallFunctionA が実行されます
	// 
	//		// シーケンス機能で呼び分けるメソッド
	//		bool seqCallFunctionA(float delta_time);
	// 
	//		// Invoke で呼び出すメソッド
	//		void calledFromInvokeA(float delta_time);
	//		void calledFromInvokeB(float delta_time);
	//
	//		中略
	// } ;
	// 
	//  --------------- Example.cpp ----------------
	// 
	// void Example::Example()
	// {
	// 
	//		// seqCallFunctionB を 1秒後に 1回だけ実行
	//		tnl_sequence_.invoke( &Example::calledFromInvokeA, 1.0f);
	// 
	//		// seqCallFunctionC を 3秒後に 0.5秒間隔で実行
	//		tnl_sequence_.invokeRepeating( &Example::calledFromInvokeB, 3.0f, 0.5f);
	// 
	//		// 指定した invoke を全てキャンセル
	//		tnl_sequence_.cancelInvoke( &Example::calledFromInvokeB ) ;
	// }
	// 
	// bool Example::seqCallFunctionA(float DeltaSeconds)
	// {
	//		// 任意の処理
	// }
	// bool Example::calledFromInvokeA(float DeltaSeconds)
	// {
	//		// 任意の処理
	// }
	// bool Example::calledFromInvokeB(float DeltaSeconds)
	// {
	//		// 任意の処理
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
		// コンストラクタ
		// arg1... 包含クラスの this ポインタを指定
		// arg2... コールバックで呼び出したい包含クラスのメンバメソッド
		Sequence(T* obj, bool (T::*func)(const float))
			: object_(obj)
			, next_(func)
			, now_(func)
			, p_now_(func)
			, p_next_(func)
		{}

		~Sequence() {}

		//---------------------------------------------------------------------------------------------------------
		// シーケンスのアップデート ( 毎フレーム呼び出せばOK )
		// arg1... フレーム間の経過時間( 秒のデルタタイム )
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
		// 比較
		inline bool isComparable(bool (T::*func)(const float)) const { return p_now_ == func; }

		//---------------------------------------------------------------------------------------------------------
		// 初期化用　シーケンスの最初の１フレームだけ true が帰る
		inline bool isStart() const { return is_start_; }

		//---------------------------------------------------------------------------------------------------------
		// シーケンスの経過時間を取得 ( 秒 )
		inline float getProgressTime() const { return sum_time_; }

		//---------------------------------------------------------------------------------------------------------
		// シーケンスの経過時間を取得 ( 秒 )
		inline uint32_t getProgressFrame() const { return sum_frame_; }

		//---------------------------------------------------------------------------------------------------------
		// シーケンスの変更
		// arg1... 次のフレームから実行させる包含クラスのメソッドを指定
		inline void change(bool (T::*func)(const float)) {
			prevs_.push(now_);
			p_prevs_.push(p_now_);
			next_ = func;
			p_next_ = func;
			is_change_ = true;
		}

		//---------------------------------------------------------------------------------------------------------
		// 1つ前のシーケンスに戻る
		// tips... 前のシーケンスが存在しなければ何もしない
		inline void undo() {
			if (prevs_.empty()) return;			
			next_ = prevs_.top();
			p_next_ = p_prevs_.top();
			prevs_.pop();
			p_prevs_.pop();
			is_change_ = true;
		}


		//---------------------------------------------------------------------------------------------------------
		// シーケンスを即座に変更
		// arg1... 実行させる包含クラスのメソッドを指定
		// tisp... 次フレームを待たず即座にシーケンスを変更する
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
		// 指定秒数経過後に実行されるメソッド
		// arg1... 実行メソッド
		// arg2... 発動までの時間
		void invoke(void (T::* func)(), float invoke_time) {
			invokes_.emplace_back( std::make_shared<Invoke>(object_, invoke_time, func) );
		}

		//---------------------------------------------------------------------------------------------------------
		// 指定秒数経過後に繰り返し実行されるメソッド
		// arg1... 実行メソッド
		// arg2... 発動までの時間
		// arg3... 何秒ごとにくりかえすか
		void invokeRepeating(void (T::* func)(), float invoke_time, float repeat_time) {
			invokes_.emplace_back(std::make_shared<Invoke>(object_, invoke_time, repeat_time, func));
		}

		//---------------------------------------------------------------------------------------------------------
		// invoke の取り消し
		// arg1... invoke or invokeRepeating で指定したメソッド
		// tips... 複数登録されていても全てキャンセルされます
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
		// 疑似コルーチン
		// 
		//

		// コルーチン機能付きシーケンス宣言マクロ
		// arg1.. 包含クラス名
		// arg2.. 初期シーケンス
		#define TNL_CO_SEQUENCE( class_name, start_func )	tnl::Sequence<class_name> tnl_sequence_ = tnl::Sequence<class_name>(this, start_func);

		// コルーチンシーケンス終了処理
		// tips.. コルーチン使用メソッドの最後に必ずコールすること
		#define TNL_SEQ_CO_END								tnl_sequence_._co_reset_call_count_(); return true ;

		// コルーチン実行フレーム数の取得
		#define TNL_SEQ_CO_PROG_FRAME						tnl_sequence_._co_get_prog_frame_()

		// コルーチン実行時間の取得
		#define TNL_SEQ_CO_PROG_TIME						tnl_sequence_._co_get_prog_time_()

		// 当該コルーチンの破棄
		#define TNL_SEQ_CO_BREAK							{ tnl_sequence_._co_break_() ; return ; }


		// コルーチン内部処理実装マクロ( フレーム数指定 )
		// arg1... 実行フレーム数 (マイナスの値で無限ループ)
		// arg2... デルタタイム
		// arg3... コルーチンで実行するユーザ定義処理( void() ラムダ式 )
		// tips... 無限ループを指定した場合でも内部で経過フレーム数をカウントしています
		// ....... フレーム数カウントは int32_t を超えると 0 にリセットされることに注意
		#define TNL_SEQ_CO_FRM_YIELD_RETURN( lim_frame, delta_time, logic )		if (tnl_sequence_._co_yield_by_frame_( lim_frame, delta_time, logic ) ) return true ;

		// コルーチン内部処理実装マクロ( 時間指定 )
		// arg1... 実行時間
		// arg2... デルタタイム
		// arg3... コルーチンで実行するユーザ定義処理( void() ラムダ式 )
		// tips... 時間指定した場合でも内部で経過フレーム数をカウントしています
		// ....... フレーム数カウントは int32_t を超えると 0 にリセットされることに注意
		#define TNL_SEQ_CO_TIM_YIELD_RETURN( lim_time, delta_time, logic )		if (tnl_sequence_._co_yield_by_time_( lim_time, delta_time, logic ) ) return true ;


		// コルーチン内部(TNL_SEQ_CO_FRM_YIELD_RETURN) の最初のフレームなら true
		#define TNL_SEQ_CO_YIELD_FRM_IS_START		( 0 == tnl_sequence_._co_get_prog_frame_() )
		// コルーチン内部(TNL_SEQ_CO_FRM_YIELD_RETURN) の最後のフレームなら true
		#define TNL_SEQ_CO_YIELD_FRM_IS_END			( tnl_sequence_._co_get_limit_frame_() == (tnl_sequence_._co_get_prog_frame_()+1) )

		// コルーチン内部(TNL_SEQ_CO_TIM_YIELD_RETURN) の最初のフレームなら true
		#define TNL_SEQ_CO_YIELD_TIM_IS_START		( 0 == tnl_sequence_._co_get_prog_frame_() )
		// コルーチン内部(TNL_SEQ_CO_TIM_YIELD_RETURN) の最初のフレームなら true
		#define TNL_SEQ_CO_YIELD_TIM_IS_END			( tnl_sequence_._co_get_limit_time_() <= tnl_sequence_._co_get_prog_time_() )


		//------------------------------------------------------------------------------------------------------------------------
		// 
		// ユーザによる直接コールは禁止
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

