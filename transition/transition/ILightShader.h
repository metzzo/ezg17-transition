#pragma once
#include <glm/glm.hpp>
class ILightShader
{
public:
	virtual ~ILightShader() = default;
	virtual void set_light_ambient(int light_index, const glm::vec3& color) const = 0;
	virtual void set_light_diffuse(int light_index, const glm::vec3& color) const = 0;
	virtual void set_light_specular(int light_index, const glm::vec3& color) const = 0;
	virtual void set_light_linear(const int light_index, const float linear) const = 0;
	virtual void set_light_quadratic(const int light_index, const float quadratic) const = 0;
};

