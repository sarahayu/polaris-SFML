#pragma once
#include <queue>
#include <map>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "listeners/EventListener.h"
#include "EventInput.h"
#include "menu/Menu.h"
#include "RealTimeInput.h"
#include "Renderer.h"
#include "RenderSettings.h"
#include "celestial-sphere/CelestialSphere.h"


class Polaris
{
public:

	Polaris();

	void loop();

private:

	const std::queue<sf::Event> checkForInput();

	sf::RenderWindow m_window;

	CelestialSphere m_celestialSphere;
	
	std::vector<std::pair<sf::Event::EventType, EventListener_UP>> m_listeners;
	//std::map<sf::Event::EventType, std::vector<EventListener_UP> > m_listenerMap;
	RenderSettings m_renderSettings;

	Menu m_menu;

	RealTimeInput m_realTime;

	Renderer m_renderer;
};