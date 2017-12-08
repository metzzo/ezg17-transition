#pragma once
#include "glheaders.h"
#include "TextureResource.h"
#include "RenderingEngine.h"
#include "MeshResource.h"
#include "TextureFBO.h"

class PostProcessingEffect {

public:
	virtual void init(RenderingEngine *engine) = 0;

	virtual void perform_effect(const TextureFBO* from, GLuint fbo_to) = 0;

};