#pragma once
#pragma once
#include "Particle.h"
#include "config.h"
using namespace sf;
using namespace std;

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

	// Texture and Sprite to display volume UI
	Texture m_volumeTextureON;
	Texture m_volumeTextureOFF;

	Sprite m_volumeUI;
	RectangleShape m_border;

	// Music to play during program, can be toggled ON/OFF
	Music m_music;


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
