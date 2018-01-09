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
#include "LookAtController.h"
#include "CarController.h"
#include "FootstepNode.h"
#include "FootstepAnimator.h"
#include "BrokenLampController.h"
#include "CameraSplineController.h"
#include "AccelerateKeyPointAction.h"
#include "ShowFeetKeyPointAction.h"

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
		60.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 75.0f, true
	);
	cam->set_bloom_params(0, 1.0, 0);
	cam->set_view_matrix(glm::lookAt(glm::vec3(6.11709, 5.40085, -9.8344), glm::vec3(-4.42165, 5.40085, -3.74445), glm::vec3(0, 1, 0)));
	root->add_node(cam);

	auto importer = new ColladaImporter(engine);
	const auto world = importer->load_node("assets/models/world1.dae");
	root->add_node(world);

	auto car_light = new LightNode("car_light", SPOT_LIGHT);
	car_light->set_attenuation(1.0, 0.027, 0.0028);
	car_light->set_color(glm::vec3(255/255.0, 255 / 255.0, 223 / 255.0), glm::vec3(1.0, 0.0, 0.0));
	car_light->set_cutoff(12.5f, 50.0f);
	car_light->set_shadow_strategy(new DirectionalShadowStrategy(1024), 0, 0);
	car_light->set_volumetric(true, 20000.0, 0.05, true, 16);
	((GroupNode*)root->find_by_name("darkroom"))->add_node(car_light);

	auto car_anim = new CarController("car_anim", car_light);
	root->add_node(car_anim);

	auto lamp = static_cast<LightNode*>(root->find_by_name("Lamp"));
	lamp->set_volumetric(true, 1000.0, 0.01, true, 8);

	auto broken_lamp = new BrokenLampController("broken_lamp", lamp);
	root->add_node(broken_lamp);

	root->add_node(new LookAtController("lookat", cam, car_light));

	ShaderResource* main_shader = engine->get_main_shader();
	auto right = new FootstepNode("emitter", main_shader, false);
	root->add_node(right);
	right->apply_transformation(Transformation::translate(glm::vec3(0, 0, 7)));
	auto left = new FootstepNode("emitter", main_shader, true);
	left->set_color(glm::vec3(0, 1, 0));
	root->add_node(left);
	left->apply_transformation(Transformation::translate(glm::vec3(-2, 0, 8)));
	FootstepAnimator* footanim = new FootstepAnimator("footanim", left, right, false, glm::vec3(0, 0, -2));
	root->add_node(footanim);

	auto tree_light = new LightNode("tree_light", SPOT_LIGHT);
	tree_light->set_attenuation(1.0, 0.0, 0.0);
	tree_light->set_color(glm::vec3(0.9, 0.9, 0.8), glm::vec3(0.8, 0.8, 0.8));
	tree_light->set_cutoff(12.5f, 50.0f);
	tree_light->set_shadow_strategy(new DirectionalShadowStrategy(4096), 0, 0);
	tree_light->set_volumetric(true, 100000000, 0.00001, false, 64);
	tree_light->set_view_matrix(glm::lookAt(glm::vec3(120, 50, -19), glm::vec3(92, 21, -19), glm::vec3(0, 1, 0)));
	((GroupNode*)root->find_by_name("treeroom"))->add_node(tree_light);

	auto cam_spline_controller = new CameraSplineController("spline_cam_controller", cam, root);

	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-2.55638, 2.61466, 17.1405), glm::vec3(-3.79753, 8.84761, -6.76775), 0));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-2.40652, 2.61466, 13.6777), glm::vec3(-3.79753, 8.84761, -6.76775), 15, { new AccelerateKeyPointAction(12, 1) }));

	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-6.45114, 5.0, 0.177606), glm::vec3(-3.79753, 8.84761, -6.76775), 18)); // front of window => looking at  lamp
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(8, 9.23145, 0.109445), glm::vec3(-3.79753, 8.84761, -6.76775), 13)); // go to wall, looking at lamp
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-6.20997, 8, -11.4241), glm::vec3(-0.89963, 0.536566, 6.61612), 18)); // at light => looking towards feet
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-1.4495, 3.92423, 11.7188), glm::vec3(-0.89963, 0.536566, 6.61612), 15, { new ShowFeetKeyPointAction(footanim) })); // behind feet => looking towards door
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.87209, 4.50955, -10.1275), glm::vec3(0.234719, 4.50955, -26.9259), 14)); // going towards door
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(0.672872, 4.50955, -22.6491), glm::vec3(0.234719, 4.50955, -26.9259), 20)); // second room
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(41.7444, 3.50056, -21.9777), glm::vec3(50.7807, 3.50056, -20.9662), 20)); // third room

	cam_spline_controller->build_spline();
	root->add_node(cam_spline_controller);

#ifdef VISUALIZE_KEYPOINTS
	auto anim = new CameraController("cam_anim", cam, root);
	root->add_node(anim);
#endif

	engine->run();

	delete world;
	delete importer;
    return 0;
}

