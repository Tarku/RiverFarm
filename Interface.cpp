#include "Interface.h"
#include "AtlasManager.h"
#include "Utils.h"
#include "Tools/ToolRegistry.h"
#include "World/Tiles/BuildableTileRegistry.h"

std::string Interface::fontName = "Assets/font.ttf";
sf::Font Interface::font;
sf::RenderWindow* Interface::window;
sf::Texture Interface::uiTexture;

sf::Texture Interface::uiRightArrow;
sf::Texture Interface::uiLeftArrow;

sf::Sprite* Interface::uiRightArrowSprite = nullptr;
sf::Sprite* Interface::uiLeftArrowSprite = nullptr;

sf::Texture Interface::uiElementsBackground;

sf::Sprite* Interface::uiIconBackground = nullptr;

unsigned char Interface::uiTextCount = 0;

/*
void Interface::CreateText(const std::string& tag, const std::string& stringToDraw, const v2f& position, const sf::Color& color, const float scale)
{
	sf::Text text = sf::Text(font, stringToDraw);

	text.setFillColor(color);
	text.setScale(v2f(scale, scale));



	Interface::elementDeclarations.insert(std::pair(tag, text));
}*/

// This uses normalized coordinates like (0.0f, 0.5f)
void Interface::CreateNormalizedText(const std::string& tag, const std::string& stringToDraw, const v2f& normalizedPosition, const v2f& adjust, const sf::Color& color, const float scale)
{
	for (const auto& [elementTag, element] : elementDeclarations)
	{
		if (elementTag == tag)
		{
			return;
		}
	}
	
	Text* text = new Text(font, stringToDraw);

	text->setFillColor(color);
	text->setScale(v2f(scale, scale));

	UINormalizedTextElement* textElement = new UINormalizedTextElement(text, normalizedPosition, adjust);

	Interface::elementDeclarations.insert(std::pair(tag, textElement));
}

void Interface::CreateNormalizedImage(const std::string& tag, const sf::Texture& textureToDraw, const v2f& normalizedPosition, const v2f& adjust, bool wobbles)
{

	UIImageElement* imgElement = new UIImageElement(textureToDraw, normalizedPosition, adjust);

	imgElement->wobbles = wobbles;

	Interface::elementDeclarations.insert(std::pair(tag, imgElement));
}

void Interface::CreateText(const std::string& tag, const std::string& stringToDraw, const v2f& position, const v2f& adjust, const sf::Color& color, const float scale)
{
	sf::Text* text = new sf::Text(font, stringToDraw);

	text->setFillColor(color);
	text->setScale(v2f(scale, scale));

	UITextElement* textElement = new UITextElement(text, position, adjust);

	Interface::elementDeclarations.insert(std::pair(tag, textElement));
}

void Interface::DrawImageElement(const std::string& tag)
{

	UIImageElement* imgElement = dynamic_cast<UIImageElement*>(elementDeclarations.at(tag));

	assert(imgElement != nullptr);

	imgElement->Draw(window);
}


void Interface::SetTextString(const std::string& tag, const std::string& newString)
{
	assert(Interface::elementDeclarations.contains(tag));

	BaseUITextElement* textElement = dynamic_cast<BaseUITextElement*>(elementDeclarations.at(tag));

	if (textElement == nullptr)
	{
		throw std::exception();
	}
	
	textElement->text->setString(sf::String(newString));
}

void Interface::Update(float dt)
{
	for (const auto& [tag, element] : elementDeclarations)
	{
		element->Update(dt);
	}
}

void Interface::Initialize(sf::RenderWindow* window)
{
	

	bool couldLoadFont = Interface::font.loadFromFile(Interface::fontName);
	assert(couldLoadFont);

	bool couldLoadUIBackground = Interface::uiElementsBackground.loadFromFile(TEXTURES_PATH + "ui_background.png");
	assert(couldLoadUIBackground);

	bool couldLoadUILeftArrow = Interface::uiLeftArrow.loadFromFile(TEXTURES_PATH + "left_arrow.png");
	assert(couldLoadUILeftArrow);

	uiLeftArrowSprite = new Sprite(Interface::uiLeftArrow);

	bool couldLoadUIRightArrow = Interface::uiRightArrow.loadFromFile(TEXTURES_PATH + "right_arrow.png");
	assert(couldLoadUIRightArrow);

	uiRightArrowSprite = new Sprite(Interface::uiRightArrow);

	Interface::uiTexture = *AtlasManager::GetAtlas(AtlasTextureID::UI);

	Interface::uiIconBackground = new sf::Sprite(Interface::uiTexture, sf::IntRect(v2i(0, 0), v2i(UI_ICON_WIDTH, UI_ICON_HEIGHT)));

	Interface::window = window;
	Utils::Log("Interface initialized.");
}

