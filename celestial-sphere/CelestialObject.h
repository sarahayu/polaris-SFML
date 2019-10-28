#pragma once
#include <SFML\Graphics.hpp>

struct CelestialObject
{
	std::string name;
	sf::Color color;
	float magnitude;
	sf::Vector3f coordinates;
};
