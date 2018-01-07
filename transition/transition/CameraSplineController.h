#pragma once
#include "AnimatorNode.h"
#include "BloomAddShader.h"
#include <spline_library/splines/uniform_cr_spline.h>
#include <spline_library/vector.h>

class MeshResource;
class TransformationNode;
class GroupNode;

struct KeyPoint
{
	glm::vec3 pos;
	glm::vec3 look_at_pos;

	KeyPoint(glm::vec3 pos, glm::vec3 look_at_pos)
	{
		this->pos = pos;
		this->look_at_pos = look_at_pos;
	}
};

class CameraSplineController :
	public AnimatorNode
{
	TransformationNode* target_;
	std::vector<KeyPoint> keypoints_;
	MeshResource* visualizer_;
	GroupNode *visualizer_container_;
	double progress_;
	GeometryNode* test_cam_;

	UniformCRSpline<Vector3> *position_spline_;
public:
	CameraSplineController(std::string name, TransformationNode *target, GroupNode *visualizer_container);
	~CameraSplineController();

	void add_keypoint(const KeyPoint &key_point);

	void update(double delta) override;
	void init(RenderingEngine* rendering_engine) override;
};

