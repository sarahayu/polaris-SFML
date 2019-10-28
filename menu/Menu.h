#pragma once
#include "GUI.h"
#include <unordered_map>
#include <assert.h>
#include <functional>

class CelestialSphere;

class Menu
{
public:
	//const float menuWidth = 350.0f;		// TODO move this?

	Menu(const sf::RenderWindow &window, RenderSettings &renderSettings, CelestialSphere &celestialSphere);

	void draw(sf::RenderWindow & window);

	bool receiveInput(const sf::Event &evnt);

	const sf::FloatRect getBounds() const;

private:

	void initializeMenu(const sf::RenderWindow & window);

	void loadCities();
	void parseSettings();
	const std::string calculateGST();
	void resetTime();
	void toggleLocator();

	struct CityInfo { float latitude; std::string timezoneStr; };
	std::map<std::string, CityInfo> m_cityInfo;

	GUI m_gui;

	CelestialSphere &r_celestialSphere;
	RenderSettings &r_renderSettings;

	bool usingCities = false;

	//sf::RenderWindow &r_window;
};	