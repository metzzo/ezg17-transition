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
	int duration;
	float at_time;

	KeyPoint(glm::vec3 pos, glm::vec3 look_at_pos, int duration)
	{
		this->pos = pos;
		this->look_at_pos = look_at_pos;
		this->duration = duration;
		this->at_time = -1;
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
	glm::vec3 current_look_at_;

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

	TransformationNode* get_target() const;

	void update(double delta) override;
	void init(RenderingEngine* rendering_engine) override;
};

