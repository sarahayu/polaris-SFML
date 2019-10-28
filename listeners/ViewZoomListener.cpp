#include "ViewZoomListener.h"
#include <assert.h>
#include <iostream>
#include "../Settings.h"

const float ZOOM_ADD = 0.1f;	// per mouse tick

ViewZoomListener::ViewZoomListener(RenderSettings & renderSettings)
	: EventListener(renderSettings)
{
}

bool ViewZoomListener::handleEvent(const sf::Event & evnt)
{
	if (!r_renderSettings.windowFocused) return false;
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) return false;
	if (!r_renderSettings.mouseGrabbed) return false;

	const float newZoom = r_renderSettings.zoomFactor + ZOOM_ADD * -evnt.mouseWheelScroll.delta;

	if (newZoom < Settings::ZOOM_MAX && newZoom > Settings::ZOOM_MIN) r_renderSettings.zoomFactor = newZoom;

	return true;
}
