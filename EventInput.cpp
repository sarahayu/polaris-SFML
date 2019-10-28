#include "EventInput.h"
#include <iostream>

bool closeEnough(const sf::Vector2i &first, const sf::Vector2i &second, const float &radius)
{
	return (std::abs(first.x - second.x) < radius && std::abs(first.y - second.y) < radius);
}

const std::string toString(const sf::Vector2i &v)
{
	return v.x + ", " + v.y;
}

EventInput::EventInput(sf::RenderWindow & window, std::queue<sf::Event>& eventQueue)
	:
	r_window(window),
	r_eventQueue(eventQueue),
	prevMousePos(sf::Mouse::getPosition(window))
{
}

void EventInput::checkForInput()
{
	sf::Event evnt;

	while (r_window.pollEvent(evnt))
	{
		switch (evnt.type)
		{
			case sf::Event::MouseMoved:
			{
				sf::Vector2i pos = sf::Mouse::getPosition(r_window);

				if (pos != sf::Vector2i(r_window.getSize() / 2U))		// sf::Mouse::setPosition calls sf::Event::Moved, therefore skip if this is caused by that
				{
					r_eventQueue.push(evnt);
				}
			}
			break;
			default:
			{
				r_eventQueue.push(evnt);
			}
		}
	}
}
