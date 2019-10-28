#include "ToggleGridlinesListener.h"

ToggleGridlinesListener::ToggleGridlinesListener(RenderSettings & renderSettings)
	: EventListener(renderSettings)
{
}

bool ToggleGridlinesListener::handleEvent(const sf::Event & evnt)
{
	if (!r_renderSettings.windowFocused) return false;

	if (evnt.key.code == sf::Keyboard::F3)
	{
		r_renderSettings.drawGridlines = !r_renderSettings.drawGridlines;
		return true;
	}

	return false;
}
