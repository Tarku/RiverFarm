#pragma once

#include "Shared.h"
#include "Inventory/Inventory.h"

class Interface
{
private:
	static sf::RenderWindow* window;
	static sf::Texture uiTexture;
	static sf::Sprite uiIconBackground;

	static sf::Texture uiElementsBackground;

	std::map<std::string, sf::Text> textDeclarations;

	static uchar uiTextCount;
public:
	static std::string fontName;
	static sf::Font font;
	static void Initialize(sf::RenderWindow* window);

	Interface() {};

	void CreateText(const std::string& tag, const std::string& text, const v2f& position, const sf::Color& color = sf::Color::White, const float scale = 1.f);
	void CreateNormalizedText(const std::string& tag, const std::string& stringToDraw, const sf::Vector2f& normalizedPosition, const sf::Color& color = sf::Color::White, const float scale = 1.f, const bool adjustHorizontally = true);

	void SetTextString(const std::string& tag, const std::string& newString);

	void DrawText(const std::string& tag);

	void ShowInventoryOverlay();
	
	void DrawUIElement(AtlasID atlasId, const sf::Vector2f& absolutePosition);
	void DrawUIElementNormalized(AtlasID atlasId, const sf::Vector2f& normalizedPosition, const bool adjustHorizontally = true);

	void Dispose();
};

