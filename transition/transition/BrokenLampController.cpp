
#include "BrokenLampController.h"
#include "LightNode.h"
#include <glm/gtc/noise.hpp>
#include "RenderingEngine.h"
#include "GroupNode.h"
#include "GeometryNode.h"
#include "MeshResource.h"

BrokenLampController::BrokenLampController(std::string name, LightNode* lamp) : AnimatorNode(name)
{
	this->lamp_ = lamp;
	this->on_diffuse_color_ = lamp->get_diffuse();
	this->on_specular_color_ = lamp->get_specular();
	this->brightness_ = 1.0;
	this->geometry_ = nullptr;
	this->progress_ = 0.0;
	this->target_swing_angle_ = glm::radians(30.0);
	this->swing_angle_ = glm::radians(0.0);
	this->last_angle_ = this->swing_angle_;
	this->last_quake_ = 10.0;
	this->quake_duration_ = 1.0;
	this->quake_intensity_ = 1.0;
	this->quake_direction_ = glm::vec3(1.0, 0.0, 0.0);
}

BrokenLampController::~BrokenLampController()
{
}

void BrokenLampController::update(double delta)
{

	const auto time = glfwGetTime();
	
	double val = glm::perlin(glm::vec2(time, 0.0));
	val *= val;

	auto &mat = this->geometry_->get_editable_mesh_resource()->get_editable_material();

	const auto diff_color = mix(on_diffuse_color_, on_diffuse_color_*0.5f, val) * float(brightness_);
	const auto spec_color = mix(on_specular_color_, on_specular_color_*0.5f, val) * float(brightness_);
	const auto ambient_color = mix(glm::vec3(1), glm::vec3(0), val) * float(brightness_);

	this->lamp_->set_color(
		diff_color, 
		spec_color
	);

	mat.set_ambient_color(ambient_color);

	this->progress_ += delta;

#ifdef DEBUG_KEYS
	if (glfwGetKey(this->get_rendering_engine()->get_window(), GLFW_KEY_SPACE))
	{
		this->quake(10.0, 2.5);
	}
#endif

	auto interpolant =  (1.0 + cos((this->progress_ - this->last_quake_ - glm::radians(90.0)) * 5))/2.0;
	interpolant *= 1.0 - glm::clamp(this->progress_ - this->last_quake_, 0.0, this->quake_duration_) / this->quake_duration_;

	auto angle = glm::mix(this->swing_angle_, this->target_swing_angle_, interpolant);
	auto angle_delta = angle - last_angle_;
	auto trafo = Transformation::rotate_around_point(angle_delta * quake_intensity_, this->quake_direction_, this->origin_pos_);


	this->whole_lamp_geometry_->apply_transformation(trafo.get_transformation_matrix(), trafo.get_inverse_transformation_matrix());

	this->lamp_->set_transformation(glm::translate(this->geometry_->get_position()));

	this->last_angle_ = angle;
}

void BrokenLampController::init(RenderingEngine* rendering_engine)
{
	AnimatorNode::init(rendering_engine);

	this->geometry_ = static_cast<GeometryNode*>(rendering_engine->get_root_node()->find_by_name("BulbHead_0"));
	this->whole_lamp_geometry_ = static_cast<GroupNode*>(rendering_engine->get_root_node()->find_by_name("LightBulb"));
	assert(this->whole_lamp_geometry_ != nullptr);

	this->origin_pos_ = this->geometry_->get_position() - glm::vec3(0,-3,0);
	this->set_brightness(0.0);

}

void BrokenLampController::set_brightness(double brightness)
{
	this->brightness_ = brightness;
}

void BrokenLampController::quake(double intensity, double duration, bool random_direction)
{
	this->last_quake_ = this->progress_;
	this->quake_intensity_ = intensity;
	this->quake_duration_ = duration;
	if (random_direction) {
		auto r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2 * glm::pi<float>();
		this->quake_direction_ = glm::normalize(glm::vec3(cos(r), 0, sin(r)));
	} else
	{
		this->quake_direction_ = glm::vec3(0.0, 0.0, 1.0);
	}
	//auto trafo = Transformation::rotate_around_point(glm::radians(15.0), glm::vec3(0.0, 1.0, 0.0), this->origin_pos_);
	//this->whole_lamp_geometry_->apply_transformation(trafo.get_transformation_matrix(), trafo.get_inverse_transformation_matrix());
}
