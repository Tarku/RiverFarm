#include "ToolDeclarations.h"

#include "../Scenes/GameScene.h"
#include "../SoundManager.h"

HandTool::HandTool()
{
	name = "Hand";
	uiIcon = { 3, 1 };
}

bool HandTool::CanBeUsedHere(World* world, const v2f& position)
{
	bool isThereDoor = world->TileAt(position, 1) == TileID::Door;

	if (isThereDoor)
		m_lastHandAction = HandAction::InteractDoor;

	bool isThereCow = false;

	for (auto entity : World::WorldEntities)
	{
		auto cowCast = dynamic_cast<CowEntity*>(entity);

		if (cowCast == 0)
		{
			continue;
		}
		
		if (cowCast->CheckIfHovered(GameScene::CameraPosition))
		{
			m_lastHandAction = HandAction::PetCow;
			isThereCow = true;
		}
	}

	return isThereDoor || isThereCow;
}

void HandTool::OnUse(World* world, const v2f& position)
{
	/*
	uchar o = TileID::StonebrickWall;
	uchar r = TileID::Air;

	uchar heart[7][7] = {
		{ r, o, o, r, o, o, r },
		{ o, r, r, o, r, r, o },
		{ o, r, r, r, r, r, o },
		{ o, r, r, r, r, r, o },
		{ r, o, r, r, r, o, r },
		{ r, r, o, r, o, r, r },
		{ r, r, r, o, r, r, r }
	};

	for (int j = 0; j < 7; j++)
	{
		for (int i = 0; i < 7; i++)
		{
			world->SetTile(
				v2f(
					position.x - 7 + i,
					position.y - 7 + j
				),
				1,
				heart[j][i]
			);
		}
	}
	*/
	switch (m_lastHandAction)
	{
	case HandAction::None:
		break;
	case HandAction::InteractDoor:
		// Toggle the door's state

		SoundManager::PlaySound("door");
		DoorTile::SetOpen(
			position,
			!DoorTile::IsOpenAt(position)
		);
		break;
	case HandAction::PetCow:
		SoundManager::PlaySound("moo");
		break;
	}

}