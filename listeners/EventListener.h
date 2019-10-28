#pragma once
#include "../RenderSettings.h"

namespace sf { class Event; }

class EventListener
{
public:
	EventListener(RenderSettings &renderSettings);
	virtual ~EventListener() {};

	virtual bool handleEvent(const sf::Event &evnt) = 0;		// return true if event modifies RenderSettings

protected:
	RenderSettings &r_renderSettings;
};