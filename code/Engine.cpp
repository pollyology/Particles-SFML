#include "Engine.h"

Engine::Engine()
{
	setupWindow(m_Window);
	//VideoMode WINDOW_MODE(WINDOW_WIDTH, WINDOW_HEIGHT);
	//m_Window.create(WINDOW_MODE, WINDOW_TITLE);
	//m_Window.setFramerateLimit(TARGET_FPS);

	//	+---------------------------+
	//	|	MUSIC INITIALIZATION	|
	//	+---------------------------+
	m_musicIndex = 0;
	loadMusic(0);

	//	+-------------------------------+
	//	|		CHARACTER SETTINGS		|
	//	+-------------------------------+
	m_characterMap[FLUTTERSHY] =
	{
		"Fluttershy",									// Set character name
		FLUTTERSHY,										// Set file path
		Color(253, 175, 192, 155),						// Fluttershy Pink
		Vector2f(1.0, 1.0),								// Set scale
		Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),	// Set position
		Vector2f(-5, 10),								// Set offset of x,y from center
		103,											// Set number of frames
		0.025f											// Set amount of frametime (seconds)

	};

	m_characterMap[RAINBOW_DASH] =
	{
		"Rainbow Dash",
		RAINBOW_DASH,
		Color(154, 218, 248, 155),						// Background color (Rainbow Dash Blue lol)
		Vector2f(0.5, 0.5),
		Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
		Vector2f(-75, 0),
		27,
		0.025f
	};

	m_characterMap[TWILIGHT_SPARKLE] =
	{
		"Twilight Sparkle",
		TWILIGHT_SPARKLE,
		Color(199, 157, 215, 155),
		Vector2f(1.0, 1.0),
		Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
		Vector2f(-10, 15),
		13,
		0.05f
	};

	m_characterMap[PINKIE_PIE] =
	{
		"Pinkie Pie",
		PINKIE_PIE,
		Color(246, 171, 199, 155),
		Vector2f(0.55, 0.55),
		Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
		Vector2f(-20, 0),
		20,
		0.0175f
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
	m_directory = settings.filename;

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
	m_frameTime = settings.frameTime; // Tweak to adjust animation speed, smaller = faster
	m_dt = 0;
}

void Engine::loadMusic(int musicIndex)
{
	if (musicIndex < 0 || musicIndex > m_musicPlaylist.size()) cout << "Invalid music index: " << musicIndex << endl;
	else
	{
		if (m_music.openFromFile(m_musicPlaylist[musicIndex]))
		{
			m_music.setLoop(true);
			m_music.setVolume(25);
			cout << "Success! Music loaded \n";
		}
	}

}

