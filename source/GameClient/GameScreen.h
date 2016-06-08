#pragma once
#include <SerraPloEngine\IScreen.h>
#include <SerraPloEngine\Sprite2D.h>
#include <SerraPloEngine/Shapes2D.h>
#include <SerraPloEngine/Text2D.h>
#include <array>
#include <map>
#pragma comment(lib, "SerraPloEngine.lib")
using namespace SerraPlo;

#define MAX_TEXTURES 3
enum class GameState { INIT, PLAY };

class AppClient;
class GameScreen : public IScreen {
public:
	explicit GameScreen() = default;
	~GameScreen() = default;

	void Build(void) override;
	void Destroy(void) override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	bool CheckColisions();
	void UpdateInit();
	void DrawInit();
	void UpdatePlay();
	void DrawPlay();

	AppClient *m_app; // Casted client main game pointer reference (IApp -> AppClient)
	GameState curState;
	float m_counterUpdate;
	float m_aliveCounter;

	std::array<std::string, MAX_TEXTURES> textures;
	Rectangle m_rectangle; // test
	struct Agent {
		Text2D nick;
		Sprite2D sprite;
		glm::ivec2 lastPosition;
		glm::ivec2 targetPosition;
		float lerpCounter = 0.0f;
	}; std::map<uint64_t, Agent> m_agents;
	Agent *m_player = nullptr;
};

