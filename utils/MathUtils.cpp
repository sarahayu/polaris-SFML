#include "MathUtils.h"
#include "StringUtils.h"
#include "OrbitalElements.h"
#include <iostream>
#include <assert.h>

const time_t mu::maketime(tm & ttime)
{
	time_t _t = mktime(&ttime);
	if (ttime.tm_isdst)
	{
		std::cout << "\nDST! : " << asctime(&ttime);
		ttime.tm_hour -= 1;
		std::cout << "\nRecalibrating... " << asctime(&ttime);
		_t = mktime(&ttime);
	}
	return _t;
}

const sf::Vector3d mu::bv2rgb(double color)
{
	double t;  double r = 0.0; double g = 0.0; double b = 0.0; if (color<-0.4) color = -0.4; if (color> 2.0) color = 2.0;
	if ((color >= -0.40) && (color<0.00)) { t = (color + 0.40) / (0.00 + 0.40); r = 0.61 + (0.11*t) + (0.1*t*t); }
	else if ((color >= 0.00) && (color<0.40)) { t = (color - 0.00) / (0.40 - 0.00); r = 0.83 + (0.17*t); }
	else if ((color >= 0.40) && (color<2.10)) { t = (color - 0.40) / (2.10 - 0.40); r = 1.00; }
	if ((color >= -0.40) && (color<0.00)) { t = (color + 0.40) / (0.00 + 0.40); g = 0.70 + (0.07*t) + (0.1*t*t); }
	else if ((color >= 0.00) && (color<0.40)) { t = (color - 0.00) / (0.40 - 0.00); g = 0.87 + (0.11*t); }
	else if ((color >= 0.40) && (color<1.60)) { t = (color - 0.40) / (1.60 - 0.40); g = 0.98 - (0.16*t); }
	else if ((color >= 1.60) && (color<2.00)) { t = (color - 1.60) / (2.00 - 1.60); g = 0.82 - (0.5*t*t); }
	if ((color >= -0.40) && (color<0.40)) { t = (color + 0.40) / (0.40 + 0.40); b = 1.00; }
	else if ((color >= 0.40) && (color<1.50)) { t = (color - 0.40) / (1.50 - 0.40); b = 1.00 - (0.47*t) + (0.1*t*t); }
	else if ((color >= 1.50) && (color<1.94)) { t = (color - 1.50) / (1.94 - 1.50); b = 0.63 - (0.6*t*t); }

	return{ r * 255.0, g * 255.0, b * 255.0 };
}

const float mu::ratioToVega(const float & mag)
{
	return std::pow(10.0f, 0.4 * -mag);
}

const float mu::degToHour(const float & deg)
{
	return deg / 15.0f;
}

const float mu::hourToDeg(const float & hour)
{
	return hour * 15.0f;
}

const float mu::toDecimalHours(const tm & ttime)
{
	return ttime.tm_hour + ttime.tm_min / 60.0f + ttime.tm_sec / 3600.0f;
}

const float mu::reduceToRange(const float & number, const Range & range)
{
	const float difference = range.max - range.min;
	assert(difference > 0.0f);
	const float mod = std::fmod(number - range.min, difference) + range.min;
	const float reduced = mod < range.min ? mod + difference : mod;
	return reduced;
}

const float mu::degToRad(const float & deg)
{
	return deg * PI / 180.0f;
}

const float mu::radToDeg(const float & rad)
{
	return rad * 180.0f / PI;
}

const sf::Vector2f mu::degToRad(const sf::Vector2f & deg)
{
	return{ degToRad(deg.x), degToRad(deg.y) };
}

const sf::Vector2f mu::celestialToDegrees(const sf::Vector2f & celestialCoords)
{
	return{ reduceToRange(hourToDeg(celestialCoords.x), { -180.0f, 180.0f }), celestialCoords.y };
}

sf::Vector3f mu::rotateAboutY(const sf::Vector3f & coords, const float & rotate)
{
	float z_ = coords.z, x_ = coords.x;
	float rotateCos = std::cos(rotate), rotateSin = std::sin(rotate);

	return{  z_ * rotateSin + x_ * rotateCos,
		coords.y,
		z_ * rotateCos - x_ * rotateSin };
}

