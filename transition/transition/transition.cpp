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
#include "CarController.h"
#include "FootstepNode.h"
#include "FootstepAnimator.h"
#include "BloomEffect.h"
#include "BrokenLampController.h"


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
		60.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 200.0f, true
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

	auto car_light = new LightNode("car_light", SPOT_LIGHT);
	car_light->set_attenuation(1.0, 0.027, 0.0028);
	car_light->set_color(glm::vec3(0.8, 0.8, 0.8), glm::vec3(1.0, 0.0, 0.0));
	car_light->set_cutoff(12.5f, 50.0f);
	car_light->set_shadow_strategy(new DirectionalShadowStrategy(1024), 0, 0);
	car_light->set_volumetric(true, 10000.0, 0.05);
	((GroupNode*)root->find_by_name("darkroom"))->add_node(car_light);

	auto car_anim = new CarController("car_anim", car_light);
	root->add_node(car_anim);

	auto lamp = static_cast<LightNode*>(root->find_by_name("Lamp"));
	lamp->set_volumetric(true, 1000.0, 0.01);

	auto broken_lamp = new BrokenLampController("broken_lamp", lamp);
	root->add_node(broken_lamp);

	root->add_node(new LookAtController("lookat", cam, car_light));
	
	auto anim = new CameraController("cam_anim", cam);
	root->add_node(anim);

	ShaderResource* main_shader = engine->get_main_shader();
	auto right = new FootstepNode("emitter", main_shader, false);
	root->add_node(right);
	right->apply_transformation(Transformation::translate(glm::vec3(0, 0, 7)));
	auto left = new FootstepNode("emitter", main_shader, true);
	root->add_node(left);
	left->apply_transformation(Transformation::translate(glm::vec3(-2, 0, 8)));
	FootstepAnimator* footanim = new FootstepAnimator("footanim", left, right, false, glm::vec3(0, 0, -2));
	root->add_node(footanim);

	//auto tree_light = new LightNode("tree_light", SPOT_LIGHT);
	auto tree_light = new LightNode("tree_light", DIRECTIONAL_LIGHT);
	//tree_light->set_attenuation(1.0, 0.0, 0.0);
	tree_light->set_color(glm::vec3(0.9, 0.9, 0.8), glm::vec3(0.8, 0.8, 0.8));
	//tree_light->set_cutoff(12.5f, 50.0f);
	tree_light->set_shadow_strategy(new DirectionalShadowStrategy(1024), 0, 0);
	//tree_light->set_volumetric(true, 1000000.0, 0.05, false);
	tree_light->set_volumetric(true, 900000.0, 0.04, false);
	tree_light->set_view_matrix(glm::lookAt(glm::vec3(140, 60, -19), glm::vec3(92, 0, -19), glm::vec3(0, 1, 0)));
	((GroupNode*)root->find_by_name("treeroom"))->add_node(tree_light);

	engine->run();

	delete world;
	delete importer;
    return 0;
}

