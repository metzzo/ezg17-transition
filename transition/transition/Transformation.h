#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transformation {

private:
	glm::mat4 transformation;
	glm::mat4 inverse_transformation;

public:
	Transformation() {
		this->transformation = this->inverse_transformation = glm::mat4(1.0f);
	}

	Transformation(const glm::mat4& transformation, const glm::mat4& inverse_transformation) {
		this->transformation = transformation;
		this->inverse_transformation = inverse_transformation;
	}

	const glm::mat4& get_transformation_matrix() const {
		return transformation;
	}

	const glm::mat4& get_inverse_transformation_matrix() const {
		return inverse_transformation;
	}

	static Transformation translate(const glm::vec3& translation);

	static Transformation scale(const glm::vec3& scaling);

	static Transformation rotate_around_world_origin(float angle, const glm::vec3& axis);

	static Transformation rotate_around_point(float angle, const glm::vec3& axis, const glm::vec3& center);

};