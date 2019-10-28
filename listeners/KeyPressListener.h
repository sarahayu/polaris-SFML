#pragma once
#include "EventListener.h"

class KeyPressListener : public EventListener
{
public:
	KeyPressListener(RenderSettings &renderSettings);

	bool handleEvent(const sf::Event &evnt);
};