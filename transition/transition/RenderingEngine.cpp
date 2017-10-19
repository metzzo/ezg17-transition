#include "stdafx.h"
#include "RenderingEngine.h"


RenderingEngine::RenderingEngine()
{
	this->root_node_ = new GroupNode(this);
}


RenderingEngine::~RenderingEngine()
{
}
