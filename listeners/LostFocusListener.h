#pragma once
#include "EventListener.h"

namespace sf { class RenderWindow; }

class LostFocusListener : public EventListener
{
public:
	LostFocusListener(RenderSettings &renderSettings, sf::RenderWindow &window);

	bool handleEvent(const sf::Event &evnt);

private:
	sf::RenderWindow &r_window;
};