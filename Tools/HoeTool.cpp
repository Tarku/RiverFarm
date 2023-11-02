#include "ToolDeclarations.h"

HoeTool::HoeTool()
{
	name = "Hoe";
	uiIcon = AtlasID( 2, 0 );
}

bool HoeTool::CanBeUsedHere(World* world, const Vector2f& position)
{
	return TileRegistry::Tiles[world->TileAt(position, 0)]->tileProperties.isArable && world->IsEmptyAt(position, 1);
}

void HoeTool::OnUse(World* world, const Vector2f& position)
{
	uchar currentTileID = world->TileAt(position, 0);

	TileID tilledTile = TileID::Cactus;

	switch (currentTileID)
	{
	case TileID::Grass:
		tilledTile = TileID::TilledSoil;
		break;
	case TileID::Dirt:
		tilledTile = TileID::TilledSoil;
		break;
	case TileID::Sand:
		tilledTile = TileID::TilledSand;
		break;
	}

	world->SetTile(position, 0, tilledTile, true);

	// 1/12 chance to get a seed from tilling the soil
	if (Utils::RandFloat(0.0f, 1.0f) < (1.f / 12.f))
	{
		CropID randomCropID = static_cast<CropID>(Utils::RandInt(0, CropRegistry::CropAmount() - 1));
		Crop* randomCrop = CropRegistry::Crops.at(randomCropID);

		if (randomCrop != nullptr)
		{
			world->AddItemEntity(position, randomCrop->seedItemDrop);
		}
	}
}