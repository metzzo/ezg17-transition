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
#include "AnimatorNode.h"


class CameraAnimationNode : public AnimatorNode {
	CameraNode *camera_;
	int dir = 2;
public:
	explicit CameraAnimationNode(const std::string& name, CameraNode* camera) :AnimatorNode(name) {
		this->camera_ = camera;
	}

	void update(double delta) override {
		//VERY WEIRD: If I do not store the position in the variable pos first, but give it to the function right away, everything gets crazy
		glm::vec3 pos = this->camera_->get_position();
		this->camera_->apply_transformation(Transformation::rotate_around_point(-25 * delta, glm::vec3(0, 1, 0), pos));
		this->camera_->apply_transformation(Transformation::translate(glm::vec3(0, 0, delta*dir)));
		if (abs(this->camera_->get_position().z) >= 10) {
			dir = -dir;
		}
	}
};

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
	
	auto anim = new CameraAnimationNode("cam_anim", cam);
	root->add_node(anim);

	engine->run();

	delete world;
	delete importer;
    return 0;
}