sf::Vector3f mu::rotateAboutX(const sf::Vector3f & coords, const float & rotate)
{
	float y_ = coords.y, z_ = coords.z;
	float rotateCos = std::cos(rotate), rotateSin = std::sin(rotate);

	return{ coords.x,
		y_ * rotateCos - z_ * rotateSin,
		y_ * rotateSin + z_ * rotateCos };
}

sf::Vector3f mu::rotateAboutZ(const sf::Vector3f & coords, const float & rotate)
{
	float x_ = coords.x, y_ = coords.y;
	float rotateCos = std::cos(rotate), rotateSin = std::sin(rotate);

	return{ x_ * rotateCos - y_ * rotateSin,
		x_ * rotateSin + y_ * rotateCos,
		coords.z };
}

const float mu::getDaysSincePlanetEpoch(const tm & ttime)
{
	tm dayCopy(ttime);
	tm epochCopy(PLANET_EPOCH);
	const float days = difftime(mktime(&dayCopy), mktime(&epochCopy)) / 60 / 60 / 24;

	return days;
}

const sf::Vector3f mu::getNormalizedEquatorialCoords(const sf::Vector3f & eclipticCoords)
{

	const float delta = std::sqrt(eclipticCoords.x * eclipticCoords.x + eclipticCoords.y * eclipticCoords.y + eclipticCoords.z * eclipticCoords.z);

	const sf::Vector3f normalizedGCCoords{
		eclipticCoords.x / delta,
		eclipticCoords.y / delta,
		eclipticCoords.z / delta
	};

	const sf::Vector3f rotatedGCCoords = rotateAboutX(normalizedGCCoords, degToRad(EPSILON));

	return rotatedGCCoords;
}

const tm mu::getGreenwichTime(const tm & localTime, const int &minuteDifference)
{
	tm gmt = localTime; gmt.tm_min -= minuteDifference; mktime(&gmt);

	return gmt;
}

const float mu::getJulianDate(const tm & gmt)
{
	int y = gmt.tm_year + 1900, m = gmt.tm_mon + 1;
	if (m == 1 || m == 2)
	{
		y -= 1;
		m += 12;
	}

	int A = (int)(y / 100.0f),
		B = 2 - A + (int)(A / 4),
		C = y < 0 ? (int)((365.25f * y) - 0.75f) : (int)(365.25f * y),
		D = gmt.tm_mday,
		E = (int)(30.6001f * (m + 1));

	const float JD = B + C + D + E + 1720994.5f;

	return JD;
}

const bool mu::gte(const float &first, const float &second, const float &precision)
{
	return first > second || std::abs(first - second) <= precision;
}

const bool mu::lte(const float &first, const float &second, const float &precision)
{
	return first < second || std::abs(first - second) <= precision;
}

const bool mu::gt(const float &first, const float &second, const float &precision)
{
	return !lte(first, second, precision);
}

const bool mu::lt(const float &first, const float &second, const float &precision)
{
	return !gte(first, second, precision);
}

const tm mu::getPlanetEpoch()
{
	tm epoch = { 0,0,12,1,1 - 1,2000 - 1900 };
	maketime(epoch);
	return epoch;
}

const float mu::getLocalSiderealTime(const tm & local, const int &minuteDifference)
{
	tm localTime(local); maketime(localTime);
	const tm gmt = getGreenwichTime(localTime, minuteDifference);
	tm prevGMT = gmt; prevGMT.tm_mday--; maketime(prevGMT);
	const float JD = getJulianDate(prevGMT);
	const float UT = toDecimalHours(gmt);

	const float T = (JD - 2451545.0f) / 36525.0f;
	const float T0 = 6.697374558f + 2400.051336f*T + 0.000025862f*T * T + UT*1.0027379093f;

	const float GST = reduceToRange(T0, { 0.0f, 24.0f });
	const float LST = reduceToRange(GST + ((float)minuteDifference / 60.0f), { 0.0f, 24.0f });

	std::cout << std::fixed;
	std::cout << "\n=== Local time: " << asctime(&localTime);
	std::cout << "\n=== GMT: " << asctime(&gmt);
	std::cout << "\n=== Julian Date : " << JD;
	std::cout << "\n=== Universal Time : " << su::toString(UT);
	std::cout << "\n=== Greenwich Sidreal Time : " << su::toString(GST);
	std::cout << "\n=== Local Sidreal Time : " << su::toString(LST);

	return LST;
}

