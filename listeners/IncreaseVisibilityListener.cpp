#include "IncreaseVisibilityListener.h"
#include <assert.h>
#include <iostream>
#include "../Settings.h"

const float BRIGHT_ADD = 0.02f;	// per mouse tick

IncreaseVisibilityListener::IncreaseVisibilityListener(RenderSettings & renderSettings)
	: EventListener(renderSettings)
{
}

bool IncreaseVisibilityListener::handleEvent(const sf::Event & evnt)
{
	if (!r_renderSettings.windowFocused) return false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) return false;
	if (!r_renderSettings.mouseGrabbed) return false;

	const float newBright = r_renderSettings.brightnessFactor + BRIGHT_ADD * -evnt.mouseWheelScroll.delta;

	if (newBright < Settings::BRIGHT_MAX && newBright > Settings::BRIGHT_MIN) r_renderSettings.brightnessFactor = newBright;

	return true;
}
