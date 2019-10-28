#include "WindowReenterListener.h"

WindowReenterListener::WindowReenterListener(RenderSettings & renderSettings, sf::RenderWindow & window)
	: EventListener(renderSettings), r_window(window)
{
}

bool WindowReenterListener::handleEvent(const sf::Event & evnt)
{
	if (evnt.mouseButton.button == sf::Mouse::Left)
	{
		if (!r_renderSettings.menuBounds.contains(sf::Vector2f(evnt.mouseButton.x, evnt.mouseButton.y)))
		{
			r_renderSettings.mouseGrabbed = true;
			r_window.setMouseCursorVisible(false);
			sf::Mouse::setPosition(sf::Vector2i(r_window.getSize() / 2U), r_window);
		}
	}
	return false;
}
