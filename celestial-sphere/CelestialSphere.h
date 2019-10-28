#pragma once
#include "CelestialObject.h"
#include "Planet.h"
#include "Moon.h"

class CelestialSphere
{
public:
	CelestialSphere();

	void update(const tm & ttime, const float &latitude, const int &minutesDifference);

	const std::vector<CelestialObject> getPlanets() const;
	const std::vector<CelestialObject> getStars() const;
	const sf::Vector3f getSunCoords() const;
	const Moon getMoon() const;

private:
	static const std::vector<Planet> loadPlanets();
	static const std::vector<CelestialObject> loadStars();

	const OrbitalElements m_earthOrbitalElems;
	const float m_moonLC0, m_moonLC1, m_moonMC0, m_moonMC1, m_moonFC0, m_moonFC1;
	const std::vector<Planet> m_planets;
	const std::vector<CelestialObject> m_stars;

	std::vector<CelestialObject> m_planetsCache;
	std::vector<CelestialObject> m_starsCache;
	sf::Vector3f m_sunCache;
	Moon m_moon;
	tm m_timeCache;
	float m_latitudeCache;
	float m_minutesDiffCache;
};