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

	//vector for Particles
	vector<Particle> m_particles;

	// Vector to hold textures for animation
	vector<Texture> m_frames;

	// Sprite to display the current frame
	Sprite m_sprite;

	int m_currentFrame; // Initial frame
	float m_animationSpeed; // Time per frame (seconds)
	float m_elapsedTime; // Time accumulator

	// Private functions for internal use only
	void input();
	void update(float dtAsSeconds);
	void draw();

public:
	// The Engine constructor
	Engine();

	// Run will call all the private functions
	void run();

};
