#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}


vec3 Camera::GetForward() const
{
	vec3 rot = radians(rotation);

	return vec3(
		cos(rot.x) * sin(rot.y),
		-sin(rot.x),
		cos(rot.x) * cos(rot.y)
	);
}

vec3 Camera::GetRight() const
{
	vec3 rot = radians(rotation);

	return vec3(
		cos(rot.z) * cos(rot.y),
		sin(rot.z),
		cos(rot.z) * -sin(rot.y)
	);
}