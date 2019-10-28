#pragma once
#include "items/Button.h"
#include "items/MenuText.h"
#include "items/MenuTextBox.h"
#include "items/HorizontalLine.h"
#include <assert.h>

struct RenderSettings;

class GUI
{
public:
	GUI(const sf::RenderWindow & window);

	void draw(sf::RenderWindow &window, const RenderSettings &renderSettings);

	bool receiveInput(const sf::Event &evnt, const RenderSettings & renderSettings);

	const sf::FloatRect getBounds() const;
	const sf::Font& getFont() const;


	template <class T>
	std::shared_ptr<T> createItem(const std::string &key, const T& tMenuItem);
	MenuTextBox::Ptr createTextBox(const std::string &key, const MenuTextBox &textBox);
	Button::Ptr createButton(const std::string &key, const Button &button);

	template <class T>
	std::shared_ptr<T> getItem(const std::string &key);
	MenuTextBox::Ptr getTextBox(const std::string &key);
	MenuText::Ptr getText(const std::string &key);

private:

	std::map<const std::string, MenuItem::Ptr> m_menuItems;

	std::vector<MenuTextBox::Ptr> m_textBoxes;		// for updating text boxes and the like
	std::vector<Button::Ptr> m_buttons;		// for checking clicks

	sf::RectangleShape m_menuRect;

	sf::Font m_fgothic;

	MenuTextBox::Ptr p_activeTextBox;
};

template<class T>
inline std::shared_ptr<T> GUI::createItem(const std::string &key, const T & menuItem)
{
	std::shared_ptr<T> itemPtr = std::make_shared<T>(menuItem);
	m_menuItems[key] = itemPtr;
	return itemPtr;
}

template<class T>
inline std::shared_ptr<T> GUI::getItem(const std::string & key)
{
	assert(m_menuItems.find(key) != m_menuItems.end());

	std::shared_ptr<T> item = std::dynamic_pointer_cast<T>(m_menuItems.at(key));
	assert(item != nullptr);

	return item;
}