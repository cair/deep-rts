#include <memory>
class zmqAI {
	int gameID;
	int playerID;

	void start();
public:
	zmqAI();
	zmqAI(int gameID, int playerID);
	static std::shared_ptr<zmqAI> zmqAI::createInstance(int gameID, int playerID);
};