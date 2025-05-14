#pragma once
#include "Particle.h"
#include "config.h"
using namespace sf;
using namespace std;

class MyText : public Text
{
public:
	void setupText(const Font& font, const RenderWindow& window, const std::string& str, unsigned int size, const Vector2f& offsetPos = { 0, 0 })
	{
		setFont(font);
		setFillColor(Color::White);
		setString(str);
		setCharacterSize(size);

		// Center origin
		setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);

		// Set position with the offset
		setPosition(window.getSize().x / 2 + offsetPos.x, window.getSize().y / 2 + offsetPos.y);
	}
};

class Engine
{
private:
	// A regular RenderWindow
	RenderWindow m_Window;

	// vector for Particles
	vector<Particle> m_particles;

	// Vector to hold textures for animation
	vector<Texture> m_frames;

	// Sprite to display the current animation frame
	Sprite m_sprite;

	int m_currentFrame; // Current frame number of animation
	float m_frameTime; // Amount of time spent per frame (seconds)
	float m_dt;       // Measure amount of elapsed time (seconds)
	
	// Texture and Sprite to display special cursor
	Texture m_cursorTexture;
	Texture m_cursorClickTexture;
	Sprite m_cursor;

	// Texture and Sprite to display volume UI
	Texture m_volumeTextureON;
	Texture m_volumeTextureOFF;

	Sprite m_volumeUI;
	RectangleShape m_border;

	// Music to play during program, can be toggled ON/OFF
	Music m_music;
	
	// Text to create Title Screen w/ 'Play' and 'Exit' options
	MyText m_gameTitle;
	MyText m_playButton;
	MyText m_exitButton;
	MyText m_specialButton;
	Font m_font;

		// Booleans for handling menu selection
		bool m_playButtonClicked;	// 'Was button clicked?'
		bool m_exitButtonClicked;
		bool m_specialButtonClicked;

	// Private functions for internal use only
	void input();
	void update(float dtAsSeconds);
	void draw();
	void init();	// New function added to initalize SFML objects

public:
	// The Engine constructor
	Engine();

	// Run will call all the private functions
	void run();

};