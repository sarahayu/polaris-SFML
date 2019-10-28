#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

namespace sf
{
	typedef sf::Vector3<double> Vector3d;
}

namespace mu
{
	const float PI = 3.141592653589793f;
	const float HYG_EPOCH_GST = 6.59881f;		// hyg database celestial coordinates are based on 2000 epoch

	const float degToHr(const float &deg);
	const float timeToDec(const tm &ttime);
	const sf::Vector3d bv2rgb(double color);
	const sf::Vector2f toRad(const sf::Vector2f &deg);
	const sf::Vector2f toDeg(const sf::Vector2f &rad);
	const sf::Vector3f toRad(const sf::Vector3f &deg);
	const sf::Vector3f toDeg(const sf::Vector3f &rad);
	const float toRad(const float & deg);
	const float toDeg(const float &rad);
	const float ratioToVega(const float &mag);		// 0-1
	const sf::Vector2f celestialToHorizontal(const sf::Vector2f &celestialCoords, const sf::Vector2f &observerCoords, const float &localSidrealTime);
	sf::Vector3f rotateAboutY(const sf::Vector3f & coords, const float & rotate);
	sf::Vector3f rotateAboutX(const sf::Vector3f & coords, const float & rotate);
	sf::Vector3f rotateAboutZ(const sf::Vector3f & coords, const float & rotate);
	sf::Vector3f rotate(const sf::Vector3f & coords, const sf::Vector3f &rotation);
	sf::Vector3f horizontalToCartesian(const sf::Vector2f & coords);
	sf::Vector2f cartesianToSG(const sf::Vector3f & coords);
	const sf::Vector2f SGToScreen(const sf::Vector2f & SGCoord, const sf::Vector2f &screenSize);
	const sf::Vector2f cartesianToScreen(const sf::Vector3f & cartesianCoords, const sf::Vector3f & rotation, const sf::Vector2f &screenSize);
	const tm toGreenwichMeanTime(const tm & localTime, const float & longtitude);
	const float toDecimalHours(const tm & ttime);
	const float getJulianDate(const tm & gmt);
	const float getGreenwichSidrealTime(const tm & gmt);
	const float getLocalSidrealTime(const tm & gmt, const float & longtitude);
}