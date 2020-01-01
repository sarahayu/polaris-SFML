#include "WindowResizedListener.h"
#include "../menu/Menu.h"

WindowResizedListener::WindowResizedListener(RenderSettings & renderSettings, const Menu &menu)
	: EventListener(renderSettings), r_menu(menu)
{
}

bool WindowResizedListener::handleEvent(const sf::Event & evnt)
{
	r_renderSettings.menuBounds = r_menu.getBounds();
	/* FIX THIS SOMETIME
	if (r_renderSettings.mouseGrabbed)
	{
		//sf::Mouse::setPosition(sf::Vector2i(evnt.size.width, evnt.size.height) / 2, r_window);
	}

	std::cout << "\n" << (std::rand() % 10000) << " Resized : " << sf::Mouse::getPosition(r_window).x << ", " << sf::Mouse::getPosition(r_window).y;
*/
	return true;
}
