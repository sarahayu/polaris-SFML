#include "CelestialSphere.h"
#include "../utils/MathUtils.h"
#include "../csv/csv.h"
#include <iostream>

const float MIN_MAG = 10.0f;

bool operator != (const tm &first, const tm &second)
{
	return first.tm_sec != second.tm_sec
		|| first.tm_min != second.tm_min
		|| first.tm_hour != second.tm_hour
		|| first.tm_mday != second.tm_mday
		|| first.tm_mon != second.tm_mon
		|| first.tm_year != second.tm_year;
}

CelestialSphere::CelestialSphere()
	: m_earthOrbitalElems{ 1.0f, 0.01671f, 0.0f, 288.064f, 174.873f, 357.529f, 0.985608f, 0.99972f }, 
	m_moonLC0(218.316f), m_moonLC1(13.176396f), m_moonMC0(134.963f), m_moonMC1(13.064993f), m_moonFC0(93.272f), m_moonFC1(13.229350f),
	m_planets(loadPlanets()), m_stars(loadStars())			// TODODOODODODODODO
{
}

void CelestialSphere::update(const tm & ttime, const float &latitude, const int &minutesDifference)
{
	std::cout << "\nHour: " << ttime.tm_hour;

	if (m_timeCache != ttime
		|| m_latitudeCache != latitude
		|| m_minutesDiffCache != minutesDifference)
	{
		m_timeCache = ttime;
		m_latitudeCache = latitude;
		m_minutesDiffCache = minutesDifference;

		const float LST = mu::getLocalSiderealTime(ttime, minutesDifference);

		m_starsCache.clear();

		for (const auto &star : m_stars)
		{
			const sf::Vector3f rotatedTime = mu::rotateAboutZ(star.coordinates, -mu::degToRad(mu::hourToDeg(LST)));
			const sf::Vector3f rotatedLocation = mu::rotateAboutY(rotatedTime, -mu::degToRad(90 - latitude));

			CelestialObject starCopy(star);
			starCopy.coordinates = rotatedLocation;

			m_starsCache.push_back(starCopy);
		}

		m_planetsCache.clear();

		for (const auto &planet : m_planets)
		{
			const sf::Vector3f newLocation = mu::getGCPlanetCoords(m_earthOrbitalElems, planet.orbitalElements, ttime);

			const sf::Vector3f rotatedTime = mu::rotateAboutZ(newLocation, -mu::degToRad(mu::hourToDeg(LST)));
			const sf::Vector3f rotatedLocation = mu::rotateAboutY(rotatedTime, -mu::degToRad(90 - latitude));

			Planet planetCopy(planet);
			planetCopy.coordinates = rotatedLocation;

			m_planetsCache.push_back(planetCopy);
		}

		const float days = mu::getDaysSincePlanetEpoch(ttime);
		const float L = std::fmod(m_moonLC0 + m_moonLC1 * days, 360.0f),
			M = std::fmod(m_moonMC0 + m_moonMC1 * days, 360.0f),
			F = std::fmod(m_moonFC0 + m_moonFC1 * days, 360.0f);

		const sf::Vector2f geocentricLonLat(mu::degToRad(L + 6.289f * std::sin(mu::degToRad(M))), mu::degToRad(5.128f * std::sin(mu::degToRad(F))));
		const sf::Vector3f eclipticCoords = mu::sphericalToCartesian(geocentricLonLat);
		const sf::Vector3f equatorialCoords = mu::getNormalizedEquatorialCoords(eclipticCoords);
		const sf::Vector3f rotatedTime = mu::rotateAboutZ(equatorialCoords, -mu::degToRad(mu::hourToDeg(LST)));
		const sf::Vector3f rotatedLocation = mu::rotateAboutY(rotatedTime, -mu::degToRad(90 - latitude));
		
		const float howManyNewMoons = (mu::getJulianDate(mu::getGreenwichTime(ttime, latitude)) - 2451549.5f) / 29.53f;
		const float cyclePercentage = (howManyNewMoons - (int)howManyNewMoons);

		m_moon.coordinates = rotatedLocation;
		m_moon.tilt = latitude + 90.0f;
		m_moon.cyclePercentage = cyclePercentage;

		const sf::Vector3f sunEquatorialCoords = mu::getSunCoords(m_earthOrbitalElems, ttime);
		const sf::Vector3f sunRotatedTime = mu::rotateAboutZ(sunEquatorialCoords, -mu::degToRad(mu::hourToDeg(LST)));
		const sf::Vector3f sunRotatedLocation = mu::rotateAboutY(sunRotatedTime, -mu::degToRad(90 - latitude));

		m_sunCache = sunRotatedLocation;
	}
}

