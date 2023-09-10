#include "Interface.h"
#include "AtlasManager.h"
#include "Utils.h"

std::string Interface::fontName = "Assets/font.ttf";
sf::Font Interface::font;
sf::RenderWindow* Interface::window;
sf::Texture Interface::uiTexture;
sf::Texture Interface::uiElementsBackground;

sf::Sprite* Interface::uiIconBackground = nullptr;

unsigned char Interface::uiTextCount = 0;

void Interface::CreateText(const std::string& tag, const std::string& stringToDraw, const v2f& position, const sf::Color& color, const float scale)
{
	sf::Text text = sf::Text(font, stringToDraw);

	text.setFillColor(color);
	text.setScale(v2f(scale, scale));

	Interface::textDeclarations.insert(std::pair(tag, text));
}

// This uses normalized coordinates like (0.0f, 0.5f)
void Interface::CreateNormalizedText(const std::string& tag, const std::string& stringToDraw, const sf::Vector2f& normalizedPosition, const sf::Color& color, const float scale, const bool adjustHorizontally)
{
	sf::Text text = sf::Text(font, stringToDraw);

	v2f absolutePosition = v2f(normalizedPosition.x * WINDOW_WIDTH, normalizedPosition.y * WINDOW_HEIGHT);

	if (adjustHorizontally)
		absolutePosition -= v2f(text.getGlobalBounds().width / 2, 0);

	text.setFillColor(color);
	text.setPosition(absolutePosition);
	text.setScale(v2f(scale, scale));

	Interface::textDeclarations.insert(std::pair(tag, text));
}

void Interface::SetTextString(const std::string& tag, const std::string& newString)
{

	if (!Interface::textDeclarations.contains(tag))
	{
		std::cerr << "Trying to set text of an inexistant text with tag '" << tag << "\'." << std::endl;
		return;
	};

	if (Interface::textDeclarations.at(tag).getString() == newString)
	{
		return;
	}

	Interface::textDeclarations.at(tag).setString(newString);
}


void Interface::Initialize(sf::RenderWindow* window)
{
	Interface::font.loadFromFile(Interface::fontName);
	Interface::uiElementsBackground.loadFromFile("Assets/ui_background.png");

	Interface::uiTexture = *AtlasManager::GetAtlas(AtlasTextureID::UI);

	Interface::uiIconBackground = new sf::Sprite(Interface::uiTexture, sf::IntRect(v2i(0, 0), v2i(UI_ICON_WIDTH, UI_ICON_HEIGHT)));

	Interface::window = window;
}

void Interface::DrawText(const std::string& tag)
{
	if (!Interface::textDeclarations.contains(tag)) std::cerr << "Trying to draw text of an inexistant text with tag '" << tag << "'.\n";
	
	sf::Text text = Interface::textDeclarations.at(tag);

	sf::Sprite uiBackgroundSprite = sf::Sprite(Interface::uiElementsBackground);

	uiBackgroundSprite.setPosition(v2f(text.getPosition().x - 4, text.getGlobalBounds().height / 2 + text.getPosition().y - 4));
	uiBackgroundSprite.setScale(v2f(text.getGlobalBounds().width + 8, text.getGlobalBounds().height + 8));

	window->draw(uiBackgroundSprite);
	window->draw(text);
}

