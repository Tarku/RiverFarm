#include "SceneDeclarations.h"
#include "SceneManager.h"

void MainMenuScene::Initialize(sf::RenderWindow* window)
{
	p_window = window;

	p_interface = Interface();

	p_interface.CreateNormalizedText(std::string("main_menu_text"), "River Farm", v2f(0.5f, 0));
}

void MainMenuScene::HandleEvents()
{

	while (p_window->pollEvent(p_event))
	{
		if (p_event.type == sf::Event::Closed)
		{
			p_window->close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			SceneManager::ChangeScene(SceneManager::gameScene);
	}
}

void MainMenuScene::Update(float dt)
{
	HandleEvents();
}
void MainMenuScene::Draw() 
{

	sf::Sprite tileBackgroundSprite = sf::Sprite(*AtlasManager::GetAtlas(AtlasTextureID::Tiles), sf::IntRect(0, 0, TILE_SIZE, TILE_SIZE));
	int tileId = 0;

	int scale = 2;

	for (int y = 0; y < (WINDOW_HEIGHT / TILE_SIZE) / scale; y++)
	{
		for (int x = 0; x < (WINDOW_WIDTH / TILE_SIZE) / scale; x++)
		{
			Tile t = TileRegistry::Tiles[TileID::Grass];

			tileBackgroundSprite.setTextureRect(sf::IntRect(t.textureId.x * TILE_SIZE, t.textureId.y * TILE_SIZE, TILE_SIZE, TILE_SIZE));
			tileBackgroundSprite.setPosition(v2f(x * TILE_SIZE * scale, y * TILE_SIZE * scale));

			tileBackgroundSprite.setScale(scale, scale);

			p_window->draw(tileBackgroundSprite);
		}
	}

	p_interface.DrawText(std::string("main_menu_text"));
}
void MainMenuScene::Dispose()
{
	p_interface.Dispose();
}