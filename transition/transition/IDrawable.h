#pragma once
class RenderingNode;
class IDrawable
{
public:
	virtual ~IDrawable() = default;
	virtual void draw(RenderingNode *rendering_node) const = 0;
};

