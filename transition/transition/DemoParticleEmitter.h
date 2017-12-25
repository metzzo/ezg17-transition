#pragma once
#include "ParticleEmitterNode.h"
#include "ComputeShader.h"
#include "ShaderResource.h"
#include "DemoParticleShader.h"

#define DEMO_MAX_PARTICLES 100
class DemoParticleEmitter : public ParticleEmitterNode {

private:
	ComputeShader *compute_shader_;
	DemoParticleShader *draw_shader_;
	GLuint ssbo_pos_id_[2];//contains pos and ttl
	GLuint ssbo_vel_id_[2];
	GLuint vao_ssbo_pos_id_[2];
	GLuint atomic_counter_id_;
	GLuint temp_buffer_id_;
	int pingpongindex_;
	GLuint particle_count_;
	

public:
	explicit DemoParticleEmitter(const std::string& name);
	virtual ~DemoParticleEmitter();

	void init(RenderingEngine *engine) override;
	void start_emitting() override;
	void stop_emitting() override;
	void update_particles(float deltaT) override;
	void draw_particles(const RenderingNode *cam) const override;

};