void Interface::ShowBuildableTileOverlay(const int currentBuildableTileIndex)
{
	CreateNormalizedText(std::format("buildable_tile_{}", currentBuildableTileIndex), "", v2f(0.5f, 0.125f), v2f(0.5f, 0.0f));

	int previousTileIndex = currentBuildableTileIndex - 1;
	int nextTileIndex = currentBuildableTileIndex + 1;

	if (previousTileIndex < 0)
		previousTileIndex = BuildableTileRegistry::BuildableTileCount() - 1;

	if (nextTileIndex < 0)
		nextTileIndex = BuildableTileRegistry::BuildableTileCount() - 1;

	if (previousTileIndex >= BuildableTileRegistry::BuildableTileCount())
		previousTileIndex = 0;

	if (nextTileIndex >= BuildableTileRegistry::BuildableTileCount())
		nextTileIndex = 0;

	BuildableTile& currentBuildableTile = BuildableTileRegistry::BuildableTiles.at(currentBuildableTileIndex);
	BuildableTile& previousBuildableTile = BuildableTileRegistry::BuildableTiles.at(previousTileIndex);
	BuildableTile& nextBuildableTile = BuildableTileRegistry::BuildableTiles.at(nextTileIndex);

	Tile* currentTile = TileRegistry::Tiles[currentBuildableTile.tileID];
	Tile* previousTile = TileRegistry::Tiles[previousBuildableTile.tileID];
	Tile* nextTile = TileRegistry::Tiles[nextBuildableTile.tileID];

	//  * Draw tile overlay
	v2f position = v2f(.5f, 0.f) * (float)WINDOW_WIDTH;

	position.x -= (UI_ICON_WIDTH);
	position.y += (UI_ICON_HEIGHT / 4);
	position.y += 30;

	v2f tilePreviewPosition = v2f(.5f, 0.f) * (float)WINDOW_WIDTH;

	tilePreviewPosition.x -= (TILE_SIZE);
	tilePreviewPosition.y += (TILE_SIZE);
	tilePreviewPosition.y += 30;
	
	AtlasID currentTileAtlasID = currentBuildableTile.tileLayer == 0 ? currentTile->groundId : currentTile->textureId;

	sf::Sprite currentTileSprite =
		sf::Sprite(
			AtlasManager::Atlases[AtlasTextureID::Tiles],
			IntRect(
				v2i(
					currentTileAtlasID.x * TILE_SIZE + currentTileAtlasID.x,
					currentTileAtlasID.y * TILE_SIZE + currentTileAtlasID.y
				),
				v2i(
					TILE_SIZE,
					TILE_SIZE
				)
			)
		);

	currentTileSprite.setPosition(tilePreviewPosition);
	currentTileSprite.setScale(v2f(2, 2));

	uiIconBackground->setPosition(position);

	window->draw(*uiIconBackground);
	window->draw(currentTileSprite);

	SetTextString(std::format("buildable_tile_{}", currentBuildableTileIndex), currentBuildableTile.name);
	DrawText(std::format("buildable_tile_{}", currentBuildableTileIndex));
	

}

