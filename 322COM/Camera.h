#pragma once
#include <glm.hpp>

using namespace glm;


class Camera
{
private:
	glm::vec3 location;
	glm::vec3 rotation;
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

	inline void SetEularRotation(vec3 rot) { rotation = rot; }
	inline vec3 GetEularRotation() const { return rotation; }

	vec3 GetForward() const;
	vec3 GetRight() const;

	inline void SetFOV(float f) { fov = f; }
	inline float GetFOV() const { return fov; }
};

