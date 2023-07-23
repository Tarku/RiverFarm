#include "Interface.h"
#include "AtlasManager.h"
#include "Utils.h"

std::string Interface::fontName = "Assets/font.ttf";
sf::Font Interface::font;
sf::RenderWindow* Interface::window;
sf::Texture Interface::uiTexture;
sf::Sprite Interface::uiIconBackground;
sf::Text Interface::textDeclarations[];
unsigned char Interface::uiTextCount = 0;

void Interface::CreateText(const std::string& stringToDraw, const v2f& position, const sf::Color& color, const float scale)
{
	sf::Text text = sf::Text(stringToDraw, font);

	text.setFillColor(color);
	text.setScale(scale, scale);

	Interface::textDeclarations[Interface::uiTextCount] = text;


	Interface::uiTextCount++;
}


void Interface::SetTextString(uchar textID, const std::string& newString)
{

	if (textID >= Interface::uiTextCount || textID < 0) return;
	if (Interface::textDeclarations[textID].getString() == newString) return;

	Interface::textDeclarations[textID].setString(newString);
}

// This uses normalized coordinates like (0.0f, 0.5f)
void Interface::CreateNormalizedText(const std::string& stringToDraw, const sf::Vector2f& normalizedPosition, const sf::Color& color, const float scale, const bool adjustHorizontally)
{
	sf::Text text = sf::Text(stringToDraw, font);

	v2f absolutePosition = v2f(normalizedPosition.x * WINDOW_WIDTH, normalizedPosition.y * WINDOW_HEIGHT);

	if (adjustHorizontally)
		absolutePosition -= v2f(text.getGlobalBounds().width / 2, 0);

	text.setFillColor(color);
	text.setPosition(absolutePosition);

	text.setFillColor(color);
	text.setScale(scale, scale);

	Interface::textDeclarations[Interface::uiTextCount] = text;

	Interface::uiTextCount++;
}

void Interface::Initialize(sf::RenderWindow* window)
{
	Interface::font.loadFromFile(Interface::fontName);
	Interface::uiTexture = *AtlasManager::GetAtlas(AtlasTextureID::UI);

	Interface::uiIconBackground = sf::Sprite(Interface::uiTexture, sf::IntRect(0, 0, UI_ICON_WIDTH, UI_ICON_HEIGHT));

	Interface::window = window;
}

void Interface::DrawText(uchar textID)
{
	if (textID >= Interface::uiTextCount || textID < 0) return;
	window->draw(Interface::textDeclarations[textID]);
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