#include <string>


class Action {
public:
	int mouseButton; // 0 = LEFT, 1 = Right
	int tileID;
	int id;

	std::string Action::toString() const {
		return "PLAYER_A_" + std::to_string(mouseButton) + "_" + std::to_string(tileID);
	}


};