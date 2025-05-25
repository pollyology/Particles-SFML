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
	float frameTime;		// Seconds per frame
};

class Engine
{
private:
	// A regular RenderWindow
	RenderWindow m_Window;

	// vector for Particles
	vector<Particle> m_particles;

	// Vector to hold textures for animation
	vector<Texture> m_frames;						// Stores the frames of a single character animation
	map<string, vector<Texture>> m_animationCache;	// Stores every character's animation frames

	// Sprite to display the current animation frame
	Sprite m_sprite;
	string m_currentCharacter;
	int m_characterIndex = 0;

		// Structures to access character
		map<string, CharacterSettings> m_characterMap;
		vector<string> m_characterList
		{
			// Edit to change order characters appear in program
			RAINBOW_DASH,
			FLUTTERSHY,
			TWILIGHT_SPARKLE,
			PINKIE_PIE
		};
			
	string m_directory;	  // The file directory containing the animation
	int m_frameCount;	 // Number of total frames in animation
	int m_currentFrame; // Current frame number of animation
	float m_frameTime; // Amount of time spent per frame (seconds)
	float m_dt;       // Measure amount of elapsed time (seconds)
	
	// Music to play during program, can be toggled ON/OFF
	Music m_music;
	int m_musicIndex;
	string m_currentSong;
		
		// Structure to access music playlist
		vector<string> m_musicPlaylist
		{
			FILE_MUSIC_1,
			FILE_MUSIC_2,
			FILE_MUSIC_3,
		};
	// Music button
	SpriteButton m_musicButton;
	Texture m_musicButtonTexture;
	Clock m_musicCooldown;	// Gives music button a cooldown to prevent bugs from rapid clicking

	// Volume button
	SpriteButton m_volumeButton;
	Texture m_volumeTextureON;
	Texture m_volumeTextureOFF;
	bool m_volumeOn = true;

	Sprite m_volumeUI;
	RectangleShape m_border;

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
		bool m_musicButtonClicked;

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
		void changeMusic();
		void loadMusic(int musicIndex);
		void loadAnimation(const CharacterSettings& settings);

public:
	// The Engine constructor
	Engine();

	// Run will call all the private functions
	void run();

};