void Engine::run()
{
	Clock clock;
	// *********** UNIT TEST ************
	// cout << "Starting Particle unit tests..." << endl;
	// Particle p(m_Window, 4, { (int)WINDOW_WIDTH / 2, (int)WINDOW_HEIGHT / 2 });
	// p.unitTests();
	// cout << "Unit tests complete.  Starting engine..." << endl;
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

	Vector2i relativePos(Mouse::getPosition(m_Window));										 // Gets mouse position relative to window size
	Vector2i const mousePos = static_cast<Vector2i>(m_Window.mapPixelToCoords(relativePos)); // Adjusts mouse position to work with window resize

	// === Update Button States ===
	m_playButton.update(mousePos, mouseLeftPressed, mouseClickPrevious);
	m_exitButton.update(mousePos, mouseLeftPressed, mouseClickPrevious);
	m_specialButton.update(mousePos, mouseLeftPressed, mouseClickPrevious);
	m_characterButton.update(mousePos, mouseLeftPressed, mouseClickPrevious);
	m_musicButton.update(mousePos, mouseLeftPressed, mouseClickPrevious);
	m_volumeButton.update(mousePos, mouseLeftPressed, mouseClickPrevious);
	
	// === Experiment with range to change shape diversity ===
	int min = 8;	// If min < 8, program will sometimes create triangle particles
	int max = 20;
	int numParticles = 5 + rand() % 6; // number of particles you want to create per click

	while (m_Window.pollEvent(event))
	{
		if (event.type == Event::Closed || event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
		{
			m_Window.close();
		}
		//	+---------------------------+
		//	|		TITLE SCREEN		|
		//	+---------------------------+
		if (m_playButton.isClicked() && !m_playButtonClicked)
		{
			m_playButtonClicked = true;
			m_particles.clear();
			cout << "Play button clicked \n";
		}
		if (m_exitButton.isClicked() && !m_exitButtonClicked && !m_playButtonClicked)
		{
			m_exitButtonClicked = true;
			m_Window.close();
			cout << "Exit button clicked \n";
		}
		//	+---------------------------+
		//	|		VOLUME BUTTON		|
		//	+---------------------------+
		static bool volumeOn = true;

		if (m_volumeButton.isClicked())
		{
			volumeOn = !volumeOn;

			cout << "Volume button clicked. Volume is now: " << (volumeOn ? "On" : "Off") << endl;
			m_music.setVolume(volumeOn ? 25 : 0);
			m_volumeButton.getSprite().setTexture(volumeOn ? m_volumeTextureON : m_volumeTextureOFF);
		}
		//	+---------------------------+
		//	|		MUSIC BUTTON		|
		//	+---------------------------+
		m_musicButtonClicked = false;
		float cooldownTime = m_musicCooldown.getElapsedTime().asSeconds();

		if (m_musicButton.isClicked() && !m_musicButtonClicked && cooldownTime > 0.3f)
		{
			m_musicButtonClicked = true;
			cout << "Change music button clicked! \n";

			changeMusic();
		}
		//	+---------------------------+
		//	|		SPECIAL BUTTON		|
		//	+---------------------------+
		m_specialButtonClicked = false;

		if (m_specialButton.isClicked() && !m_specialButtonClicked)
		{
			m_specialButtonClicked = true;

			int random = rand() % (2 - 1 + 1) + 1;
			for (int i = 0; i < random; i++)
			{
				specialEvent();
			}
			//cout << "Special button clicked! \n";
		}
		//	+---------------------------+
		//	|		CHANGE CHARACTER	|
		//	+---------------------------+
		if (!mouseLeftPressed) m_characterButtonClicked = false;

		if (m_characterButton.isClicked() && !m_characterButtonClicked)
		{
			m_characterButtonClicked = true;
			cout << "Change character button clicked! \n";
			changeCharacter();
		}
		//	+---------------------------+
		//	|		PARTICLES LOGIC		|
		//	+---------------------------+
		if (mouseLeftPressed && m_playButtonClicked)
		{
			if (mouseClickPrevious)	// If holding click, limit particles made per frame
			{
				numParticles = rand() % 3 + 1;
			}

			for (int i = 0; i < numParticles; i++)
			{
				int random = rand() % (max - min + 1) + min;
				if (random % 2 == 0) { random++; } // Ensures random number is odd, even numbers create 'floppy' shapes
				int numPoints = random;

				Particle particle(m_Window, numPoints, mousePos);
				particle.setTTL(1.75);
				m_particles.emplace_back(particle);
			}
			//cout << "Current mouse click : " << mousePos.x << ", " << mousePos.y << endl;
			//cout << "Patricle count: " << m_particles.size() << endl;
		}

		mouseClickPrevious = mouseLeftPressed;	// Always keep this at the end of the input loop
	}
}

void Engine::update(float dtAsSeconds)
{
	// Start music when 'Play' selected
	if (m_playButtonClicked && m_music.getStatus() != sf::Music::Playing)
	{
		m_music.play();  // Start music when play button is clicked
	}
	 
	// Logic for updating animation and drawing frames
	m_dt += dtAsSeconds;

	if (m_dt >= m_frameTime)
	{
		m_dt -= m_frameTime;
		m_currentFrame = (m_currentFrame + 1) % m_frames.size(); // After reaching last frame, loops animation back to frame 0
		m_sprite.setTexture(m_frames[m_currentFrame]);			// Updates sprite texture to draw current frame
	}

	FloatRect killbox(0, WINDOW_HEIGHT, WINDOW_WIDTH, 100); // Despawn particles that fall off-screen

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
		m_Window.draw(m_characterButton);

		// Draw particles
		for (auto& particle : m_particles)
		{
			m_Window.draw(particle);
		}

		m_musicButton.draw(m_Window);
		m_volumeButton.draw(m_Window);
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
	m_characterButton.Button::setup(m_font, m_Window, "change character?", 22, Vector2f(-380, 245));		// Change Character Button

	// Volume Button
	if (m_volumeTextureON.loadFromFile(FILE_VOLUME_ON)) cout << "Success! Volume on texture loaded \n";
	if (m_volumeTextureOFF.loadFromFile(FILE_VOLUME_OFF)) cout << "Success! Volume off texture loaded \n";
	m_volumeButton.setButtonScale(Vector2f(2.0, 2.0));
	m_volumeButton.setHoverScale(Vector2f(2.2, 2.2));
	m_volumeButton.setup(m_volumeTextureON, m_Window, Vector2f(0, 0));

		// Sets padding of border from window 
		float padding = 15.0f;
		Vector2f position(WINDOW_WIDTH - m_volumeTextureON.getSize().x - padding, WINDOW_HEIGHT - m_volumeTextureON.getSize().y - padding);
		m_volumeUI.setPosition(position);
		m_volumeButton.setButtonPosition(position);

	// Music Button
	if (m_musicButtonTexture.loadFromFile(FILE_MUSIC_ON)) cout << "Success! Music texture loaded \n";
	m_musicButton.setButtonScale(Vector2f(2.125, 2.125));
	m_musicButton.setHoverScale(Vector2f(2.3375, 2.3375));
	m_musicButton.setup(m_musicButtonTexture, m_Window, Vector2f(0, 0));
	m_musicButton.setButtonPosition({ position.x - 40, position.y + 2});	// Call setButtonPosition after setup to override default settings
}

void Engine::specialEvent()
{
	m_spawnBox = FloatRect(0, -50, WINDOW_WIDTH, 5);	// left, top, width, height
	int min = 8;
	int max = 13;
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
	m_characterIndex = (m_characterIndex + 1) % m_characterList.size();
	m_currentCharacter = m_characterList[m_characterIndex];

	loadAnimation(m_characterMap[m_currentCharacter]);
	cout << "Changed character to " << m_characterMap[m_currentCharacter].name << endl;
}

void Engine::changeMusic()
{
	//cout << "Music index before: " << m_musicIndex << endl;
	m_music.stop();
	m_musicIndex = (m_musicIndex + 1) % m_musicPlaylist.size();
	loadMusic(m_musicIndex);
	m_music.play();

	//cout << "Music index after: " << m_musicIndex << endl;
	cout << "Now playing: " << SONG_LIST[m_musicIndex] << endl;
}


