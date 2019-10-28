#pragma once
#include "MenuItem.h"
#include <functional>

class MenuTextBox : public MenuItem
{
public:
	typedef std::shared_ptr<MenuTextBox> Ptr;

	MenuTextBox(const std::string &name, const sf::Text &text, const sf::Vector2f &location, const sf::Vector2f &padding, const sf::Vector2f &parentLocation = sf::Vector2f(), const sf::Color &color = sf::Color::White, const sf::Color &boxColor = sf::Color::Transparent, const int &maxBuffer = 10);

	void drawItem(sf::RenderWindow &window);

	void connect(const std::function<void()> &callback);
	void backspace();
	const bool isWithinBounds(const sf::Vector2f &location);
	const std::string getName() const;
	void addText(const std::string &text);
	const std::string getValue() const;
	void setValue(const std::string &str);
	void callback();

	bool active = false;

private:
	float m_textHeight;	// const height, box changes with underscores for some reason
	int m_maxBuffer;
	sf::Text m_text;
	sf::RectangleShape m_box;
	std::string m_value;
	std::string m_name;
	sf::Vector2f m_padding;
	std::function<void()> m_callback;
};