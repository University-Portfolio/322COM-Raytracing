#pragma once
#include "Material.h"
#include "Scene.h"
#include "Object.h"
#include "Texture.h"


/**
* Material that will respond to physical properties e.g. Shadows, lighting, reflections etc.
*/
class PhysicalMaterial : public Material
{
private:
	float m_shininess = 10.0f;
	float m_smoothness = 1.0f;
	float m_reflectivity = 0.0f;
	float m_refractionIndex = 1.0f;

	float m_refractionRatio = 1.0f;

	Texture* m_texture = nullptr;

public:
	virtual ~PhysicalMaterial();

	/**
	* Fetch the desired base colour (Not affected by physics) for an object's pixel in a scene
	* @param scene			The scene that is currently being tested with
	* @param ray			The ray that was previously used to fetch the object
	* @param hit			The hit information about the object/pixel to colour
	* @param recursionCount	How many recursive lookups have occured
	* @returns The correct colour that this pixel should be
	*/
	virtual Colour FetchBaseColour(const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount);

	/**
	* Fetch the desired colour for an object's pixel in a scene
	* @param scene			The scene that is currently being tested with
	* @param ray			The ray that was previously used to fetch the object
	* @param hit			The hit information about the object/pixel to colour
	* @param recursionCount	How many recursive lookups have occured
	* @returns The correct colour that this pixel should be
	*/
	virtual Colour FetchColour(const Scene* scene, Ray ray, PixelHitInfo& hit, int recursionCount) override;

	/**
	* Getters and Setters
	*/
public:
	inline void SetTexture(std::string texturePath) { m_texture = new Texture(texturePath); }
	inline Texture* GetTexture() { return m_texture; }

	inline void SetSmoothness(float val) { m_smoothness = val; }
	inline float GetSmoothness() const { return m_smoothness; }

	inline void SetShininess(float val) { m_shininess = val; }
	inline float GetShininess() const { return m_shininess; }

	inline void SetReflectivity(float val) { m_reflectivity = clamp(0.0f, 1.0f, val); }
	inline float GetReflectivity() const { return m_reflectivity; }

	inline void SetRefractionIndex(float val) { m_refractionIndex = clamp(0.0f, 100.0f, val); m_refractionRatio = 1.0003f / m_refractionIndex; }
	inline float GetRefractionIndex() const { return m_refractionIndex; }
};

