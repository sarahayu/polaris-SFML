#include "GainedFocusListener.h"

GainedFocusListener::GainedFocusListener(RenderSettings & renderSettings, sf::RenderWindow &window)
	: EventListener(renderSettings), r_window(window)
{
}

bool GainedFocusListener::handleEvent(const sf::Event & evnt)
{
	r_renderSettings.windowFocused = true;
	r_window.setMouseCursorVisible(false);

	return false;
}
