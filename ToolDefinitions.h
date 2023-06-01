#pragma once

#include "Shared.h"
#include "World.h"
#include "Tile.h"
#include "Inventory.h"
#include "TileRegistry.h"

class AgriculturalTool
{
public:
	std::string name = "Base Tool";
	AtlasID uiIcon;

	virtual inline bool CanBeUsedHere(World* world, const sf::Vector2f& position) { return false; };
	virtual inline void OnUse(World* world, const sf::Vector2f& position) {};
};

class AxeTool : public AgriculturalTool
{
public:
	AxeTool();

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