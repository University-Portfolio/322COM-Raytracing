#pragma once
#include <glm.hpp>


struct Ray
{
public:
	glm::vec3 origin;
	glm::vec3 direction;

	Ray(glm::vec3 origin, glm::vec3 direction)
	{
		this->origin = origin;
		this->direction = glm::normalize(direction);
	}
};
