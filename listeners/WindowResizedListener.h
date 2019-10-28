#pragma once
#include "EventListener.h"

namespace sf { class RenderWindow; }

class WindowResizedListener : public EventListener
{
public:
	WindowResizedListener(RenderSettings &renderSettings, const sf::RenderWindow &window);

	bool handleEvent(const sf::Event &evnt);

private:
	const sf::RenderWindow &r_window;
};