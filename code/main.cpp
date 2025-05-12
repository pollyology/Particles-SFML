#include "Engine.h"

int main()
{
	// Seed the random number generator
	srand(time(0));

	// Initalize textures for cursor
	//Texture CURSOR_T;
	//CURSOR_T.loadFromFile(FILE_CURSOR);

	//Texture CURSOR_CLICK_T;
	//CURSOR_CLICK_T.loadFromFile(FILE_CURSOR_CLICK);

	//Sprite cursor(CURSOR_T);
	//cursor.setScale(1.0f, 1.0f);

	// Intialize music to be played during program
	Music music;
	if (!music.openFromFile(FILE_MUSIC))
	{
		cout << "Unable to load music file \n";
	}
	music.setLoop(true);
	music.setVolume(25);
	music.play();

	// Declare an instance of Engine
	Engine engine;
	// Start the engine
	engine.run();
	// Quit in the usual way when the engine is stopped
	return 0;
}