#include "Button.h"
#include <iostream>

void drawMargin(sf::RenderWindow &window, const sf::FloatRect &bounds)
{
	sf::RectangleShape foo({ bounds.width, bounds.height });
	foo.setOutlineColor(sf::Color::Red);
	foo.setFillColor(sf::Color::Transparent);
	foo.setOutlineThickness(1.0f);
	foo.setPosition(bounds.left, bounds.top);

	window.draw(foo);
}


Button::Button(const sf::Text & text, const sf::Vector2f & location, const sf::Vector2f & boxSize, const sf::Color & color, const std::string & iconFilename, const sf::Vector2f & parentLocation)
	: MenuItem(location, parentLocation), m_text(text), m_box(boxSize)
{
	m_text.setFillColor(color);
	m_box.setFillColor(sf::Color::Transparent);
	m_box.setOutlineColor(color);
	m_box.setOutlineThickness(1.0f);

	sf::FloatRect textBounds = m_text.getGlobalBounds();
	const sf::Font &textFont = *text.getFont();
	textBounds.height = textFont.getGlyph(81, text.getCharacterSize(), false).bounds.height;		// set bounds to height of character 'Q'
	float lineSpace = textFont.getLineSpacing(text.getCharacterSize()) - textBounds.height;
	float upDownPadding = (boxSize.y - textBounds.height) / 2.0f;

	if (iconFilename != "")
	{
		hasIcon = true;
		float iconWidth = textBounds.height + 6.0f;
		
		m_iconTex.loadFromFile(iconFilename);
		m_iconTex.setSmooth(true);
		m_icon.setSize({ iconWidth, iconWidth });
		m_icon.setTexture(&m_iconTex);
		m_icon.setFillColor(color);
		float contentWidth = iconWidth + textBounds.width + 10.0f;
		sf::Vector2f upLeft((boxSize.x - contentWidth) / 2.0f, upDownPadding);
		m_iconOffset = upLeft  + sf::Vector2f(0.0f, -3.0f);
		m_textOffset = upLeft + sf::Vector2f(iconWidth + 10.0f, -lineSpace);
	}
	else
	{
		m_textOffset = { (boxSize.x - textBounds.width) / 2.0f, upDownPadding - lineSpace };
	}

}

void Button::drawItem(sf::RenderWindow & window)
{
	sf::Vector2f buttonLoc = m_parentLocation + m_location;

	m_box.setPosition(buttonLoc);
	m_text.setPosition(buttonLoc + m_textOffset);

	window.draw(m_box);
	window.draw(m_text);
	//drawMargin(window, m_text.getGlobalBounds());


	if (hasIcon)
	{
		m_icon.setTexture(&m_iconTex);
		m_icon.setPosition(buttonLoc + m_iconOffset);

		window.draw(m_icon);
		//drawMargin(window, m_icon.getGlobalBounds());
	}
}

void Button::connect(const std::function<void()>& callback)
{
	m_callback = callback;
}

void Button::setLabel(const std::string & text, const std::string & iconFilename)
{
	m_text.setString(text);
	sf::FloatRect textBounds = m_text.getGlobalBounds();
	sf::Vector2f boxSize(m_box.getSize());
	const sf::Font &textFont = *m_text.getFont();
	textBounds.height = textFont.getGlyph(81, m_text.getCharacterSize(), false).bounds.height;		// set bounds to height of character 'Q'
	float lineSpace = textFont.getLineSpacing(m_text.getCharacterSize()) - textBounds.height;
	float upDownPadding = (boxSize.y - textBounds.height) / 2.0f;
	float iconWidth = textBounds.height + 6.0f;

	if (iconFilename != "")
	{
		hasIcon = true;

		m_iconTex.loadFromFile(iconFilename);
		m_iconTex.setSmooth(true);
		m_icon.setSize({ iconWidth, iconWidth });
		m_icon.setTexture(&m_iconTex);
		m_icon.setFillColor(m_text.getFillColor());
	}

	if (hasIcon) // may have already had icon from before
	{
		float contentWidth = iconWidth + textBounds.width + 10.0f;
		sf::Vector2f upLeft((boxSize.x - contentWidth) / 2.0f, upDownPadding);
		m_iconOffset = upLeft + sf::Vector2f(0.0f, -3.0f);
		m_textOffset = upLeft + sf::Vector2f(iconWidth + 10.0f, -lineSpace);
	}
	else
	{
		hasIcon = false;
		m_textOffset = { (boxSize.x - textBounds.width) / 2.0f, upDownPadding - lineSpace };
	}
}

const bool Button::isWithinBounds(const sf::Vector2f & location)
{
	return m_box.getGlobalBounds().contains(location);
}

void Button::callback()
{
	if (m_callback) m_callback();
}
