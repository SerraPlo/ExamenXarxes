#pragma once
#include <string>
#include <SerraPloEngine\IScreen.h>
#include <SerraPloEngine\Text2D.h>
#pragma comment(lib, "SerraPloEngine.lib")
using namespace SerraPlo;

#define TEXT_WIDTH 700
#define TEXT_HEIGHT 100
#define NICK_MAX_SIZE 10

enum class LoginState { ASK_NICK, WAIT_SERVER, WAIT_CLIENTS };

class AppClient;
class LoginScreen : public IScreen {
public:
	explicit LoginScreen() = default;
	~LoginScreen() = default;
	void Build(void) override;
	void Destroy(void) override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	AppClient *m_app; // Casted client main game pointer reference (IApp -> AppClient)
	LoginState curState;
	float m_counterSend{ 0 };
	std::string m_inputText{ "" };
	// Text to show on screen
	Text2D m_textLogin;
	Text2D m_textNick;
	Text2D m_textLoading;
};

