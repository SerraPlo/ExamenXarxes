#pragma once
#include <vector>

namespace SerraPlo {

	class IApp;		// Forward declaration of the main game app interface
	class IScreen;	// Forward declaration of the game screen interface

	// Screen list class to store the whole game screens
	class ScreenList {
	protected:
		std::vector<IScreen*> m_screens;	// Array of screens
		IApp* m_gameApp;					// Reference pointer to current game app	

		// Get the current screen object
		IScreen* GetCurScreen() const;
	public:
		int curScreenIndex;			// Index of the current running screen
		int prevScreenIndex;
		explicit ScreenList(IApp* game);
		~ScreenList();

		IScreen* SetScreen(int index);
		void AddScreen(IScreen* newScreen, int index);
		void Destroy();
	};

}