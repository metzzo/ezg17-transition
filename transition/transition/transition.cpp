// transition.cpp : Definiert den Einstiegspunkt f�r die Konsolenanwendung.
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
#include "RoomEnableKeyPoint.h"
#include "AnimationAction.h"
#include "DoorAnimation.h"
#include "ClearColorAction.h"
#include "PianoAnimation.h"
#include <fstream>
#include "HallLightIncreaseAction.h"
#include "FootColorAction.h"
#include "CullOffAction.h"
#include "BloomAction.h"
#include "StopAction.h"
#include "LightDecreaseAction.h"
#include "FinalParticlesNode.h"
#include "ParticleEmitAction.h"
#include "EndCreditsAction.h"

int main(){
	int window_width = 1600;
	int window_height = 900;
	bool window_fullscreen = false;
	int refresh_rate = 60;

	std::ifstream config("config.txt");
	if (config.is_open())
	{
		std::string line;
		while (std::getline(config, line))
		{
			auto sep = line.find("=");
			auto param = line.substr(0, sep);
			auto value = line.substr(sep + 1);

			if (param == "width")
			{
				window_width = std::stoi(value);
			} else if (param == "height")
			{
				window_height = std::stoi(value);
			}
			else if (param == "fullscreen")
			{
				window_fullscreen = std::stoi(value);
			} else if (param == "refreshrate") {
				refresh_rate = std::stoi(value);
			} else
			{
				std::cout << "Unknown Parameter " << param << std::endl;
			}
		}
		config.close();
	}

	auto engine = new RenderingEngine(glm::ivec2(window_width, window_height), window_fullscreen, refresh_rate);
	auto root = engine->get_root_node();

	const auto cam = new CameraNode("MainCamera",
		engine->get_viewport(),
		60.0f, float(window_width) / float(window_height), 0.05f, 95.0f, true
	);
	cam->set_bloom_params(1, 1.0, 1);
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
	car_light->set_volumetric(true, 20000.0, 0.05, true, 32);
	static_cast<GroupNode*>(root->find_by_name("darkroom"))->add_node(car_light);

	auto car_anim = new CarController("car_anim", car_light);
	root->add_node(car_anim);

	auto lamp = static_cast<LightNode*>(root->find_by_name("Lamp"));
	lamp->set_volumetric(true, 1000.0, 0.01, true, 8);

	auto broken_lamp = new BrokenLampController("broken_lamp", lamp);
	root->add_node(broken_lamp);

	auto tree_light = new LightNode("tree_light", SPOT_LIGHT);
	tree_light->set_attenuation(1.0, 0.0, 0.0);
	tree_light->set_color(glm::vec3(0.9, 0.9, 0.8), glm::vec3(0.8, 0.8, 0.8));
	tree_light->set_cutoff(12.5f, 50.0f);
	tree_light->set_shadow_strategy(new DirectionalShadowStrategy(4096), 0, 0);
	tree_light->set_volumetric(true, 100000000, 0.00001, false, 64);
	tree_light->set_view_matrix(glm::lookAt(glm::vec3(120, 50, -19), glm::vec3(92, 21, -19), glm::vec3(0, 1, 0)));
	static_cast<GroupNode*>(root->find_by_name("treeroom"))->add_node(tree_light);

	//Adapt shadow strategy for living room
	LightNode* living_room_light = static_cast<LightNode*>(root->find_by_name("Point"));
	living_room_light->set_shadow_strategy(living_room_light->get_shadow_strategy(), 0.05, 0.2);

	//Hall Lights
	LightNode* hall_light_1 = static_cast<LightNode*>(root->find_by_name("GangLicht_"));
	LightNode* hall_light_2 = static_cast<LightNode*>(root->find_by_name("GangLicht2_"));
	hall_light_1->set_color(glm::vec3(0), glm::vec3(0));
	hall_light_2->set_color(glm::vec3(0), glm::vec3(0));
	GeometryNode* hall_bulb_1 = static_cast<GeometryNode*>(root->find_by_name("Sphere_002_0"));
	GeometryNode* hall_bulb_2 = static_cast<GeometryNode*>(root->find_by_name("Sphere_003_0"));
	hall_bulb_1->get_editable_mesh_resource()->get_editable_material().set_ambient_color(glm::vec3(0));
	hall_bulb_2->get_editable_mesh_resource()->get_editable_material().set_ambient_color(glm::vec3(0));
	HallLightIncreaseAction* hall_action_1 = new HallLightIncreaseAction(hall_bulb_1, hall_light_1, 8, 3);
	HallLightIncreaseAction* hall_action_2 = new HallLightIncreaseAction(hall_bulb_2, hall_light_2, 8, 3);

	auto door1 = static_cast<GeometryNode*>(root->find_by_name("Door1_0"));
	glm::vec3 d1angle = door1->get_position();
	auto door2a = static_cast<GeometryNode*>(root->find_by_name("DoorADoor_0"));
	auto door2b = root->find_by_name("DoorAHandle_0");
	glm::vec3 d2angle = door2a->get_position();
	auto door3a = static_cast<GeometryNode*>(root->find_by_name("DoorADoor_001_0"));
	auto door3b = root->find_by_name("DoorAHandle_001_0");
	glm::vec3 d3angle = door3a->get_position();

	//auto feet_pos = (glm::vec3(0, 0, 7) + glm::vec3(-2, 0, 8)) / 2.0f;
	auto feet_pos = (glm::vec3(3.8, 0, 5) + glm::vec3(1.8, 0, 5)) / 2.0f;

	ShaderResource* main_shader = engine->get_main_shader();
	auto right = new FootstepNode("emitter", main_shader, false);
	root->add_node(right);
	right->apply_transformation(Transformation::translate(glm::vec3(3.8, 0, 5)));
	auto left = new FootstepNode("emitter", main_shader, true);
	root->add_node(left);
	left->apply_transformation(Transformation::translate(glm::vec3(1.8, 0, 5)));
	FootstepAnimator* footanim = new FootstepAnimator("footanim", left, right, false, glm::vec3(0, 0, -2), broken_lamp);
	root->add_node(footanim);

	auto tree_plane = static_cast<GeometryNode*>(root->find_by_name("Cube_0"));
	AlphaTextureResource* alpha = new AlphaTextureResource("assets/gfx/treeflooralpha1.png");
	engine->register_resource(alpha);
	tree_plane->get_editable_mesh_resource()->get_editable_material().set_alpha_texture(alpha);
	tree_plane->get_editable_mesh_resource()->get_editable_material().set_alpha_cutoff(0);

	auto blood_plane = static_cast<GeometryNode*>(root->find_by_name("Bloodstain_0"));
	alpha = new AlphaTextureResource("assets/gfx/bloooda.png");
	engine->register_resource(alpha);
	blood_plane->get_editable_mesh_resource()->get_editable_material().set_alpha_texture(alpha);
	blood_plane->get_editable_mesh_resource()->get_editable_material().set_alpha_cutoff(0);

	FinalParticlesNode* particle_node = new FinalParticlesNode("Final");
	particle_node->apply_transformation(Transformation::translate(glm::vec3(87.5, 0, -20)));
	root->add_node(particle_node);

	auto cam_spline_controller = new CameraSplineController("spline_cam_controller", cam, root);
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-1.91211, 2.66449, 17.5909), glm::vec3(-5.39691, 9.64562, -8.49919), 0)); // back
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-2.52911, 2.66449, 12.9266), glm::vec3(-5.39691, 9.64562, -8.49919), 9, { new AccelerateKeyPointAction(12, 4), new LightKeyPointAction(broken_lamp, cam) })); // start

	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(6.35672, 8.20666, -0.0386879), glm::vec3(-8.86503, 8.20666, -0.0328253), 6)); // in front of window lookiong outside window

	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.42228, 9.55154, -8.97531), feet_pos, 12)); // moving towards feet
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-5.87092, 9.04411, -9.09759), feet_pos, 5)); // still moving
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-8.01213, 7.58564, -0.557837), feet_pos, 5)); // still
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-7.82971, 7.58564, 12.3606), glm::vec3(2.74811, 3.93666, -25.5096), 5)); // in corner, behind feet
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-0.0275997, 3.93666, 12.0401), glm::vec3(2.74811, 3.93666, -25.5096), 5)); // behind feet

	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.12464, 3.93666, 10.4443), glm::vec3(2.74811, 3.93666, -25.5096), 5)); // moving towards door
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.39148, 3.93666, 5.31498), glm::vec3(2.74811, 3.93666, -25.5096), 5));
	DoorAnimation* door1anim = new DoorAnimation("Door1Anim", door1, d1angle, 240.f / 55.f, true, true);
	root->add_node(door1anim);
	PianoAnimation* pianoanim = new PianoAnimation("PianoAnim", root->find_by_name("ThePiano"));
	root->add_node(pianoanim);
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.73851, 3.93666, -1.35578), glm::vec3(2.74811, 3.93666, -25.5096), 5, { new AnimationAction(door1anim, 0.1), new AnimationAction(pianoanim) }));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.12215, 3.93666, -8.7304), glm::vec3(2.74811, 3.93666, -25.5096), 5, { new FootColorAction(left, glm::vec3(1, 0.5, 0)), new FootColorAction(right, glm::vec3(1, 0.5, 0)), new RoomEnableKeyPointAction(1, true, 0.4) }));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(3.23171, 3.93666, -12.9764), glm::vec3(2.74811, 3.93666, -25.5096), 5, { new LightDecreaseAction(lamp, 4, 1)}));

	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.91994, 3.93666, -16.3544), glm::vec3(-1.14446, 1.79147, -25.2764), 5, { new RoomEnableKeyPointAction(0, false) })); // turning around chair
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(6.16065, 3.93666, -24.8615), glm::vec3(-1.14446, 1.79147, -25.2764), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(0.285496, 3.93666, -28.8241), glm::vec3(-1.14446, 1.79147, -25.2764), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-10.3406, 7.34465, -25.8606), glm::vec3(-5.50127, 3.93666, -34.3371), 5)); // looking at photos

																																		  // looking at photos
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-8.43955, 5.22302, -30.7933), glm::vec3(-5.50127, 3.93666, -34.3371), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-5.56638, 3.36887, -31.824), glm::vec3(10.0, 3.5, -28.0), 5));
	//cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(-2.76393, 3.36887, -28.3218), glm::vec3(-5.50127, 3.93666, -34.3371), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(5.547, 4.14, -32.16), glm::vec3(10.0, 3.5, -28.0), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(4.6, 5, -26.16), glm::vec3(10.0, 3.5, -28.0), 2.5));

	// going outside
	auto door2aanim = new DoorAnimation("Door2aAnim", door2a, d2angle, 1);
	auto door2banim = new DoorAnimation("Door2bAnim", door2b, d2angle, 1);
	root->add_node(door2aanim);
	root->add_node(door2banim);
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.0, 7.5871, -23.6803), glm::vec3(50.1475, 7.5871, -21.8012), 7, { new AnimationAction(door2aanim), new AnimationAction(door2banim), new RoomEnableKeyPointAction(2, true), hall_action_1, hall_action_2, new FootColorAction(left, glm::vec3(1, 1, 0), 1.7), new FootColorAction(right, glm::vec3(1,1,0), 2.5) }));

	// hallway
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(2.61161, 7.5871, -21.6465), glm::vec3(50.1475, 7.5871, -21.8012), 7, { hall_action_1, hall_action_2, new LightDecreaseAction(living_room_light, 5, 0) }));

	auto treelight = static_cast<LightNode*>(root->find_by_name("Sun_"));
	treelight->set_color(glm::vec3(0), glm::vec3(0));
	auto treelightaction = new HallLightIncreaseAction(nullptr, treelight, 3.5, 3, 0.8);
	auto door3aanim = new DoorAnimation("Door3aAnim", door3a, d3angle, 1.2, false);
	auto door3banim = new DoorAnimation("Door3bAnim", door3b, d3angle, 1.2, false);
	root->add_node(door3aanim);
	root->add_node(door3banim);
	//glm::vec4 brightbackground = glm::vec4(9 / 255.0, 94 / 255.0, 232 / 255.0, 1);
	glm::vec4 brightbackground = glm::vec4(35 / 255.0, 110 / 255.0, 232 / 255.0, 1);
	//glm::vec3 planecolor = glm::vec3(0.9, 0.9, 0.4);
	glm::vec3 planecolor = glm::vec3(1, 1, 0.3);
	static_cast<GeometryNode*>(root->find_by_name("Cube_0"))->get_editable_mesh_resource()->get_editable_material().set_diffuse_color(planecolor);
	BloomAction* blomp = new BloomAction(cam, glm::vec2(1, 0), glm::vec2(0.8, 1), 8, 3);
	BloomAction* blex = new BloomAction(cam, glm::vec2(0.8, 1), glm::vec2(0, 2), 4, 1);
	EndCreditsAction *ecs = new EndCreditsAction(cam);
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(23.6967, 4.77344, -21.4799), glm::vec3(91.0045, 4.77344, -19.635), 7, { new BloomAction(cam, glm::vec2(1, 1), glm::vec2(1, 0), 4.5, 2), new CullOffAction(), treelightaction, new ClearColorAction(brightbackground), new AnimationAction(door3aanim), new AnimationAction(door3banim), new RoomEnableKeyPointAction(1, false), new RoomEnableKeyPointAction(3, true) }));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(47.7877, 4.77344, -22.2962), glm::vec3(91.0045, 12, -19.635), 5, {  new FootColorAction(left, glm::vec3(0, 1, 0)), new FootColorAction(right, glm::vec3(0, 1, 0)) }));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(55.1978, 4.77344, -22.1224), glm::vec3(91.0045, 12, -19.635), 5, { new ParticleEmitAction(particle_node, 2.0) }));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(62.5513, 4.77344, -21.8042), glm::vec3(91.0045, 12, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(70.5708, 4.77344, -21.4571), glm::vec3(91.0045, 12, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(77.0854, 4.77344, -21.1752), glm::vec3(91.0045, 12, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(84.3001, 4.77344, -11.5358), glm::vec3(91.0045, 12, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 16.3427, -16.337), glm::vec3(91.0045, 11, -19.635), 5, { blomp, new RoomEnableKeyPointAction(2, false) }));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 18, -16.337), glm::vec3(100, 50, -19.635), 5, { blomp }));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 30.3427, -16.337), glm::vec3(91.0045, 200, -19.635), 5, { blomp, blex, ecs }));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 35.3427, -16.337), glm::vec3(91.0045, 200, -19.635), 10, { ecs }));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 40.3427, -16.337), glm::vec3(91.0045, 200, -19.635), 5, {new StopAction(engine)}));    
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 50.3427, -16.337), glm::vec3(91.0045, 200, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 60.3427, -16.337), glm::vec3(91.0045, 200, -19.635), 5));
	cam_spline_controller->add_keypoint(new KeyPoint(glm::vec3(87.4164, 70.3427, -16.337), glm::vec3(91.0045, 200, -19.635), 5));


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

