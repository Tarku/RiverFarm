#pragma once

#include "Shared.h"
#include "Inventory/Inventory.h"

class Interface
{
private:
	static sf::RenderWindow* window;
	static sf::Texture uiTexture;
	static sf::Sprite uiIconBackground;

	static sf::Text textDeclarations[32];

	static uchar uiTextCount;
public:
	static std::string fontName;
	static sf::Font font;
	enum InterfaceState
	{
		MainGame
	};

	static void Initialize(sf::RenderWindow* window);


	static void CreateText(const std::string& text, const v2f& position, const sf::Color& color = sf::Color::White, const float scale = 1.f);
	static void CreateNormalizedText(const std::string& stringToDraw, const sf::Vector2f& normalizedPosition, const sf::Color& color = sf::Color::White, const float scale = 1.f, const bool adjustHorizontally = true);

	static void SetTextString(uchar textID, const std::string& newString);

	static void DrawText(uchar textID);

	static void ShowInventoryOverlay();
	
	static void DrawUIElement(AtlasID atlasId, const sf::Vector2f& absolutePosition);
	static void DrawUIElementNormalized(AtlasID atlasId, const sf::Vector2f& normalizedPosition, const bool adjustHorizontally = true);

	static void Dispose();
};

