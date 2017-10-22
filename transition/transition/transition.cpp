// transition.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include <iostream>
#include "conio.h"
#include "RenderingEngine.h"
#include "CameraNode.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GeometryNode.h"
#include "MeshResource.h"
#include "GroupNode.h"

int main()
{
	auto engine = RenderingEngine(glm::ivec2(800, 600));
	auto root = engine.get_root_node();

	const auto cam = new CameraNode("MainCamera",
		engine.get_viewport(),
		glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f)
	);
	root->add_node(cam);

	const auto mesh = new MeshResource();
	engine.register_resource(mesh);

	const auto triangle = new GeometryNode("Triangle", mesh);
	root->add_node(triangle);

	engine.run();

    return 0;
}

