#include "WindowResizedListener.h"
#include <iostream>

WindowResizedListener::WindowResizedListener(RenderSettings & renderSettings, const sf::RenderWindow & window)
	: EventListener(renderSettings), r_window(window)
{
}

bool WindowResizedListener::handleEvent(const sf::Event & evnt)
{
	/* FIX THIS SOMETIME
	if (r_renderSettings.mouseGrabbed)
	{
		//sf::Mouse::setPosition(sf::Vector2i(evnt.size.width, evnt.size.height) / 2, r_window);
	}

	std::cout << "\n" << (std::rand() % 10000) << " Resized : " << sf::Mouse::getPosition(r_window).x << ", " << sf::Mouse::getPosition(r_window).y;
*/
	return true;
}
