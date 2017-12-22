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
	GLuint ssbo_pos_id_[2];
	GLuint vao_ssbo_id_[2];
	GLuint atomic_counter_id_;
	GLuint temp_buffer_id_;
	int pingpongindex_;
	GLuint particle_count_;

	struct DemoParticle {
		glm::vec3 position;
		float timetolive;
		glm::vec3 velocity;
		float padding;

		DemoParticle(glm::vec3 position, float ttl, glm::vec3 velocity) {
			this->position = position;
			this->timetolive = ttl;
			this->velocity = velocity;
		}
	};


public:
	explicit DemoParticleEmitter(const std::string& name);
	virtual ~DemoParticleEmitter();

	void init(RenderingEngine *engine) override;
	void start_emitting() override;
	void stop_emitting() override;
	void update_particles(float deltaT) override;
	void draw_particles(const RenderingNode *cam) const override;

};