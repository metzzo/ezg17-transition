#pragma once
class ShaderResource;
class IDrawable
{
public:
	virtual ~IDrawable() = default;
	virtual void draw(ShaderResource *shader) const = 0;
};

