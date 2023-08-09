#pragma once

#include "Scene.h"
#include "../Shared.h"
#include "../World/World.h"
#include "../Interface.h"
#include "../Entities/EntityDeclarations.h"
#include "../Inventory/Inventory.h"
#include "../Tools/ToolDeclarations.h"

class GameScene : public IScene
{
private:
	short m_currentToolIndex = 0;

	float m_fps = 0;
	float m_gameTime = 0;
	
	long m_ticks = 0;

	AgriculturalTool* m_currentTool;

	v2f m_cameraPosition = v2f(0, 0);
	v2f m_mousePosition = v2f(0, 0);

	World m_world;

public:
	static PlayerEntity player;
	static bool showChunkBorders;

	inline GameScene() {};

	void Initialize(sf::RenderWindow* window) override;

	void HandleEvents();


	v2f ScreenToWorld(const v2f& position);

	void Update(float dt) override;
	void Draw() override;

	void Dispose() override;
};

class MainMenuScene : public IScene
{
public:
	inline MainMenuScene() {};

	void Initialize(sf::RenderWindow* window) override;

	void HandleEvents();

	void Update(float dt) override;
	void Draw() override;

	void Dispose() override;
};