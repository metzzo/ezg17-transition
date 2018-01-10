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
public:
	BrokenLampController(std::string name, LightNode *lamp);
	~BrokenLampController();

	void update(double delta) override;
	void init(RenderingEngine* rendering_engine) override;
	void set_brightness(double brightness);
};
