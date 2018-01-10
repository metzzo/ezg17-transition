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
#include "LightKeyPointAction.h"

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

	auto tree_light = new LightNode("tree_light", SPOT_LIGHT);
	tree_light->set_attenuation(1.0, 0.0, 0.0);
	tree_light->set_color(glm::vec3(0.9, 0.9, 0.8), glm::vec3(0.8, 0.8, 0.8));
	tree_light->set_cutoff(12.5f, 50.0f);
	tree_light->set_shadow_strategy(new DirectionalShadowStrategy(4096), 0, 0);
	tree_light->set_volumetric(true, 100000000, 0.00001, false, 64);
	tree_light->set_view_matrix(glm::lookAt(glm::vec3(120, 50, -19), glm::vec3(92, 21, -19), glm::vec3(0, 1, 0)));
	((GroupNode*)root->find_by_name("treeroom"))->add_node(tree_light);

	auto cam_spline_controller = new CameraSplineController("spline_cam_controller", cam, root);

	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-0.792052, 2.75029, 17.6397), glm::vec3(-8.24572, 6, -0.00862241), 0));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-2.47167, 2.75029, 13.1526), glm::vec3(-8.24572, 6, -0.00862241), 10, { new AccelerateKeyPointAction(12, 1), new LightKeyPointAction(broken_lamp, cam) })); // beginning
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.79751, 8, 0.205841), glm::vec3(-8.24572, 6, -0.00862241), 9)); // looking outside window
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-4.97889, 9, -10.0065), glm::vec3(-8.24572, 6, -0.00862241), 9)); // behind pointlight looking outside window
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(7.5401, 6.6805, -4.51286), glm::vec3(0.652233, 2.00272, 5.77127), 8));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.39103, 4.47128, 12.2159), glm::vec3(0.652233, 2.00272, 5.77127), 8));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.86342, 4.67726, 13.229), glm::vec3(2.00122, 1.34711, 7.50525), 0.1, { new AccelerateKeyPointAction(12, 0) })); // behind feet, looking at feet
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.49173, 4.67726, 7.46964), glm::vec3(3.00646, 4.7618, -18.3715), 8)); // going outside
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.28939, 4.67726, 0.917417), glm::vec3(3.00646, 4.7618, -18.3715), 8)); // going outside
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.21971, 4.67726, -9.19597), glm::vec3(3.00646, 4.7618, -18.3715), 8)); // going outside
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.27103, 4.67726, -15.3473), glm::vec3(3.00646, 4.7618, -18.3715), 8)); // going outside
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(20.2455, 4.67726, -18.7901), glm::vec3(20.2455, 4.67726, -20.7901), 8)); // hallway


	/*cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-1.92962, 2.48193, 17.4937), glm::vec3(-4.1285, 8.93705, -9.08716), 0)); // farther from behind
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-2.28499, 2.48193, 13.1978), glm::vec3(-4.1285, 8.93705, -9.08716), 15, { new AccelerateKeyPointAction(12, 0), new LightKeyPointAction(broken_lamp, cam) })); // from behind to pointlight
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(5.74652, 7.04351, 0.273838), glm::vec3(0, 0, -2), 15)); // looking outside the window (normal to it)
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-6.02512, 8.63123, -8.76214), glm::vec3(0, 0, -2), 18)); // looking to the boxes from the pointlight
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(0.101833, 5.40085, 15.1507), glm::vec3(0, 0, -2), 14, { new AccelerateKeyPointAction(12, 0) })); // going towards door
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(0.672872, 4.50955, -22.6491), glm::vec3(0.234719, 4.50955, -26.9259), 20)); // second room
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(0.672872, 4.50955, -22.6491), glm::vec3(0.234719, 4.50955, -28.9259), 20)); // second room
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(0.672872, 4.50955, -22.6491), glm::vec3(0.234719, 4.50955, -30.9259), 20)); // second room
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(41.7444, 3.50056, -21.9777), glm::vec3(50.7807, 3.50056, -20.9662), 20)); // third room*/


	/*cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-2.55638, 2.61466, 17.1405), glm::vec3(-3.79753, 8.84761, -6.76775), 0));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-2.40652, 2.61466, 13.6777), glm::vec3(-3.79753, 8.84761, -6.76775), 15)); // , { new AccelerateKeyPointAction(12, 0), new LightKeyPointAction(broken_lamp, cam) }));

	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-6.45114, 5.0, 0.177606), glm::vec3(-3.79753, 8.84761, -6.76775), 18)); // front of window => looking at  lamp
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(8, 9.23145, 0.109445), glm::vec3(-3.79753, 8.84761, -6.76775), 13)); // go to wall, looking at lamp
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-6.20997, 8, -11.4241), glm::vec3(-0.89963, 0.536566, 6.61612), 18 + 15, { new ShowFeetKeyPointAction(footanim) })); // at light => looking towards feet
	//cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-1.4495, 3.92423, 11.7188), glm::vec3(-0.89963, 0.536566, 6.61612), 15, { new ShowFeetKeyPointAction(footanim) })); // behind feet => looking towards door
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(0.101833, 5.40085, 15.1507), glm::vec3(2.61463, 5.40085, -15.9474), 14, { new AccelerateKeyPointAction(12, 0) })); // going towards door
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(0.672872, 4.50955, -22.6491), glm::vec3(0.234719, 4.50955, -26.9259), 20)); // second room
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(41.7444, 3.50056, -21.9777), glm::vec3(50.7807, 3.50056, -20.9662), 20)); // third room*/

	cam_spline_controller->build_spline();
	root->add_node(cam_spline_controller);

	ShaderResource* main_shader = engine->get_main_shader();
	auto right = new FootstepNode("emitter", main_shader, false);
	root->add_node(right);
	right->apply_transformation(Transformation::translate(glm::vec3(0, 0, 7)));
	auto left = new FootstepNode("emitter", main_shader, true);
	left->set_color(glm::vec3(0, 1, 0));
	root->add_node(left);
	left->apply_transformation(Transformation::translate(glm::vec3(-2, 0, 8)));
	FootstepAnimator* footanim = new FootstepAnimator("footanim", left, right, false, glm::vec3(0, 0, -2), cam_spline_controller);
	root->add_node(footanim);

#ifdef VISUALIZE_KEYPOINTS
	auto anim = new CameraController("cam_anim", cam, root);
	root->add_node(anim);
#endif

	engine->run();

	delete world;
	delete importer;
    return 0;
}

