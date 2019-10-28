#include "MenuText.h"

MenuText::MenuText(const sf::Text & text, const sf::Vector2f location, const sf::Vector2f parentLocation, const sf::Color color)
	: MenuItem(location, parentLocation), m_text(text)
{
	m_text.setFillColor(color);
	m_text.setOrigin(0.0f, 0.0f);
	m_text.setPosition(location);
}

void MenuText::drawItem(sf::RenderWindow & window)
{
	m_text.setPosition(m_parentLocation + m_location);

	window.draw(m_text);
}

void MenuText::setString(const std::string & str)
{
	m_text.setString(str);
}
