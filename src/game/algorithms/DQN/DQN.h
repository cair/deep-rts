#ifndef WARC2SIM_DQN_H
#define WARC2SIM_DQN_H


#include "../base/Algorithm.h"
#include <memory>
#include <random>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <caffe/caffe.hpp>
#include <caffe/layers/memory_data_layer.hpp>
#include <boost/functional/hash.hpp>
#include <boost/optional.hpp>
#include "../../lib/ActionVector.h"
#include <SFML/Graphics/RenderWindow.hpp>


DEFINE_bool(gpu, false, "Use GPU to brew Caffe");
DEFINE_bool(gui, false, "Open a GUI window");
DEFINE_string(rom, "breakout.bin", "Atari 2600 ROM to play");
DEFINE_string(solver, "dqn_solver.prototxt", "Solver parameter file (*.prototxt)");
DEFINE_int32(memory, 500000, "Capacity of replay memory");
DEFINE_int32(explore, 1000000, "Number of iterations needed for epsilon to reach 0.1");
DEFINE_double(gamma, 0.95, "Discount factor of future rewards (0,1]");
DEFINE_int32(memory_threshold, 100, "Enough amount of transitions to start learning");
DEFINE_int32(skip_frame, 3, "Number of frames skipped");
DEFINE_bool(show_frame, false, "Show the current frame in CUI");
DEFINE_string(model, "", "Model file to load");
DEFINE_bool(evaluate, false, "Evaluation mode: only playing a game, no updates");
DEFINE_double(evaluate_with_epsilon, 0.05, "Epsilon value to be used in evaluation mode");
DEFINE_double(repeat_games, 1, "Number of games played in evaluation mode");


constexpr auto kRawFrameHeight = 210;
constexpr auto kRawFrameWidth = 160;
constexpr auto kCroppedFrameSize = 84;
constexpr auto kCroppedFrameDataSize = kCroppedFrameSize * kCroppedFrameSize;
constexpr auto kInputFrameCount = 4;
constexpr auto kInputDataSize = kCroppedFrameDataSize * kInputFrameCount;
constexpr auto kMinibatchSize = 32;
constexpr auto kMinibatchDataSize = kInputDataSize * kMinibatchSize;
constexpr auto kGamma = 0.95f;
constexpr auto kOutputCount = 18;

using FrameData = std::array<uint8_t, kCroppedFrameDataSize>;
using FrameDataSp = std::shared_ptr<FrameData>;
using InputFrames = std::array<FrameDataSp, 4>;
using Transition = std::tuple<InputFrames, Action, float, boost::optional<FrameDataSp>>;

using FramesLayerInputData = std::array<float, kMinibatchDataSize>;
using TargetLayerInputData = std::array<float, kMinibatchSize * kOutputCount>;
using FilterLayerInputData = std::array<float, kMinibatchSize * kOutputCount>;


class ActionVector;
class DQN: public Algorithm {
	public:
		DQN(Player& player,
			const ActionVector& legal_actions,
			const std::string& solver_param,
			const int replay_memory_capacity,
			const double gamma) :
			Algorithm(player),
			legal_actions_(legal_actions),
			solver_param_(solver_param),
			replay_memory_capacity_(replay_memory_capacity),
			gamma_(gamma),
			current_iter_(0),
			random_engine(0) {}

		/**
		* Initialize DQN. Must be called before calling any other method.
		*/
		void Initialize();

		/**
		* Load a trained model from a file.
		*/
		void LoadTrainedModel(const std::string& model_file);

		/**
		* Select an action by epsilon-greedy.
		*/
		Action SelectAction(const InputFrames& input_frames, double epsilon);

		/**
		* Add a transition to replay memory
		*/
		void AddTransition(const Transition& transition);

		/**
		* Update DQN using one minibatch
		*/
		void Update();

		int memory_size() const { return replay_memory_.size(); }
		int current_iteration() const { return current_iter_; }

	private:
		using SolverSp = std::shared_ptr<caffe::Solver<float>>;
		using NetSp = boost::shared_ptr<caffe::Net<float>>;
		using BlobSp = boost::shared_ptr<caffe::Blob<float>>;
		using MemoryDataLayerSp = boost::shared_ptr<caffe::MemoryDataLayer<float>>;

		std::pair<Action, float> SelectActionGreedily(const InputFrames& last_frames);
		std::vector<std::pair<Action, float>> SelectActionGreedily(
			const std::vector<InputFrames>& last_frames);
		void InputDataIntoLayers(
			const FramesLayerInputData& frames_data,
			const TargetLayerInputData& target_data,
			const FilterLayerInputData& filter_data);

		const ActionVector legal_actions_;
		const std::string solver_param_;
		const int replay_memory_capacity_;
		const double gamma_;
		int current_iter_;
		std::deque<Transition> replay_memory_;
		SolverSp solver_;
		NetSp net_;
		BlobSp q_values_blob_;
		MemoryDataLayerSp frames_input_layer_;
		MemoryDataLayerSp target_input_layer_;
		MemoryDataLayerSp filter_input_layer_;
		TargetLayerInputData dummy_input_data_;
		std::mt19937 random_engine;

		/**
		* Preprocess an ALE screen (downsampling & grayscaling)
		*/
		FrameDataSp PreprocessScreen(const sf::RenderWindow& raw_screen);

		/**
		* Draw a frame as a string
		*/
		std::string DrawFrame(const FrameData& frame);

	};




#endif //WARC2SIM_DQN_H