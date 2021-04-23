#ifndef AUDIO_HPP_
#define AUDIO_HPP_

#include "main.hpp"
#include "structs.hpp"


namespace vector2 {
	void audioTest();
	sounds openAudio(int rate, int channels, int bufSize);

	Mix_Chunk *initEffect(const char* file);
	Mix_Music *initMusic(const char* file);

	std::map<std::string, Mix_Chunk*> mapEffect (Mix_Chunk* c, std::string name, std::map<std::string, Mix_Chunk*> m);
	std::map<std::string, Mix_Chunk*> mapEffect (Mix_Chunk* c, std::string name);
	std::map<std::string, Mix_Chunk*> mapEffect (const char *n, std::string name, std::map<std::string, Mix_Chunk*> m);
	std::map<std::string, Mix_Chunk*> mapEffect (const char *n, std::string name);

	std::map<std::string, Mix_Music*> mapMusic (Mix_Music* c, std::string name, std::map<std::string, Mix_Music*> m);
	std::map<std::string, Mix_Music*> mapMusic (Mix_Music* c, std::string name);
	std::map<std::string, Mix_Music*> mapMusic (const char *n, std::string name, std::map<std::string, Mix_Music*> m);
	std::map<std::string, Mix_Music*> mapMusic (const char *n, std::string name);

	void freeMapEffect(std::map<std::string, Mix_Chunk*> effectMap);
	void freeMapEffect(std::map<std::string, Mix_Chunk*> effectMap, std::string key);

	void freeMapMusic(std::map<std::string, Mix_Music*> musicMap);
	void freeMapMusic(std::map<std::string, Mix_Music*> musicMap, std::string key);

	void closeAudio(sounds s);
}

#endif /* AUDIO_HPP_ */
