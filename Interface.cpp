#include "Interface.h"

std::string Interface::fontName = "font.ttf";
sf::Font Interface::font;
sf::RenderWindow* Interface::window;
sf::Texture Interface::uiTexture;
sf::Sprite Interface::uiIconBackground;

void Interface::Initialize(sf::RenderWindow* window)
{
	std::cout << &Interface::uiTexture << std::endl;

	Interface::font.loadFromFile(Interface::fontName);
	Interface::uiTexture.loadFromFile("ui.png");

	Interface::uiIconBackground = sf::Sprite(Interface::uiTexture, sf::IntRect(0, 0, UI_ICON_WIDTH, UI_ICON_HEIGHT));

	Interface::window = window;
}

void Interface::DrawText(const std::string& text, const sf::Vector2f& absolutePosition, const sf::Color& color, const float scale)
{
	sf::Text t = sf::Text(text, font);

	t.setScale(scale, scale);

	t.setFillColor(color);
	t.setPosition(absolutePosition);
	
	window->draw(t);
}
void Interface::DrawTextNormalized(const std::string& text, const sf::Vector2f& normalizedPosition, const sf::Color& color, const float scale, const bool adjustHorizontally)
{
	sf::Text t = sf::Text(text, font);

	t.setScale(scale, scale);

	v2f absolutePosition = v2f(normalizedPosition.x * WINDOW_WIDTH, normalizedPosition.y * WINDOW_HEIGHT);

	if (adjustHorizontally)
		absolutePosition -= v2f(t.getGlobalBounds().width / 2, 0);

	t.setFillColor(color);
	t.setPosition(absolutePosition);

	window->draw(t);
}

void Interface::DrawUIElement(AtlasID atlasID, const sf::Vector2f& absolutePosition)
{
	if (atlasID == AtlasID{0, 0}) // These coordinates are reserved
		return;


	sf::Sprite s = sf::Sprite(Interface::uiTexture, sf::IntRect(atlasID.x * UI_ICON_WIDTH, atlasID.y * UI_ICON_HEIGHT, UI_ICON_WIDTH, UI_ICON_HEIGHT));

	Interface::uiIconBackground.setPosition(absolutePosition);
	Interface::uiIconBackground.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	s.setPosition(absolutePosition);
	s.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	window->draw(Interface::uiIconBackground);
	window->draw(s);
}

void Interface::DrawUIElementNormalized(AtlasID atlasID, const sf::Vector2f& normalizedPosition, const bool adjustHorizontally)
{
	if (atlasID == AtlasID{ 0, 0 }) // These coordinates are reserved
		return;


	sf::Sprite s = sf::Sprite(Interface::uiTexture, sf::IntRect(atlasID.x * UI_ICON_WIDTH, atlasID.y * UI_ICON_HEIGHT, UI_ICON_WIDTH, UI_ICON_HEIGHT));

	v2f absolutePosition = { normalizedPosition.x * WINDOW_WIDTH, normalizedPosition.y * WINDOW_HEIGHT };

	if (adjustHorizontally)
	{
		absolutePosition -= v2f((s.getGlobalBounds().width * TEXTURE_SCALE) / 2, 0);
	}

	Interface::uiIconBackground.setPosition(absolutePosition);
	Interface::uiIconBackground.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	s.setPosition(absolutePosition);
	s.setScale(TEXTURE_SCALE, TEXTURE_SCALE);

	window->draw(Interface::uiIconBackground);
	window->draw(s);
}
void Interface::Dispose()
{
}