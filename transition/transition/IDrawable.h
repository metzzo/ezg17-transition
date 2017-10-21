#pragma once
#include "ITransformable.h"

class IDrawable : ITransformable
{
public:
	virtual ~IDrawable() = default;
	virtual void draw() const = 0;
};

