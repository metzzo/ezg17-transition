// transition.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include <iostream>
#include "RenderingEngine.h"
#include "CameraNode.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GroupNode.h"
#include "ColladaImporter.h"
#include "LightNode.h"
#include "CameraController.h"


int main()
{
	int WINDOW_WIDTH = 1600;
	int WINDOW_HEIGHT = 900;
	bool WINDOW_FULLSCREEN = false;
	int REFRESH_RATE = 60;

	auto engine = new RenderingEngine(glm::ivec2(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_FULLSCREEN, REFRESH_RATE);
	auto root = engine->get_root_node();

	const auto cam = new CameraNode("MainCamera",
		engine->get_viewport(),
		glm::perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f)		
	);
	cam->set_view_matrix(glm::lookAt(glm::vec3(0, 5, 0), glm::vec3(-10, 3, 0), glm::vec3(0, 1, 0)));
	root->add_node(cam);

	auto importer = new ColladaImporter(engine);
	//const auto world = importer->load_node("assets/models/gitti_d.dae");
	const auto world = importer->load_node("assets/models/world1.dae");
	root->add_node(world);

	auto dir_light = new LightNode("test", DIRECTIONAL_LIGHT);
	dir_light->set_attenuation(1.0, 0.25, 0.44);
	dir_light->set_color(glm::vec3(0.5, 0.0, 0.0), glm::vec3(0.7, 0.0, 0.0));
	dir_light->set_shadow_casting(true, 1024);
	root->add_node(dir_light);
	
	auto anim = new CameraController("cam_anim", cam, engine);
	root->add_node(anim);

	engine->run();

	delete world;
	delete importer;
    return 0;
}

