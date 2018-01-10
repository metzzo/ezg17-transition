
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
}

void BrokenLampController::init(RenderingEngine* rendering_engine)
{
	this->geometry_ = static_cast<GeometryNode*>(rendering_engine->get_root_node()->find_by_name("BulbHead_0"));
}

void BrokenLampController::set_brightness(double brightness)
{
	this->brightness_ = brightness;
}
