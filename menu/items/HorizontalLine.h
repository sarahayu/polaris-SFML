#pragma once
#include "MenuItem.h"

class HorizontalLine : public MenuItem
{
public:
	HorizontalLine(const sf::Vector2f location, const float &width, const sf::Vector2f &parentLocation = sf::Vector2f(), const sf::Color &color = sf::Color::White, const float &thickness = 1.0f);

	void drawItem(sf::RenderWindow &window);

private:
	sf::RectangleShape m_horizLine;
};
