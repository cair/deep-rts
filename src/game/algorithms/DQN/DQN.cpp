#define CPU_ONLY

#include "DQN.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <sstream>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <glog/logging.h>
#include "../../lib/prettyprint.hpp"



/**
* Convert pixel_t (NTSC) to RGB values.
* Each value range [0,255]
*/
const std::array<int, 3> PixelToRGB(const int& pixel) {
	constexpr int ntsc_to_rgb[] = {
		0x000000, 0, 0x4a4a4a, 0, 0x6f6f6f, 0, 0x8e8e8e, 0,
		0xaaaaaa, 0, 0xc0c0c0, 0, 0xd6d6d6, 0, 0xececec, 0,
		0x484800, 0, 0x69690f, 0, 0x86861d, 0, 0xa2a22a, 0,
		0xbbbb35, 0, 0xd2d240, 0, 0xe8e84a, 0, 0xfcfc54, 0,
		0x7c2c00, 0, 0x904811, 0, 0xa26221, 0, 0xb47a30, 0,
		0xc3903d, 0, 0xd2a44a, 0, 0xdfb755, 0, 0xecc860, 0,
		0x901c00, 0, 0xa33915, 0, 0xb55328, 0, 0xc66c3a, 0,
		0xd5824a, 0, 0xe39759, 0, 0xf0aa67, 0, 0xfcbc74, 0,
		0x940000, 0, 0xa71a1a, 0, 0xb83232, 0, 0xc84848, 0,
		0xd65c5c, 0, 0xe46f6f, 0, 0xf08080, 0, 0xfc9090, 0,
		0x840064, 0, 0x97197a, 0, 0xa8308f, 0, 0xb846a2, 0,
		0xc659b3, 0, 0xd46cc3, 0, 0xe07cd2, 0, 0xec8ce0, 0,
		0x500084, 0, 0x68199a, 0, 0x7d30ad, 0, 0x9246c0, 0,
		0xa459d0, 0, 0xb56ce0, 0, 0xc57cee, 0, 0xd48cfc, 0,
		0x140090, 0, 0x331aa3, 0, 0x4e32b5, 0, 0x6848c6, 0,
		0x7f5cd5, 0, 0x956fe3, 0, 0xa980f0, 0, 0xbc90fc, 0,
		0x000094, 0, 0x181aa7, 0, 0x2d32b8, 0, 0x4248c8, 0,
		0x545cd6, 0, 0x656fe4, 0, 0x7580f0, 0, 0x8490fc, 0,
		0x001c88, 0, 0x183b9d, 0, 0x2d57b0, 0, 0x4272c2, 0,
		0x548ad2, 0, 0x65a0e1, 0, 0x75b5ef, 0, 0x84c8fc, 0,
		0x003064, 0, 0x185080, 0, 0x2d6d98, 0, 0x4288b0, 0,
		0x54a0c5, 0, 0x65b7d9, 0, 0x75cceb, 0, 0x84e0fc, 0,
		0x004030, 0, 0x18624e, 0, 0x2d8169, 0, 0x429e82, 0,
		0x54b899, 0, 0x65d1ae, 0, 0x75e7c2, 0, 0x84fcd4, 0,
		0x004400, 0, 0x1a661a, 0, 0x328432, 0, 0x48a048, 0,
		0x5cba5c, 0, 0x6fd26f, 0, 0x80e880, 0, 0x90fc90, 0,
		0x143c00, 0, 0x355f18, 0, 0x527e2d, 0, 0x6e9c42, 0,
		0x87b754, 0, 0x9ed065, 0, 0xb4e775, 0, 0xc8fc84, 0,
		0x303800, 0, 0x505916, 0, 0x6d762b, 0, 0x88923e, 0,
		0xa0ab4f, 0, 0xb7c25f, 0, 0xccd86e, 0, 0xe0ec7c, 0,
		0x482c00, 0, 0x694d14, 0, 0x866a26, 0, 0xa28638, 0,
		0xbb9f47, 0, 0xd2b656, 0, 0xe8cc63, 0, 0xfce070, 0
	};
	const auto rgb = ntsc_to_rgb[pixel];
	const auto r = rgb >> 16;
	const auto g = (rgb >> 8) & 0xFF;
	const auto b = rgb & 0xFF;
	return{ r, g, b };
}

