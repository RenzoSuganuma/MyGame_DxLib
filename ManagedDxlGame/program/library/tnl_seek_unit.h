#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include "tnl_using.h"
#include "tnl_shared_factory.h"

namespace tnl {

	TNL_SHARED_FACTORY_CLASS(SeekUnit, tnl::SharedFactory<SeekUnit>)
	public:
		SeekUnit(){}

		virtual ~SeekUnit() {}

		enum class ePlayMode {
			SINGLE,		// 単発再生
			REPEAT,		// リピート再生
			REFLECTION	// 反転再生( リピート )
		};
		enum class ePlayDir {
			FORWARD,	// 通常
			REVERSE		// 逆再生
		};
		enum class eFrameType {
			PREV,		// 前のフレーム
			CURRENT,	// 現在のフレーム
			NEXT		// 次のフレーム
		};

		SeekUnit(const double fix_fps
			, const double total_time
			, uint32_t total_frame_num
			, const ePlayMode& play_mode
			, const ePlayDir& play_dir) {
			fix_fps_ = fix_fps;
			setTotalTime(total_time);
			setTotalFrameNum(total_frame_num);
			setPlayState(play_mode, play_dir);
		}

		//-----------------------------------------------------------------------------------------------------------------
		// arg1... 固定フレームレート
		// arg2... 全体の再生時間
		// arg3... 総フレーム数
		// arg4... ePlayMode
		// arg5... ePlayDir
		// tips... 総フレーム数に 0 を設定するとフレーム管理ではなく時間の割合で管理されるものと想定します
		//         そのため総フレーム数に 0 を設定した場合 メソッド名に Frame と付いているものは非推奨です
		static Shared<SeekUnit> Create(
			const double fix_fps
			, const double total_time
			, uint32_t total_frame_num
			, const ePlayMode& play_mode
			, const ePlayDir& play_dir) {
			Shared<SeekUnit> unit = std::make_shared<SeekUnit>(
				fix_fps
				, total_time
				, total_frame_num
				, play_mode
				, play_dir );
			return unit;
		}

		// 再生( 停止した場所から再生 )
		inline void play() { is_playing_ = true; }
		// 停止
		inline void stop() { is_playing_ = false; }

		// リスタート( 始めから再生 )
		void restart();

		// シーク場所を 0 ～ 1.0 の割合で指定した場所へ飛ばす
		void jumpSeekRate(const double seek_rate);

		// シーク場所を フレーム番号 で設定した場所へ飛ばす
		void jumpSeekFrame(uint32_t frame);

		// アップデート処理( 毎フレーム実行 )
		void update(const double delta_time);


		//-------------------------------------------------------------------------------------------------------
		//
		// setter
		//
		// 
		// 再生ステート設定
		void setPlayState(const ePlayMode& play_mode, const ePlayDir& play_dir);

		// 全体のスピードを設定
		inline void setTimeScale(const double time_scale) { time_scale_ = time_scale; }

		// フレームの再生開始時に１度だけ実行されるコールバックを設定
		// arg1... フレーム番号
		// arg2... コールバック
		void setFrameTriggerFunction(const uint32_t& frame, std::function<void()> func);

		//-------------------------------------------------------------------------------------------------------
		//
		// getter
		//
		inline uint32_t getTotalFrameNum() { return total_frame_num_; }

		// 現在の全体のシーク時間の割合を 0 ～ 1.0 で返す
		inline double getSeekRate() { return seek_position_ / total_time_; }

		// 現在のフレームのフレームあたりの再生時間の割合
		double getSeekRateInFrame();

		// フレーム番号を取得
		// arg1... eFrameType
		uint32_t getSeekFrame(const eFrameType& e_frame);

	private:
		bool is_playing_ = false;
		double fix_fps_ = 60.0f;
		double time_scale_ = 1.0f;
		double total_time_ = 0;
		double seek_position_ = 0;
		int32_t direction_ = 1;
		uint32_t total_frame_num_ = 0;
		std::function<void()> call_of_trigger_ = nullptr;
		std::vector<std::function<void()>> frame_functions_storage_;
		ePlayMode play_mode_ = ePlayMode::SINGLE;
		ePlayDir play_dir_ = ePlayDir::FORWARD;

		void seekProcess(float delta_time);

		// 全体の再生時間を設定
		inline void setTotalTime(const double total_time) { total_time_ = total_time; }

		// 総フレーム数の設定
		void setTotalFrameNum(const uint32_t frame_num = 0);

	};


}

