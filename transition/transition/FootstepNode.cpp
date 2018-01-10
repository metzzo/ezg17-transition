#include "FootstepNode.h"
#include "FootParticleShader.h"
#include <iostream>
#include "TextureResource.h"
#include "RenderingEngine.h"
#define _USE_MATH_DEFINES
#include <math.h>

FootstepNode::FootstepNode(const std::string& name, ShaderResource *main_shader, bool left) : ParticleEmitterNode(name)
{
	this->draw_shader_ = new FootParticleShader();
	this->compute_shader_ = new ComputeShader("assets/shaders/foot_part.comp");
	this->main_shader_ = main_shader;
	this->pingpongindex_ = 0;
	this->particle_count_ = 0;
	this->foot_node_ = nullptr;
	this->left_ = left;
}

FootstepNode::~FootstepNode()
{
	delete this->compute_shader_;
	glDeleteBuffers(2, ssbo_pos_id_);
	glDeleteBuffers(2, ssbo_vel_id_);
	glDeleteBuffers(2, ssbo_col_id_);
	glDeleteBuffers(1, &atomic_counter_id_);
	glDeleteBuffers(1, &temp_buffer_id_);
	glDeleteVertexArrays(2, vao_ssbo_pos_id_);
}

void FootstepNode::init(RenderingEngine * engine)
{
	compute_shader_->init();
	compute_shader_->use();
	//SBOs
	glGenBuffers(2, ssbo_pos_id_);
	glGenBuffers(2, ssbo_vel_id_);
	glGenBuffers(2, ssbo_col_id_);
	for (int i = 0; i <= 1; i++) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_pos_id_[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, DEMO_MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_vel_id_[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, DEMO_MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo_col_id_[i]);
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
	const GLuint indata_pos_layout = 0;
	const GLuint indata_col_layout = 1;
	glGenVertexArrays(2, vao_ssbo_pos_id_);
	for (int i = 0; i <= 1; i++) {
		glBindVertexArray(vao_ssbo_pos_id_[i]);
		glBindBuffer(GL_ARRAY_BUFFER, ssbo_pos_id_[i]);
		glEnableVertexAttribArray(indata_pos_layout);
		glVertexAttribPointer(indata_pos_layout, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, ssbo_col_id_[i]);
		glEnableVertexAttribArray(indata_col_layout);
		glVertexAttribPointer(indata_col_layout, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	}
	glBindVertexArray(0);

	/*TextureResource *texture = new TextureResource("assets/gfx/footstep2.png");
	texture->init();
	engine->register_resource(texture);*/
	TextureResource *alpha = new AlphaTextureResource("assets/gfx/footstep1alpha3.png");
	alpha->init();
	engine->register_resource(alpha);
	MeshResource *step_mesh = MeshResource::create_sprite(nullptr, alpha, left_);
	step_mesh->get_editable_material().set_opacity(0);
	step_mesh->get_editable_material().set_alpha_cutoff(0.0);
	step_mesh->get_editable_material().set_ambient_color(color_);
	step_mesh->init();
	engine->register_resource(step_mesh);
	this->foot_node_ = new GeometryNode(this->get_name().append("-foot1"), step_mesh);
	this->foot_node_->init(engine);
	this->foot_node_->apply_transformation(Transformation::rotate_around_world_origin(90, glm::vec3(-1, 0, 0)));
	this->foot_node_->apply_transformation(Transformation::translate(glm::vec3(0, 0.01, 0)));
	this->foot_node_->apply_transformation(Transformation::scale(glm::vec3(1.3)));
	this->foot_node_->apply_transformation(this->get_transformation(), this->get_inverse_transformation());
}

void FootstepNode::shine(float time)
{
	const int TTL = 10;
	std::vector<glm::vec4> data_pos;
	std::vector<glm::vec4> data_col;
	std::vector<glm::vec4> data_vel;

	for (int i = 0; i < 50 && (particle_count_ + i + 1 <= DEMO_MAX_PARTICLES); i++) {
		float angle = ((float)rand() / RAND_MAX) * 2 * M_PI;
		float distance = ((float)rand() / RAND_MAX);// rand();
		float speed = ((float)rand() / RAND_MAX)*0.4 + 0.5;
		float ttl = 4.5 + ((float)rand() / RAND_MAX - 0.5);
		float x = cos(angle)*distance;
		float z = sin(angle)*distance;
		data_pos.push_back(glm::vec4(x, 0, z, ttl));
		glm::vec4 vel = glm::normalize(glm::vec4(x, 0.3, z, 0));
		vel.w = speed;
		data_vel.push_back(vel);
		data_col.push_back(glm::vec4(1, color_distribution_[0](rand_engine_), color_distribution_[1](rand_engine_), 1));
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
	since_emitting = 0;
	shine_length_ = time;
}

void FootstepNode::start_emitting()
{
	shine(4);
}

void FootstepNode::stop_emitting()
{
	is_emitting_ = false;
}

void FootstepNode::update_particles(float deltaT)
{
	this->foot_node_->get_editable_mesh_resource()->get_editable_material().set_ambient_color(color_);

	float alpha = (shine_length_ - since_emitting)/ shine_length_;
	if (alpha > 0) {
		//alpha = alpha*alpha;
		//alpha = sqrt(alpha);
		//alpha = (pow((alpha - 0.5) / 4, 1.0 / 3.0) + 0.5)*(1-sqrt(1-alpha));
		//alpha = pow(1 - sqrt(1 - alpha), 2);
		//alpha = 1-sqrt(1-pow(alpha,2));
		alpha = 0.5*(pow((alpha - 0.5) / 4, 1.0 / 3.0) + 0.5)*(1 - sqrt(1 - alpha)) + 0.5*(1 - sqrt(1 - pow(alpha, 2)));
	}
	foot_node_->get_editable_mesh_resource()->get_editable_material().set_opacity(alpha);
	since_emitting += deltaT;
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
	glUniform1ui(8, DEMO_MAX_PARTICLES);
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

	if (particle_count_ == 0) {
		stop_emitting();
	}
}

void FootstepNode::draw_particles(const RenderingNode *cam) const
{
	main_shader_->use();
	main_shader_->set_camera_uniforms(cam);
	foot_node_->draw(main_shader_);

	draw_shader_->use();
	draw_shader_->set_camera_uniforms(cam);
	draw_shader_->set_modelmat_uniforms(this->get_transformation());
	glBindVertexArray(this->vao_ssbo_pos_id_[this->pingpongindex_]);
	glDrawArrays(GL_POINTS, 0, particle_count_);
	glBindVertexArray(0);
}

void FootstepNode::set_color(const glm::vec3& color)
{
	color_ = color;
}