const sf::Vector3f mu::getHCPlanetCoords(const OrbitalElements & orbitalElems, const tm & ttime)
{
	const float days = getDaysSincePlanetEpoch(ttime);
	const float meanAnom = orbitalElems.meanAnomalyEpoch + orbitalElems.anglePerDay * days;
	
	float E, F;
	const float fractionM = meanAnom / 360.0f;
	const float maxIter = 30;
	const float m = PI * 2 * (fractionM - std::floor(fractionM));

	if (orbitalElems.eccentricity < 0.8) E = m;
	else E = PI;

	F = E - orbitalElems.eccentricity * std::sin(m) - m;

	int i = 0;
	while (i < maxIter)
	{
		E = E - F / (1.0 - orbitalElems.eccentricity * std::cos(E));
		F = E - orbitalElems.eccentricity * std::sin(E) - m;
		i++;
	}

	const float S = std::sin(E), C = std::cos(E);
	const float fak = std::sqrt(1.0 - orbitalElems.eccentricity * orbitalElems.eccentricity);
	const float primaryPhi = std::atan2(fak * S, C - orbitalElems.eccentricity) / K;
	const float trueAnom = primaryPhi < 0 ? 360 + primaryPhi : primaryPhi;

	const float distToSun = (orbitalElems.lenSemimajorAxis * (1.0f - orbitalElems.eccentricity * orbitalElems.eccentricity)) / (1 + orbitalElems.eccentricity * std::cos(degToRad(trueAnom)));

	const float cosPeriapsis = std::cos(degToRad(orbitalElems.argPeriapsis + trueAnom));
	const float sinPeriapsis = std::sin(degToRad(orbitalElems.argPeriapsis + trueAnom));
	const float cosLon = std::cos(degToRad(orbitalElems.lonAscendingNode));
	const float sinLon = std::sin(degToRad(orbitalElems.lonAscendingNode));
	const float cosInclination = std::cos(degToRad(orbitalElems.inclination));

	const sf::Vector3f heliocentricCoords = {
		distToSun * (cosLon * cosPeriapsis - sinLon * cosInclination * sinPeriapsis),
		distToSun * (sinLon * cosPeriapsis + cosLon * cosInclination * sinPeriapsis),
		distToSun * std::sin(degToRad(orbitalElems.inclination)) * sinPeriapsis
	};

	return heliocentricCoords;
}

const sf::Vector3f mu::getGCPlanetCoords(const OrbitalElements & earth, const OrbitalElements & planet, const tm & ttime)
{
	const sf::Vector3f earthCoords = getHCPlanetCoords(earth, ttime),
		planetCoords = getHCPlanetCoords(planet, ttime);
	const sf::Vector3f geocentricCoords = planetCoords - earthCoords;
	const sf::Vector3f rotatedGCCoords = getNormalizedEquatorialCoords(geocentricCoords);

	return rotatedGCCoords;
}

const sf::Vector3f mu::getSunCoords(const OrbitalElements & earth, const tm & ttime)
{
	const sf::Vector3f earthCoords = getHCPlanetCoords(earth, ttime);
	const sf::Vector3f geocentricCoords = -earthCoords;
	const sf::Vector3f rotatedGCCoords = getNormalizedEquatorialCoords(geocentricCoords);

	return rotatedGCCoords;
}

const sf::Vector2f mu::wrapCoordinates(const sf::Vector2f & coords)
{
	float longtitude = coords.x;
	float latitude = reduceToRange(coords.y, { -180.0f, 180.0f });
	bool flip = false;

	if (latitude > 90.0) 
	{
		latitude = 180.0 - latitude;
		flip = true;
	}
	else if (latitude < -90.0) 
	{
		latitude = -180.0 - latitude;
		flip = true;
	}

	if (flip) longtitude += 180.0f;

	longtitude = reduceToRange(longtitude, { -180.0f, 180.0f });

	return{ longtitude, latitude };
}

const sf::Vector3f mu::sphericalToCartesian(const sf::Vector2f & sphereCoords)
{
	const float radius = std::cos(sphereCoords.y);

	return{ radius * std::cos(sphereCoords.x),
		radius * std::sin(sphereCoords.x),
		std::sin(sphereCoords.y)
	};
}

const sf::Vector2f mu::cartesianToStereographical(const sf::Vector3f & cartesianCoords)		// facing positive y
{
	return{ cartesianCoords.x / (1.0f + cartesianCoords.y) , cartesianCoords.z / (1.0f + cartesianCoords.y) };
}
