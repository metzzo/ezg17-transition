#pragma once
#include "ParticleEmitterNode.h"
#include "ComputeShader.h"
#include "ShaderResource.h"
#include "FootParticleShader.h"
#include "MeshResource.h"
#include "GeometryNode.h"
#include <random>

#define DEMO_MAX_PARTICLES 500
class FootstepNode : public ParticleEmitterNode {

private:
	ComputeShader *compute_shader_;
	FootParticleShader *draw_shader_;
	ShaderResource *main_shader_;
	GeometryNode *foot_node_;
	GLuint ssbo_pos_id_[2];//contains pos and ttl
	GLuint ssbo_col_id_[2];
	GLuint ssbo_vel_id_[2];
	GLuint vao_ssbo_pos_id_[2];
	GLuint atomic_counter_id_;
	GLuint temp_buffer_id_;
	int pingpongindex_;
	GLuint particle_count_;
	bool is_emitting_ = false;
	float since_emitting = 10;
	bool left_ = false;
	glm::vec3 color_ = glm::vec3(1, 0, 0);
	float shine_length_ = 4;

	std::default_random_engine rand_engine_ = std::default_random_engine(10);
	std::normal_distribution<float> color_distribution_[2] = { std::normal_distribution<float>(0.7, 0.3), std::normal_distribution<float>(0.3, 0.2) };
	//std::normal_distribution<glm::vec3>(glm::vec3(1, 0.7, 0.3), glm::vec3(0, 0.2, 0.3))
	

public:
	explicit FootstepNode(const std::string& name, ShaderResource *main_shader, bool left);
	virtual ~FootstepNode();

	void init(RenderingEngine *engine) override;
	void shine(float time);
	void start_emitting() override;
	void stop_emitting() override;
	void update_particles(float deltaT) override;
	void draw_particles(const RenderingNode *cam) const override;

	void apply_transformation(const glm::mat4& mat, const glm::mat4& imat) override {
		ParticleEmitterNode::apply_transformation(mat, imat);
		if (this->foot_node_ != nullptr) {
			this->foot_node_->apply_transformation(mat, imat);
		}
	}

	//This has to be redefined, or else the other method is hiding it...
	void apply_transformation(const Transformation& transformation) override {
		ParticleEmitterNode::apply_transformation(transformation);//indirectly calls the other one
	}

	void set_enabled(bool enabled) override {
		ParticleEmitterNode::set_enabled(enabled);
		foot_node_->set_enabled(enabled);
	}

	void set_color(const glm::vec3& color);
};