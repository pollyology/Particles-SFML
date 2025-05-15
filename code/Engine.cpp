#include "Engine.h"

Engine::Engine()
{
	VideoMode WINDOW_MODE(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_Window.create(WINDOW_MODE, WINDOW_TITLE);
	m_Window.setFramerateLimit(TARGET_FPS);

	//	+---------------------------+
	//	|	MUSIC INITIALIZATION	|
	//	+---------------------------+
	if (m_music.openFromFile(FILE_MUSIC)) cout << "Music file loaded \n";
	m_music.setLoop(true);
	m_music.setVolume(25);

	//	+-------------------------------+
	//	|		CHARACTER SETTINGS		|
	//	+-------------------------------+
	m_characterMap[FLUTTERSHY] =
	{
		FLUTTERSHY,										// Set character/folder name
		Color(253, 175, 192, 155),						// Fluttershy Pink
		Vector2f(1.0, 1.0),								// Set scale
		Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),	// Set position
		Vector2f(-5, 10),								// Set offset of x,y from center
		103												// Set number of frames
	};

	m_characterMap[RAINBOW_DASH] =
	{
		RAINBOW_DASH,
		Color(154, 218, 248, 155),						// Background color (Rainbow Dash Blue lol)
		Vector2f(0.5, 0.5),
		Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
		Vector2f(-75, 0),
		27
	};

	//	+-------------------------------+
	//	|		ANIMATION HANDLING		|
	//	+-------------------------------+
		m_currentCharacter = RAINBOW_DASH;
		loadAnimation(m_characterMap[m_currentCharacter]); // Load default animation (Rainbow Dash)

	//	+-------------------------------+
	//	|		UI INITIALIZATION		|
	//	+-------------------------------+
		init();
}

void Engine::loadAnimation(const CharacterSettings& settings)
{
	m_frames.clear();
	m_directory = settings.name;

	// Load Animation
	int frameCount = settings.frameCount; // Sets number to character's frame count setting

	for (int i = 0; i < frameCount; i++)
	{
		Texture texture;
		texture.loadFromFile("assets/animation" + m_directory + "/frame_" + std::to_string(i) + ".png");
		m_frames.push_back(texture);
	}

	// Sprite Settings
	m_sprite = Sprite(m_frames[0]);
	m_sprite.setScale(settings.scale); // Tweak to adjust sprite size
		
		// Set origin and position
		FloatRect bounds = m_sprite.getLocalBounds();
		m_sprite.setOrigin(bounds.width / 2, bounds.height / 2);
		m_sprite.setPosition(settings.position + settings.offset);

	// Animation Settings
	m_currentFrame = 0;
	m_frameTime = 0.025f; // Tweak to adjust animation speed, smaller = faster
	m_dt = 0;
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
		FloatRect playButtonBounds = m_playButton.getGlobalBounds();
		FloatRect exitButtonBounds = m_exitButton.getGlobalBounds();

		//	+---------------------------+
		//	|		TITLE SCREEN		|
		//	+---------------------------+
		if (event.type == Event::Closed || event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
		{
			m_Window.close();
		}

		// === Button Hover ===
		if (event.type == Event::MouseMoved)
		{

			if (playButtonBounds.contains(static_cast<Vector2f>(mousePos)))
			{
				m_playButton.setColor(Color::Yellow);
				m_playButton.setScale(Vector2f(1.075, 1.075));
			}
			else if (exitButtonBounds.contains(static_cast<Vector2f>(mousePos)))
			{
				m_exitButton.setColor(Color::Yellow);
				m_exitButton.setScale(Vector2f(1.075, 1.075));
			}
			else
			{
				m_playButton.setColor(Color::White);
				m_playButton.setScale(Vector2f(1.0, 1.0));
				m_exitButton.setColor(Color::White);
				m_exitButton.setScale(Vector2f(1.0, 1.0));
			}
		}
		// === Button Click ===
		if (mouseLeftPressed && !mouseClickPrevious)
		{
			if (playButtonBounds.contains(static_cast<Vector2f>(mousePos)))
			{
				m_playButtonClicked = true;
				cout << "Play button clicked \n";
			}
		}
		if (mouseLeftPressed && !mouseClickPrevious)
		{
			if (exitButtonBounds.contains(static_cast<Vector2f>(mousePos)))
			{
				m_exitButtonClicked = true;
				m_Window.close();
			}
		}

		//	+---------------------------+
		//	|		PARTICLES LOGIC		|
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
				particle.setTTL(10);
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

		//	+---------------------------+
		//	|		SPECIAL BUTTON		|
		//	+---------------------------+
		m_specialButtonClicked = false;
		FloatRect buttonBounds = m_specialButton.getGlobalBounds();
		buttonBounds.height -= 5;

		if (buttonBounds.contains(static_cast<Vector2f>(mousePos)) && !mouseClickPrevious)
		{
			m_specialButton.setScale(Vector2f(1.075, 1.075));

			if (mouseLeftPressed)
			{
				m_specialButton.setColor(Color::Color(230, 230, 230));
				if (!mouseClickPrevious)
				{
					specialEvent();
					changeCharacter();
					cout << "Special button clicked. \n";
					m_specialButtonClicked = !m_specialButtonClicked;
				}
			}
		}
		else if (!m_specialButtonClicked)
		{
			m_specialButton.setColor(Color::White);
			m_specialButton.setScale(Vector2f(1.0, 1.0));
		}
		mouseClickPrevious = mouseLeftPressed;
	}
}

