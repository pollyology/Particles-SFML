#include "Engine.h"

Engine::Engine()
{
	VideoMode WINDOW_MODE(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_Window.create(WINDOW_MODE, WINDOW_TITLE);
	m_Window.setFramerateLimit(TARGET_FPS);

	//	+-------------------------------+
	//	|		ANIMATION HANDLING		|
	//	+-------------------------------+
	int frameCount = 27; // number of total frames in animation folder
	for (int i = 0; i < frameCount; i++)
	{
		Texture texture;
		texture.loadFromFile("assets/animation/frame_" + std::to_string(i) + ".png");
		m_frames.push_back(texture);
	}
	m_sprite.setTexture(m_frames[0]);
	m_sprite.setScale(Vector2f(0.5, 0.5));

	m_currentFrame = 0; // Initial frame
	m_animationSpeed = 0.025f; // Time per frame (seconds)
	m_elapsedTime = 0.0f; // Time accumulator
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

	while (m_Window.isOpen())
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

	// === Experiment with range to change shape diversity ===
	int min = 8;	// If min < 8, program will sometimes create triangle particles
	int max = 20;
	int numParticles = 5; // number of particles you want to create per click

	while (m_Window.pollEvent(event))
	{
		Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);

		if (event.type == Event::Closed)
		{
			m_Window.close();
		}

		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
		{
			for (int i = 0; i < numParticles; i++)
			{
				int random = rand() % (max - min + 1) + min;
				if (random % 2 == 0) { random++; } // Ensures random number is odd, even points create 'floppy' shapes
				int numPoints = random;

				Particle particle(m_Window, numPoints, mousePos);
				m_particles.emplace_back(particle);
			}
			cout << "Current mouse click : " << mousePos.x << ", " << mousePos.y << endl;
			cout << "Patricle count: " << m_particles.size() << endl;
		}

		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
		{
			m_Window.close();
		}
	}
}

void Engine::update(float dtAsSeconds)
{
	// Accumulate elapsed time for frame change
	m_elapsedTime += dtAsSeconds;

	// Check if it's time to update the frame
	if (m_elapsedTime >= m_animationSpeed)
	{
		m_elapsedTime -= m_animationSpeed; // Reset elapsed time
		m_currentFrame = (m_currentFrame + 1) % m_frames.size(); // Loop through frames
		m_sprite.setTexture(m_frames[m_currentFrame]); // Set new frame texture
	}

	for (auto it = m_particles.begin(); it != m_particles.end();)
	{
		if (it->getTTL() > 0.0)
		{
			it->update(dtAsSeconds);
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
	Text text;
	Font font;
	font.loadFromFile(FONT_FILE);
	text.setFont(font);
	text.setFillColor(Color::White);
	text.setString("Hello, this is a test.");

	// Drawing
	Color color(154, 218, 248, 155); // Background color

	m_Window.clear(color);
	m_Window.draw(text);
	m_Window.draw(m_sprite); // Draw the sprite with the current frame

	// Draw particles
	for (auto& particle : m_particles)
	{
		m_Window.draw(particle);
	}

	m_Window.display();
}
