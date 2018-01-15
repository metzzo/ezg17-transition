#pragma once
#include "ParticleEmitterNode.h"
#include "ComputeShader.h"
#include "ShaderResource.h"
#include "FootParticleShader.h"
#include "MeshResource.h"

#define FINAL_MAX_PARTICLES 1000
class FinalParticlesNode : public ParticleEmitterNode {

private:
	ComputeShader *compute_shader_;
	FootParticleShader *draw_shader_;
	GLuint ssbo_pos_id_[2];//contains pos and ttl
	GLuint ssbo_col_id_[2];
	GLuint ssbo_vel_id_[2];
	GLuint vao_ssbo_pos_id_[2];
	GLuint atomic_counter_id_;
	GLuint temp_buffer_id_;
	int pingpongindex_;
	GLuint particle_count_;
	bool is_emitting_ = false;

	void create_particles(int count);


public:
	explicit FinalParticlesNode(const std::string& name);
	virtual ~FinalParticlesNode();

	void init(RenderingEngine *engine) override;
	void start_emitting() override;
	void stop_emitting() override;
	void update_particles(float deltaT) override;
	void draw_particles(const RenderingNode *cam) const override;
};