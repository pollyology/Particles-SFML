#include "Engine.h"

Engine::Engine()
{
	m_Window.create(VideoMode::getDesktopMode);
}

void Engine::run()
{
	Clock clock;

	while (m_Window.isOpen)
	{
		clock.restart();
		input();
		update();
		draw();
	}
}

void Engine::input()
{
	Event queue;
	while (m_Window.pollEvent(queue))
	{
		Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
		int numPoints;
		int numParticles = 5; // number of particles you want to create

		if (event.type == Event::Closed)
		{
			m_Window.close();
		}

		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
		{
			for (int i = 0; i < numParticles; i++)
			{
				Particle(m_Window, numPoints, mousePos);
			}
		}

		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Esc) 
		{
			m_Window.close();
		}
	}

}

void Engine::update(float dtAsSeconds)
{
	for (auto it : m_particles.begin(); it != m_particles.end();)
	{
		if (it->getTTL() > 0.0)
		{
			it->.update(dt);
			it++;
		}
		else
		{
			it = m_particles.erase(it);
		}

	}
}

void Engine::draw()
{
	m_Window.clear();

	for (auto& particle : m_particles)
	{
		m_Window.draw(particle);
	}

	m_Window.display();
}