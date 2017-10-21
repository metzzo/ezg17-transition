#pragma once
#include <glm/glm.hpp>

class IDrawable
{
	glm::mat4 trafo_;
public:
	virtual ~IDrawable() = default;
	virtual void draw() const = 0;

	virtual void set_transformation(const glm::mat4& trafo)
	{
		this->trafo_ = trafo;
	}

	virtual const glm::mat4& get_transformation() const
	{
		return this->trafo_;
	}
};

