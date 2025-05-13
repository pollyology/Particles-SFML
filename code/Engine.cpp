#include "Engine.h"

Engine::Engine()
{
	VideoMode WINDOW_MODE(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_Window.create(WINDOW_MODE, WINDOW_TITLE);
	m_Window.setFramerateLimit(TARGET_FPS);

	//	+---------------------------+
	//	|	MUSIC INITIALIZATION	|
	//	+---------------------------+
		if (m_music.openFromFile(FILE_MUSIC)) cout << "Music filed loaded \n";
		m_music.setLoop(true);
		m_music.setVolume(25);

	//	+-------------------------------+
	//	|		ANIMATION HANDLING		|
	//	+-------------------------------+
		int frameCount = 27; // Manually set this to number of total frames in animation folder
		for (int i = 0; i < frameCount; i++)
		{
			Texture texture;
			texture.loadFromFile("assets/animation/frame_" + std::to_string(i) + ".png");
			m_frames.push_back(texture);
		}
		m_sprite.setTexture(m_frames[0]);
		m_sprite.setScale(Vector2f(0.5, 0.5)); // Tweak to adjust sprite size
		m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2);
		m_sprite.setPosition(m_Window.getSize().x / 2 - 75, m_Window.getSize().y / 2);

		m_currentFrame = 0;
		m_frameTime = 0.025f; // Tweak to adjust animation speed, smaller = faster
		m_dt = 0;

	//	+-------------------------------+
	//	|		UI INITIALIZATION		|
	//	+-------------------------------+
		init();
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
	static bool mouseClickPrevious = false;	// Tracks if mouse was clicked last frame
	bool const mouseLeftPressed = Mouse::isButtonPressed(Mouse::Left);


	// === Experiment with range to change shape diversity ===
	int min = 8;	// If min < 8, program will sometimes create triangle particles
	int max = 20;
	int numParticles = 5; // number of particles you want to create per click

	while (m_Window.pollEvent(event))
	{
		Vector2i mousePos(Mouse::getPosition(m_Window)); // Gets mouse position relative to window size

		//	+---------------------------+
		//	|		TITLE SCREEN		|
		//	+---------------------------+
		if (event.type == Event::Closed)
		{
			m_Window.close();
		}

		if (event.type == Event::KeyPressed && Keyboard::Enter)
		{
			m_playButtonSelected = true;
			cout << "Pressing enter \n";
			m_music.play();
			/*switch (event.key.code)	Tracks which key is pressed
			{
			case Keyboard::Up:
			{
				if (!m_playButtonSelected)
				{
					m_playButtonSelected = true;
					m_exitButtonSelected = false;
					cout << "Up key pressed \n";
				}
			}
			case Keyboard::Down:
			{
				if (!m_exitButtonSelected)
				{
					m_playButtonSelected = false;
					m_exitButtonSelected = true;
				}
			}
			case Keyboard::Enter:
			{
				if (m_playButtonSelected) m_playButtonPressed = true;
				else m_exitButtonPressed == true;
				cout << "Enter key pressed \n";
				break;
			}
			*/
		}


		//	+---------------------------+
		//	|		PARTICLE LOGIC		|
		//	+---------------------------+
		if (mouseLeftPressed)
		{
			if (mouseClickPrevious)	// If holding click, limit particles made per frame
			{
				numParticles = 1;
			}

			for (int i = 0; i < numParticles; i++)
			{
				int random = rand() % (max - min + 1) + min;
				if (random % 2 == 0) { random++; } // Ensures random number is odd, even numbers create 'floppy' shapes
				int numPoints = random;

				Particle particle(m_Window, numPoints, mousePos);
				m_particles.emplace_back(particle);
			}
			cout << "Current mouse click : " << mousePos.x << ", " << mousePos.y << endl;
			cout << "Patricle count: " << m_particles.size() << endl;
		}

		//	+---------------------------+
		//	|		VOLUME BUTTON		|
		//	+---------------------------+
		static bool volumeButtonClicked = false;
		FloatRect iconBounds = m_border.getGlobalBounds();

		if (iconBounds.contains(static_cast<Vector2f>(mousePos)))
		{
			m_border.setOutlineColor(Color::White);

			if (mouseLeftPressed && !mouseClickPrevious) // Registers as single click
			{
				volumeButtonClicked = !volumeButtonClicked;
				cout << "Volume button clicked. Volume is now: " << (!volumeButtonClicked ? "On" : "Off") << endl;

				volumeButtonClicked ? m_music.setVolume(0) : m_music.setVolume(25);
				m_volumeUI.setTexture(!volumeButtonClicked ? m_volumeTextureON : m_volumeTextureOFF);
			}
		}
		else
		{
			m_border.setOutlineColor(Color::Transparent);
		}

		mouseClickPrevious = mouseLeftPressed;
	}
}

