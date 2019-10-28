#include "GUI.h"
#include "../RenderSettings.h"
#include "../utils/StringUtils.h"
#include <iostream>

const float MENU_WIDTH = 350.0f;

GUI::GUI(const sf::RenderWindow & window)
	: p_activeTextBox(nullptr)
{
	m_fgothic.loadFromFile("resources/franklin-gothic.ttf");

	sf::Vector2f menuLoc(window.getSize().x - MENU_WIDTH, 0.0f);
	m_menuRect.setPosition(menuLoc);
	m_menuRect.setFillColor(sf::Color(8, 8, 8, 173));
	m_menuRect.setSize({ MENU_WIDTH, (float)window.getSize().y });
}

void GUI::draw(sf::RenderWindow & window, const RenderSettings & renderSettings)
{
	sf::Vector2f windowSize(window.getSize());
	window.setView(sf::View(windowSize / 2.0f, windowSize));

	sf::Vector2f menuLoc(window.getSize().x - MENU_WIDTH, 0.0f);
	m_menuRect.setPosition(menuLoc);
	m_menuRect.setSize({ MENU_WIDTH, (float)window.getSize().y });
	window.draw(m_menuRect);

	for (auto &itemPair : m_menuItems)
	{
		const MenuItem::Ptr &item = itemPair.second;
		item->setParentLocation(menuLoc);
		item->draw(window);
	}
}

bool GUI::receiveInput(const sf::Event & evnt, const RenderSettings & renderSettings)
{
	if (!renderSettings.windowFocused) return false;

	switch (evnt.type)
	{
	case sf::Event::MouseButtonPressed:
	{
		sf::Vector2i mousePos(evnt.mouseButton.x, evnt.mouseButton.y);

		for (MenuTextBox::Ptr &tBox : m_textBoxes)
		{
			if (!tBox->show) continue;

			if (tBox->isWithinBounds(sf::Vector2f(mousePos)))
			{
				std::cout << "\nPressed! " << tBox->getName();
				if (p_activeTextBox != nullptr) p_activeTextBox->active = false;

				tBox->active = true;
				p_activeTextBox = tBox;
				return true;
			}
		}

		// did not click any text box //

		if (p_activeTextBox != nullptr)
		{
			std::cout << "\nTextbox : " << p_activeTextBox->getName();
			//parseSettings();
			p_activeTextBox->callback();
			p_activeTextBox->active = false;
			p_activeTextBox = nullptr;
			return true;
		}
	}
	break;
	case sf::Event::MouseButtonReleased:
	{
		if (evnt.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2f buttonLoc(evnt.mouseButton.x, evnt.mouseButton.y);
			for (auto &button : m_buttons)
			{
				if (!button->show) continue;

				if (button->isWithinBounds(buttonLoc))
				{
					button->callback();
					return true;
				}

			}
		}
	}
	break;
	case sf::Event::KeyPressed:
	{
		if (p_activeTextBox != nullptr)
		{
			if (evnt.key.code == sf::Keyboard::BackSpace)
			{
				p_activeTextBox->backspace();
				return true;
			}

			if (evnt.key.code == sf::Keyboard::Return)
			{
				std::cout << "\nTextbox : " << p_activeTextBox->getName();
				std::cout << "\nValue : " << p_activeTextBox->getValue();
				//parseSettings();
				p_activeTextBox->callback();
				p_activeTextBox->active = false;
				p_activeTextBox = nullptr;
				return true;
			}
		}
	}
	break;
	case sf::Event::TextEntered:
	{
		if (p_activeTextBox != nullptr)
		{
			if (su::isChar(evnt.text.unicode))
			{
				std::string c;
				c = (char)evnt.text.unicode;
				p_activeTextBox->addText(c);

				return true;
			}
		}
	}
	break;
	}

	return false;
}

const sf::FloatRect GUI::getBounds() const
{
	return m_menuRect.getGlobalBounds();
}

const sf::Font & GUI::getFont() const
{
	return m_fgothic;
}

MenuTextBox::Ptr GUI::createTextBox(const std::string & key, const MenuTextBox & textBox)
{
	MenuTextBox::Ptr pTextBox = createItem(key, textBox);
	m_textBoxes.push_back(pTextBox);
	return pTextBox;
}

Button::Ptr GUI::createButton(const std::string & key, const Button & button)
{
	Button::Ptr pButton = createItem(key, button);
	m_buttons.push_back(pButton);
	return pButton;
}

MenuTextBox::Ptr GUI::getTextBox(const std::string & key)
{
	return getItem<MenuTextBox>(key);
}

MenuText::Ptr GUI::getText(const std::string & key)
{
	return getItem<MenuText>(key);
}
