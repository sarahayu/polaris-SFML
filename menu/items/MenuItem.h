#pragma once
#include <SFML\Graphics.hpp>
#include <memory>

class MenuItem
{
public:
	typedef std::shared_ptr<MenuItem> Ptr;

	//MenuItem();
	MenuItem(const sf::Vector2f location);
	MenuItem(const sf::Vector2f location, const sf::Vector2f parentLocation);

	void setParentLocation(const sf::Vector2f parentLocation);	// top left

	void draw(sf::RenderWindow &window);

	void toggleShow();

	bool show = true;

protected:

	virtual void drawItem(sf::RenderWindow &window) = 0;

	sf::Vector2f m_location;		// top left
	sf::Vector2f m_parentLocation;
};