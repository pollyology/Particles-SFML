#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;
class Button : public Text
{
public:
	void setup(const Font& font, const RenderWindow& window, const string& str, unsigned int size, const Vector2f& offsetPos = { 0, 0 });
	void update(Vector2i mousePos, bool mousePressed, bool mousePreviouslyPressed);

	// Set button settings
	void setColor(const Color& color) { m_color = color; }
	void setHoverColor(const Color& color) { m_hoverColor = color; }
	void setScale(const Vector2f& scale) { m_scale = scale; }
	void setHoverScale(const Vector2f& scale) { m_hoverScale = scale; }


	// Get button state
	bool isClicked() const { return m_isClicked; }
	bool isHovered() const { return m_isHovered; }

private:
	bool m_isHovered;
	bool m_isClicked;
	Color m_color;
	Color m_hoverColor;
	Vector2f m_scale;
	Vector2f m_hoverScale;
};