#pragma once
#include "menu/Menu.h"
#include <SFML/Graphics.hpp>

struct RenderSettings;
struct CelestialObject;

class Renderer
{
public:
	Renderer(
		sf::RenderWindow &window,
		Menu &menu,
		const RenderSettings &renderSettings,
		const CelestialSphere &celestialSphere);

	void render();

private:

	struct Range { float first, last; };

	void drawCardinals(sf::Text &text, const sf::Vector2f &location);

	void renderCelestialObjects(const std::vector<CelestialObject> &object, const sf::Texture &texture);
	void forAboutCircle(const Range &range, const float &step, const std::function<void(float)> &func);

	sf::Font m_roboto;
	sf::Font m_timesNewRoman;
	sf::Text m_starName;
	sf::Text m_N, m_E, m_S, m_W;
	sf::View m_starView;
	sf::View m_menuView;
	//sf::Shader m_starGlow;
	sf::Texture m_starGlowTex;
	sf::Texture m_planetGlowTex;
	sf::Texture m_moonTex;
	sf::CircleShape m_moonSprite;
	sf::CircleShape m_sunSprite;

	std::vector<sf::IntRect> m_moonTexRects;

	const CelestialSphere &r_celestialSphere;
	sf::RenderWindow &r_window;
	Menu &r_menu;
	const RenderSettings &r_renderSettings;
};