/**
* Convert RGB values to a grayscale value [0,255].
*/
uint8_t RGBToGrayscale(const std::array<int, 3>& rgb) {
	assert(rgb[0] >= 0 && rgb[0] <= 255);
	assert(rgb[1] >= 0 && rgb[1] <= 255);
	assert(rgb[2] >= 0 && rgb[2] <= 255);
	// Normalized luminosity grayscale
	return rgb[0] * 0.21 + rgb[1] * 0.72 + rgb[2] * 0.07;
}

uint8_t PixelToGrayscale(const int pixel) {
	return RGBToGrayscale(PixelToRGB(pixel));
}

FrameDataSp PreprocessScreen(const sf::RenderWindow& raw_screen) {
		
	sf::Vector2u windowSize = raw_screen.getSize();
	sf::Texture texture;
	texture.create(windowSize.x, windowSize.y);
	texture.update(raw_screen);
	sf::Image screenshot = texture.copyToImage();

	assert(raw_screen.getSize().x == kRawFrameWidth);
	assert(raw_screen.getSize().y == kRawFrameHeight);
	const auto raw_pixels = screenshot.getPixelsPtr();
	auto screen = std::make_shared<FrameData>();
	assert(kRawFrameHeight > kRawFrameWidth);
	const auto x_ratio = kRawFrameWidth / static_cast<double>(kCroppedFrameSize);
	const auto y_ratio = kRawFrameHeight / static_cast<double>(kCroppedFrameSize);
	for (auto i = 0; i < kCroppedFrameSize; ++i) {
		for (auto j = 0; j < kCroppedFrameSize; ++j) {
			const auto first_x = static_cast<int>(std::floor(j * x_ratio));
			const auto last_x = static_cast<int>(std::floor((j + 1) * x_ratio));
			const auto first_y = static_cast<int>(std::floor(i * y_ratio));
			const auto last_y = static_cast<int>(std::floor((i + 1) * y_ratio));
			auto x_sum = 0.0;
			auto y_sum = 0.0;
			uint8_t resulting_color = 0.0;
			for (auto x = first_x; x <= last_x; ++x) {
				double x_ratio_in_resulting_pixel = 1.0;
				if (x == first_x) {
					x_ratio_in_resulting_pixel = x + 1 - j * x_ratio;
				}
				else if (x == last_x) {
					x_ratio_in_resulting_pixel = x_ratio * (j + 1) - x;
				}
				assert(
					x_ratio_in_resulting_pixel >= 0.0 &&
					x_ratio_in_resulting_pixel <= 1.0);
				for (auto y = first_y; y <= last_y; ++y) {
					double y_ratio_in_resulting_pixel = 1.0;
					if (y == first_y) {
						y_ratio_in_resulting_pixel = y + 1 - i * y_ratio;
					}
					else if (y == last_y) {
						y_ratio_in_resulting_pixel = y_ratio * (i + 1) - y;
					}
					assert(
						y_ratio_in_resulting_pixel >= 0.0 &&
						y_ratio_in_resulting_pixel <= 1.0);
					const auto grayscale =
						PixelToGrayscale(
							raw_pixels[static_cast<int>(y * kRawFrameWidth + x)]);
					resulting_color +=
						(x_ratio_in_resulting_pixel / x_ratio) *
						(y_ratio_in_resulting_pixel / y_ratio) * grayscale;
				}
			}
			(*screen)[i * kCroppedFrameSize + j] = resulting_color;
		}
	}
	return screen;
}

std::string DrawFrame(const FrameData& frame) {
	std::ostringstream o;
	for (auto row = 0; row < kCroppedFrameSize; ++row) {
		for (auto col = 0; col < kCroppedFrameSize; ++col) {
			o << std::hex <<
				static_cast<int>(frame[row * kCroppedFrameSize + col] / 16);
		}
		o << std::endl;
	}
	return o.str();
}

std::string PrintQValues(
	const std::vector<float>& q_values, const ActionVector& actions) {
	assert(!q_values.empty());
		
	assert(!actions.empty());
	assert(q_values.size() == actions.size());
	std::ostringstream actions_buf;
	std::ostringstream q_values_buf;
	for (auto i = 0; i < q_values.size(); ++i) {
		const auto a_str =
			boost::algorithm::replace_all_copy(actions.actionToString(i), "PLAYER_A_", "");
		const auto q_str = std::to_string(q_values[i]);
		const auto column_size = std::max(a_str.size(), q_str.size()) + 1;
		actions_buf.width(column_size);
		actions_buf << a_str;
		q_values_buf.width(column_size);
		q_values_buf << q_str;
	}
	actions_buf << std::endl;
	q_values_buf << std::endl;
	return actions_buf.str() + q_values_buf.str();
}

