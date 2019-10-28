#include "MenuItem.h"

/*
MenuItem::MenuItem()
{
}
*/


MenuItem::MenuItem(const sf::Vector2f location)
	: m_location(location), m_parentLocation(location)
{
}

MenuItem::MenuItem(const sf::Vector2f location, const sf::Vector2f parentLocation)
	: m_location(location), m_parentLocation(parentLocation)
{
}

void MenuItem::setParentLocation(const sf::Vector2f parentLocation)
{
	m_parentLocation = parentLocation;
}

void MenuItem::draw(sf::RenderWindow & window)
{
	if (show) drawItem(window);
}

void MenuItem::toggleShow()
{
	show = !show;
}
