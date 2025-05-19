#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

std::string const WINDOW_TITLE = "Particles!";
constexpr int WINDOW_WIDTH = 1920 / 2;
constexpr int WINDOW_HEIGHT = 1080 / 2;
constexpr int TARGET_FPS = 30;
constexpr int ANTIALIAS_LEVEL = 8;

// Window Settings
inline void setupWindow(sf::RenderWindow& window)
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIALIAS_LEVEL;

    window.create(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Default, settings);

    auto const desktop = sf::VideoMode::getDesktopMode();
    auto windowSize = window.getSize();
    window.setPosition({ static_cast<int>(desktop.width / 2 - windowSize.x / 2),
        static_cast<int>(desktop.height / 2 - windowSize.y / 2) });

    window.setFramerateLimit(TARGET_FPS);

    auto applied = window.getSettings();
    std::cout << "Anti-Aliasing: " << applied.antialiasingLevel << "x\n";
}

// Fonts
std::string const FONT_FILE = "assets/fonts/Celsius Flower.ttf";

// Music
std::string const FILE_MUSIC_1 = "assets/music/MLP_Extended.flac";
std::string const FILE_MUSIC_2 = "assets/music/Make_A_Wish_Extended.flac";
std::string const FILE_MUSIC_3 = "assets/music/Remember_Summer_Days.flac";

    // Song List
    string const SONG_LIST[3] =
    {
        "My Little Pony Theme Song",
        "Make a Wish (Extended Version)",
        "Remember Summer Days",
    };

// Sprites
std::string const FILE_VOLUME_ON = "assets/images/Volume_On.png";
std::string const FILE_VOLUME_OFF = "assets/images/Volume_Mute.png";
std::string const FILE_MUSIC_ON = "assets/images/Music_On.png";

// File Paths
std::string const RAINBOW_DASH = "/rainbow-dash";
std::string const FLUTTERSHY = "/fluttershy";
std::string const TWILIGHT_SPARKLE = "/twilight-sparkle";
std::string const PINKIE_PIE = "/pinkie-pie";