template <typename Dtype>
bool HasBlobSize(
	const caffe::Blob<Dtype>& blob,
	const int num,
	const int channels,
	const int height,
	const int width) {
	return blob.num() == num &&
		blob.channels() == channels &&
		blob.height() == height &&
		blob.width() == width;
}

void DQN::LoadTrainedModel(const std::string& model_bin) {
	net_->CopyTrainedLayersFrom(model_bin);
}

void DQN::Initialize() {
	// Initialize net and solver
	caffe::SolverParameter solver_param;
	caffe::ReadProtoFromTextFileOrDie(solver_param_, &solver_param);

	solver_.reset(caffe::SolverRegistry<float>::CreateSolver(solver_param));
	net_ = solver_->net();

	// Cache pointers to blobs that hold Q values
	q_values_blob_ = net_->blob_by_name("q_values");

	// Initialize dummy input data with 0
	std::fill(dummy_input_data_.begin(), dummy_input_data_.end(), 0.0);

	// Cache pointers to input layers
	frames_input_layer_ =
		boost::dynamic_pointer_cast<caffe::MemoryDataLayer<float>>(
			net_->layer_by_name("frames_input_layer"));
	assert(frames_input_layer_);
	assert(HasBlobSize(
		*net_->blob_by_name("frames"),
		kMinibatchSize,
		kInputFrameCount,
		kCroppedFrameSize,
		kCroppedFrameSize));
	target_input_layer_ =
		boost::dynamic_pointer_cast<caffe::MemoryDataLayer<float>>(
			net_->layer_by_name("target_input_layer"));
	assert(target_input_layer_);
	assert(HasBlobSize(
		*net_->blob_by_name("target"), kMinibatchSize, kOutputCount, 1, 1));
	filter_input_layer_ =
		boost::dynamic_pointer_cast<caffe::MemoryDataLayer<float>>(
			net_->layer_by_name("filter_input_layer"));
	assert(filter_input_layer_);
	assert(HasBlobSize(
		*net_->blob_by_name("filter"), kMinibatchSize, kOutputCount, 1, 1));
}

Action DQN::SelectAction(const InputFrames& last_frames, const double epsilon) {


	assert(epsilon >= 0.0 && epsilon <= 1.0);
	Action action = SelectActionGreedily(last_frames).first;
	if (std::uniform_real_distribution<>(0.0, 1.0)(random_engine) < epsilon) {

		// Select randomly
		const auto random_idx = std::uniform_int_distribution<int>(0, legal_actions_.size() - 1)(random_engine);
		action = legal_actions_[random_idx];
		std::cout << action.toString() << " (random)";
	} else {
		std::cout << action.toString() << " (greedy)";
	}
	std::cout << " epsilon:" << epsilon << std::endl;
	return action;
}

std::pair<Action, float> DQN::SelectActionGreedily(const InputFrames& last_frames) {
	return SelectActionGreedily(std::vector<InputFrames>{ {last_frames}}).front();
}

std::vector<std::pair<Action, float>> DQN::SelectActionGreedily(
	const std::vector<InputFrames>& last_frames_batch) {
	assert(last_frames_batch.size() <= kMinibatchSize);
	std::array<float, kMinibatchDataSize> frames_input;
	for (auto i = 0; i < last_frames_batch.size(); ++i) {
		// Input frames to the net and compute Q values for each legal actions
		for (auto j = 0; j < kInputFrameCount; ++j) {
			const auto& frame_data = last_frames_batch[i][j];
			std::copy(
				frame_data->begin(),
				frame_data->end(),
				frames_input.begin() + i * kInputDataSize +
				j * kCroppedFrameDataSize);
		}
	}
	InputDataIntoLayers(frames_input, dummy_input_data_, dummy_input_data_);
	net_->ForwardPrefilled(nullptr);

	std::vector<std::pair<Action, float>> results;
	results.reserve(last_frames_batch.size());
	for (auto i = 0; i < last_frames_batch.size(); ++i) {
		// Get the Q values from the net
		const auto action_evaluator = [&](Action action) {
			const auto q = q_values_blob_->data_at(i, static_cast<int>(action.id), 0, 0);
			assert(!std::isnan(q));
			return q;
		};

		std::vector<float> q_values(legal_actions_.size());
		std::transform(
			legal_actions_.actions.begin(),
			legal_actions_.actions.end(),
			q_values.begin(),
			action_evaluator);
		if (last_frames_batch.size() == 1) {
			std::cout << PrintQValues(q_values, legal_actions_);
		}

		// Select the action with the maximum Q value
		const auto max_idx =
			std::distance(
				q_values.begin(),
				std::max_element(q_values.begin(), q_values.end()));
		results.emplace_back(legal_actions_[max_idx], q_values[max_idx]);
	}
	return results;
}

