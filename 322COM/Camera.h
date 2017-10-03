#pragma once
#include <glm.hpp>

using namespace glm;


class Camera
{
private:
	glm::vec3 location;
	float fov = 90;

public:
	Camera();
	~Camera();
	
	/**
	* Getters & Setters
	*/
public:

	inline void SetLocation(vec3 loc) { location = loc; }
	inline vec3 GetLocation() const { return location; }

	inline void SetFOV(float f) { fov = f; }
	inline float GetFOV() const { return fov; }
};

