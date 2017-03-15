#include "../serialization/ScoreItem_generated.h"
#include "../serialization/ScoreLog_generated.h"
#include "../serialization/ScorePlayer_generated.h"
#include "../player/Player.h"
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/util.h>
#include <vector>
class ScoreLogSerializer {
	std::vector<flatbuffers::Offset<Score::ScoreItem>> elements; // Elements per iteration

	std::vector<std::vector<flatbuffers::Offset<Score::ScoreItem>>> itemsPerPlayer;
	uint8_t nPlayers;
	flatbuffers::FlatBufferBuilder builder;
public:
	ScoreLogSerializer(uint8_t nPlayers): nPlayers(nPlayers), builder(1024) {
		elements.reserve(4);
		itemsPerPlayer.resize(4);
		
	}

	void serialize(int gameID, std::string name) {

		std::vector<flatbuffers::Offset<Score::ScorePlayer>> flatPlayers;

		for (int i = 0; i < nPlayers; i++) {
			auto scorePlayer = Score::CreateScorePlayer(builder, i, builder.CreateVector(itemsPerPlayer[i]));
			builder.Finish(scorePlayer);
			flatPlayers.push_back(scorePlayer);
		}

		auto gameLog = Score::CreateScoreLog(builder, gameID, builder.CreateVector(flatPlayers));
		builder.Finish(gameLog);

		flatbuffers::SaveFile(name.c_str(), (char*)builder.GetBufferPointer(), builder.GetSize(), true);



	}

	void nextTick(int tick) {

		for (int i = 0; i < nPlayers; i++) {
			itemsPerPlayer[i].push_back(elements[i]);
		}
		elements.clear();

	}

	void addElement(uint8_t playerID, Player &player) {

		auto t = std::vector<uint32_t>(std::begin(player.actionStatistics), std::end(player.actionStatistics));
		flatbuffers::Offset<Score::ScoreItem> scoreItem = Score::CreateScoreItem(builder, player.apm, player.getScore(), builder.CreateVector(t));
		builder.Finish(scoreItem);

		elements.push_back(scoreItem);

	}
	
	void reset() {
		itemsPerPlayer.clear();
		itemsPerPlayer.resize(4);
		elements.clear();
		builder.Clear();
	}


};
