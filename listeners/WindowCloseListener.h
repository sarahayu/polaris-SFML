#pragma once
#include "EventListener.h"

namespace sf { class RenderWindow; }

class WindowCloseListener : public EventListener
{
public:
	WindowCloseListener(RenderSettings &renderSettings, sf::RenderWindow &window);

	bool handleEvent(const sf::Event &evnt);

private:

	sf::RenderWindow &r_window;
};