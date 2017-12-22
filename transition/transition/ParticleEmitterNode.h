#pragma once
#include "TransformationNode.h"
#include "IDrawable.h"
#include "RenderingNode.h"

class ParticleEmitterNode : public TransformationNode {
	
public:
	ParticleEmitterNode(const std::string& name) : TransformationNode(name) {}

	virtual void start_emitting() = 0;
	virtual void stop_emitting() = 0;
	virtual void update_particles(float deltaT) = 0;
	virtual void draw_particles(const RenderingNode *cam) const = 0;

	std::vector<ParticleEmitterNode*> get_particle_emitter_nodes() override;
	std::vector<IDrawable*> get_drawables() override;
};