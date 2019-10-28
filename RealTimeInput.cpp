#include "RealTimeInput.h"
#include "RenderSettings.h"
#include "Settings.h"
#include <iostream>

const float BRIGHTNESS_FACTOR = 0.5f;
const float ZOOM_FACTOR = 0.5f;

RealTimeInput::RealTimeInput(RenderSettings & renderSettings)
	: r_renderSettings(renderSettings)
{
}

bool RealTimeInput::checkForInput()
{
	float deltaTime = m_clock.restart().asSeconds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			float add = -ZOOM_FACTOR * deltaTime;
			if (r_renderSettings.zoomFactor + add > Settings::ZOOM_MIN)
			{
				r_renderSettings.zoomFactor += add;
				return true;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			float add = ZOOM_FACTOR * deltaTime;
			if (r_renderSettings.zoomFactor + add < Settings::ZOOM_MAX)
			{
				r_renderSettings.zoomFactor += add;
				return true;
			}
		}
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			float add = -BRIGHTNESS_FACTOR * deltaTime;
			if (r_renderSettings.brightnessFactor + add > Settings::BRIGHT_MIN)
			{
				r_renderSettings.brightnessFactor += add;
				std::cout << "\n" << r_renderSettings.brightnessFactor;
				return true;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			float add = BRIGHTNESS_FACTOR * deltaTime;
			if (r_renderSettings.brightnessFactor + add < Settings::BRIGHT_MAX)
			{
				r_renderSettings.brightnessFactor += add;
				std::cout << "\n" << r_renderSettings.brightnessFactor;
				return true;
			}
		}
	}

	return false;
}
