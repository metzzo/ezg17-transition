#pragma once
#include <glm/vec2.hpp>
#include <vector>
struct GLFWwindow;
class GroupNode;
class IResource;
class IDrawable;
class CameraNode;
class MainShader;
class LightNode;
class AnimatorNode;
class ParticleEmitterNode;
struct GLFWwindow;
class DirectionalDepthShader;
class OmniDirectionalDepthShader;

class RenderingEngine
{
	GLFWwindow *window_;
	GroupNode *root_node_;

	std::vector<IDrawable*> drawables_;
	std::vector<IDrawable*> transparent_drawables_;
	std::vector<IResource*>  resources_;
	std::vector<LightNode*> light_nodes_;
	std::vector<AnimatorNode*> animator_nodes_;
	std::vector<ParticleEmitterNode*> particle_emitter_nodes_;

	glm::ivec2 viewport_;
	bool fullscreen_;
	int refresh_rate_;

	MainShader *main_shader_;
	DirectionalDepthShader *directional_depth_shader_;
	OmniDirectionalDepthShader *omni_directional_depth_shader_;
public:
	explicit RenderingEngine::RenderingEngine(const glm::ivec2 viewport, bool fullscreen, int refresh_rate);
	~RenderingEngine();

	void register_resource(IResource *resource);

	void run();

	GroupNode* get_root_node() const
	{
		return this->root_node_;
	}

	const glm::ivec2& get_viewport() const
	{
		return viewport_;
	}

	MainShader *get_main_shader() const
	{
		return this->main_shader_;
	}

	DirectionalDepthShader *get_directional_depth_shader() const
	{
		return this->directional_depth_shader_;
	}
	
	OmniDirectionalDepthShader *get_omni_directional_depth_shader() const
	{
		return this->omni_directional_depth_shader_;
	}

	GLFWwindow* get_window() const {
		return this->window_;
	}
};