void Interface::ShowInventoryOverlay()
{
	int itemsAmount = static_cast<int>(ItemRegistry::Items.size());

	sf::Sprite itemIconSprite = sf::Sprite(*AtlasManager::GetAtlas(AtlasTextureID::Items));
	sf::Sprite uiBackgroundSprite = sf::Sprite(Interface::uiElementsBackground);


	std::vector<int> itemsThatShouldBeShown = std::vector<int>();

	for (int itemId = 0; itemId < itemsAmount; itemId++)
	{
		if (Inventory::GetAmount((ItemID) itemId) > 0)
		{
			itemsThatShouldBeShown.push_back(itemId);
		}
	}

	int itemCounter = 0;

	for (int shownItemId : itemsThatShouldBeShown)
	{
		int itemAmount = Inventory::GetAmount((ItemID) shownItemId);
		Item* item = ItemRegistry::Items[shownItemId];

		itemIconSprite.setTextureRect(sf::IntRect(v2i(item->atlasID.x * TILE_SIZE, item->atlasID.y * TILE_SIZE), v2i(TILE_SIZE, TILE_SIZE)));
		itemIconSprite.setScale(v2f(2, 2));

		itemIconSprite.setPosition(
			v2f(
				WINDOW_WIDTH - TILE_SIZE * 2,
				itemCounter * TILE_SIZE * 2 + 4
			)
		);


		sf::Text text = sf::Text( font, std::format("{}", itemAmount));

		text.setFillColor(sf::Color(255, 255, 255));
		text.setScale(v2f(1, 1));

		text.setPosition(
			v2f(
				WINDOW_WIDTH - 2 * TILE_SIZE - text.getGlobalBounds().width - 4.f,
				itemCounter * 2 * TILE_SIZE * 1.f
			)
		);


		uiBackgroundSprite.setPosition(v2f(WINDOW_WIDTH - 2 * TILE_SIZE - text.getGlobalBounds().width - 8, itemCounter * 2 * TILE_SIZE + 1));
		
		v2f uiBackgroundSize = v2f(
			SCALED_TILE_SIZE + text.getGlobalBounds().width + 8,
			2 * TILE_SIZE - 2
		);
		uiBackgroundSprite.setScale(uiBackgroundSize);

		window->draw(uiBackgroundSprite);
		window->draw(itemIconSprite);
		window->draw(text);

		itemCounter++;

	}
}

void Interface::DrawUIElement(AtlasID atlasID, const sf::Vector2f& absolutePosition, AtlasTextureID textureAtlas)
{
	sf::Sprite uiElementSprite = sf::Sprite(*AtlasManager::GetAtlas(textureAtlas), sf::IntRect(v2i(atlasID.x * UI_ICON_WIDTH, atlasID.y * UI_ICON_HEIGHT), v2i(UI_ICON_WIDTH, UI_ICON_HEIGHT)));
	
	Interface::uiIconBackground->setPosition(absolutePosition);
	Interface::uiIconBackground->setScale(v2f(TEXTURE_SCALE, TEXTURE_SCALE));

	uiElementSprite.setPosition(absolutePosition);
	uiElementSprite.setScale(v2f(TEXTURE_SCALE, TEXTURE_SCALE));

	window->draw(*Interface::uiIconBackground);
	window->draw(uiElementSprite);
}

void Interface::DrawUIElementNormalized(AtlasID atlasID, const sf::Vector2f& normalizedPosition, const bool adjustHorizontally, AtlasTextureID textureAtlas)
{
	sf::Sprite uiElementSprite = sf::Sprite(*AtlasManager::GetAtlas(textureAtlas), sf::IntRect(v2i(atlasID.x * UI_ICON_WIDTH, atlasID.y * UI_ICON_HEIGHT), v2i(UI_ICON_WIDTH, UI_ICON_HEIGHT)));

	v2f absolutePosition = { normalizedPosition.x * WINDOW_WIDTH, normalizedPosition.y * WINDOW_HEIGHT };

	if (adjustHorizontally)
	{
		absolutePosition -= v2f((uiElementSprite.getGlobalBounds().width * TEXTURE_SCALE) / 2, 0);
	}

	Interface::uiIconBackground->setPosition(absolutePosition);
	Interface::uiIconBackground->setScale(v2f(TEXTURE_SCALE, TEXTURE_SCALE));

	uiElementSprite.setPosition(absolutePosition);
	uiElementSprite.setScale(v2f(TEXTURE_SCALE, TEXTURE_SCALE));

	window->draw(*Interface::uiIconBackground);
	window->draw(uiElementSprite);
}

void Interface::Dispose()
{
}