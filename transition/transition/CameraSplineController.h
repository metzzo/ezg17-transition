#pragma once
#include "AnimatorNode.h"
#include "BloomAddShader.h"
#include <spline_library/splines/uniform_cr_spline.h>
#include <spline_library/vector.h>

#define VISUALIZE_KEYPOINTS

class MeshResource;
class TransformationNode;
class GroupNode;

struct KeyPoint
{
	glm::vec3 pos;
	glm::vec3 look_at_pos;
	float duration;
	float at_time;
	float end_time;

	KeyPoint(glm::vec3 pos, glm::vec3 look_at_pos, float duration_)
	{
		this->pos = pos;
		this->look_at_pos = look_at_pos;
		this->duration = duration_;
	}
};

class CameraSplineController :
	public AnimatorNode
{
	TransformationNode* target_;
	std::vector<KeyPoint> keypoints_;
	double progress_;
	double duration_;
	int current_segment_;
	float total_arc_length_;

	UniformCRSpline<Vector3> *position_spline_;

#ifdef VISUALIZE_KEYPOINTS
	MeshResource* keypoint_visualizer_;
	MeshResource* tween_visualizer_;
	MeshResource* cam_visualizer_;
	GeometryNode* test_cam_;
	GroupNode *visualizer_container_;
#endif
public:
	CameraSplineController(std::string name, TransformationNode *target, GroupNode *visualizer_container);
	~CameraSplineController();

	void add_keypoint(const KeyPoint &key_point);
	void build_spline();

	void update(double delta) override;
	void init(RenderingEngine* rendering_engine) override;
};

