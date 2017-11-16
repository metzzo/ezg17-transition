#include "RenderingEngine.h"
#include "RenderingNode.h"
#include "glheaders.h"
#include <iostream>
#include "GroupNode.h"
#include "IResource.h"
#include "MainShader.h"
#include "GLDebugContext.h"
#include "AnimatorNode.h"

RenderingEngine::RenderingEngine(const glm::ivec2 viewport, bool fullscreen, int refresh_rate)
{
	this->root_node_ = new GroupNode("root");
	this->viewport_ = viewport;
	this->fullscreen_ = fullscreen;
	this->refresh_rate_ = refresh_rate;

	this->main_shader_ = new MainShader();
	this->register_resource(this->main_shader_);
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

	const auto window = glfwCreateWindow(this->viewport_.x, this->viewport_.y, "Transition", monitor, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);


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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	for (auto& resource : resources_)
	{
		resource->init();
	}

	this->root_node_->init(this);

	this->drawables_ = this->root_node_->get_drawables();
	this->rendering_nodes_ = this->root_node_->get_rendering_nodes();
	this->light_nodes_ = this->root_node_->get_light_nodes();
	this->animator_nodes_ = this->root_node_->get_animator_nodes();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	double last_time = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		double current_time = glfwGetTime();
		double delta = current_time - last_time;
		//std::cout << 1/delta << std::endl;
		last_time = current_time;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}

		for (auto& animator_node : this->animator_nodes_)
		{
			animator_node->update(delta);
		}

		for (auto& rendering_node : this->rendering_nodes_)
		{
			rendering_node->render(this->drawables_, this->light_nodes_);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	for (auto& resource : resources_) {
		delete resource;
	}
}
