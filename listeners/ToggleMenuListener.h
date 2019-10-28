#pragma once
#include "EventListener.h"

class Menu;

class ToggleMenuListener : public EventListener
{
public:
	ToggleMenuListener(RenderSettings &renderSettings, const Menu &menu);

	bool handleEvent(const sf::Event &evnt);

private:
	const Menu &r_menu;
};