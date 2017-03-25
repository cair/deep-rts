#include <string>
#include <map>

class Algorithm;
class AIRepository
{
public:
	static AIRepository& getInstance()
	{
		static AIRepository instance; // Guaranteed to be destroyed.
		return instance;
	}
private:
	AIRepository() {}                    // Constructor? (the {} brackets) are needed here.
	std::map<std::string, Algorithm*> availableAI;
public:
	AIRepository(AIRepository const&) = delete;
	void operator=(AIRepository const&) = delete;

	void addAI(Algorithm *ai);

	Algorithm *getAI(std::string aiName);
};