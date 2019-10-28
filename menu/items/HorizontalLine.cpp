#include "HorizontalLine.h"

HorizontalLine::HorizontalLine(const sf::Vector2f location, const float & width, const sf::Vector2f & parentLocation, const sf::Color & color, const float & thickness)
	: MenuItem(location, parentLocation), m_horizLine({ width, thickness })
{
	m_horizLine.setFillColor(color);
}

void HorizontalLine::drawItem(sf::RenderWindow & window)
{
	m_horizLine.setPosition(m_location + m_parentLocation);
	window.draw(m_horizLine);
}
