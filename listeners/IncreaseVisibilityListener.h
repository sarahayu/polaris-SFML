#pragma once
#include "EventListener.h"

namespace sf { class RenderWindow; }

class IncreaseVisibilityListener : public EventListener
{
public:
	IncreaseVisibilityListener(RenderSettings &renderSettings);

	bool handleEvent(const sf::Event &evnt);
};