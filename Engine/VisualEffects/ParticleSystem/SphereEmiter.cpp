/*
 * Pick random point inside a sphere
 * See here: http://math.stackexchange.com/questions/87230/picking-random-points-in-the-volume-of-sphere-with-uniform-probability
 * 
 * Choose azimuthal = Random (-180, 180)
 * Choose polar = arccos (2 * Random (0, 1) - 1)
 * These are Spherical Coordinates of a random point on the surface of a sphere
 *
 * To convert from spherical coordinates to cartesian coordinates will do:
 * [x] = r * cos (azimuthal) * sin (polar)
 * [y] = r * sin (azimuthal) * sin (polar)
 * [z] = r * cos (polar)
 *
 * Currently this are coordinates of a random point on the surface of a 
 * sphere with radius r. To translate it inside the sphere will assume:
 * r = radial * CubicRoot (Random (0, 1))
 * where radial is the actual radius of the desired sphere.
 *
 * The random point inside the sphere will have coordinates ([x], [y], [z]).
 */

#ifdef _MSC_VER
	#define _USE_MATH_DEFINES
#endif

#include "SphereEmiter.h"

#include <cmath>

#include "Core/Math/Matrix.h"

#include "Particle.h"

#include "Core/Random/Random.h"

SphereEmiter::SphereEmiter (float radius) :
	_radius (radius)
{

}

void SphereEmiter::SetRadius (float radius)
{
	_radius = radius;
}

glm::vec3 SphereEmiter::GetParticlePosition ()
{
	glm::vec3 position;

	float azimuthal = Random::Instance ().RangeF ((float) -M_PI, (float) M_PI);
	float polar = acos (2 * Random::Instance ().RangeF (0, 1) - 1);
	float radial = _radius * cbrt (Random::Instance ().RangeF (0, 1));

	position.x = radial * cos (azimuthal) * sin (polar);
	position.y = radial * sin (azimuthal) * sin (polar);
	position.z = radial * cos (polar);

	// Calculate by translation
	position += this->GetTransform ()->GetPosition ();

	return position;
}