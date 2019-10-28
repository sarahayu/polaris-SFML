#include "LostFocusListener.h"

LostFocusListener::LostFocusListener(RenderSettings & renderSettings, sf::RenderWindow &window)
	: EventListener(renderSettings), r_window(window)
{
}

bool LostFocusListener::handleEvent(const sf::Event & evnt)
{
	r_renderSettings.windowFocused = false;
	r_window.setMouseCursorVisible(true);

	return false;
}