const std::vector<CelestialObject> CelestialSphere::getPlanets() const
{
	return m_planetsCache;
}

const std::vector<CelestialObject> CelestialSphere::getStars() const
{
	return m_starsCache;
}

const sf::Vector3f CelestialSphere::getSunCoords() const
{
	return m_sunCache;
}

const Moon CelestialSphere::getMoon() const
{
	return m_moon;
}

const std::vector<Planet> CelestialSphere::loadPlanets()
{
	return std::vector<Planet>
	{
		Planet(CelestialObject{ "Mercury", sf::Color(221, 204, 170), -0.03f,{ 0.0f,0.0f,0.0f } }, OrbitalElements{ 0.3871f, 0.20563f, 7.005f, 29.125f, 48.331f, 174.795f, 4.092317f, 0.37073f }),
		Planet(CelestialObject{ "Venus", sf::Color(255,238,255), -0.03f,{ 0.0f,0.0f,0.0f } }, OrbitalElements{ 0.72333f, 0.00677f, 3.395f, 54.884f, 76.68f, 50.416f, 1.602136f, 0.7233f }),
		Planet(CelestialObject{ "Mars", sf::Color(238, 119, 51), -0.03f,{ 0.0f,0.0f,0.0f } }, OrbitalElements{ 1.52368f, 0.0934f, 1.85f, 286.502f, 49.558f, 19.373f, 0.524039f, 1.51039f }),
		Planet(CelestialObject{ "Jupiter", sf::Color(255, 238, 204), -0.03f,{ 0.0f,0.0f,0.0f } }, OrbitalElements{ 5.2026f, 0.04849f, 1.303f, 273.867f, 100.464f, 20.02f, 0.083056f, 5.19037f }),
		Planet(CelestialObject{ "Saturn", sf::Color(255, 255, 216), -0.03f,{ 0.0f,0.0f,0.0f } }, OrbitalElements{ 9.55491f, 0.05551f, 2.489f, 339.391f, 113.666f, 317.021f, 0.033371f, 9.52547f }),
		Planet(CelestialObject{ "Uranus", sf::Color(178, 208, 223), -0.03f,{ 0.0f,0.0f,0.0f } }, OrbitalElements{ 19.21845f, 0.0463f, 0.773f, 98.999f, 74.006f, 141.05f, 0.011698f, 19.17725f }),
		Planet(CelestialObject{ "Neptune", sf::Color(187, 221, 255), -0.03f,{ 0.0f,0.0f,0.0f } }, OrbitalElements{ 30.11039f, 0.00899f, 1.77f, 276.34f, 131.784f, 256.225f, 0.005965f, 30.10796f })

	};
}

const std::vector<CelestialObject> CelestialSphere::loadStars()
{
	std::vector<CelestialObject> stars;

	io::CSVReader<5> in("resources/hyg.csv");

	in.read_header(io::ignore_no_column, "ProperName",
		"RA", "Dec", "Mag", "ColorIndex");

	std::string properName;

	double d_ra, d_dec, d_mag, d_color;

	while (in.read_row(properName, d_ra, d_dec, d_mag, d_color))
	{
		float ra = (float)d_ra;
		float dec = (float)d_dec;
		float mag = (float)d_mag;

		//if (mag > MIN_MAG) continue;

		sf::Vector3d rgb = mu::bv2rgb(d_color);
		sf::Color color(rgb.x, rgb.y, rgb.z);

		sf::Vector3f cartesian = mu::sphericalToCartesian(mu::degToRad(mu::celestialToDegrees({ ra,dec })));

		stars.push_back({ properName, color, mag, cartesian });
	}

	return stars;
}
