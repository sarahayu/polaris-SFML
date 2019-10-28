#pragma once
#include <SFML/Graphics.hpp>

struct RenderSettings;

class RealTimeInput
{
public:
	RealTimeInput(RenderSettings &renderSettings);

	bool checkForInput();

private:
	RenderSettings &r_renderSettings;
	sf::Clock m_clock;
};