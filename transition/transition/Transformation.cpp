#include "Transformation.h"
#include <iostream>

Transformation Transformation::translate(const glm::vec3& translation) {
	return Transformation(glm::translate(translation), glm::translate(-translation));
}

Transformation Transformation::scale(const glm::vec3& scaling) {
	return Transformation(glm::scale(scaling), glm::scale(glm::vec3(1.0) / scaling));
}

Transformation Transformation::rotate_around_world_origin(float angle, const glm::vec3& axis) {
	return Transformation(glm::rotate(glm::radians(angle), axis), glm::rotate(glm::radians(-angle), axis));
}

Transformation Transformation::rotate_around_point(float angle, const glm::vec3& axis, const glm::vec3& center) {
	glm::mat4 m1 = glm::translate(center);
	glm::mat4 m2a = glm::rotate(glm::radians(+angle), axis);
	glm::mat4 m2b = glm::rotate(glm::radians(-angle), axis);
	glm::mat4 m3 = glm::translate(-center);
	glm::mat4 forward = m1 * m2a * m3;
	glm::mat4 backward = m1 * m2b * m3;
	glm::mat4 test = forward * backward;
	return Transformation(forward, backward);
}