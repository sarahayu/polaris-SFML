#pragma once
#include "MenuItem.h"
#include <functional>

class Button : public MenuItem
{
public:

	typedef std::shared_ptr<Button> Ptr;

	Button(const sf::Text &text, const sf::Vector2f &location, const sf::Vector2f &boxSize, const sf::Color &color = sf::Color::White, const std::string &iconFilename = "", const sf::Vector2f &parentLocation = sf::Vector2f());

	void drawItem(sf::RenderWindow &window);
	void connect(const std::function<void()> &callback);
	void setLabel(const std::string &text, const std::string &iconFilename = "");
	const bool isWithinBounds(const sf::Vector2f &location);
	void callback();

private:
	sf::Text m_text;
	sf::RectangleShape m_box;
	bool hasIcon = false;
	sf::RectangleShape m_icon;
	sf::Texture m_iconTex;
	std::function<void()> m_callback;

	sf::Vector2f m_textOffset;
	sf::Vector2f m_iconOffset;
};