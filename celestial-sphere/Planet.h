#pragma once
#include <SFML/Graphics.hpp>
#include "CelestialObject.h"
#include "../utils/OrbitalElements.h"

struct Planet : public CelestialObject
{
	Planet(const CelestialObject &celestialObject, const OrbitalElements &_orbitalElements) 
		: CelestialObject(celestialObject), orbitalElements(_orbitalElements) {}		// do this because VS isn't C++17 compliant >:(
	OrbitalElements orbitalElements;
};