#pragma once
#include "PostProcessingEffect.h"
#include "TextureFBO.h"
#include "DummyShader.h"

/*
This is just here for test purposes. Does not change the input image.
*/
class DummyEffect : public PostProcessingEffect {
private:
	MeshResource *screenMesh_;
	DummyShader* shader_;
	glm::ivec2 viewport_;

public:
	DummyEffect();

	virtual void init(RenderingEngine *engine, CameraNode *camera) override;

	virtual void perform_effect(const TextureFBO *from, GLuint fbo_to) override;

};