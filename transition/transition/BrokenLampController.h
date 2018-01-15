#pragma once
#include "AnimatorNode.h"
#include "AccelerateKeyPointAction.h"

class LightNode;

class BrokenLampController : public AnimatorNode
{
	LightNode* lamp_;
	glm::vec3 on_specular_color_;
	glm::vec3 on_diffuse_color_;
	double brightness_;
	GeometryNode* geometry_;
	GroupNode* whole_lamp_geometry_;
	double progress_;
	glm::vec3 origin_pos_;
	double swing_angle_;
	double target_swing_angle_;
	double last_angle_;
	double last_quake_;
	double quake_intensity_;
	double quake_duration_;
	glm::vec3 quake_direction_;
public:
	BrokenLampController(std::string name, LightNode *lamp);
	~BrokenLampController();

	void update(double delta) override;
	void init(RenderingEngine* rendering_engine) override;
	void set_brightness(double brightness);
	void quake(double intensity, double duration, bool random_direction=true);
};