void Engine::update(float dtAsSeconds)
{
	// Logic for updating animation and drawing frames
	m_dt += dtAsSeconds;

	if (m_dt >= m_frameTime)
	{
		m_dt -= m_frameTime;
		m_currentFrame = (m_currentFrame + 1) % m_frames.size(); // After reaching last frame, loops animation back to frame 0
		m_sprite.setTexture(m_frames[m_currentFrame]);			// Updates sprite texture to draw current frame
	}

	// Logic for updating particles and TTL
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
	// Drawing
	Color color(154, 218, 248, 155); // Background color (Rainbow Dash Blue lol)

	m_Window.clear(color);

	if (!m_playButtonSelected)
	{
		m_Window.draw(m_gameTitle);
		m_Window.draw(m_playButton);
		//m_Window.draw(m_exitButton);
	}

	else
	{
		m_Window.draw(m_sprite); // Draw the sprite with the current frame
		m_Window.draw(m_volumeUI);
		m_Window.draw(m_border);

		// Draw particles
		for (auto& particle : m_particles)
		{
			m_Window.draw(particle);
		}
	}

	m_Window.display();
}

void Engine::init()
{
	//	+---------------------------+
	//	|		TITLE SCREEN		|
	//	+---------------------------+

	// Title screen booleans
	m_playButtonPressed = false;
	m_playButtonSelected = false;
	m_exitButtonPressed = false;
	m_exitButtonSelected = false;

	// Load font from file, them assign font to Text objects
	m_font.loadFromFile(FONT_FILE);

	m_gameTitle.setFont(m_font);
	m_playButton.setFont(m_font);
	m_exitButton.setFont(m_font);

		// Game Title
		m_gameTitle.setFillColor(Color::White);
		m_gameTitle.setString("Particles");
		m_gameTitle.setCharacterSize(50);
		m_gameTitle.setOrigin(m_gameTitle.getLocalBounds().width / 2, m_gameTitle.getLocalBounds().height / 2);
		m_gameTitle.setPosition(m_Window.getSize().x / 2, m_Window.getSize().y / 2 - 150);

		// Play Button
		m_playButton.setFillColor(Color::White);
		m_playButton.setString("Play");
		m_playButton.setCharacterSize(25);
		m_playButton.setOrigin(m_playButton.getLocalBounds().width / 2, m_playButton.getLocalBounds().height / 2);
		m_playButton.setPosition(m_Window.getSize().x / 2, m_Window.getSize().y / 2 - 50);
		
		// Exit Button
		m_exitButton.setFillColor(Color::White);
		m_exitButton.setString("Exit");
		m_exitButton.setCharacterSize(25);
		m_exitButton.setOrigin(m_exitButton.getLocalBounds().width / 2, m_exitButton.getLocalBounds().height / 2);
		m_exitButton.setPosition(m_Window.getSize().x / 2, m_Window.getSize().y / 2 - 25);


	// Volume Texture
	if (m_volumeTextureON.loadFromFile(FILE_VOLUME_ON)) cout << "Volume on texture loaded \n";
	if (m_volumeTextureOFF.loadFromFile(FILE_VOLUME_OFF)) cout << "Volume off texture loaded \n";

	Vector2f textureSize = static_cast<Vector2f>(m_volumeTextureON.getSize());

	// Volume Sprite
	m_volumeUI.setTexture(m_volumeTextureON);
	m_volumeUI.setOrigin(textureSize / 2.0f);
	m_volumeUI.setScale(Vector2f(1.0, 1.0));
	m_volumeUI.setColor(Color::White);	// This is supposed to make the volume sprite white, but doesn't work?

		// Sets padding of border from window 
		float padding = 15.0f;
		Vector2f position(WINDOW_WIDTH - textureSize.x - padding, WINDOW_HEIGHT - textureSize.y - padding);
		m_volumeUI.setPosition(position);

	// Volume Border
	Vector2f borderSize = textureSize + Vector2f(padding / 2.0f, padding / 2.0f); // Aw yeah baby, centers border outline around icon

	m_border.setSize(borderSize);
	m_border.setOrigin(borderSize / 2.0f);
	m_border.setPosition(position);
	m_border.setFillColor(Color::Transparent);
	m_border.setOutlineColor(Color::White);
	m_border.setOutlineThickness(2.0f);
}
