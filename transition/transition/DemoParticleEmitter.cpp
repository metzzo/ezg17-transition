#include "DemoParticleEmitter.h"
#include "DemoParticleShader.h"
#include <iostream>

DemoParticleEmitter::DemoParticleEmitter(const std::string& name) : ParticleEmitterNode(name)
{
	this->draw_shader_ = new DemoParticleShader();
	this->compute_shader_ = new ComputeShader("assets/shaders/demo_part.comp");
	this->pingpongindex_ = 0;
}

DemoParticleEmitter::~DemoParticleEmitter()
{
	delete this->compute_shader_;
}

void DemoParticleEmitter::init(RenderingEngine * engine)
{
	compute_shader_->init();
	compute_shader_->use();
	//SBOs
	glGenBuffers(2, ssbo_pos_id_);
	glGenBuffers(2, ssbo_vel_id_);
	for (int i = 0; i <= 1; i++) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pos_id_[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, DEMO_MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vel_id_[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, DEMO_MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	}

	//Atomic Counter
	glGenBuffers(1, &atomic_counter_id_);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter_id_);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);

	GLuint value = 0;
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &value);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

	glGenBuffers(1, &temp_buffer_id_);
	glBindBuffer(GL_COPY_WRITE_BUFFER, temp_buffer_id_);
	glBufferData(GL_COPY_WRITE_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_READ);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

	draw_shader_->init();
	draw_shader_->use();
	//VAOs -> FOR RENDERING
	const GLuint indata_layout = 0;
	glGenVertexArrays(2, vao_ssbo_pos_id_);
	for (int i = 0; i <= 1; i++) {
		glBindVertexArray(vao_ssbo_pos_id_[i]);
		glBindBuffer(GL_ARRAY_BUFFER, ssbo_pos_id_[i]);
		glEnableVertexAttribArray(indata_layout);
		glVertexAttribPointer(indata_layout, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
	}
	glBindVertexArray(0);

	start_emitting(); //TODO-remove this
}

void DemoParticleEmitter::start_emitting()
{
	const int TTL = 10;
	std::vector<glm::vec4> data_pos;
	std::vector<glm::vec4> data_vel;

	data_pos.push_back(glm::vec4(0, 0, -6, 10));
	data_pos.push_back(glm::vec4(1, 0, -6, 10));
	data_vel.push_back(glm::vec4(0, 0.5, 0, 0));
	data_vel.push_back(glm::vec4(0, 0.5, 0, 0));
	particle_count_ = 2;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pos_id_[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particle_count_ * sizeof(glm::vec4), &data_pos[0]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vel_id_[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, particle_count_ * sizeof(glm::vec4), &data_vel[0]);
}

void DemoParticleEmitter::stop_emitting()
{
	//TODO
}

void DemoParticleEmitter::update_particles(float deltaT)
{
	compute_shader_->use();
	//set uniforms
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_pos_id_[pingpongindex_]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo_vel_id_[pingpongindex_]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo_pos_id_[!pingpongindex_]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo_vel_id_[!pingpongindex_]);
	pingpongindex_ = !pingpongindex_;
	//ATOMIC BUFFER?
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, atomic_counter_id_);
	glUniform1ui(5, particle_count_);
	glUniform1ui(6, DEMO_MAX_PARTICLES);
	glUniform1f(7, deltaT);	//deltaT
	
	GLuint groups = particle_count_ / (16 * 16) + 1;
	glDispatchCompute(groups, 1, 1);

	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter_id_);
	glBindBuffer(GL_COPY_WRITE_BUFFER, temp_buffer_id_);
	glCopyBufferSubData(GL_ATOMIC_COUNTER_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(GLuint));

	GLuint *counterValue = (GLuint*)glMapBufferRange(GL_COPY_WRITE_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
	particle_count_ = counterValue[0];
	counterValue[0] = 0;
	glUnmapBuffer(GL_COPY_WRITE_BUFFER);
	glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_ATOMIC_COUNTER_BUFFER, 0, 0, sizeof(GLuint));

	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

void DemoParticleEmitter::draw_particles(const RenderingNode *cam) const
{
	draw_shader_->use();
	draw_shader_->set_camera_uniforms(cam);
	draw_shader_->set_modelmat_uniforms(this->get_transformation());
	glBindVertexArray(this->vao_ssbo_pos_id_[this->pingpongindex_]);
	glDrawArrays(GL_POINTS, 0, particle_count_);
	glBindVertexArray(0);
}
