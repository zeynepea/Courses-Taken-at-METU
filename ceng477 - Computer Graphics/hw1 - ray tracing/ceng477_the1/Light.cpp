#include "Light.h"

/* Constructor. Implemented for you. */
PointLight::PointLight(const Vector3f & position, const Vector3f & intensity)
    : position(position), intensity(intensity)
{
}

// Compute the contribution of light at point p using the
// inverse square law formula
Vector3f PointLight::computeLightContribution(const Vector3f& p)
{
	Vector3f distance;
	distance.x = p.x - position.x;
	distance.y = p.y - position.y;
	distance.z = p.z - position.z;

	float d2 = distance * distance;
	Vector3f irradiance;

	if(d2 == 0){
		irradiance = Vector3f(0,0,0);
	}
	else{
		irradiance = intensity / d2;
	}
	return irradiance;
}
