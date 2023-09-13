#pragma once

#include "../Shared.h"
#include "../World/World.h"
#include "../World/Tiles/Tile.h"
#include "../World/Tiles/TileRegistry.h"
#include "../Inventory/Inventory.h"

class Interface;

class AgriculturalTool
{
public:
	std::string name;
	std::string description;
	AtlasID uiIcon;

	virtual inline bool CanBeUsedHere(World* world, const sf::Vector2f& position) { return false; };
	virtual inline void OnUse(World* world, const sf::Vector2f& position) {};
	virtual inline void OnRightClick(World* world, const v2f& position) {};

	virtual void Draw(Interface* inter);
};

class AxeTool : public AgriculturalTool
{
public:
	AxeTool();

	virtual bool CanBeUsedHere(World* world, const sf::Vector2f& position) override;
	virtual void OnUse(World* world, const sf::Vector2f& position) override;

};

class ScytheTool : public AgriculturalTool
{
public:
	ScytheTool();

	virtual bool CanBeUsedHere(World* world, const sf::Vector2f& position) override;
	virtual void OnUse(World* world, const sf::Vector2f& position) override;
};

class HoeTool : public AgriculturalTool
{
public:
	HoeTool();

	virtual bool CanBeUsedHere(World* world, const sf::Vector2f& position) override;
	virtual void OnUse(World* world, const sf::Vector2f& position) override;
};


class ShovelTool : public AgriculturalTool
{

public:
	ShovelTool();

	virtual bool CanBeUsedHere(World* world, const sf::Vector2f& position) override;
	virtual void OnUse(World* world, const sf::Vector2f& position) override;
};

class SeedBagTool : public AgriculturalTool
{
private:
	CropID m_currentCrop = CropID::CropBarley;

public:
	SeedBagTool();

	virtual bool CanBeUsedHere(World* world, const sf::Vector2f& position) override;
	virtual void OnUse(World* world, const sf::Vector2f& position) override;
	virtual void OnRightClick(World* world, const v2f& position) override;

	virtual void Draw(Interface* inter) override;
};

class MilkPailTool : public AgriculturalTool
{
private:
	enum MilkPailAction
	{
		None = -1,
		ActionMilkCow,
		ActionTakeWater
	};

	MilkPailAction m_lastPailAction = None;
	CowEntity* m_lastCowMilked = nullptr;

public:
	MilkPailTool();

	virtual bool CanBeUsedHere(World* world, const sf::Vector2f& position) override;
	virtual void OnUse(World* world, const sf::Vector2f& position) override;
};