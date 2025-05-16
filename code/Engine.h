#pragma once
#include "Particle.h"
#include "config.h"
#include "Button.h"
using namespace sf;
using namespace std;

struct CharacterSettings
{
	string name;			// Character name
	string filename;		// Folder name
	Color backgroundColor;	// Character color
	Vector2f scale;			// Custom scale
	Vector2f position;		// Custom position
	Vector2f offset;		// Offset from origin
	int frameCount;			// Number of frames
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
	string m_currentCharacter;
	map<string, CharacterSettings> m_characterMap;

	string m_directory;	  // The file directory containing the animation
	int m_frameCount;	 // Number of total frames in animation
	int m_currentFrame; // Current frame number of animation
	float m_frameTime; // Amount of time spent per frame (seconds)
	float m_dt;       // Measure amount of elapsed time (seconds)
	
	// Texture and Sprite to display volume UI
	Texture m_volumeTextureON;
	Texture m_volumeTextureOFF;

	Sprite m_volumeUI;
	RectangleShape m_border;

	// Music to play during program, can be toggled ON/OFF
	Music m_music;
	
	// Text to create Title Screen w/ 'Play' and 'Exit' options
	Button m_gameTitle;
	Button m_playButton;
	Button m_exitButton;
	Button m_specialButton;
	Button m_characterButton;
	Font m_font;

		// Booleans for handling menu selection
		bool m_playButtonClicked;	// 'Was button clicked?'
		bool m_exitButtonClicked;
		bool m_specialButtonClicked;
		bool m_characterButtonClicked;

		// Special Event
		FloatRect m_spawnBox;


	// Private functions for internal use only
	void input();
	void update(float dtAsSeconds);
	void draw();

		// New functions added to initalize SFML objects
		void init();
		void specialEvent();
		void changeCharacter();
		void loadAnimation(const CharacterSettings& settings);

public:
	// The Engine constructor
	Engine();

	// Run will call all the private functions
	void run();

};
