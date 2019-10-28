#include "Renderer.h"
#include "csv/csv.h"
#include "RenderSettings.h"
#include "utils/MathUtils.h"
#include "celestial-sphere/CelestialSphere.h"
#include <iostream>

const float MIN_MAG = 10.0f;
const float BRIGHTEST_MAG = -1.44f;
const float DIMMEST_MAG = MIN_MAG;
const float MAG_SCALE = DIMMEST_MAG - BRIGHTEST_MAG;
const sf::Vector2f SKY_SIZE(10000.0f, 10000.0f);
const float VEGA_RAD = 120.f;		// radius of Vega
const float MAX_STAR_RAD = 140.f;	// radius of Sirius
const float FONT_SIZE = 140.f;
const float NAME_OFFSET = 300.f;
const float DEF_FIELD_VIEW = 1.f;


const sf::Vector2f operator * (const sf::Vector2f &first, const sf::Vector2f &second)
{
	return{ first.x * second.x, first.y * second.y };
}

const sf::FloatRect viewRect(const sf::View &view)
{
	return sf::FloatRect(view.getCenter() - view.getSize() / 2.0f, view.getSize());
}

const sf::Vector2f cartesianToScreen(const sf::Vector3f &coords, const sf::Vector3f &rotation)
{
	const float dist = 0.f;
	const sf::Vector3f rotatedView = mu::rotateAboutX(mu::rotateAboutZ(coords, rotation.z), rotation.x);
	if (dist + rotatedView.y < 0) return{ 10000000,10000000 };
	const sf::Vector2f SGCoords(rotatedView.x / (dist + rotatedView.y), rotatedView.z / (dist + rotatedView.y));
	const sf::Vector2f primaryScreenCords = SGCoords * (SKY_SIZE / 2.0f);
	const sf::Vector2f screenCoords(primaryScreenCords.x, -primaryScreenCords.y);
	return screenCoords;
}

void Renderer::drawCardinals(sf::Text &text, const sf::Vector2f &location)
{
	text.setCharacterSize(500 * r_renderSettings.zoomFactor);
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.width / 2.0f, bounds.height * -0.5f);
	text.setPosition(location);
	r_window.draw(text);
}


Renderer::Renderer(sf::RenderWindow & window, Menu &menu, const RenderSettings & renderSettings, const CelestialSphere &celestialSphere)
	:
	r_celestialSphere(celestialSphere),
	r_window(window),
	r_menu(menu),
	r_renderSettings(renderSettings)
{
	m_roboto.loadFromFile("resources/roboto.ttf");
	m_timesNewRoman.loadFromFile("resources/times-new-roman.ttf");
	m_starName.setFont(m_roboto);
	m_starName.setFillColor(sf::Color(255, 255, 255, 100));
	m_moonSprite.setRadius(120.f);
	m_moonSprite.setOrigin(120.f, 120.f);
	m_sunSprite.setRadius(120.f);
	m_sunSprite.setOrigin(120.f, 120.f);
	m_sunSprite.setFillColor(sf::Color(198, 187, 62));

	m_N = sf::Text("N", m_timesNewRoman);
	m_N.setFillColor(sf::Color(255, 255, 255, 70));
	m_E = m_N, m_S = m_N, m_W = m_N;
	m_E.setString("E");
	m_S.setString("S");
	m_W.setString("W");
	m_N.setOrigin(m_N.getGlobalBounds().width / 2, m_N.getGlobalBounds().height / 2);
	m_E.setOrigin(m_E.getGlobalBounds().width / 2, m_E.getGlobalBounds().height / 2);
	m_S.setOrigin(m_S.getGlobalBounds().width / 2, m_S.getGlobalBounds().height / 2);
	m_W.setOrigin(m_N.getGlobalBounds().width / 2, m_W.getGlobalBounds().height / 2);

	m_starGlowTex.loadFromFile("resources/star-glow.png");
	m_planetGlowTex.loadFromFile("resources/planet-glow.png");
	m_moonTex.loadFromFile("resources/moon-phases.png");
	m_moonSprite.setTexture(&m_moonTex);

	const sf::Vector2i MOONTEX_SIZE(50, 49);

	for (int y = 0; y < m_moonTex.getSize().y; y += MOONTEX_SIZE.y)
	{
		for (int x = 0; x < m_moonTex.getSize().x; x += MOONTEX_SIZE.x)
		{
			m_moonTexRects.push_back({ {x,y},MOONTEX_SIZE });
		}
	}
}

