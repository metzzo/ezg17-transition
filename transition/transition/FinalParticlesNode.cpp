#include "FinalParticlesNode.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

FinalParticlesNode::FinalParticlesNode(const std::string & name) : ParticleEmitterNode(name)
{
	this->draw_shader_ = new FootParticleShader();
	this->compute_shader_ = new ComputeShader("assets/shaders/foot_part.comp");
	this->pingpongindex_ = 0;
	this->particle_count_ = 0;
}

FinalParticlesNode::~FinalParticlesNode()
{
	delete this->compute_shader_;
	glDeleteBuffers(2, ssbo_pos_id_);
	glDeleteBuffers(2, ssbo_vel_id_);
	glDeleteBuffers(2, ssbo_col_id_);
	glDeleteBuffers(1, &atomic_counter_id_);
	glDeleteBuffers(1, &temp_buffer_id_);
	glDeleteVertexArrays(2, vao_ssbo_pos_id_);
}

void FinalParticlesNode::init(RenderingEngine * engine)
{
	compute_shader_->init();
	compute_shader_->use();
	//SBOs
	glGenBuffers(2, ssbo_pos_id_);
	glGenBuffers(2, ssbo_vel_id_);
	glGenBuffers(2, ssbo_col_id_);
	for (int i = 0; i <= 1; i++) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pos_id_[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, FINAL_MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vel_id_[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, FINAL_MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_col_id_[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, FINAL_MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
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
	const GLuint indata_pos_layout = 0;
	const GLuint indata_col_layout = 1;
	glGenVertexArrays(2, vao_ssbo_pos_id_);
	for (int i = 0; i <= 1; i++) {
		glBindVertexArray(vao_ssbo_pos_id_[i]);
		glBindBuffer(GL_ARRAY_BUFFER, ssbo_pos_id_[i]);
		glEnableVertexAttribArray(indata_pos_layout);
		glVertexAttribPointer(indata_pos_layout, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, ssbo_col_id_[i]);
		glEnableVertexAttribArray(indata_col_layout);
		glVertexAttribPointer(indata_col_layout, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	}
	glBindVertexArray(0);

}



void FinalParticlesNode::create_particles(int count)
{
	std::vector<glm::vec4> data_pos;
	std::vector<glm::vec4> data_col;
	std::vector<glm::vec4> data_vel;

	for (int i = 0; i < count && (particle_count_ + i + 1 <= FINAL_MAX_PARTICLES); i++) {
		float angle = ((float)rand() / RAND_MAX) * 2 * M_PI;
		float distance = ((float)rand() / RAND_MAX) * 10;// rand();
		float speed = ((float)rand() / RAND_MAX)*0.5 + 0.8;
		float ttl = 3 + ((float)rand() / RAND_MAX - 0.5) * 2;
		float x = cos(angle)*distance;
		float z = sin(angle)*distance;
		data_pos.push_back(glm::vec4(x, 0, z, ttl));
		glm::vec4 vel = glm::normalize(glm::vec4(0, 1, 0, 0));
		vel.w = speed;
		data_vel.push_back(vel);
		data_col.push_back(glm::vec4(0, 0.5, 0, 1));
	}

	if (data_pos.size() > 0) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pos_id_[pingpongindex_]);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, particle_count_ * sizeof(glm::vec4), data_pos.size() * sizeof(glm::vec4), &data_pos[0]);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_col_id_[pingpongindex_]);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, particle_count_ * sizeof(glm::vec4), data_col.size() * sizeof(glm::vec4), &data_col[0]);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vel_id_[pingpongindex_]);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, particle_count_ * sizeof(glm::vec4), data_vel.size() * sizeof(glm::vec4), &data_vel[0]);
		particle_count_ += data_pos.size();
		is_emitting_ = true;
	}
}

void FinalParticlesNode::start_emitting()
{
	is_emitting_ = true;
}

void FinalParticlesNode::stop_emitting()
{
	is_emitting_ = false;
}

void FinalParticlesNode::update_particles(float deltaT)
{
	if (!is_emitting_) return;
	compute_shader_->use();
	//set uniforms
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo_pos_id_[pingpongindex_]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo_vel_id_[pingpongindex_]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo_col_id_[pingpongindex_]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo_pos_id_[!pingpongindex_]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo_vel_id_[!pingpongindex_]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssbo_col_id_[!pingpongindex_]);
	pingpongindex_ = !pingpongindex_;
	//ATOMIC BUFFER?
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 6, atomic_counter_id_);
	glUniform1ui(7, particle_count_);
	glUniform1ui(8, FINAL_MAX_PARTICLES);
	glUniform1f(9, deltaT);	//deltaT

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

	create_particles(deltaT * 120);
}

void FinalParticlesNode::draw_particles(const RenderingNode * cam) const
{
	draw_shader_->use();
	draw_shader_->set_camera_uniforms(cam);
	draw_shader_->set_modelmat_uniforms(this->get_transformation(), glm::vec2(0.1, 0.1), 2);
	glBindVertexArray(this->vao_ssbo_pos_id_[this->pingpongindex_]);
	glDrawArrays(GL_POINTS, 0, particle_count_);
	glBindVertexArray(0);
}
