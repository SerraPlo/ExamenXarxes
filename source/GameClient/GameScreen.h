#pragma once
#include <SerraPloEngine\IScreen.h>
#include <SerraPloEngine\Sprite2D.h>
#include <SerraPloEngine/Shapes2D.h>
#pragma comment(lib, "SerraPloEngine.lib")
using namespace SerraPlo;

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
	AppClient *m_app; // Casted client main game pointer reference (IApp -> AppClient)
	Sprite2D m_image;
	Rectangle m_rectangle;
};

