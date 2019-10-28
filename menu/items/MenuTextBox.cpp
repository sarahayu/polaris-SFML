#include "MenuTextBox.h"
#include <iostream>

const sf::Vector2f size(const sf::FloatRect &bounds)
{
	return sf::Vector2f(bounds.width, bounds.height);
}

const std::string f(const sf::FloatRect &fl)
{
	return "" + std::to_string(fl.left) + ", " + std::to_string(fl.top) + ", " + std::to_string(fl.width) + ", " + std::to_string(fl.height);
}

const std::string v(const sf::Vector2f &ve)
{
	return "" + std::to_string(ve.x) + ", " + std::to_string(ve.y);
}

MenuTextBox::MenuTextBox(const std::string &name, const sf::Text & text, const sf::Vector2f & location, const sf::Vector2f &padding, const sf::Vector2f & parentLocation, const sf::Color & color, const sf::Color & boxColor, const int &maxBuffer)
	: MenuItem(location, parentLocation), m_name(name), m_text(text), m_padding(padding), m_value(text.getString()), m_textHeight(text.getFont()->getGlyph(65, text.getCharacterSize(), false).bounds.height), m_maxBuffer(maxBuffer)
{
	m_box.setFillColor(boxColor);
	m_text.setFillColor(color);
}

void MenuTextBox::drawItem(sf::RenderWindow & window)
{
	m_text.setPosition(m_parentLocation + m_location + m_padding - sf::Vector2f(0.0f, 5.0f));
	m_box.setPosition(m_parentLocation + m_location);

	if (active) m_text.setString(m_value + "_");
	else m_text.setString(m_value);

	sf::Vector2f boxSize(m_text.getGlobalBounds().width + m_padding.x * 2.0f, m_textHeight + m_padding.y * 2.0f);
	m_box.setSize(boxSize);

	//std::cout << "\nBounds : " << f(m_text.getGlobalBounds());

	window.draw(m_box);
	window.draw(m_text);
}

void MenuTextBox::connect(const std::function<void()>& callback)
{
	m_callback = callback;
}

void MenuTextBox::backspace()
{
	m_value = m_value.substr(0, m_value.size() - 1);
}

const bool MenuTextBox::isWithinBounds(const sf::Vector2f & location)
{
	return m_box.getGlobalBounds().contains(location);
}

const std::string MenuTextBox::getName() const
{
	return m_name;
}

void MenuTextBox::addText(const std::string & text)
{
	if ((m_value + text).length() <= m_maxBuffer) m_value += text;
}

const std::string MenuTextBox::getValue() const
{
	return m_value;
}

void MenuTextBox::setValue(const std::string & str)
{
	if (str.length() > m_maxBuffer) m_value = str.substr(0, m_maxBuffer);
	else m_value = str;
}

void MenuTextBox::callback()
{
	if (m_callback) m_callback();
}
