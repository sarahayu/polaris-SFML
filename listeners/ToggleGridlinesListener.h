#pragma once
#include "EventListener.h"

class Menu;

class ToggleGridlinesListener : public EventListener
{
public:
	ToggleGridlinesListener(RenderSettings &renderSettings);

	bool handleEvent(const sf::Event &evnt);
};