void Renderer::render()
{
	r_window.clear(sf::Color::Black);

	float aspectRatio = (float)r_window.getSize().x / r_window.getSize().y;
	sf::Vector2f viewSize = SKY_SIZE * DEF_FIELD_VIEW * r_renderSettings.zoomFactor;
	viewSize.x *= aspectRatio;

	m_starView.setSize(viewSize);
	m_starView.setCenter(0.0f, 0.0f);

	sf::FloatRect largerView({ m_starView.getCenter() - viewSize / 2.0f - sf::Vector2f(100.0f, 100.0f) }, viewSize + sf::Vector2f(200.0f, 200.0f));

	r_window.setView(m_starView);
	
	if (r_renderSettings.drawGridlines)
	{
		// --------------------------- Draw Longtitudes -------------------------------------- //

		sf::VertexArray gridLines(sf::Lines);
		for (float offsetPhi = 0.0f; mu::lt(offsetPhi, mu::PI); offsetPhi += mu::PI / 6.0f)
		{
			sf::Color lineColor;
			if (std::fmod(offsetPhi, mu::PI / 2.0f) <= 0.1f) lineColor = sf::Color(30, 30, 30);
			else lineColor = sf::Color(15, 15, 15);

			for (float phi = mu::PI / 64; mu::lte(phi, mu::PI * 2); phi += mu::PI / 64)
			{
				const sf::Vector3f rotateBy(-r_renderSettings.viewOffset.y, 0.0f, -(r_renderSettings.viewOffset.x + offsetPhi));
				const sf::Vector2f firstPoint(cartesianToScreen({ std::cos(phi - mu::PI / 64), 0.0f, std::sin(phi - mu::PI / 64) }, rotateBy)),
					secondPoint(cartesianToScreen({ std::cos(phi), 0.0f, std::sin(phi) }, rotateBy));
				if (!largerView.contains(firstPoint) || !largerView.contains(secondPoint)) continue;
				gridLines.append(sf::Vertex(firstPoint, lineColor));
				gridLines.append(sf::Vertex(secondPoint, lineColor));
			}

		}

		// --------------------------- Draw Latitudes -------------------------------------- //
		for (float offsetPhi = 0.0f; mu::lt(offsetPhi, mu::PI / 2); offsetPhi += mu::PI / 12.0f)
		{
			float radius = std::sin(offsetPhi);
			for (float phi = mu::PI / 64; mu::lte(phi, mu::PI * 2); phi += mu::PI / 64)
			{
				const sf::Vector2f firstPoint(cartesianToScreen({ std::cos(phi - mu::PI / 64) * radius, std::sin(phi - mu::PI / 64) * radius, std::cos(offsetPhi) }, { -r_renderSettings.viewOffset.y, 0.0f, 0.0f })),
					secondPoint(cartesianToScreen({ std::cos(phi) * radius, std::sin(phi) * radius, std::cos(offsetPhi) }, { -r_renderSettings.viewOffset.y, 0.0f, 0.0f }));
				if (!largerView.contains(firstPoint) || !largerView.contains(secondPoint)) continue;
				gridLines.append(sf::Vertex(firstPoint, sf::Color(15, 15, 15)));
				gridLines.append(sf::Vertex(secondPoint, sf::Color(15, 15, 15)));
			}
		}
		r_window.draw(gridLines);
	}

	// --------------------------- Draw Planets and Stars -------------------------------------- //
	renderCelestialObjects(r_celestialSphere.getStars(), m_starGlowTex);
	renderCelestialObjects(r_celestialSphere.getPlanets(), m_planetGlowTex);

	// --------------------------- Draw Moon -------------------------------------- //
	const Moon &moon = r_celestialSphere.getMoon();
	const sf::Vector2f moonCoords = cartesianToScreen(moon.coordinates, { -r_renderSettings.viewOffset.y, 0.0f, -r_renderSettings.viewOffset.x });
	if (viewRect(m_starView).contains(moonCoords))
	{
		m_moonSprite.setPosition(moonCoords);
		m_moonSprite.setTextureRect(m_moonTexRects[std::round((m_moonTexRects.size() - 1) * moon.cyclePercentage)]);
		m_moonSprite.setRotation(moon.tilt);
		r_window.draw(m_moonSprite);
	}

	// --------------------------- Draw Sun -------------------------------------- //
	const sf::Vector2f sunCoords = cartesianToScreen(r_celestialSphere.getSunCoords(), { -r_renderSettings.viewOffset.y, 0.0f, -r_renderSettings.viewOffset.x });
	if (viewRect(m_starView).contains(sunCoords))
	{
		m_sunSprite.setPosition(sunCoords);
		r_window.draw(m_sunSprite);
	}	

	// --------------------------- Draw Land -------------------------------------- //

	const float stepY = mu::PI / 6.0f;
	const float stepX = mu::PI / 16.0f;
	const float sphereRad = 1.0f;

	sf::VertexArray land(sf::Quads);

	for (float phiY = 0.0f; phiY <= mu::PI / 2.0f; phiY += stepY)
	{
		float radius = std::cos(phiY);
		float radius2 = std::cos(phiY + stepY);
		float z = -std::sin(phiY) * sphereRad;
		float z2 = -std::sin(phiY + stepY) * sphereRad;

		for (float phi = 0.0f; phi <= mu::PI * 2.0f; phi += stepX)
		{
			float sinPhi1 = std::sin(phi), sinPhi2 = std::sin(phi + stepX), cosPhi1 = std::cos(phi), cosPhi2 = std::cos(phi + stepX);
			float x  = cosPhi1 * radius  * sphereRad;
			float x2 = cosPhi2 * radius  * sphereRad;
			float x3 = cosPhi1 * radius2 * sphereRad;
			float x4 = cosPhi2 * radius2 * sphereRad;
			float y  = sinPhi1 * radius  * sphereRad;
			float y2 = sinPhi2 * radius  * sphereRad;
			float y3 = sinPhi1 * radius2 * sphereRad;
			float y4 = sinPhi2 * radius2 * sphereRad;

			sf::Vector2f point1(cartesianToScreen({ x,y,z }, { -r_renderSettings.viewOffset.y, 0.0f, 0.0f }));
			sf::Vector2f point2(cartesianToScreen({ x2,y2,z }, { -r_renderSettings.viewOffset.y, 0.0f, 0.0f }));
			sf::Vector2f point3(cartesianToScreen({ x4,y4,z2 }, { -r_renderSettings.viewOffset.y, 0.0f, 0.0f }));
			sf::Vector2f point4(cartesianToScreen({ x3,y3,z2 }, { -r_renderSettings.viewOffset.y, 0.0f, 0.0f }));

			if (!largerView.contains(point1)
				&& !largerView.contains(point2)
				&& !largerView.contains(point3)
				&& !largerView.contains(point4)) continue;

			land.append(sf::Vertex(point1, sf::Color(0, 0, 0, 230)));
			land.append(sf::Vertex(point2, sf::Color(0, 0, 0, 230)));
			land.append(sf::Vertex(point3, sf::Color(0, 0, 0, 230)));
			land.append(sf::Vertex(point4, sf::Color(0, 0, 0, 230)));
		}
	}

	r_window.draw(land);

	// --------------------------- Draw Horizon -------------------------------------- //
	sf::VertexArray horizon(sf::LineStrip);
	for (float phi = 0.0f; phi <= mu::PI * 2.01; phi += mu::PI / 16.0f)
		horizon.append(sf::Vertex(cartesianToScreen({ std::cos(phi), std::sin(phi), 0.0f }, { -r_renderSettings.viewOffset.y, 0.0f, 0.0f }), sf::Color(30, 30, 30)));
	r_window.draw(horizon);

	const sf::Vector2f north = cartesianToScreen({ -1.0f, 0.0f, 0.0f }, { -r_renderSettings.viewOffset.y, 0.0f, -r_renderSettings.viewOffset.x });
	const sf::Vector2f east = cartesianToScreen({ 0.0f, 1.0f, 0.0f }, { -r_renderSettings.viewOffset.y, 0.0f, -r_renderSettings.viewOffset.x });
	const sf::Vector2f south = cartesianToScreen({ 1.0f, 0.0f, 0.0f }, { -r_renderSettings.viewOffset.y, 0.0f, -r_renderSettings.viewOffset.x });
	const sf::Vector2f west = cartesianToScreen({ 0.0f, -1.0f, 0.0f }, { -r_renderSettings.viewOffset.y, 0.0f, -r_renderSettings.viewOffset.x });
	drawCardinals(m_N, north);
	drawCardinals(m_E, east);
	drawCardinals(m_S, south);
	drawCardinals(m_W, west);

	if (r_renderSettings.displayMenu) r_menu.draw(r_window);

	r_window.display();
}

