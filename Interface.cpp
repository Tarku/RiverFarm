#include "Interface.h"
#include "AtlasManager.h"
#include "Utils.h"

std::string Interface::fontName = "Assets/font.ttf";
sf::Font Interface::font;
sf::RenderWindow* Interface::window;
sf::Texture Interface::uiTexture;
sf::Texture Interface::uiElementsBackground;

sf::Sprite Interface::uiIconBackground;

unsigned char Interface::uiTextCount = 0;

void Interface::CreateText(const std::string& tag, const std::string& stringToDraw, const v2f& position, const sf::Color& color, const float scale)
{
	sf::Text text = sf::Text(stringToDraw, font);

	text.setFillColor(color);
	text.setScale(scale, scale);

	Interface::textDeclarations[tag] = text;


	Interface::uiTextCount++;
}


void Interface::SetTextString(const std::string& tag, const std::string& newString)
{

	if (!Interface::textDeclarations.contains(tag)) std::cerr << "Trying to set text of an inexistant text with tag '" << tag << "'.\n";
	if (Interface::textDeclarations[tag].getString() == newString) return;

	Interface::textDeclarations[tag].setString(newString);
}

// This uses normalized coordinates like (0.0f, 0.5f)
void Interface::CreateNormalizedText(const std::string& tag, const std::string& stringToDraw, const sf::Vector2f& normalizedPosition, const sf::Color& color, const float scale, const bool adjustHorizontally)
{
	sf::Text text = sf::Text(stringToDraw, font);

	v2f absolutePosition = v2f(normalizedPosition.x * WINDOW_WIDTH, normalizedPosition.y * WINDOW_HEIGHT);

	if (adjustHorizontally)
		absolutePosition -= v2f(text.getGlobalBounds().width / 2, 0);

	text.setFillColor(color);
	text.setPosition(absolutePosition);

	text.setFillColor(color);
	text.setScale(scale, scale);

	Interface::textDeclarations[tag] = text;

	Interface::uiTextCount++;
}

void Interface::Initialize(sf::RenderWindow* window)
{
	Interface::font.loadFromFile(Interface::fontName);
	Interface::uiElementsBackground.loadFromFile("Assets/ui_background.png");

	Interface::uiTexture = *AtlasManager::GetAtlas(AtlasTextureID::UI);

	Interface::uiIconBackground = sf::Sprite(Interface::uiTexture, sf::IntRect(0, 0, UI_ICON_WIDTH, UI_ICON_HEIGHT));

	Interface::window = window;
}

void Interface::DrawText(const std::string& tag)
{
	if (!Interface::textDeclarations.contains(tag)) std::cerr << "Trying to draw text of an inexistant text with tag '" << tag << "'.\n";
	
	sf::Text text = Interface::textDeclarations[tag];

	sf::Sprite uiBackgroundSprite = sf::Sprite(Interface::uiElementsBackground);

	uiBackgroundSprite.setPosition(v2f(text.getPosition().x - 4, text.getGlobalBounds().height / 2 + text.getPosition().y - 4));
	uiBackgroundSprite.setScale(v2f(text.getGlobalBounds().width + 8, text.getGlobalBounds().height + 8));

	window->draw(uiBackgroundSprite);
	window->draw(text);
}

void Interface::ShowInventoryOverlay()
{
	int itemsAmount = ItemRegistry::Items.size();

	sf::Sprite itemIconSprite = sf::Sprite(*AtlasManager::GetAtlas(AtlasTextureID::Items), sf::IntRect(0, 0, 16, 16));
	sf::Sprite uiBackgroundSprite = sf::Sprite(Interface::uiElementsBackground);

	for (int i = 0; i < itemsAmount; i++)
	{

		Item* item = ItemRegistry::Items[i];

		itemIconSprite.setTextureRect(sf::IntRect(item->atlasID.x * TILE_SIZE, item->atlasID.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));
		itemIconSprite.setScale(2, 2);

		itemIconSprite.setPosition(
			v2f(
				WINDOW_WIDTH - TILE_SIZE * 2,
				i * TILE_SIZE * 2 + 4
			)
		);


		sf::Text text = sf::Text(std::format("{}", Inventory::GetAmount((ItemID) i)), font);

		text.setFillColor(sf::Color(255, 255, 255));
		text.setScale(1, 1);

		text.setPosition(
			v2f(
				WINDOW_WIDTH - SCALED_TILE_SIZE - text.getGlobalBounds().width - 4,
				i * SCALED_TILE_SIZE
			)
		);


		uiBackgroundSprite.setPosition(v2f(WINDOW_WIDTH - SCALED_TILE_SIZE - text.getGlobalBounds().width - 8, i * SCALED_TILE_SIZE + 1));
		uiBackgroundSprite.setScale(SCALED_TILE_SIZE + text.getGlobalBounds().width + 8, SCALED_TILE_SIZE - 2);

		window->draw(uiBackgroundSprite);
		window->draw(itemIconSprite);
		window->draw(text);
	}
}

void Interface::DrawUIElement(AtlasID atlasID, const sf::Vector2f& absolutePosition)
{
	if (atlasID.x == 0 && atlasID.y == 0) // These coordinates are reserved
	{
		return;
	}

	sf::Sprite* s = new sf::Sprite(Interface::uiTexture, sf::IntRect(atlasID.x * UI_ICON_WIDTH, atlasID.y * UI_ICON_HEIGHT, UI_ICON_WIDTH, UI_ICON_HEIGHT));

	Interface::uiIconBackground.setPosition(absolutePosition);
	Interface::uiIconBackground.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	s->setPosition(absolutePosition);
	s->setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	window->draw(Interface::uiIconBackground);
	window->draw(*s);

	delete s;
}

void Interface::DrawUIElementNormalized(AtlasID atlasID, const sf::Vector2f& normalizedPosition, const bool adjustHorizontally)
{
	if (atlasID.x == 0 && atlasID.y == 0) // These coordinates are reserved, don't use them
	{
		Utils::Log("hehehe");
		return;
	}


	sf::Sprite* s = new sf::Sprite(Interface::uiTexture, sf::IntRect(atlasID.x * UI_ICON_WIDTH, atlasID.y * UI_ICON_HEIGHT, UI_ICON_WIDTH, UI_ICON_HEIGHT));

	v2f absolutePosition = { normalizedPosition.x * WINDOW_WIDTH, normalizedPosition.y * WINDOW_HEIGHT };

	if (adjustHorizontally)
	{
		absolutePosition -= v2f((s->getGlobalBounds().width * TEXTURE_SCALE) / 2, 0);
	}

	Interface::uiIconBackground.setPosition(absolutePosition);
	Interface::uiIconBackground.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	s->setPosition(absolutePosition);
	s->setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	window->draw(Interface::uiIconBackground);
	window->draw(*s);

	delete s;
}

void Interface::Dispose()
{
}