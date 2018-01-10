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

	auto feet_pos = (glm::vec3(0, 0, 7) + glm::vec3(-2, 0, 8)) / 2.0f;

	auto cam_spline_controller = new CameraSplineController("spline_cam_controller", cam, root);
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-1.91211, 2.66449, 17.5909), glm::vec3(-5.39691, 9.64562, -8.49919), 0)); // back
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-2.52911, 2.66449, 12.9266), glm::vec3(-5.39691, 9.64562, -8.49919), 9, { new AccelerateKeyPointAction(12, 4), new LightKeyPointAction(broken_lamp, cam) })); // start

	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(6.35672, 8.20666, -0.0386879), glm::vec3(-8.86503, 8.20666, -0.0328253), 6)); // in front of window lookiong outside window

	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.42228, 9.55154, -8.97531), feet_pos, 12)); // moving towards feet
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-5.87092, 9.04411, -9.09759), feet_pos, 5)); // still moving
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-8.01213, 7.58564, -0.557837), feet_pos, 5)); // still
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-7.82971, 7.58564, 14.3606), glm::vec3(2.74811, 3.93666, -25.5096), 5)); // in corner, behind feet
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-0.0275997, 3.93666, 12.0401), glm::vec3(2.74811, 3.93666, -25.5096), 5)); // behind feet

	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.12464, 3.93666, 10.4443), glm::vec3(2.74811, 3.93666, -25.5096), 5)); // moving towards door
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.39148, 3.93666, 5.31498), glm::vec3(2.74811, 3.93666, -25.5096), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.73851, 3.93666, -1.35578), glm::vec3(2.74811, 3.93666, -25.5096), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.12215, 3.93666, -8.7304), glm::vec3(2.74811, 3.93666, -25.5096), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.23171, 3.93666, -12.9764), glm::vec3(2.74811, 3.93666, -25.5096), 5));

	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.91994, 3.93666, -16.3544), glm::vec3(-1.14446, 1.79147, -25.2764), 5)); // turning around chair
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(6.16065, 3.93666, -24.8615), glm::vec3(-1.14446, 1.79147, -25.2764), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(0.285496, 3.93666, -28.8241), glm::vec3(-1.14446, 1.79147, -25.2764), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-10.3406, 7.34465, -25.8606), glm::vec3(-5.50127, 3.93666, -34.3371), 5)); // looking at photos

																																		  // looking at photos
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-8.43955, 5.22302, -30.7933), glm::vec3(-5.50127, 3.93666, -34.3371), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-5.56638, 3.36887, -31.824), glm::vec3(-5.50127, 3.93666, -34.3371), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-2.76393, 3.36887, -28.3218), glm::vec3(-5.50127, 3.93666, -34.3371), 5));

	// going outside
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-0.00909492, 7.5871, -23.6803), glm::vec3(50.1475, 7.5871, -21.8012), 5));

	// hallway
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.61161, 7.5871, -21.6465), glm::vec3(50.1475, 7.5871, -21.8012), 5));


	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(23.6967, 4.77344, -21.4799), glm::vec3(91.0045, 4.77344, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(47.7877, 4.77344, -22.2962), glm::vec3(91.0045, 4.77344, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(55.1978, 4.77344, -22.1224), glm::vec3(91.0045, 4.77344, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(62.5513, 4.77344, -21.8042), glm::vec3(91.0045, 4.77344, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(70.5708, 4.77344, -21.4571), glm::vec3(91.0045, 4.77344, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(77.0854, 4.77344, -21.1752), glm::vec3(91.0045, 4.77344, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(84.3001, 4.77344, -11.5358), glm::vec3(91.0045, 4.77344, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 16.3427, -16.337), glm::vec3(91.0045, 4.77344, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 18, -16.337), glm::vec3(91.0045, 4.77344, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 30.3427, -16.337), glm::vec3(91.0045, 200, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 40.3427, -16.337), glm::vec3(91.0045, 200, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 50.3427, -16.337), glm::vec3(91.0045, 200, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 60.3427, -16.337), glm::vec3(91.0045, 200, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 70.3427, -16.337), glm::vec3(91.0045, 200, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 80.3427, -16.337), glm::vec3(91.0045, 200, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 90.3427, -16.337), glm::vec3(91.0045, 200, -19.635), 5));



	/*
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-0.792052, 2.75029, 17.6397), glm::vec3(-8.24572, 6, -0.00862241), 0));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-2.47167, 2.75029, 13.1526), glm::vec3(-8.24572, 6, -0.00862241), 10, { new AccelerateKeyPointAction(12, 1)/*, new LightKeyPointAction(broken_lamp, cam) })); // beginning
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.79751, 8, 0.205841), glm::vec3(-8.24572, 6, -0.00862241), 9)); // looking outside window
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-4.97889, 9, -10.0065), glm::vec3(-8.24572, 6, -0.00862241), 9)); // behind pointlight looking outside window
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-9.07077, 8.19345, -6.57903), glm::vec3(-6.93525, 5.16555, -1.25925), 0));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-7.45869, 9.07482, -0.557905), glm::vec3(-0.47431, 0.8369, 6.04253), 0));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.39103, 4.47128, 12.2159), glm::vec3(0.652233, 2.00272, 5.77127), 8));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.86342, 4.67726, 13.229), glm::vec3(3.00646, 4.7618, -18.3715), 0, { new AccelerateKeyPointAction(12, 0) })); // behind feet, looking at feet
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.49173, 4.67726, 7.46964), glm::vec3(3.00646, 4.7618, -18.3715), 3)); // going outside
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.28939, 4.67726, 0.917417), glm::vec3(3.00646, 4.7618, -18.3715), 3)); // going outside
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.21971, 4.67726, -9.19597), glm::vec3(3.00646, 4.7618, -18.3715), 3)); // going outside

	// living room
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.13419, 4.94461, -13.5829), glm::vec3(-1.555, 1.55026, -24.9079), 5)); // entering room 2 looking at chair
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(4.05927, 4.94461, -22.754), glm::vec3(-1.555, 1.55026, -24.9079), 5)); // in front of chair looking at chair
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-8.56649, 8.15391, -20.3215), glm::vec3(-1.555, 1.55026, -24.9079), 5)); // at corner 1 looking at chair
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-9.98949, 5.38644, -27.6355), glm::vec3(-5.70414, 2.48224, -34.3158), 5)); // half way to the pictures, looking at pictures
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-7.38124, 3.93454, -32.1539), glm::vec3(-5.70414, 2.48224, -34.3158), 15)); // in front of pictures, looking at pictures


	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.6073, 3.93454, -26.7826), glm::vec3(6.65277, 3.93454, -33.1944), 2)); // going towards the door, looking at shelf

	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(4.12398, 3.93454, -20.5158), glm::vec3(11.483, 5.09146, -21.5294), 2)); // going towards the door, looking at door
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(6.76678, 4.91991, -22.3691), glm::vec3(11.483, 5.09146, -21.5294), 2)); // going towards the door, looking at door

	// hall way
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(13.4815, 4.91991, -21.3224), glm::vec3(49.0939, 4.91991, -21.9576), 1));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(18.0386, 4.91991, -21.7634), glm::vec3(49.0939, 4.91991, -21.9576), 1));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(24.5586, 4.91991, -22.0505), glm::vec3(49.0939, 4.91991, -21.9576), 1));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(32.127, 4.91991, -22.3839), glm::vec3(49.0939, 4.91991, -21.9576), 1));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(40.0309, 4.91991, -22.732), glm::vec3(49.0939, 4.91991, -21.9576), 1));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(40.8905, 4.91991, -21.8263), glm::vec3(59.1312, 4.91991, -22.1257), 1));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(45.7112, 4.91991, -21.6177), glm::vec3(59.1312, 4.91991, -22.1257), 1));*/


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
	FootstepAnimator* footanim = new FootstepAnimator("footanim", left, right, false, glm::vec3(0, 0, -2));
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