void Renderer::renderCelestialObjects(const std::vector<CelestialObject> & objects, const sf::Texture & texture)
{
	sf::Clock timer;
	sf::VertexArray objectArr(sf::Quads);

	for (const CelestialObject &object : objects)
	{
		const float brightness = mu::ratioToVega(object.magnitude * r_renderSettings.brightnessFactor);

		if (brightness < 0.05f) continue;

		const sf::Vector2f screenCoords = cartesianToScreen(object.coordinates, { -r_renderSettings.viewOffset.y, 0.0f, -r_renderSettings.viewOffset.x });

		if (!viewRect(m_starView).contains(screenCoords)) continue;		// not even gonna be onscreen, SKIP

		float radius = std::min(MAX_STAR_RAD, brightness * VEGA_RAD) * r_renderSettings.zoomFactor;
		sf::Color color = object.color;

		color.a = std::min(255.0f, 255.0f * (brightness));

		const float rad = 20.0f;
		const sf::Vector2f texSize(texture.getSize());

		objectArr.append(sf::Vertex({ screenCoords.x - radius,screenCoords.y - radius }, color, { 0.0f,texSize.y }));
		objectArr.append(sf::Vertex({ screenCoords.x + radius,screenCoords.y - radius }, color, { texSize.x,texSize.y }));
		objectArr.append(sf::Vertex({ screenCoords.x + radius,screenCoords.y + radius }, color, { texSize.x,0.0f }));
		objectArr.append(sf::Vertex({ screenCoords.x - radius,screenCoords.y + radius }, color, { 0.0f,0.0f }));

		if (!r_renderSettings.displayName) continue;
		if (object.name.length() == 0) continue;

		m_starName.setString(object.name);
		m_starName.setCharacterSize(FONT_SIZE * r_renderSettings.zoomFactor);
		sf::FloatRect bounds = m_starName.getLocalBounds();
		m_starName.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
		m_starName.setPosition(screenCoords - sf::Vector2f(0.0f, NAME_OFFSET * r_renderSettings.zoomFactor));

		r_window.draw(m_starName);
	}

	std::cout << "\nRendering " << objectArr.getVertexCount() / 4 << " objects in " << std::fixed << timer.getElapsedTime().asSeconds() << " seconds "/* << r_renderSettings.brightnessFactor*/;

	r_window.draw(objectArr, &texture);
}

void Renderer::forAboutCircle(const Range & range, const float & step, const std::function<void(float)>& func)
{
	for (float offsetPhi = range.first; offsetPhi < range.last; offsetPhi += step) func(offsetPhi);
}