void Engine::update(float dtAsSeconds)
{
	// Start music when 'Play' selected
	if (!m_playButtonClicked) m_music.play();	// I have no idea why this works

	// Logic for updating animation and drawing frames
	m_dt += dtAsSeconds;

	if (m_dt >= m_frameTime)
	{
		m_dt -= m_frameTime;
		m_currentFrame = (m_currentFrame + 1) % m_frames.size(); // After reaching last frame, loops animation back to frame 0
		m_sprite.setTexture(m_frames[m_currentFrame]);			// Updates sprite texture to draw current frame
	}

	FloatRect killbox(0, m_Window.getSize().y, m_Window.getSize().x, 100);

	// Logic for updating particles and TTL
	for (auto it = m_particles.begin(); it != m_particles.end();)
	{
		if (it->getTTL() > 0.0)
		{
			it->update(dtAsSeconds);
			it++;
		}
		else if (it->getPosition().y > killbox.top)
		{
			it = m_particles.erase(it);
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
	const auto& settings = m_characterMap[m_currentCharacter];
	Color color = settings.backgroundColor;

	m_Window.clear(color);

	if (!m_playButtonClicked)
	{
		m_Window.draw(m_gameTitle);
		m_Window.draw(m_playButton);
		m_Window.draw(m_exitButton);
	}
	else
	{
		m_Window.draw(m_sprite); // Draw the sprite with the current frame
		m_Window.draw(m_specialButton);

		// Draw particles
		for (auto& particle : m_particles)
		{
			m_Window.draw(particle);
		}

		m_Window.draw(m_volumeUI);
		m_Window.draw(m_border);
		//m_Window.draw(m_cursor);
	}

	m_Window.display();
}

void Engine::init()
{
	//	+---------------------------+
	//	|		TITLE SCREEN		|
	//	+---------------------------+

	// Title screen booleans
	m_playButtonClicked = false, m_exitButtonClicked = false;

	// Load font from file, them assign font to Text objects
	m_font.loadFromFile(FONT_FILE);

	m_gameTitle.Button::setup(m_font, m_Window, "Particles", 50, Vector2f(0, -175));						// Game Title
	m_playButton.Button::setup(m_font, m_Window, "Play", 25,Vector2f(0, -60));								// Play Button
	m_exitButton.Button::setup(m_font, m_Window, "Exit", 25, Vector2f(0, -25));								// Exit Button
	m_specialButton.Button::setup(m_font, m_Window, "i love computer science!", 50, Vector2f(0, -200));		// Special Button

	// Volume Texture
	if (m_volumeTextureON.loadFromFile(FILE_VOLUME_ON)) cout << "Volume on texture loaded \n";
	if (m_volumeTextureOFF.loadFromFile(FILE_VOLUME_OFF)) cout << "Volume off texture loaded \n";
	
	Vector2f textureSize = static_cast<Vector2f>(m_volumeTextureON.getSize());

	// Volume Sprite
	m_volumeUI.setTexture(m_volumeTextureON);
	m_volumeUI.setOrigin(textureSize / 2.0f);
	m_volumeUI.setScale(Vector2f(2.0, 2.0));
	m_volumeUI.setColor(Color::White);	// This is supposed to make the volume sprite white, but doesn't work?

		// Sets padding of border from window 
		float padding = 15.0f;
		Vector2f position(WINDOW_WIDTH - textureSize.x - padding, WINDOW_HEIGHT - textureSize.y - padding);
		m_volumeUI.setPosition(position);

	// Volume Border
	Vector2f borderSize = Vector2f(m_volumeUI.getLocalBounds().width + (padding + 5), m_volumeUI.getLocalBounds().height + (padding + 5)); // Aw yeah baby, centers border outline around icon

	m_border.setSize(borderSize);
	m_border.setOrigin(borderSize / 2.0f);
	m_border.setPosition(position);
	m_border.setFillColor(Color::Transparent);
	m_border.setOutlineColor(Color::White);
	m_border.setOutlineThickness(2.0f);
}

void Engine::specialEvent()
{
	m_spawnBox = FloatRect(0, -50, m_Window.getSize().x, 5);	// left, top, width, height
	int min = 8;
	int max = 15;
	int numParticles = rand() % 10 + 7;
	
	int random = rand() % (max - min + 1) + min;
	if (random % 2 == 0) { random++; } // Ensures random number is odd, even numbers create 'floppy' shapes
	int numPoints = random;

		for (int i = 0; i < numParticles; i++)
		{
			float x = m_spawnBox.left + static_cast<float>(rand()) / RAND_MAX * m_spawnBox.width;
			float y = m_spawnBox.top + static_cast<float>(rand()) / RAND_MAX * m_spawnBox.height;
			Vector2i randomPos(static_cast<int>(x), static_cast<int>(y));

			Particle particle(m_Window, numPoints, randomPos);

			// Setting physics for special particles
			const float vx = 1.0f; 
			const float vy = 5.0f;
			float ttl = 3.0f + static_cast<float>(rand()) / RAND_MAX * 3.0f; // Between 3 and 6

			particle.setVelocityX(vx);
			particle.setVelocityY(vy);
			particle.setTTL(ttl);

			m_particles.emplace_back(particle);
		}

	//cout << "Exited special event. \n";
}

void Engine::changeCharacter()
{
//	+---------------------------+
//	|		CHANGE CHARACTER	|
//	+---------------------------+
	if (m_currentCharacter == RAINBOW_DASH) m_currentCharacter = FLUTTERSHY;
	else m_currentCharacter = RAINBOW_DASH;

	loadAnimation(m_characterMap[m_currentCharacter]);
}


