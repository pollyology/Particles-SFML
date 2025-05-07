#include "Engine.h"

int main()
{
	// Declare an instance of Engine
	Engine engine;
	// Start the engine
	engine.run();
	// Quit in the usual way when the engine is stopped
	return 0;

	const string FILE_BACKGROUND;
	const string FILE_MUSIC;
	const string FILE_CURSOR;

	Music music;
	if (!music.openFromFile(FILE_MUSIC))
	{
		cout << "Unable to load music file \n";
	}

	music.setLoop(true);
	music.play();

	Texture cursorTexture;
	cursorTexture.loadFromFile(FILE_CURSOR);

	Sprite cursor(cursorTexture);
	cursor.setScale(1.0f, 1.0f);

	vector<Texture> frames;
	Sprite sprite;
	Clock clock;

	float frameTime = 0.1f;
	int frameCount = frame.size(); // number of frames

	for (int i = 0; i < frameCount; i++)
	{
		Texture texture;
		texture.loadFromFile("animation/frame" + std::to_string(i) + ".png");
		frames.push_back(texture);
	}

	sprite.setTexture(frames[0]);
}