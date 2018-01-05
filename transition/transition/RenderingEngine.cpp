#include "RenderingEngine.h"
#include "RenderingNode.h"
#include "glheaders.h"
#include <iostream>
#include "GroupNode.h"
#include "IResource.h"
#include "MainShader.h"
#include "GLDebugContext.h"
#include "AnimatorNode.h"
#include "DirectionalDepthShader.h"
#include "CameraNode.h"
#include "ParticleEmitterNode.h"
#include "OmniDirectionalDepthShader.h"
#include "ComputeShader.h"
#include "FrustumG.h"

RenderingEngine::RenderingEngine(const glm::ivec2 viewport, bool fullscreen, int refresh_rate)
{
	this->root_node_ = new GroupNode("root");
	this->viewport_ = viewport;
	this->fullscreen_ = fullscreen;
	this->refresh_rate_ = refresh_rate;
	this->window_ = nullptr;

	this->main_shader_ = new MainShader();
	this->register_resource(this->main_shader_);

	this->directional_depth_shader_ = new DirectionalDepthShader();
	this->register_resource(this->directional_depth_shader_);

	this->omni_directional_depth_shader_ = new OmniDirectionalDepthShader();
	this->register_resource(this->omni_directional_depth_shader_);
}

RenderingEngine::~RenderingEngine()
{
	delete this->root_node_;
}

void RenderingEngine::register_resource(IResource* resource)
{
	this->resources_.push_back(resource);
}

void error_callback(int error, const char* description)
{
	std::cout << "Error: " << std::string(description) << std::endl;
}

void RenderingEngine::run()
{
	glfwSetErrorCallback(error_callback);

	glfwInit();

#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = nullptr;
	if (fullscreen_) {
		monitor = glfwGetPrimaryMonitor();
		glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate_);
	}

	window_ = glfwCreateWindow(this->viewport_.x, this->viewport_.y, "Transition", monitor, nullptr);
	if (window_ == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window_);

	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return;
	}

	//Set DebugContext Callback
#if _DEBUG
	// Query the OpenGL function to register your callback function.
	const PFNGLDEBUGMESSAGECALLBACKPROC _glDebugMessageCallback = PFNGLDEBUGMESSAGECALLBACKPROC(glfwGetProcAddress("glDebugMessageCallback"));

	// Register your callback function.
	if (_glDebugMessageCallback != nullptr) {
		_glDebugMessageCallback(DebugCallback, nullptr);
	}

	// Enable synchronous callback. This ensures that your callback function is called
	// right after an error has occurred. 
	if (_glDebugMessageCallback != nullptr) {
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}
#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	for (auto& resource : resources_)
	{
		resource->init();
	}

	this->root_node_->init(this);

	this->drawables_ = this->root_node_->get_drawables();
	this->transparent_drawables_ = this->root_node_->get_transparent_drawables();
	this->light_nodes_ = this->root_node_->get_light_nodes();
	this->animator_nodes_ = this->root_node_->get_animator_nodes();
	this->particle_emitter_nodes_ = this->root_node_->get_particle_emitter_nodes();

	const auto main_camera = static_cast<CameraNode*>(this->root_node_->find_by_name("MainCamera"));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	double last_time = glfwGetTime();
	double last_fps_time = 0;
	while (!glfwWindowShouldClose(window_))
	{
		double current_time = glfwGetTime();
		double delta = current_time - last_time;
		if (current_time - last_fps_time > 1) {
			std::cout << 1 / delta << std::endl;
			last_fps_time = current_time;
		}
		last_time = current_time;

		if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window_, true);
		}
		/*if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
			for (auto& particle_node : this->particle_emitter_nodes_) {
				particle_node->start_emitting();
			}
		}*/

		for (auto& animator_node : this->animator_nodes_)
		{
			animator_node->update(delta);
		}
		for (auto& particle_node : this->particle_emitter_nodes_) {
			particle_node->update_particles(delta);
		}

		main_camera->render(this->drawables_, this->transparent_drawables_, this->particle_emitter_nodes_, this->light_nodes_);

		glfwSwapBuffers(window_);
		glfwPollEvents();
	}
	glfwTerminate();

	for (auto& resource : resources_) {
		delete resource;
	}
}
