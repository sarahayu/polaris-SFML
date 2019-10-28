#pragma once
#include <memory>

class EventListener;
namespace sf { class Event; }

typedef std::unique_ptr<EventListener> EventListener_UP;
typedef std::unique_ptr<sf::Event> Event_UP;