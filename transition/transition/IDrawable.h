#pragma once
#include "ITransformable.h"
class RenderingNode;
class IDrawable : public ITransformable
{
public:
	virtual ~IDrawable() = default;
	virtual void draw(RenderingNode *rendering_node) const = 0;
};

