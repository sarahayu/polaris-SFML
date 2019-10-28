#include "UnbindMouseListener.h"
#include <iostream>

UnbindMouseListener::UnbindMouseListener(RenderSettings & renderSettings, sf::RenderWindow &window)
	: EventListener(renderSettings), r_window(window)
{
}

bool UnbindMouseListener::handleEvent(const sf::Event & evnt)
{
	if (!r_renderSettings.windowFocused) return false;

	if (evnt.key.code == sf::Keyboard::Escape)
	{
		r_renderSettings.mouseGrabbed = false;
		r_window.setMouseCursorVisible(true);
	}
	return false;
}
