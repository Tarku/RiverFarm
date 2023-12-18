#pragma once

#include "../../Shared.h"

enum TileID;
enum ItemID;
class World;

class BuildableTileIngredient
{
public:
	int cost;
	ItemID itemID;

	BuildableTileIngredient(
		int cost,
		ItemID itemID
	);
};

class BuildableTile
{
public:
	string name;

	int tileLayer;
	TileID tileID;

	std::vector<BuildableTileIngredient> tileIngredients;

	BuildableTile(
		string name,
		int tileLayer,
		TileID tileID,
		std::vector<BuildableTileIngredient> tileIngredients
	);

	bool CanBePlacedHere(const v2f& position, World* world);
};