#include "Engine.h"

Engine::Engine()
{
	m_Window.create(VideoMode::getDesktopMode);
}

void Engine::run()
{
	Clock clock;
	// *********** UNIT TEST ************
	cout << "Starting Particle unit tests..." << endl;
    Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
    p.unitTests();
    cout << "Unit tests complete.  Starting engine..." << endl;
	// **********************************

	while (m_Window.isOpen)
	{
		Time time = clock.restart();
		float dt = time.asSeconds();
		input();
		update(dt);
		draw();
	}
}

void Engine::input()
{
	Event event;
	m_Window.setFrameLimit(60);

	// TO DO: Experiment using range of only odd numbers
	int min = 25;
	int max = 50;
	int random = rand() % (max - min + 1) + min; 

	while (m_Window.pollEvent(event))
	{
		Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
		int numPoints = random;
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

		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) 
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
			it->.update(dtAsSeconds);
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