void Interface::ShowToolOverlay(const int currentToolIndex)
{
	int previousToolIndex = currentToolIndex - 1;
	int nextToolIndex = currentToolIndex + 1;

	if (previousToolIndex < 0)
		previousToolIndex = ToolRegistry::ToolCount() - 1;

	if (nextToolIndex < 0)
		nextToolIndex = ToolRegistry::ToolCount() - 1;

	if (previousToolIndex >= ToolRegistry::ToolCount())
		previousToolIndex = 0;

	if (nextToolIndex >= ToolRegistry::ToolCount())
		nextToolIndex = 0;

	AgriculturalTool* currentTool = ToolRegistry::Tools.at(currentToolIndex);
	AgriculturalTool* previousTool = ToolRegistry::Tools.at(previousToolIndex);
	AgriculturalTool* nextTool = ToolRegistry::Tools.at(nextToolIndex);



	if (previousTool != nullptr)
	{
		v2f position = v2f(.5f, 0.f) * (float)WINDOW_WIDTH;

		position.x -= (UI_ICON_WIDTH * 2.75f);
		position.y += (UI_ICON_HEIGHT / 3);
		position.y += 30;

		DrawUIElement(previousTool->uiIcon, position, UI, 0.75f);
	}

	if (nextTool != nullptr)
	{
		v2f position = v2f(.5f, 0.f) * (float)WINDOW_WIDTH;

		position.x += (UI_ICON_WIDTH * 1.25f);
		position.y += (UI_ICON_HEIGHT / 3);
		position.y += 30;

		DrawUIElement(nextTool->uiIcon, position, UI, 0.75f);
	}
	//  * Draw tool overlay
	if (currentTool != nullptr)
	{
		v2f position = v2f(.5f, 0.f) * (float)WINDOW_WIDTH;

		position.x -= (UI_ICON_WIDTH);
		position.y += (UI_ICON_HEIGHT / 4);
		position.y += 30;

		DrawUIElement(currentTool->uiIcon, position, UI, 1.0f);
	}

	// * Drawing left arrow

	if (&Interface::uiLeftArrowSprite != nullptr)
	{
		sf::FloatRect bounds = Interface::uiLeftArrowSprite->getGlobalBounds();

		v2f position = v2f(0.5f, 0.f) * (float)WINDOW_WIDTH;

		position.x -= (UI_ICON_WIDTH * 2.75f);
		position.x -= bounds.width * 1.5f;
		position.y += (UI_ICON_HEIGHT / 2);
		position.y += 30;

		Interface::uiLeftArrowSprite->setPosition(position);

		window->draw(*Interface::uiLeftArrowSprite);
	}

	// * Drawing right arrow

	if (&Interface::uiRightArrowSprite != nullptr)
	{
		sf::FloatRect bounds = Interface::uiRightArrowSprite->getGlobalBounds();

		v2f position = v2f(0.5f, 0.f) * (float)WINDOW_WIDTH;

		position.x += (UI_ICON_WIDTH * 2.75f);
		position.x += bounds.width * 0.5f;
		position.y += (UI_ICON_HEIGHT / 2);
		position.y += 30;

		Interface::uiRightArrowSprite->setPosition(position);

		window->draw(*Interface::uiRightArrowSprite);
	}
}

void Interface::DrawText(const std::string& tag)
{
	if (!Interface::elementDeclarations.contains(tag)) std::cerr << "Trying to draw text of an inexistant text with tag '" << tag << "'.\n";


	BaseUITextElement* textElement = dynamic_cast<BaseUITextElement*>(elementDeclarations.at(tag));

	if (textElement == nullptr)
	{
		return;
	}

	UINormalizedTextElement* normalizedTextElement = dynamic_cast<UINormalizedTextElement*>(textElement);

	if (normalizedTextElement != nullptr)
	{

		textElement->absolutePosition = v2f(normalizedTextElement->normalizedPosition.x * WINDOW_WIDTH, normalizedTextElement->normalizedPosition.y * WINDOW_HEIGHT);
		
	}

	textElement->text->setPosition(
		textElement->absolutePosition
		-
		v2f(textElement->adjust.x * textElement->text->getGlobalBounds().width, textElement->adjust.y * textElement->text->getGlobalBounds().height)
	);

	sf::Sprite uiBackgroundSprite = sf::Sprite(Interface::uiElementsBackground);

	int backgroundWidth = Interface::uiElementsBackground.getSize().x;
	int backgroundHeight = Interface::uiElementsBackground.getSize().y;
	
	int linesOfText = 1;

	for (auto& character : textElement->text->getString())
	{
		if (character == '\n')
		{
			linesOfText++;
		}
	}

	Text textCopy = Text(*textElement->text);

	textCopy.setFillColor(sf::Color(64, 64, 64, 255));

	textCopy.move(v2f(3, 3));

	window->draw(uiBackgroundSprite);

	window->draw(textCopy);
	window->draw(*textElement->text);
}

BaseUITextElement& Interface::GetText(const std::string& tag)
{
	auto element = elementDeclarations.at(tag);

	BaseUITextElement* textElement = dynamic_cast<BaseUITextElement*>(element);

	if (textElement == 0)
	{

	}

		return *textElement;
}

