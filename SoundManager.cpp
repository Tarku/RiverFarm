#include "SoundManager.h"

int SoundManager::BASE_SFX_COUNT = 32;
int SoundManager::BASE_MUSIC_COUNT = 4;

float SoundManager::MinPitchVariation = 0.9f;
float SoundManager::MaxPitchVariation = 1.1f;

int SoundManager::sfxId = 0;

std::map<std::string, sf::SoundBuffer*> SoundManager::m_sfxBuffers = std::map<std::string, sf::SoundBuffer*>();
std::map<std::string, sf::SoundBuffer*> SoundManager::m_musicBuffers = std::map<std::string, sf::SoundBuffer*>();

sf::Sound* SoundManager::m_sound = nullptr;
sf::Music* SoundManager::m_currentMusic = nullptr;

void SoundManager::Initialize()
{
}

void SoundManager::LoadSound(const std::string& filepath, const std::string& tag)
{
	if (m_sfxBuffers.contains(tag))
	{
		Utils::Log(
			std::format("Can't load sound at '{}'. Cause: tag '{}' already exists.", filepath, tag)
		);

		return;
	}

	sf::SoundBuffer* buffer = new sf::SoundBuffer();

	
	bool couldLoadFilepath = buffer->loadFromFile(filepath);

	if (!couldLoadFilepath)
	{
		Utils::Log(
			std::format("Couldn't load sound at file path '{}'. Please give a valid filepath.", filepath)
		);
	}

	m_sfxBuffers.insert(std::make_pair(tag, buffer));
}

void SoundManager::PlaySound(const std::string& tag, bool pitchRandomness)
{
	if (!m_sfxBuffers.contains(tag))
	{
		Utils::Log(
			std::format("Can't play sound with tag '{}'. Cause: no sound associated with tag '{}'.", tag, tag)
		);

		return;
	}

	m_sound = new sf::Sound(
		*m_sfxBuffers.at(tag)
	);

	if (pitchRandomness)
		m_sound->setPitch(Utils::RandFloat(MinPitchVariation, MaxPitchVariation));

	m_sound->play();

}