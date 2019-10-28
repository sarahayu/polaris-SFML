#include "KeyPressListener.h"

KeyPressListener::KeyPressListener(RenderSettings & renderSettings)
	: EventListener(renderSettings)
{
}

bool KeyPressListener::handleEvent(const sf::Event & evnt)
{
	if (!r_renderSettings.windowFocused) return false;

	if (evnt.key.code == sf::Keyboard::F2)
	{
		r_renderSettings.displayName = !r_renderSettings.displayName;
		return true;
	}

	return false;
}
