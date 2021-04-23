#include "main.hpp"
#include "audio.hpp"

std::string get_current_dir() {
   char buff[FILENAME_MAX]; //create string buffer to hold path
   GetCurrentDir( buff, FILENAME_MAX );
   std::string current_working_dir(buff);
   return current_working_dir;
}

int audioTest()
{
	std::cout << get_current_dir() << std::endl;
	irrklang::ISoundEngine* sEngine = irrklang::createIrrKlangDevice();

	if (!sEngine) return 1;

	std::cout << "Waiting for audio engine to stabilize..." << std::endl;
	sleep(6);
	std::cout << "Done!" << std::endl;

	sEngine->play2D("resources/Primitives.mp3");

	char i = 0;

	while (i != 'q')
	{
		std::cout << "Press a key to play an effect.";
		std::cin >> i;
		if (i != 'q') sEngine->play2D("resources/fire.mp3");
	}

	sEngine->drop();
	return 0;
}
