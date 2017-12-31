#pragma once
#include "glheaders.h"
#include "TextureResource.h"
#include "RenderingEngine.h"
#include "MeshResource.h"
#include "TextureFBO.h"

/*
Interface for PostProcessingEffects
*/
class PostProcessingEffect {

public:
	virtual ~PostProcessingEffect() = default;
	/*
	Initializes the effect.
	If you create shaders in this function, make sure to initialize them here right away, because the rendering engine will not do that anymore
	*/
	virtual void init(RenderingEngine *engine, CameraNode *camera) = 0;

	/*
	Performs the effect.
	from is the TextureFBO which contains the original image. fbo_to is the id of the framebuffer to render the result into
	*/
	virtual void perform_effect(const TextureFBO* from, GLuint fbo_to) = 0;

};