void Interface::ShowInventoryOverlay()
{
	int itemsAmount = static_cast<int>(ItemRegistry::Items.size());

	sf::Sprite itemIconSprite = sf::Sprite(*AtlasManager::GetAtlas(AtlasTextureID::Items));
	sf::Sprite uiBackgroundSprite = sf::Sprite(Interface::uiElementsBackground);
	int backgroundWidth = uiElementsBackground.getSize().x;

	float fontScreenRatio = 30.0f / WINDOW_HEIGHT;
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
				WINDOW_WIDTH * 0.95f - TILE_SIZE * 2,
				30 + itemCounter * TILE_SIZE * 2 + 4
			)
		);


		sf::Text text = sf::Text( font, std::format("{}", itemAmount));

		text.setFillColor(sf::Color(255, 255, 255));
		text.setScale(v2f(1, 1));

		text.setPosition(
			v2f(
				WINDOW_WIDTH * 0.95f - 2 * TILE_SIZE - text.getGlobalBounds().width - 4.f,
				30 + itemCounter * 2 * TILE_SIZE * 1.f
			)
		);


		uiBackgroundSprite.setPosition(
			v2f(
				WINDOW_WIDTH * 0.95f - 2 * TILE_SIZE - text.getGlobalBounds().width - 8,
				30 + itemCounter * 2 * TILE_SIZE + 1
			)
		);
		
		v2f uiBackgroundSize = v2f(
			SCALED_TILE_SIZE + text.getGlobalBounds().width + 8,
			fontScreenRatio + 2 * TILE_SIZE - 2
		);
		uiBackgroundSprite.setScale(uiBackgroundSize / (float) backgroundWidth);

		window->draw(uiBackgroundSprite);
		window->draw(itemIconSprite);
		window->draw(text);

		itemCounter++;

	}
}

void Interface::DrawUIElement(AtlasID atlasID, const sf::Vector2f& absolutePosition, AtlasTextureID textureAtlas, float scale)
{
	sf::Sprite uiElementSprite = sf::Sprite(*AtlasManager::GetAtlas(textureAtlas), sf::IntRect(v2i(atlasID.x * UI_ICON_WIDTH, atlasID.y * UI_ICON_HEIGHT), v2i(UI_ICON_WIDTH, UI_ICON_HEIGHT)));
	
	Interface::uiIconBackground->setPosition(absolutePosition);
	Interface::uiIconBackground->setScale(v2f(TEXTURE_SCALE, TEXTURE_SCALE) * scale);

	uiElementSprite.setPosition(absolutePosition);
	uiElementSprite.setScale(v2f(TEXTURE_SCALE, TEXTURE_SCALE) * scale);

	window->draw(*Interface::uiIconBackground);
	window->draw(uiElementSprite);
}

void Interface::DrawUIElementNormalized(AtlasID atlasID, const sf::Vector2f& normalizedPosition, const bool adjustHorizontally, AtlasTextureID textureAtlas, float scale)
{
	sf::Sprite uiElementSprite = sf::Sprite(*AtlasManager::GetAtlas(textureAtlas), sf::IntRect(v2i(atlasID.x * UI_ICON_WIDTH, atlasID.y * UI_ICON_HEIGHT), v2i(UI_ICON_WIDTH, UI_ICON_HEIGHT)));

	v2f absolutePosition = { normalizedPosition.x * WINDOW_WIDTH, normalizedPosition.y * WINDOW_HEIGHT };

	if (adjustHorizontally)
	{
		absolutePosition -= v2f((uiElementSprite.getGlobalBounds().width * TEXTURE_SCALE * scale) / 2, 0);
	}

	Interface::uiIconBackground->setPosition(absolutePosition);
	Interface::uiIconBackground->setScale(v2f(TEXTURE_SCALE, TEXTURE_SCALE) * scale);

	uiElementSprite.setPosition(absolutePosition);
	uiElementSprite.setScale(v2f(TEXTURE_SCALE, TEXTURE_SCALE) * scale);

	window->draw(*Interface::uiIconBackground);
	window->draw(uiElementSprite);
}

void Interface::Dispose()
{
	/*
	delete Interface::uiIconBackground;
	delete Interface::uiLeftArrowSprite;
	delete Interface::uiRightArrowSprite;*/

	// elementDeclarations.clear();
}