#include "WindowCloseListener.h"
#include <SFML/Graphics.hpp>
#include <assert.h>

WindowCloseListener::WindowCloseListener(RenderSettings & renderSettings, sf::RenderWindow &window) 
	: EventListener(renderSettings), r_window(window)
{
}

bool WindowCloseListener::handleEvent(const sf::Event & evnt)
{
	r_window.close();

	return true;
}
