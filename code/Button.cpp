#include "Button.h"

void Button::setup(const Font& font, const RenderWindow& window, const string& str, unsigned int size, const Vector2f& offsetPos = { 0, 0 })
{
	// Default text settings
	setFont(font);
	setFillColor(Color::White);
	setString(str);
	setCharacterSize(size);

	// Default button state settings
	m_isHovered = false;
	m_isClicked = false;
	m_color = Color::White;
	m_hoverColor = Color::Yellow;
	m_scale = Vector2f(1.0f, 1.0f);
	m_hoverScale = Vector2f(1.075, 1.075f);

	// Center origin
	setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);

	// Set position with the offset
	setPosition(window.getSize().x / 2 + offsetPos.x, window.getSize().y / 2 + offsetPos.y);
}

void Button::update(Vector2i mousePos, bool mousePressed, bool mousePreviouslyPressed)
{
	FloatRect bounds = getGlobalBounds();
	bool hovered = bounds.contains(static_cast<Vector2f>(mousePos));
	m_isClicked = false;

	if (hovered)
	{
		if (!m_isHovered)
		{
			m_isHovered = true;
			setFillColor(m_hoverColor);
			setScale(m_hoverScale);
		}

		if (mousePressed && !mousePreviouslyPressed) m_isClicked = true;
	}
	else if (m_isHovered)
	{
		m_isHovered = false;
		setFillColor(m_color);
		setScale(m_scale);
	}
}