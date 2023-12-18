#include "MainMenuScene.h"
#include "GameScene.h"
#include "OptionsScene.h"
#include "SceneManager.h"
#include "../SoundManager.h"

using namespace sf;

void MainMenuScene::Initialize(sf::RenderWindow* window)
{
	p_window = window;
	p_interface = Interface();
	MainMenuSelectionElement::Initialize(window);

	

	bool couldLoadTitleBanner = titleBanner.loadFromFile(TEXTURES_PATH + "title_image.png");

	if (!couldLoadTitleBanner)
	{
		Utils::Log(std::string("Couldn't load title banner!"));
	}

	int counter = 0;
	float fontScreenRatio = 30.f / WINDOW_HEIGHT;

	for (auto& element : m_mainMenuElements)
	{
		p_interface.CreateNormalizedText(std::format("main_menu_selection_element_{}", counter), element.label, v2f(0.5f, 0.5f + counter * fontScreenRatio * 2), v2f(0.5f, 0.5f));
			counter++;
	}

	p_interface.CreateNormalizedImage(std::string("title_image"), titleBanner, v2f(0.5f, 0.05f), v2f(0.5, 0.f), true);

	//p_interface.CreateNormalizedText(std::string("main_menu_text"), "River Farm", v2f(0.5f, 0), v2f(0.5f, 0.f), Color::Yellow, 2.f);

}

RenderWindow* MainMenuScene::MainMenuSelectionElement::window = nullptr;

void MainMenuScene::MainMenuSelectionElement::cb_StartGame()
{
	SceneManager::ChangeScene(new GameScene());

}


void MainMenuScene::MainMenuSelectionElement::cb_QuitGame()
{
	window->close();
}


void MainMenuScene::MainMenuSelectionElement::cb_Options()
{

	SceneManager::ChangeScene(new OptionsScene());
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

			case Keyboard::Up:
				m_currentMMenuElementID--;
				break;
			case Keyboard::Down:
				m_currentMMenuElementID++;
				break;

			case Keyboard::Num8:
				SoundManager::PlaySound("chop");
				break;
			}

		}
	}

	if (OptionsManager::IsEnterActive())
	{
		m_mainMenuElements.at(m_currentMMenuElementID).callback();
	}

	int menuElementsCount = (int) m_mainMenuElements.size();

	if (m_currentMMenuElementID < 0)
	{
		m_currentMMenuElementID = menuElementsCount - 1;
	}

	if (m_currentMMenuElementID > menuElementsCount - 1)
	{
		m_currentMMenuElementID = 0;
	}
}

void MainMenuScene::Update(float dt)
{
	HandleEvents();

	v2i viMousePosition = Mouse::getPosition(*p_window);
	v2f vfMousePosition = v2f((float) viMousePosition.x, (float) viMousePosition.y);

	int counter = 0;

	p_interface.Update(dt);

	for (auto& element : m_mainMenuElements)
	{

		std::string tag = std::format("main_menu_selection_element_{}", counter);

		Text* text = p_interface.GetText(tag).text;

		bool isSelected = false;
		
		if (text->getGlobalBounds().contains(vfMousePosition))
			m_currentMMenuElementID = counter;

		if (counter == m_currentMMenuElementID)
			isSelected = true;

		Color fillColor = isSelected ? Color::Yellow : Color::White;
		float textScale = isSelected ? 1.1f : 1.0f;
		
		text->setFillColor(fillColor);
		text->setScale(v2f(textScale, textScale));

		counter++;
	}
}

void MainMenuScene::Draw() 
{
	sf::Sprite tileBackgroundSprite = sf::Sprite(*AtlasManager::GetAtlas(AtlasTextureID::Tiles));
	

	int counter = (int) m_mainMenuElements.size();

	for (int i = counter - 1; i >= 0; i--)
	{
		p_interface.DrawText(std::format("main_menu_selection_element_{}", i));
	}

	p_interface.DrawImageElement(std::string("title_image"));

}
void MainMenuScene::Dispose()
{
	p_interface.Dispose();
}