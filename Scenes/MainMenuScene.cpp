#include "SceneDeclarations.h"
#include "SceneManager.h"

using namespace sf;

void MainMenuScene::Initialize(sf::RenderWindow* window)
{
	p_window = window;
	p_interface = Interface();
	MainMenuSelectionElement::Initialize(window);

	int counter = 0;
	float fontScreenRatio = 30.f / WINDOW_HEIGHT;

	for (auto& element : m_mainMenuElements)
	{
		p_interface.CreateNormalizedText(std::format("main_menu_selection_element_{}", counter), element.label, v2f(0.5f, 0.5f + counter * fontScreenRatio), v2f(0.5f, 0.5f));
			counter++;
	}


	p_interface.CreateNormalizedText(std::string("main_menu_text"), "River Farm", v2f(0.5f, 0), v2f(0.5f, 0.f));

}

RenderWindow* MainMenuScene::MainMenuSelectionElement::window = nullptr;

void MainMenuScene::MainMenuSelectionElement::cb_StartGame()
{
	SceneManager::ChangeScene(SceneManager::gameScene);

}


void MainMenuScene::MainMenuSelectionElement::cb_QuitGame()
{
	window->close();
}


void MainMenuScene::MainMenuSelectionElement::cb_Options()
{

}


void MainMenuScene::HandleEvents()
{
	while (p_window->pollEvent(p_event))
	{
		if (p_event.type == Event::Closed)
		{
			p_window->close();
		}

		if (p_event.type == Event::KeyPressed)
		{
			switch (p_event.key.code)
			{
			case Keyboard::Escape:
				p_window->close();
				break;

			case Keyboard::Enter:
				m_mainMenuElements.at(m_currentMMenuElementID).callback();
				break;
			case Keyboard::Up:
				m_currentMMenuElementID--;
				break;
			case Keyboard::Down:
				m_currentMMenuElementID++;
				break;
			}

			char menuElementsCount = m_mainMenuElements.size();

			if (m_currentMMenuElementID < 0)
			{
				m_currentMMenuElementID = menuElementsCount - 1;
			}

			if (m_currentMMenuElementID > menuElementsCount - 1)
			{
				m_currentMMenuElementID = 0;
			}
		}
	}
}

void MainMenuScene::Update(float dt)
{
	HandleEvents();

	int counter = 0;

	for (auto& element : m_mainMenuElements)
	{
		bool isSelected = (counter == m_currentMMenuElementID);

		std::string tag = std::format("main_menu_selection_element_{}", counter);

		Color fillColor = isSelected ? Color::Yellow : Color::White;
		float textScale = isSelected ? 1.2f : 1.f;

		Text& text = p_interface.GetText(tag);
		
		text.setFillColor(fillColor);
		text.setScale(v2f(textScale, textScale));

		counter++;
	}
}

void MainMenuScene::Draw() 
{
	sf::Sprite tileBackgroundSprite = sf::Sprite(*AtlasManager::GetAtlas(AtlasTextureID::Tiles));
	
	p_interface.DrawText(std::string("main_menu_text"));

	int counter = 0;

	for (auto& element : m_mainMenuElements)
	{
		p_interface.DrawText(std::format("main_menu_selection_element_{}", counter));
		counter++;
	}

}
void MainMenuScene::Dispose()
{
	p_interface.Dispose();
}