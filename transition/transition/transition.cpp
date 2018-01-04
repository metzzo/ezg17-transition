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
#include "GeometryNode.h"
#include "DirectionalShadowStrategy.h"
#include "RenderingEngine.h"
#include "OmniDirectionalShadowStrategy.h"
#include "LookAtController.h"

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
	cam->set_view_matrix(glm::lookAt(glm::vec3(-2, 5, -7), glm::vec3(0, 0, -16), glm::vec3(0, 1, 0)));
	root->add_node(cam);

	auto importer = new ColladaImporter(engine);
	//const auto world = importer->load_node("assets/models/gitti_d.dae");
	const auto world = importer->load_node("assets/models/world1.dae");
	root->add_node(world);

	/*auto dir_light = new LightNode("test", DIRECTIONAL_LIGHT);
	dir_light->set_attenuation(1.0, 0.07, 0.017);
	dir_light->set_color(glm::vec3(0.8, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0));
	dir_light->set_shadow_casting(true, 1024);
	dir_light->set_view_matrix(glm::lookAt(glm::vec3(0, 5, 0), glm::vec3(-10, 3, 0), glm::vec3(0, 1, 0)));
	
	root->add_node(dir_light);*/

	auto spot_light = new LightNode("test", SPOT_LIGHT);
	spot_light->set_attenuation(1.0, 0.027, 0.0028);
	spot_light->set_color(glm::vec3(0.8, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0));
	spot_light->set_cutoff(12.5f, 25.0f);
	spot_light->set_shadow_strategy(new DirectionalShadowStrategy(1024));
	spot_light->set_volumetric(true, 10000.0, 0.05);
	//spot_light->set_volumetric(true, 500.0, 0.1);
	spot_light->set_view_matrix(glm::lookAt(glm::vec3(-2, 5, -7), glm::vec3(0, 0, -16), glm::vec3(0, 1, 0)));
	root->add_node(spot_light);

	auto anim2 = new CameraController("cam_anim", cam);
	root->add_node(anim2);

	root->add_node(new LookAtController("lookat", cam, spot_light));
	
	engine->run();

	delete world;
	delete importer;
    return 0;
}

