#include "tnl_seek_unit.h"

namespace tnl {

	//-------------------------------------------------------------------------------------------------------
	void SeekUnit::restart() {
		setPlayState(play_mode_, play_dir_);
		play();
		if (frame_functions_storage_.empty()) return;
		uint32_t start_frame = (ePlayDir::REVERSE == play_dir_) ? total_frame_num_ - 1 : 0;
		if (!frame_functions_storage_[start_frame]) return;
		if (call_of_trigger_) return;
		call_of_trigger_ = frame_functions_storage_[start_frame];
	}

	//-------------------------------------------------------------------------------------------------------
	void SeekUnit::jumpSeekRate(const double seek_rate) {
		seek_position_ = total_time_ * seek_rate;
		seek_position_ = std::clamp<double>(seek_position_, 0, total_time_);
	}

	//-------------------------------------------------------------------------------------------------------
	void SeekUnit::jumpSeekFrame(uint32_t frame) {
		frame = std::clamp<uint32_t>(frame, 0, (total_frame_num_ - 1));
		seek_position_ = (static_cast<double>(frame) / static_cast<double>(total_frame_num_)) * total_time_;
		seek_position_ = std::clamp<double>(seek_position_, 0, total_time_);
		if (frame_functions_storage_.empty()) return;
		if (!frame_functions_storage_[frame]) return;
		call_of_trigger_ = frame_functions_storage_[frame];
	}

	//-------------------------------------------------------------------------------------------------------
	void SeekUnit::update(const double delta_time) {
		if (!is_playing_) return;
		uint32_t frame = getSeekFrame(eFrameType::CURRENT);
		seekProcess(static_cast<float>(delta_time));
		if (frame_functions_storage_.empty()) return;
		uint32_t update_frame = getSeekFrame(eFrameType::CURRENT);
		update_frame %= total_frame_num_;
		if (update_frame != frame) {
			if (!call_of_trigger_ && frame_functions_storage_[update_frame]) {
				call_of_trigger_ = frame_functions_storage_[update_frame];
			}
		}
		if (!call_of_trigger_) return;
		call_of_trigger_();
		call_of_trigger_ = nullptr;
	}


	//-------------------------------------------------------------------------------------------------------
	void SeekUnit::setFrameTriggerFunction(const uint32_t& frame, std::function<void()> func) {
		if (frame_functions_storage_.empty()) return;
		if (frame_functions_storage_.size() <= frame) return;
		frame_functions_storage_[frame] = func;
		uint32_t start_frame = (ePlayDir::REVERSE == play_dir_) ? total_frame_num_ - 1 : 0;
		if (start_frame == frame) call_of_trigger_ = func;
	}

	//-------------------------------------------------------------------------------------------------------
	void SeekUnit::setPlayState(const ePlayMode& play_mode, const ePlayDir& play_dir) {
		play_dir_ = play_dir;
		play_mode_ = play_mode;
		if (ePlayDir::REVERSE == play_dir_) {
			jumpSeekRate(1.0);
			direction_ = -1;
		}
		else {
			jumpSeekRate(0.0);
			direction_ = 1;
		}
	}

	//-------------------------------------------------------------------------------------------------------
	void SeekUnit::seekProcess(const float delta_time) {

		double d_delta_time = static_cast<double>(delta_time);
		double time = seek_position_ + d_delta_time * time_scale_ * static_cast<double>(direction_);
		if (0 >= time || time >= total_time_) {
			double over_time = (0 < time) ? (time - total_time_) : time;
			if (ePlayMode::SINGLE == play_mode_) {
				seek_position_ = time;
				if (0 == total_frame_num_) {
					seek_position_ = std::clamp(seek_position_, 0.0, total_time_);
				}
				else {
					seek_position_ = std::clamp(seek_position_, 0.0, total_time_ - (total_time_ / total_frame_num_));
				}
				stop();
			}
			else if (ePlayMode::REPEAT == play_mode_) {
				restart();
				seek_position_ += (d_delta_time * time_scale_ * static_cast<double>(direction_)) - over_time;
			}
			else if (ePlayMode::REFLECTION == play_mode_) {
				direction_ *= -1;
				seek_position_ += ((total_time_ / total_frame_num_) * static_cast<double>(direction_)) - over_time;
			}
		}
		else {
			seek_position_ = time;
		}

	}

	//-------------------------------------------------------------------------------------------------------
	void SeekUnit::setTotalFrameNum(const uint32_t frame_num) {
		if (0 == frame_num) {
			total_frame_num_ = static_cast<uint32_t>(total_time_ / (1.0 / fix_fps_));
			return;
		}
		total_frame_num_ = frame_num;
		frame_functions_storage_.resize(total_frame_num_);
	}

	//-------------------------------------------------------------------------------------------------------
	double SeekUnit::getSeekRateInFrame() {
		double frame_time = total_time_ / static_cast<double>(total_frame_num_);
		double frame_now = static_cast<double>(getSeekFrame(eFrameType::CURRENT));
		double rate = 0;
		if (direction_ > 0) {
			rate = (seek_position_ - (frame_now * frame_time)) / frame_time;
		}
		else {
			rate = 1.0f - ((seek_position_ - (frame_now * frame_time)) / frame_time);
		}
		rate = std::clamp<double>(rate, 0, 1.0);
		return rate;
	}

	//-------------------------------------------------------------------------------------------------------
	uint32_t SeekUnit::getSeekFrame(const eFrameType& e_frame) {

		uint32_t frame_now = static_cast<uint32_t>(std::floor(static_cast<double>(total_frame_num_) * getSeekRate()));
		if (eFrameType::CURRENT == e_frame) return frame_now % total_frame_num_;

		int32_t dir = (eFrameType::NEXT == e_frame) ? direction_ : -direction_;

		uint32_t frame = static_cast<int>(frame_now) + dir;
		if (total_frame_num_ <= frame || 0 > frame) {
			if (ePlayMode::SINGLE == play_mode_) {
				frame = std::clamp<int32_t>(frame, 0, total_frame_num_ - 1);
			}
			else if (ePlayMode::REPEAT == play_mode_) {
				frame %= static_cast<int>(total_frame_num_);
				frame = static_cast<uint32_t>(std::abs(static_cast<int>(frame))) * (total_frame_num_ - 1);
			}
			else if (ePlayMode::REFLECTION == play_mode_) {
				frame += -(dir * 2);
			}
		}
		return frame;
	}


}