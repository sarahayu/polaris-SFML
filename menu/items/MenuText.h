#pragma once
#include "MenuItem.h"

class MenuText : public MenuItem
{
public:

	typedef std::shared_ptr<MenuText> Ptr;

	MenuText(const sf::Text &text, const sf::Vector2f location, const sf::Vector2f parentLocation = sf::Vector2f(), const sf::Color color = sf::Color::White);

	void drawItem(sf::RenderWindow &window);
	void setString(const std::string &str);

private:
	sf::Text m_text;
};
