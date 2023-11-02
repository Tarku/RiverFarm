#include "OptionsScene.h"
#include "MainMenuScene.h"
#include "SceneManager.h"
#include "../Shared.h"

using namespace sf;

void OptionsScene::Initialize(sf::RenderWindow* window)
{
	p_window = window;
	p_interface = Interface();

	p_interface.CreateNormalizedText(std::string("options_text"), "Options", v2f(0.5f, 0), v2f(0.5f, 0.f), Color::Yellow, 2.f);

	for (auto element : m_optionsSelectionElements)
	{
		element.Initialize(&p_interface);
	}
}

void OptionsScene::HandleEvents()
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
			case sf::Keyboard::Escape:
				SceneManager::ChangeScene(new MainMenuScene());
				break;
			}
		}

		if (OptionsManager::IsForwardsActive())
		{
			m_currentOptionsElementID--;
		} 

		if (OptionsManager::IsBackwardsActive())
		{
			m_currentOptionsElementID++;
		}

		if (m_currentOptionsElementID < 0)
		{
			m_currentOptionsElementID = (int) m_optionsSelectionElements.size() - 1;
		}

		if (m_currentOptionsElementID >= (int)m_optionsSelectionElements.size())
		{
			m_currentOptionsElementID = 0;
		}
		m_optionsSelectionElements.at(m_currentOptionsElementID).HandleEvents(&p_event);
	}


}

void OptionsScene::Update(float dt)
{
	HandleEvents();
	for (auto& element : m_optionsSelectionElements)
	{
		element.Update(dt);
	}
}

void OptionsScene::Draw()
{
	p_interface.DrawText(std::string("options_text"));

	for (auto& element : m_optionsSelectionElements)
	{
		element.Draw();
	}
}

void OptionsScene::Dispose()
{
	OptionsSelectionElement& element = m_optionsSelectionElements.at(m_currentOptionsElementID);
	
	element.selectionsCallbacks.at(element.selectionIndex)();

	p_interface.Dispose();
}
