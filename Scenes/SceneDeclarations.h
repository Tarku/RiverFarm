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

	const float MAX_TOOL_COOLDOWN = 0.25f;
	float m_toolCooldown = 0;

	sf::Texture m_daynightCycleOverlay;

	AgriculturalTool* m_currentTool;

	v2f m_cameraPosition = v2f(0, 0);
	v2f m_mousePosition = v2f(0, 0);

	World m_world;

	enum class MenuMode
	{
		NormalMode,
		BuildingMode
	};

	MenuMode m_currentMenuMode = MenuMode::NormalMode;
	

public:
	inline static PlayerEntity* player = nullptr;
	inline static bool showChunkBorders = false;

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
private:
	const v2f m_startingBlockPosition { 1, 0 };
	v2f m_blockPosition = m_startingBlockPosition;
	
	float tileId = 0;

	float m_tileChangeTimer = 0;

	float m_angle = 0;
public:
	inline MainMenuScene() {};

	void Initialize(sf::RenderWindow* window) override;

	void HandleEvents();

	void Update(float dt) override;
	void Draw() override;

	void Dispose() override;
};