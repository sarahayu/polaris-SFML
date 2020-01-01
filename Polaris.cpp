#include "Polaris.h"
#include "listeners/GainedFocusListener.h"
#include "listeners/IncreaseVisibilityListener.h"
#include "listeners/KeyPressListener.h"
#include "listeners/LostFocusListener.h"
#include "listeners/ToggleGridlinesListener.h"
#include "listeners/ToggleMenuListener.h"
#include "listeners/UnbindMouseListener.h"
#include "listeners/WindowCloseListener.h"
#include "listeners/WindowReenterListener.h"
#include "listeners/WindowResizedListener.h"
#include "listeners/ViewMoveListener.h"
#include "listeners/ViewZoomListener.h"
#include <iostream>

const unsigned int WINDOW_WIDTH = 1200U, WINDOW_HEIGHT = 900U;

template <class T>
void printQ(std::queue<T> q)
{
	std::cout << "\nContents : ";

	std::cout << "[ " << q.front().type;
	q.pop();

	while (!q.empty())
	{
		std::cout << ", " << q.front().type;
		q.pop();
	}

	std::cout << " ]";
}

const std::queue<sf::Event> Polaris::checkForInput()
{
	sf::Event evnt;
	std::queue<sf::Event> eventQueue;

	while (m_window.pollEvent(evnt))
	{
		switch (evnt.type)
		{
			case sf::Event::MouseMoved:
			{
				sf::Vector2i pos = sf::Mouse::getPosition(m_window);

				if (pos != sf::Vector2i(m_window.getSize() / 2U))		// sf::Mouse::setPosition calls sf::Event::Moved, therefore skip if this is caused by that
				{
					eventQueue.push(evnt);
				}
			}
			break;
			case sf::Event::Resized:
				m_renderSettings.windowSize = sf::Vector2f(evnt.size.width, evnt.size.height);
			default:
			{
				eventQueue.push(evnt);
			}
		}
	}

	return eventQueue;
}

Polaris::Polaris()
	:
	m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Polaris"),
	m_menu(m_window,m_renderSettings, m_celestialSphere),
	m_realTime(m_renderSettings),
	m_renderer(m_window, m_menu, m_renderSettings, m_celestialSphere)
{
	//m_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Polaris");

	m_renderSettings.menuBounds = m_menu.getBounds();
	m_renderSettings.windowSize = sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_window.setMouseCursorVisible(false);											// TODO remove later
	m_window.setKeyRepeatEnabled(false);

	sf::Mouse::setPosition(sf::Vector2i(m_window.getSize()/ 2U), m_window);

	m_listeners.push_back({ sf::Event::Closed,std::make_unique<WindowCloseListener>(m_renderSettings, m_window) });
	m_listeners.push_back({ sf::Event::GainedFocus,std::make_unique<GainedFocusListener>(GainedFocusListener(m_renderSettings, m_window)) });
	m_listeners.push_back({ sf::Event::Resized,std::make_unique<WindowResizedListener>(WindowResizedListener(m_renderSettings, m_menu)) });
	m_listeners.push_back({ sf::Event::MouseMoved,std::make_unique<ViewMoveListener>(ViewMoveListener(m_renderSettings, m_window)) });
	m_listeners.push_back({ sf::Event::MouseWheelScrolled,std::make_unique<ViewZoomListener>(ViewZoomListener(m_renderSettings)) });
	m_listeners.push_back({ sf::Event::MouseWheelScrolled,std::make_unique<IncreaseVisibilityListener>(IncreaseVisibilityListener(m_renderSettings)) });
	m_listeners.push_back({ sf::Event::KeyPressed,std::make_unique<UnbindMouseListener>(UnbindMouseListener(m_renderSettings, m_window)) });
	m_listeners.push_back({ sf::Event::KeyPressed,std::make_unique<KeyPressListener>(KeyPressListener(m_renderSettings)) });
	m_listeners.push_back({ sf::Event::KeyPressed,std::make_unique<ToggleMenuListener>(ToggleMenuListener(m_renderSettings, m_menu)) });
	m_listeners.push_back({ sf::Event::KeyPressed,std::make_unique<ToggleGridlinesListener>(ToggleGridlinesListener(m_renderSettings)) });
	m_listeners.push_back({ sf::Event::LostFocus,std::make_unique<LostFocusListener>(LostFocusListener(m_renderSettings, m_window)) });
	m_listeners.push_back({ sf::Event::MouseButtonPressed,std::make_unique<WindowReenterListener>(WindowReenterListener(m_renderSettings, m_window)) });

	m_renderer.render();

	loop();
}

void Polaris::loop()
{
	while (m_window.isOpen())
	{
		std::queue<sf::Event> eventQueue = checkForInput();
		bool renderChanged = false;

		//if (!m_eventQueue.empty()) printQ(m_eventQueue);

		while (!eventQueue.empty())
		{
			const sf::Event &evnt = eventQueue.front();
			eventQueue.pop();

			if (m_renderSettings.displayMenu)
			{
				if (m_menu.receiveInput(evnt))
				{
					renderChanged = true;
					break;
				}
			}

			for (auto &listener : m_listeners)
				if (listener.first == evnt.type)
					renderChanged = listener.second->handleEvent(evnt) || renderChanged;

		}

		renderChanged = m_realTime.checkForInput() || renderChanged;

		if (renderChanged)
		{
			m_renderer.render();
		}
	}
}
