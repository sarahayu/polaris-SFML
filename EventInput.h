#pragma once
#include <SFML/Graphics.hpp>
#include "UPDeclarations.h"
#include <queue>

class EventInput
{
public:
	EventInput(sf::RenderWindow &window);
	
	void checkForInput(std::queue<sf::Event> &eventQueue);

private:
	sf::RenderWindow &r_window;
};