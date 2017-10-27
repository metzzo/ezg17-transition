// transition.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include <iostream>
#include "RenderingEngine.h"
#include "CameraNode.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GroupNode.h"
#include "ColladaImporter.h"
#include "DirectionalLightNode.h"

int main()
{
	auto engine = new RenderingEngine(glm::ivec2(800, 600));
	auto root = engine->get_root_node();

	const auto cam = new CameraNode("MainCamera",
		engine->get_viewport(),
		glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f)		//TODO: Set according to window size
	);
	cam->set_view_matrix(glm::lookAt(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	root->add_node(cam);

	auto importer = new ColladaImporter(engine);
	const auto world = importer->load_node("assets/models/gitti.dae");
	root->add_node(world);

	const auto dir_light = new DirectionalLightNode("dir_light1");
	dir_light->set_view_matrix(glm::lookAt(glm::vec3(-5, -5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	root->add_node(dir_light);

	engine->run();

	delete world;
	delete importer;
    return 0;
}

