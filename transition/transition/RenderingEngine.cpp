#include "RenderingEngine.h"
#include "RenderingNode.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

RenderingEngine::RenderingEngine(const int width, const int height)
{
	this->root_node_ = new GroupNode();
	this->width_ = width;
	this->height_ = height;
}


RenderingEngine::~RenderingEngine()
{
	delete this->root_node_;
}

void RenderingEngine::run()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const auto window = glfwCreateWindow(this->width_, this->height_, "Transition", nullptr, nullptr);
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

	this->root_node_->init(this);

	this->drawables_ = this->root_node_->get_drawables();
	this->rendering_nodes_ = this->root_node_->get_rendering_nodes();


	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto& rendering_node : this->rendering_nodes_)
		{
			rendering_node->render(this->drawables_);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}