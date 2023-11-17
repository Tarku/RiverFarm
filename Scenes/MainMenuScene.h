#include "Scene.h"
#include "../Shared.h"
#include <functional>

class MainMenuScene : public IScene
{
private:
	sf::Texture titleBanner;

	struct MainMenuSelectionElement
	{
		static sf::RenderWindow* window;

		string label;
		voidfunc callback;

		static inline void Initialize(sf::RenderWindow* window)
		{
			MainMenuSelectionElement::window = window;
		}

		static void cb_StartGame();
		static void cb_QuitGame();
		static void cb_Options();

		inline MainMenuSelectionElement(const std::string& label, std::function<void()> callbackFunction)
		{
			this->label = label;
			this->callback = callbackFunction;
		}
	};


	std::vector<MainMenuSelectionElement> m_mainMenuElements = std::vector<MainMenuSelectionElement>
	{
		MainMenuSelectionElement(std::string("Start game"), MainMenuSelectionElement::cb_StartGame),
		MainMenuSelectionElement(std::string("Options"), MainMenuSelectionElement::cb_Options),
		MainMenuSelectionElement(std::string("Quit game"), MainMenuSelectionElement::cb_QuitGame)
	};
	int m_currentMMenuElementID = 0;


public:
	inline MainMenuScene() {};

	void Initialize(sf::RenderWindow* window) override;

	void HandleEvents();

	void Update(float dt) override;
	void Draw() override;

	void Dispose() override;
};