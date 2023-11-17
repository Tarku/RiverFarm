#include "Shared.h"
#include "Utils.h"

#include <map>
#include <SFML/Audio.hpp>

class SoundManager
{
private:
	static int BASE_SFX_COUNT;
	static int BASE_MUSIC_COUNT;

	static int sfxId;

	static std::map<std::string, sf::SoundBuffer*> m_sfxBuffers;
	static std::map<std::string, sf::SoundBuffer*> m_musicBuffers;

	static sf::Music* m_currentMusic;
	static sf::Sound* m_sound;

public:
	static float MinPitchVariation;
	static float MaxPitchVariation;

	static void Initialize();

	static void LoadSound(const std::string& filepath, const std::string& tag);
	static void LoadMusic(const std::string& filepath, const std::string& tag);

	static void PlaySound(const std::string& tag, bool pitchRandomness = true);
	static void PlayMusic(const std::string& tag);

};