#include "audio.hpp"

std::string get_current_dir() {
   char buff[FILENAME_MAX]; //create string buffer to hold path
   GetCurrentDir( buff, FILENAME_MAX );
   std::string current_working_dir(buff);
   return current_working_dir;
}

using namespace vector2;

sounds vector2::openAudio(int rate, int channels, int bufSize)
{
	if (SDL_Init( SDL_INIT_AUDIO ) < 0)
	{
		printf("SDL Couldn't initialize!");
	}

	if( Mix_OpenAudio( rate, MIX_DEFAULT_FORMAT, channels, bufSize ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
	}
	return (sounds){std::map<std::string, Mix_Music*>(), std::map<std::string, Mix_Chunk*>()};
}

Mix_Music* vector2::initMusic(const char* file)
{
	Mix_Music *m = Mix_LoadMUS( file );
	if( m == NULL )
	{
		printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
	};
	return m;
}

Mix_Chunk* vector2::initEffect(const char* file)
{
	Mix_Chunk *m = Mix_LoadWAV( file );
	if( m == NULL )
	{
		printf( "Failed to load effect! SDL_mixer Error: %s\n", Mix_GetError() );
	};
	return m;
}

std::map<std::string, Mix_Chunk*> vector2::mapEffect (Mix_Chunk* c, std::string name, std::map<std::string, Mix_Chunk*> m)
{
	m[name] = c;
	return m;
}
std::map<std::string, Mix_Chunk*> vector2::mapEffect (Mix_Chunk* c, std::string name)
{
	std::map<std::string, Mix_Chunk*> m;
	m[name] = c;
	return m;
}
std::map<std::string, Mix_Chunk*> vector2::mapEffect (const char *n, std::string name, std::map<std::string, Mix_Chunk*> m)
{
	m[name] = initEffect(n);
	return m;
}
std::map<std::string, Mix_Chunk*> vector2::mapEffect (const char *n, std::string name)
{
	std::map<std::string, Mix_Chunk*> m;
	m[name] = initEffect(n);
	return m;
}

std::map<std::string, Mix_Music*> vector2::mapMusic (Mix_Music* c, std::string name, std::map<std::string, Mix_Music*> m)
{
	m[name] = c;
	return m;
}
std::map<std::string, Mix_Music*> vector2::mapMusic (Mix_Music* c, std::string name)
{
	std::map<std::string, Mix_Music*> m;
	m[name] = c;
	return m;
}
std::map<std::string, Mix_Music*> vector2::mapMusic (const char *n, std::string name, std::map<std::string, Mix_Music*> m)
{
	m[name] = initMusic(n);
	return m;
}
std::map<std::string, Mix_Music*> vector2::mapMusic (const char *n, std::string name)
{
	std::map<std::string, Mix_Music*> m;
	m[name] = initMusic(n);
	return m;
}

void vector2::freeMapEffect(std::map<std::string, Mix_Chunk*> effectMap)
{
	for (auto const& m : effectMap)
	{
		Mix_FreeChunk(effectMap[m.first]);
		effectMap[m.first] = NULL;
	}
}
void vector2::freeMapEffect(std::map<std::string, Mix_Chunk*> effectMap, std::string key)
{
	if (effectMap[key])
	{
		Mix_FreeChunk(effectMap[key]);
		effectMap[key] = NULL;
	}
}

void vector2::freeMapMusic(std::map<std::string, Mix_Music*> musicMap)
{
	for (auto const& m : musicMap)
	{
		Mix_FreeMusic(musicMap[m.first]);
		musicMap[m.first] = NULL;
	}
}
void vector2::freeMapMusic(std::map<std::string, Mix_Music*> musicMap, std::string key)
{
	if (musicMap[key])
	{
		Mix_FreeMusic(musicMap[key]);
		musicMap[key] = NULL;
	}
}

void vector2::audioTest()
{
	std::cout << get_current_dir() << std::endl;

	sounds sound = openAudio(48000, 2, 256);

	sound.mus = mapMusic("../resources/audio/Exit Strategy.ogg",           "Exit Strategy", sound.mus);
	sound.mus = mapMusic("../resources/audio/Primitives.ogg",              "Primitives",    sound.mus);
	sound.mus = mapMusic("../resources/audio/Moon Patrol De-Rastered.ogg", "Moon Patrol",   sound.mus);

	sound.eff = mapEffect("../resources/audio/fire.wav",                   "Pew",           sound.eff);

	Mix_PlayMusic(sound.mus["Moon Patrol"], -1);
	char i = ' ';

	std::cout << "i / k: volume +/- 1 | u / j: volume +/- 10 | q: quit | v: pew sound" << std::endl;

	while (i != 'q')
	{
		std::cin >> i;
		int cVolume = Mix_VolumeMusic(-1);
		if (i == 'i') Mix_VolumeMusic(cVolume + (cVolume < 128 ? (1) : 0));
		else if (i == 'k') Mix_VolumeMusic(cVolume - (cVolume > 0 ? 1 : 0));
		else if (i == 'u') Mix_VolumeMusic(cVolume + (cVolume + 10 <= 128 ? 10 : 0));
		else if (i == 'j') Mix_VolumeMusic(cVolume - (cVolume - 10 >= 0 ? 10 : 0));
		else if (i == 'p') std::cout << cVolume << std::endl;
		else if (i == 'v') Mix_PlayChannel(-1, sound.eff["Pew"], 0);
	}
	Mix_HaltMusic();
	closeAudio(sound);
}

void vector2::closeAudio(sounds s)
{
	freeMapEffect(s.eff);
	freeMapMusic(s.mus);
	Mix_Quit();
	SDL_Quit();
}
