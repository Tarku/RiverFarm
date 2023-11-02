#include "Scene.h"
#include "../World/World.h"
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

	bool m_isCurrentToolTargetInRange = false;

	sf::Texture m_daynightCycleOverlay;

	AgriculturalTool* m_currentTool = nullptr;

	bool m_drawChunkBorders = false;
	bool m_drawDebugMenu = false;

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


	inline GameScene() {};

	void Initialize(sf::RenderWindow* window) override;

	void HandleEvents();


	v2f ScreenToWorld(const v2f& position);

	void DrawUI();
	void DrawTileAtMouseUI(const v2f& worldMousePosition);
	void DrawCursorToolUseUI(const v2f& worldMousePosition);

	void DrawBiomeDebugInfoUI(const v2f& worldMousePosition);

	void Update(float dt) override;
	void Draw() override;

	void Dispose() override;

};