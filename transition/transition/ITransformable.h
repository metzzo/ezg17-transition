#pragma once
#include <glm/glm.hpp>

class ITransformable
{
	glm::mat4 trafo_;
public:
	virtual ~ITransformable() = default;

	virtual void set_transformation(const glm::mat4& trafo)
	{
		this->trafo_ = trafo;
	}

	virtual const glm::mat4& get_transformation() const
	{
		return this->trafo_;
	}
};

