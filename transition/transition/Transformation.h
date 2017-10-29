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

	static Transformation translate(const glm::vec3& translation) {
		return Transformation(glm::translate(translation), glm::translate(-translation));
	}

	static Transformation scale(const glm::vec3& scaling) {
		return Transformation(glm::scale(scaling), glm::scale(glm::vec3(1.0) / scaling));
	}

	static Transformation rotate_around_world_origin(float angle, const glm::vec3& axis) {
		return Transformation(glm::rotate(glm::radians(angle), axis), glm::rotate(glm::radians(-angle), axis));
	}

	static Transformation rotate_around_point(float angle, const glm::vec3& axis, const glm::vec3& center) {
		return Transformation(glm::translate(center) * glm::rotate(glm::radians(angle), axis) * glm::translate(-center),
			glm::translate(center) * glm::rotate(glm::radians(-angle), axis) * glm::translate(-center));
	}

};