#pragma once
#include "EventListener.h"

class Menu;

class WindowResizedListener : public EventListener
{
public:
	WindowResizedListener(RenderSettings &renderSettings, const Menu &menu);

	bool handleEvent(const sf::Event &evnt);

private:
	const Menu &r_menu;
};