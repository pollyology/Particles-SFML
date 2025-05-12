#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

std::string const WINDOW_TITLE = "Particle Project";
constexpr int WINDOW_WIDTH = 1920 / 2;
constexpr int WINDOW_HEIGHT = 1080 / 2;
constexpr int TARGET_FPS = 60;

// Fonts
std::string const FONT_FILE = "assets/fonts/Celsius Flower.ttf";

// Music
std::string const FILE_MUSIC = "assets/music/MLP_Extended.flac";

// Sprites
std::string const FILE_CURSOR = "assets/images/Wand_Cursor.png";
std::string const FILE_CURSOR_CLICK = "assets/images/Wand_Cursor_Click.png";
std::string const FILE_VOLUME_ON = "assets/images/Volume_On.png";
std::string const FILE_VOLUME_OFF = "assets/images/Volume_Mute.png";