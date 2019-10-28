#pragma once
#include "EventListener.h"

namespace sf { class RenderWindow; }

class ViewZoomListener : public EventListener
{
public:
	ViewZoomListener(RenderSettings &renderSettings);

	bool handleEvent(const sf::Event &evnt);
};