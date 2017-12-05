#include "DepthOnlyShader.h"


DepthOnlyShader::DepthOnlyShader() : ShaderResource("assets/shaders/depth_shader.vs", "assets/shaders/depth_shader.fs")
{

}

DepthOnlyShader::~DepthOnlyShader()
{
}

void DepthOnlyShader::init()
{
	ShaderResource::init();

}

void DepthOnlyShader::set_camera_uniforms(const RenderingNode* node)
{
}

void DepthOnlyShader::set_model_uniforms(const GeometryNode* node)
{
}
