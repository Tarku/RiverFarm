#include "EntityDeclarations.h"
#include "../World/Tiles/TileRegistry.h"
#include "../Interface.h"

CowEntity::CowEntity(const v2f& position)
{
	this->atlasID = { 1, 0 };
	this->speed = 2.1f;

	RandomizeDirection();

	this->position = position;
}

void CowEntity::Update(World* world, float dt)
{
	inWater = (world->TileAt(position.x + .5f, position.y + .5f, 0) == TileID::Water);

	m_moveTimer += dt;

	if (m_moveTimer > MaxMoveTimer && m_currentAIState == Wandering)
	{
		m_moveTimer = 0;
		RandomizeDirection();
	}

	m_hunger += dt * 0.02f;

	if (m_currentAIState == FoodTargetLocked)
	{
		/*
			When a food target (e.g.some flowers) has been found,
			then the cow will move towards it until it reaches the food source.
			When the cow reaches the food source, it will eat it then return to
			its normal Wandering state.
		*/
		float dx = m_foodTarget.x - position.x;
		float dy = m_foodTarget.y - position.y;

		float dist = sqrt(dx * dx + dy * dy);

		velocity = v2f(dx / dist, dy / dist);

		/*
		Utils::Log(std::format("Going to food target at {};{} (dist: {})", m_foodTarget.x, m_foodTarget.y, dist));
		Utils::Log(std::format("Velocity {};{}", velocity.x, velocity.y));*/

		if ((int)position.x == (int)m_foodTarget.x && (int)position.y == (int)m_foodTarget.y)
		{
			world->SetTile(m_foodTarget, 1, 0, false);

			m_hunger = 0;
			m_currentAIState = Wandering;
			return;
		}


	}
	if (m_currentAIState == LookingForFood)
	{
		/* 
			In the LookingForFood state, the cow will try
			to find any food source in a 9x9 square that
			has for center the cow.
		*/

		for (int yO = -4; yO < 5; yO++)
		{
			for (int xO = -4; xO < 5; xO++)
			{
				if (world->TileAt(position.x + xO, position.y + yO, 1) == TileID::Flowers)
				{
					m_foodTarget = v2f(position.x + xO, position.y + yO);
					//Utils::Log(std::format("Found food target at {};{}", m_foodTarget.x, m_foodTarget.y));
					m_currentAIState = FoodTargetLocked;
					return;
				}
			}
		}
	}
	if (m_hunger > 0.8 && m_currentAIState == Wandering)
	{
		/*

			When hungry enough, the cow will be starting to look for food.
		*/

		m_currentAIState = LookingForFood;
	}



	float milkFillingFactor = m_hunger > 0.9f ? 0.1f : (m_hunger > 0.8f ? 0.25f : 1);

	if (milkFullness >= 1)
		milkFullness = 1;
	else
	{
		int hourInTicks = TimeConstants::TICKS_PER_MINUTE * TimeConstants::MINUTES_PER_HOUR;

		float inverse = 1.f / hourInTicks;

		milkFullness += dt * milkFillingFactor * inverse;
	}

	float actualSpeed = inWater ? speed / 2 : speed;

	if (WillCollideWithBlock(velocity * dt, world))
	{
		RandomizeDirection();
	}
	else
	{

		position += velocity * dt * actualSpeed;
	}

}

void CowEntity::RandomizeDirection()
{
	float angle = Utils::RandInt(0, 360) * RADIAN;



	this->velocity.x = cos(angle);
	this->velocity.y = sin(angle);
}

void CowEntity::Draw(sf::RenderWindow* window, v2f cameraPosition)
{
	MovingEntity::Draw(window, cameraPosition);

	std::string stateString = ("");

	switch (m_currentAIState)
	{
	case Wandering:
		stateString = "Wander";
		break;
	case LookingForFood:
		stateString = "SeekFood";
		break;
	case FoodTargetLocked:
		stateString = "FoodTargetFound";
		break;
	default:
		stateString = "Error";
		break;
	}

	if (p_isHovered)
	{

		sf::Text fullnessText = sf::Text(
			Interface::font,
			sf::String(
				std::format(
					"{} {}%",
					stateString,
					round(milkFullness * 100)
				)
			), 14u);

		fullnessText.setFillColor(sf::Color(255, 255, 255, 255));
		fullnessText.setOutlineColor(sf::Color(0, 0, 0, 255));

		sf::FloatRect textSize = fullnessText.getGlobalBounds();
		fullnessText.setPosition(v2f(position.x * (float)SCALED_TILE_SIZE - cameraPosition.x - textSize.width / 2.f + SCALED_TILE_SIZE / 2, position.y * (float)SCALED_TILE_SIZE - cameraPosition.y - 14));
	
		window->draw(fullnessText);
	}

}