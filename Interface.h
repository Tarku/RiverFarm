#pragma once

#include "Shared.h"

#define UI_ICON_WIDTH (int) 32
#define UI_ICON_HEIGHT (int) 32

class Interface
{
private:
	static std::string fontName;
	static sf::Font font;
	static sf::RenderWindow* window;
	static sf::Texture uiTexture;
	static sf::Sprite uiIconBackground;

	static sf::Text textDeclarations[32];

	static unsigned char uiTextCount;
public:
	enum InterfaceState
	{
		MainGame
	};

	static void Initialize(sf::RenderWindow* window);


	static void CreateText(const std::string& text, const v2f& position, const sf::Color& color = sf::Color::White, const float scale = 1.f);
	static void CreateNormalizedText(const std::string& text, const sf::Vector2f& normalizedPosition, const sf::Color& color = sf::Color::White, const float scale = 1.f, const bool adjustHorizontally = true);

	static void SetTextString(uchar textID, const std::string& newString);

	static void DrawText(unsigned char textID);
	static void DrawTextNormalized();
	
	static void DrawUIElement(AtlasID atlasId, const sf::Vector2f& absolutePosition);
	static void DrawUIElementNormalized(AtlasID atlasId, const sf::Vector2f& normalizedPosition, const bool adjustHorizontally = true);

	static void Dispose();
};