void DQN::AddTransition(const Transition& transition) {
	if (replay_memory_.size() == replay_memory_capacity_) {
		replay_memory_.pop_front();
	}
	replay_memory_.push_back(transition);
}

void DQN::Update() {
	std::cout << "iteration: " << current_iter_++ << std::endl;

	// Sample transitions from replay memory
	std::vector<int> transitions;
	transitions.reserve(kMinibatchSize);
	for (auto i = 0; i < kMinibatchSize; ++i) {
		const auto random_transition_idx =
			std::uniform_int_distribution<int>(0, replay_memory_.size() - 1)(
				random_engine);
		transitions.push_back(random_transition_idx);
	}

	// Compute target values: max_a Q(s',a)
	std::vector<InputFrames> target_last_frames_batch;
	for (const auto idx : transitions) {
		const auto& transition = replay_memory_[idx];
		if (!std::get<3>(transition)) {
			// This is a terminal state
			continue;
		}
		// Compute target value
		InputFrames target_last_frames;
		for (auto i = 0; i < kInputFrameCount - 1; ++i) {
			target_last_frames[i] = std::get<0>(transition)[i + 1];
		}
		target_last_frames[kInputFrameCount - 1] = std::get<3>(transition).get();
		target_last_frames_batch.push_back(target_last_frames);
	}
	const auto actions_and_values =
		SelectActionGreedily(target_last_frames_batch);

	FramesLayerInputData frames_input;
	TargetLayerInputData target_input;
	FilterLayerInputData filter_input;
	std::fill(target_input.begin(), target_input.end(), 0.0f);
	std::fill(filter_input.begin(), filter_input.end(), 0.0f);
	auto target_value_idx = 0;
	for (auto i = 0; i < kMinibatchSize; ++i) {
		const auto& transition = replay_memory_[transitions[i]];
		const Action action = std::get<1>(transition);
		assert(static_cast<int>(action.id) < kOutputCount);
		const auto reward = std::get<2>(transition);
		assert(reward >= -1.0 && reward <= 1.0);
		const auto target = std::get<3>(transition) ?
			reward + gamma_ * actions_and_values[target_value_idx++].second :
			reward;
		assert(!std::isnan(target));
		target_input[i * kOutputCount + static_cast<int>(action.id)] = target;
		filter_input[i * kOutputCount + static_cast<int>(action.id)] = 1;
		VLOG(1) << "filter:" << action.toString() << " target:" << target;
		for (auto j = 0; j < kInputFrameCount; ++j) {
			const auto& frame_data = std::get<0>(transition)[j];
			std::copy(
				frame_data->begin(),
				frame_data->end(),
				frames_input.begin() + i * kInputDataSize +
				j * kCroppedFrameDataSize);
		}
	}
	InputDataIntoLayers(frames_input, target_input, filter_input);
	solver_->Step(1);
	// Log the first parameter of each hidden layer
	VLOG(1) << "conv1:" <<
		net_->layer_by_name("conv1_layer")->blobs().front()->data_at(1, 0, 0, 0);
	VLOG(1) << "conv2:" <<
		net_->layer_by_name("conv2_layer")->blobs().front()->data_at(1, 0, 0, 0);
	VLOG(1) << "ip1:" <<
		net_->layer_by_name("ip1_layer")->blobs().front()->data_at(1, 0, 0, 0);
	VLOG(1) << "ip2:" <<
		net_->layer_by_name("ip2_layer")->blobs().front()->data_at(1, 0, 0, 0);
}

void DQN::InputDataIntoLayers(
	const FramesLayerInputData& frames_input,
	const TargetLayerInputData& target_input,
	const FilterLayerInputData& filter_input) {
	frames_input_layer_->Reset(
		const_cast<float*>(frames_input.data()),
		dummy_input_data_.data(),
		kMinibatchSize);
	target_input_layer_->Reset(
		const_cast<float*>(target_input.data()),
		dummy_input_data_.data(),
		kMinibatchSize);
	filter_input_layer_->Reset(
		const_cast<float*>(filter_input.data()),
		dummy_input_data_.data(),
		kMinibatchSize);
}

