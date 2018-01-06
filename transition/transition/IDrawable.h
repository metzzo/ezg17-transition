#pragma once
class ShaderResource;
class IDrawable
{
public:
	virtual ~IDrawable() = default;
	virtual void draw(ShaderResource *shader) const = 0;
	virtual float get_bounding_sphere_radius() const = 0;//Yes this is ugly, but wurscht
	virtual glm::vec3 get_position() const = 0;
	virtual bool is_enabled() const = 0;
};

