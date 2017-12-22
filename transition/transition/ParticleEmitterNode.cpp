#include "ParticleEmitterNode.h"

std::vector<ParticleEmitterNode*> ParticleEmitterNode::get_particle_emitter_nodes()
{
	return{ this };
}

std::vector<IDrawable*> ParticleEmitterNode::get_drawables()
{
	return{ };
}
