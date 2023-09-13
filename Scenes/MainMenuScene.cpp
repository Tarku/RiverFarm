#include "SceneDeclarations.h"
#include "SceneManager.h"

void MainMenuScene::Initialize(sf::RenderWindow* window)
{
	p_window = window;

	p_interface = Interface();

	p_interface.CreateNormalizedText(std::string("main_menu_text"), "River Farm", v2f(0.5f, 0), v2f(0.5f, 0.f));
}

void MainMenuScene::HandleEvents()
{

	while (p_window->pollEvent(p_event))
	{
		if (p_event.type == sf::Event::Closed)
		{
			p_window->close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			p_window->close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			SceneManager::ChangeScene(SceneManager::gameScene);
		}
	}
}

void MainMenuScene::Update(float dt)
{
	HandleEvents();

	m_angle += dt;

	if (m_angle > TWO_PI)
		m_angle = 0;


	m_tileChangeTimer += dt;

	if (m_tileChangeTimer > 1)
	{
		m_tileChangeTimer = 0;
		tileId++;
	}

	if (tileId >= TileRegistry::TileCount())
		tileId = 0;

	m_blockPosition.x += 5 * dt * (m_startingBlockPosition.x * sinf(m_angle) - m_startingBlockPosition.y * cosf(m_angle));
	m_blockPosition.y += 5 * dt * (m_startingBlockPosition.x * cosf(m_angle) + m_startingBlockPosition.y * sinf(m_angle));

	Utils::Log(std::format("bx : {}; by : {}", roundf(m_blockPosition.x), roundf(m_blockPosition.y)));
}
void MainMenuScene::Draw() 
{

	sf::Sprite tileBackgroundSprite = sf::Sprite(*AtlasManager::GetAtlas(AtlasTextureID::Tiles), sf::IntRect(v2i(0, 0), v2i(TILE_SIZE, TILE_SIZE)));
	
	const float scale = 2.f;

	v2f transformedBlockPosition = (m_blockPosition) + v2f(16.f, 16.f);

	const int horizontalSquares = (int) ( (WINDOW_WIDTH / TILE_SIZE) / scale ) ;
	const int verticalSquares = (int) ( (WINDOW_HEIGHT / TILE_SIZE) / scale );

	int totalSquares = horizontalSquares * verticalSquares;


	for (int y = 0; y < verticalSquares; y++)
	{
		for (int x = 0; x < horizontalSquares; x++)
		{
			Tile* t = TileRegistry::Tiles[tileId];
			
			tileBackgroundSprite.setTextureRect(sf::IntRect(v2i(t->textureId.x * TILE_SIZE, t->textureId.y * TILE_SIZE), v2i(TILE_SIZE, TILE_SIZE)));
			tileBackgroundSprite.setPosition(v2f(x * TILE_SIZE * scale, y * TILE_SIZE * scale));

			tileBackgroundSprite.setScale(v2f(scale, scale));

			p_window->draw(tileBackgroundSprite);
		}
	}

	p_interface.DrawText(std::string("main_menu_text"));
}
void MainMenuScene::Dispose()
{
	p_interface.Dispose();
}