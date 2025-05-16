#include "Button.h"

void Button::setup(const Font& font, const RenderWindow& window, const string& str, unsigned int size, const Vector2f& offsetPos)
{
	// Default button state settings
	m_isHovered = false;
	m_isClicked = false;
	m_color = Color::White;
	m_hoverColor = Color::Yellow;
	m_scale = Vector2f(1.0f, 1.0f);
	m_hoverScale = Vector2f(1.075, 1.075f);

	// Default text settings
	setFont(font);
	setFillColor(Color::White);
	setString(str);
	setCharacterSize(size);
	setScale(m_scale);
	setHoverScale(m_hoverScale);

	// Center origin
	FloatRect bounds = getLocalBounds();
	setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

	// Set position with the offset
	setPosition(window.getSize().x / 2 + offsetPos.x, window.getSize().y / 2 + offsetPos.y);

	// Set button bounds
	m_buttonBounds = getGlobalBounds();
}

void Button::update(Vector2i mousePos, bool mousePressed, bool mousePreviouslyPressed)
{
	bool hovered = m_buttonBounds.contains(static_cast<Vector2f>(mousePos));
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