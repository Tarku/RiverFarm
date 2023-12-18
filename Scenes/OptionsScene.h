#include "Scene.h"
#include "SceneManager.h"
#include "../OptionsManager.h"
#include "../Shared.h"
#include "../Includes/SFML/Window/Mouse.hpp"

class OptionsScene : public IScene
{
private:
	struct OptionsSelectionElement
	{
		inline static int counter = 0;
		inline static Interface* interface = nullptr;

		inline static void cb_SetAZERTYLayout()
		{
			OptionsManager::currentInputType = OptionsManager::InputType::KeyboardAZERTY;
		}
		inline static void cb_SetQWERTYLayout()
		{
			OptionsManager::currentInputType = OptionsManager::InputType::KeyboardQWERTY;

		}
		inline static void cb_SetControllerInput()
		{
			OptionsManager::currentInputType = OptionsManager::InputType::Controller;

		}

		inline static void cb_DoNothing()
		{

		}

		int elementId = 0;
		int selectionIndex = 0;

		std::string label{};

		std::string labelTextTag{};
		std::string selectionTextTag{};

		std::vector<std::string> selectionsNames{};
		int selectionsNamesSize = 0;

		std::vector<std::function<void()>> selectionsCallbacks{};
		int selectionsCallbacksSize = 0;



		inline OptionsSelectionElement(const std::string& label, const std::vector<std::string>& selectionsNames, const std::vector<std::function<void()>> selectionsCallbacks)
		{
			this->label = label;

			this->selectionsNames = selectionsNames;
			selectionsNamesSize = static_cast<int>(this->selectionsNames.size());

			this->selectionsCallbacks = selectionsCallbacks;
			selectionsCallbacksSize = static_cast<int>(this->selectionsCallbacks.size());

			elementId = OptionsSelectionElement::counter;

			labelTextTag = std::format("options_selection_element_label{}", counter);
			selectionTextTag = std::format("options_selection_element_selection{}", counter);

			OptionsSelectionElement::counter++;
		}

		inline void Initialize(Interface* _interface)
		{
			OptionsSelectionElement::interface = _interface;


			float fontScreenRatio = 30.f / WINDOW_HEIGHT;

			OptionsSelectionElement::interface->CreateNormalizedText(labelTextTag, label, v2f(0.1f, 0.5f + elementId * fontScreenRatio), v2f(0, 0.5f));
			OptionsSelectionElement::interface->CreateNormalizedText(selectionTextTag, selectionsNames.at(0), v2f(0.9f, 0.5f + elementId * fontScreenRatio), v2f(1.f, 0.5f));
		}

		inline void HandleEvents(sf::Event* event)
		{
			if (event->type == sf::Event::KeyPressed)
			{
				switch (event->key.code)
				{
				case sf::Keyboard::Left:
					selectionIndex--;
					break;
				case sf::Keyboard::Right:
					selectionIndex++;
					break;
				default:
					break;
				}

				if (selectionIndex < 0)
					selectionIndex = selectionsNamesSize - 1;

				if (selectionIndex >= selectionsNamesSize)
					selectionIndex = 0;
			}
		}

		inline void Update(float dt)
		{
			v2i viMousePosition = sf::Mouse::getPosition(*OptionsSelectionElement::interface->window);
			v2f vfMousePosition = v2f((float) viMousePosition.x, (float) viMousePosition.y);

			sf::Text* text = OptionsSelectionElement::interface->GetText(labelTextTag).text;

			bool isSelected = false;

			if (text->getGlobalBounds().contains(vfMousePosition))
				elementId = counter;

			if (counter == elementId)
				isSelected = true;

			sf::Color fillColor = isSelected ? sf::Color::Yellow : sf::Color::White;
			float textScale = isSelected ? 1.1f : 1.0f;

			text->setFillColor(fillColor);
			text->setScale(v2f(textScale, textScale));

			OptionsSelectionElement::interface->SetTextString(selectionTextTag, selectionsNames.at(selectionIndex));

			counter++;
		}

		inline void Draw()
		{
			OptionsSelectionElement::interface->DrawText(labelTextTag);
			OptionsSelectionElement::interface->DrawText(selectionTextTag);
		}
	};

	std::vector<OptionsSelectionElement> m_optionsSelectionElements = std::vector<OptionsSelectionElement>
	{
		OptionsSelectionElement(
			string("Keyboard Input type"),
			strings {
				string("AZERTY layout"),
				string("QWERTY layout "),
				string("Controller")
			},
			voidfuncs
			{
				OptionsSelectionElement::cb_SetAZERTYLayout,
				OptionsSelectionElement::cb_SetQWERTYLayout,
					OptionsSelectionElement::cb_SetControllerInput
			}
		),

		OptionsSelectionElement(
			string("Test option 1"),
			strings {
				string("Option 1"),
				string("Option 2"),
				string("Option 3"),
				string("Option 4")
			},
			voidfuncs {
				OptionsSelectionElement::cb_DoNothing,
				OptionsSelectionElement::cb_DoNothing,
				OptionsSelectionElement::cb_DoNothing,
				OptionsSelectionElement::cb_DoNothing
			}
		),

		OptionsSelectionElement(
			string("Test option 2"),
			strings {
				string("Option 1"),
				string("Option 2"),
				string("Option 3"),
				string("Option 4")
			},
			voidfuncs {
				OptionsSelectionElement::cb_DoNothing,
				OptionsSelectionElement::cb_DoNothing,
				OptionsSelectionElement::cb_DoNothing,
				OptionsSelectionElement::cb_DoNothing
			}
		)
	};

	int m_currentOptionsElementID = 0;

public:
	inline OptionsScene() {};

	void Initialize(sf::RenderWindow* window) override;

	void HandleEvents();

	void Update(float dt) override;
	void Draw() override;

	void Dispose() override;

};