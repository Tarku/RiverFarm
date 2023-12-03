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
	static std::shared_ptr<sf::Sound> m_sound;

public:
	static float MinPitchVariation;
	static float MaxPitchVariation;

	static void Initialize();

	static void LoadSound(const string& filepath, const string& tag);
	static void LoadMusic(const string& filepath, const string& tag);

	static void LoadSounds(std::vector<string>& tags);

	static void PlaySound(const string& tag, bool pitchRandomness = true);
	static void PlayMusic(const string& tag);

};