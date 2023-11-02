#pragma once

#include "Shared.h"
#include "Inventory/Inventory.h"
#include "AtlasManager.h"

class UIElement
{
public:

	v2f absolutePosition;
	v2f normalizedPosition;
	v2f adjust;

	virtual void Update(float dt) abstract;
};

class BaseUITextElement : public UIElement
{
public:
	sf::Text* text = nullptr;

	inline BaseUITextElement(sf::Text* _text) : text(_text)
	{
		
	};
};

class UITextElement : public BaseUITextElement
{
public:
	inline UITextElement(sf::Text* _text, const v2f& _absolutePosition, const v2f& _adjust) : BaseUITextElement(_text)
	{
		this->absolutePosition = _absolutePosition;
		this->adjust = _adjust;
	}

	inline void Update(float dt) override
	{

	}
};
class UINormalizedTextElement : public BaseUITextElement
{
public:
	inline UINormalizedTextElement(sf::Text* _text, const v2f& _normalizedPosition, const v2f& _adjust) : BaseUITextElement(_text)
	{
		this->normalizedPosition = _normalizedPosition;
		this->adjust = _adjust;
	}

	inline void Update(float dt) override
	{

	}
};

class UIImageElement : public UIElement
{
private:
	bool wobbleClockwise = true;
public:
	sf::Sprite sprite;

	bool wobbles = false;
	float wobbleAngle = 0;
	float wobbleSpeed = 2;

	inline UIImageElement(const sf::Texture& _texture, const v2f& _normalizedPosition, const v2f& _adjust) : sprite(sf::Sprite(_texture))
	{
		this->normalizedPosition = _normalizedPosition;
		this->adjust = _adjust;

	}

	inline void Update(float dt) override
	{
		if (wobbles)
		{
			if (wobbleClockwise)
				wobbleAngle += wobbleSpeed * dt;
			else
				wobbleAngle -= wobbleSpeed * dt;

			if (wobbleAngle >= TWO_PI && wobbleClockwise == true)
			{ 
				wobbleClockwise = false;
			}
			if (wobbleAngle <= 0 && wobbleClockwise == false)
			{
				wobbleClockwise = true;
			}
		}
	}

	inline void Draw(sf::RenderWindow* window)
	{
		
		sprite.setScale(v2f(1 + abs(sinf(wobbleAngle)) / 2, 1 + abs(sinf(wobbleAngle)) / 2));
		auto bounds = sprite.getGlobalBounds();
		sprite.setPosition(
			v2f(normalizedPosition.x * WINDOW_WIDTH, normalizedPosition.y * WINDOW_HEIGHT)
			-
			v2f(adjust.x * bounds.width, adjust.y * bounds.height)
		);
		
		window->draw(sprite);

	}
	
};

class Interface
{
private:
	static sf::Texture uiTexture;
	static sf::Sprite* uiIconBackground;


	static sf::Texture uiRightArrow;
	static sf::Texture uiLeftArrow;

	static sf::Sprite* uiRightArrowSprite;
	static sf::Sprite* uiLeftArrowSprite;

	static sf::Texture uiElementsBackground;
	

	std::map<const std::string, UIElement*> elementDeclarations;

	static uchar uiTextCount;
public:
	static sf::RenderWindow* window;
	static std::string fontName;
	static sf::Font font;
	static void Initialize(sf::RenderWindow* window);

	Interface() {};

	void CreateText(const std::string& tag, const std::string& stringToDraw, const v2f& position, const v2f& adjust, const sf::Color& color = sf::Color::White, const float scale = 1.f);
	void CreateNormalizedText(const std::string& tag, const std::string& stringToDraw, const sf::Vector2f& normalizedPosition, const v2f& adjust, const sf::Color& color = sf::Color::White, const float scale = 1.0f);

	void SetTextString(const std::string& tag, const std::string& newString);
	
	BaseUITextElement& GetText(const std::string& tag);
	void DrawText(const std::string& tag);

	void ShowInventoryOverlay();
	
	void ShowToolOverlay(const int currentToolId);

	void CreateNormalizedImage(const std::string& tag, const sf::Texture& textureToDraw, const v2f& normalizedPosition, const v2f& adjust, bool wobbles);

	void DrawUIElement(AtlasID atlasId, const sf::Vector2f& absolutePosition, AtlasTextureID textureAtlas = AtlasTextureID::UI, float scale = 1.f);
	void DrawUIElementNormalized(AtlasID atlasId, const sf::Vector2f& normalizedPosition, const bool adjustHorizontally = true, AtlasTextureID textureAtlas = AtlasTextureID::UI, float scale = 1.f);

	void DrawImageElement(const std::string& tag);
	void Dispose();

	void Update(float dt);
};

