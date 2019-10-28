#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

struct OrbitalElements;

namespace sf
{
	typedef sf::Vector3<double> Vector3d;
}

namespace mu
{
	const tm getPlanetEpoch();

	struct Range { float min, max; };
	const float PI = 3.141592653589793f;
	const float K = PI / 180.0f;
	const float HYG_EPOCH_GST = 6.59881f;		// hyg database celestial coordinates are based on 2000 epoch
	const float EPSILON = 23.4397f;		// earth tilt as of 2000
	const tm PLANET_EPOCH = getPlanetEpoch();

	const time_t maketime(tm &ttime);	// similar to mktime but disregards daylight savings time
	const sf::Vector3d bv2rgb(double color);
	const float ratioToVega(const float &mag);		// 0-1
	const float degToHour(const float &deg);
	const float hourToDeg(const float &hour);
	const float toDecimalHours(const tm &ttime);
	const float reduceToRange(const float &number, const Range &range);
	const float degToRad(const float &deg);
	const float radToDeg(const float &rad);
	const sf::Vector2f degToRad(const sf::Vector2f &deg);
	const sf::Vector2f celestialToDegrees(const sf::Vector2f &celestialCoords);
	sf::Vector3f rotateAboutY(const sf::Vector3f & coords, const float & rotate);
	sf::Vector3f rotateAboutX(const sf::Vector3f & coords, const float & rotate);
	sf::Vector3f rotateAboutZ(const sf::Vector3f & coords, const float & rotate);
	const float getDaysSincePlanetEpoch(const tm &ttime);
	const sf::Vector3f getNormalizedEquatorialCoords(const sf::Vector3f &eclipticCoords);
	const tm getGreenwichTime(const tm & localTime, const int &minuteDifference);
	const float getJulianDate(const tm &gmt);
	const bool gte(const float &first, const float &second, const float &precision = 0.1f);
	const bool lte(const float &first, const float &second, const float &precision = 0.1f);
	const bool gt(const float &first, const float &second, const float &precision = 0.1f);
	const bool lt(const float &first, const float &second, const float &precision = 0.1f);

	const float getLocalSiderealTime(const tm &localTime, const int &minuteDifference);
	const sf::Vector3f getHCPlanetCoords(const OrbitalElements &orbitalElems, const tm &ttime);		// heliocentric coords
	const sf::Vector3f getGCPlanetCoords(const OrbitalElements &earth, const OrbitalElements &planet, const tm &ttime);		// geocentric coords
	const sf::Vector3f getSunCoords(const OrbitalElements &earth, const tm &ttime);

	const sf::Vector2f wrapCoordinates(const sf::Vector2f &coords);
	const sf::Vector3f sphericalToCartesian(const sf::Vector2f &sphereCoords);
	const sf::Vector2f cartesianToStereographical(const sf::Vector3f &cartesianCoords);
}