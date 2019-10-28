#include "ViewMoveListener.h"
#include <assert.h>
#include <iostream>

const float PI = 3.14159265358f;
const float MOVE_SPEED = PI / 50.0f / 8.0f;

ViewMoveListener::ViewMoveListener(RenderSettings & renderSettings, const sf::RenderWindow &window)
	: EventListener(renderSettings), r_window(window)
{
}

bool ViewMoveListener::handleEvent(const sf::Event & evnt)
{
	if (!r_renderSettings.windowFocused) return false;
	if (!r_renderSettings.mouseGrabbed) return false;

	const sf::Vector2i windowCenter(r_window.getSize() / 2U);
	const sf::Vector2i movement = windowCenter - sf::Vector2i(evnt.mouseMove.x, evnt.mouseMove.y);

	sf::Vector2f total = r_renderSettings.viewOffset + sf::Vector2f(movement.x, movement.y) * MOVE_SPEED;

	total.y = std::min(std::max(-PI / 2.0f, total.y), PI / 2.0f);

	r_renderSettings.viewOffset = total;

	sf::Mouse::setPosition(windowCenter, r_window);

	return true;
}
