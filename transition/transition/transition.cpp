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

class GittenbergerNode : public AnimatorNode
{
	Node *gitti_;
public:
	explicit GittenbergerNode(const std::string& name)
		: AnimatorNode(name)
	{
		this->gitti_ = nullptr;
	}

	void init(RenderingEngine* rendering_engine) override
	{
		AnimatorNode::init(rendering_engine);

		this->gitti_ = rendering_engine->get_root_node()->find_by_name("Sphere");
	}

	void update(double delta) override
	{
		this->gitti_->apply_transformation(Transformation::rotate_around_point(25.0 * delta, glm::vec3(0, 1, 0), glm::vec3(-4, 0, 0)));
	}
};

int main()
{
	int WINDOW_WIDTH = 800;
	int WINDOW_HEIGHT = 800;
	bool WINDOW_FULLSCREEN = false;
	int REFRESH_RATE = 60;

	auto engine = new RenderingEngine(glm::ivec2(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_FULLSCREEN, REFRESH_RATE);
	auto root = engine->get_root_node();

	const auto cam = new CameraNode("MainCamera",
		engine->get_viewport(),
		glm::perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f)		
	);
	cam->set_view_matrix(glm::lookAt(glm::vec3(5, 5, 5), glm::vec3(-4, 0, 0), glm::vec3(0, 1, 0)));
	root->add_node(cam);

	auto importer = new ColladaImporter(engine);
	const auto world = importer->load_node("assets/models/gitti_d.dae");
	root->add_node(world);

	const auto dir_light = new LightNode("dir_light1", DIRECTIONAL_LIGHT);
	dir_light->set_color(glm::vec3(1), glm::vec3(1));

	auto mat_dir = glm::lookAt(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	dir_light->set_transformation(mat_dir, inverse(mat_dir));
	//root->add_node(dir_light);


	const auto point_light = new LightNode("point_light1", POINT_LIGHT);
	point_light->set_params(0.07, 0.017);
	point_light->set_color(glm::vec3(1), glm::vec3(1));

	auto mat_point = glm::translate(glm::mat4(), glm::vec3(-5.0f, 5.0f, 5.0f));
	point_light->set_transformation(mat_point, inverse(mat_point));
	root->add_node(point_light);

	auto gitti = new GittenbergerNode("gitti_animator");
	root->add_node(gitti);

	engine->run();

	delete world;
	delete importer;
    return 0;
}

