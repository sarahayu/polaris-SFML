#pragma once
#include <SFML/Graphics.hpp>

struct RenderSettings
{
	float brightnessFactor = 0.28f;
	bool displayName = true;
	float zoomFactor = 1.0f;
	sf::Vector2f viewOffset;
	//sf::Vector2f skyOffset;
	bool mouseGrabbed = true;
	bool windowFocused = true;
	bool displayMenu = true;
	bool drawGridlines = true;
	sf::FloatRect menuBounds;	// for mouse reenter
	sf::FloatRect menuView;
	float GST = 0.0f;
	sf::Vector2f windowSize;
};