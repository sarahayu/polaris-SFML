#include "ToggleMenuListener.h"
#include "../menu/Menu.h"

ToggleMenuListener::ToggleMenuListener(RenderSettings & renderSettings, const Menu &menu)
	: EventListener(renderSettings), r_menu(menu)
{
}

bool ToggleMenuListener::handleEvent(const sf::Event & evnt)
{
	if (!r_renderSettings.windowFocused) return false;

	if (evnt.key.code == sf::Keyboard::F1)
	{
		if (r_renderSettings.displayMenu)
		{
			r_renderSettings.displayMenu = false;
			r_renderSettings.menuBounds = sf::FloatRect();
		}
		else
		{
			r_renderSettings.displayMenu = true;
			r_renderSettings.menuBounds = r_menu.getBounds();
		}
		return true;
	}

	return false;
}
