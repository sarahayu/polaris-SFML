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
	m_window.setMouseCursorVisible(false);											// TODO remove later
	m_window.setKeyRepeatEnabled(false);

	sf::Mouse::setPosition(sf::Vector2i(m_window.getSize()/ 2U), m_window);

	m_listenerMap[sf::Event::Closed].push_back(std::make_unique<WindowCloseListener>(m_renderSettings, m_window));
	m_listenerMap[sf::Event::GainedFocus].push_back(std::make_unique<GainedFocusListener>(GainedFocusListener(m_renderSettings, m_window)));
	m_listenerMap[sf::Event::MouseMoved].push_back(std::make_unique<ViewMoveListener>(ViewMoveListener(m_renderSettings, m_window)));
	m_listenerMap[sf::Event::MouseWheelScrolled].push_back(std::make_unique<ViewZoomListener>(ViewZoomListener(m_renderSettings)));
	m_listenerMap[sf::Event::MouseWheelScrolled].push_back(std::make_unique<IncreaseVisibilityListener>(IncreaseVisibilityListener(m_renderSettings)));
	m_listenerMap[sf::Event::KeyPressed].push_back(std::make_unique<UnbindMouseListener>(UnbindMouseListener(m_renderSettings, m_window)));
	m_listenerMap[sf::Event::KeyPressed].push_back(std::make_unique<KeyPressListener>(KeyPressListener(m_renderSettings)));
	m_listenerMap[sf::Event::KeyPressed].push_back(std::make_unique<ToggleMenuListener>(ToggleMenuListener(m_renderSettings, m_menu)));
	m_listenerMap[sf::Event::KeyPressed].push_back(std::make_unique<ToggleGridlinesListener>(ToggleGridlinesListener(m_renderSettings)));
	m_listenerMap[sf::Event::LostFocus].push_back(std::make_unique<LostFocusListener>(LostFocusListener(m_renderSettings, m_window)));
	m_listenerMap[sf::Event::MouseButtonPressed].push_back(std::make_unique<WindowReenterListener>(WindowReenterListener(m_renderSettings, m_window)));
	m_listenerMap[sf::Event::Resized].push_back(std::make_unique<WindowResizedListener>(WindowResizedListener(m_renderSettings, m_window)));

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

			auto &listeners = m_listenerMap.find(evnt.type);

			if (listeners != m_listenerMap.end())
			{
				for (auto &listener : listeners->second)
				{
					renderChanged = listener->handleEvent(evnt) || renderChanged;
				}
			}

		}

		renderChanged = m_realTime.checkForInput() || renderChanged;

		if (renderChanged)
		{
			m_renderer.render();
		}
	}
}
