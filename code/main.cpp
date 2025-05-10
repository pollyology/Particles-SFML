#include "Engine.h"

int main()
{
	// Seed the random number generator
	srand(time(0));
	
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

	const string FILE_BACKGROUND;
	const string FILE_CURSOR;

	Texture cursorTexture;
	cursorTexture.loadFromFile(FILE_CURSOR);

	Sprite cursor(cursorTexture);
	cursor.setScale(1.0f, 1.0f);
}