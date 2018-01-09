#pragma once
#include "AnimatorNode.h"
#include "BloomAddShader.h"
#include <spline_library/splines/uniform_cr_spline.h>
#include <spline_library/vector.h>

#define VISUALIZE_KEYPOINTS

class MeshResource;
class TransformationNode;
class GroupNode;
class CameraSplineController;

class IKeyPointAction
{
public:
	virtual ~IKeyPointAction() = default;
	virtual void update(CameraSplineController *controller, float delta, float tween) = 0;
};

struct KeyPoint
{
	glm::vec3 pos;
	glm::vec3 look_at_pos;
	int duration;
	float at_time;
	std::vector<IKeyPointAction*> actions;

	KeyPoint(glm::vec3 pos, glm::vec3 look_at_pos, int duration) : KeyPoint(pos, look_at_pos, duration, {})
	{
	}

	KeyPoint(glm::vec3 pos, glm::vec3 look_at_pos, int duration, std::vector<IKeyPointAction*> actions)
	{
		this->pos = pos;
		this->look_at_pos = look_at_pos;
		this->duration = duration;
		this->at_time = -1;
		this->actions = actions;
	}

	explicit KeyPoint(const KeyPoint *keypoint)
	{
		this->pos = keypoint->pos;
		this->look_at_pos = keypoint->look_at_pos;
		this->duration = keypoint->duration;
		this->at_time = keypoint->at_time;
		this->actions = keypoint->actions;
	}
};

class CameraSplineController :
	public AnimatorNode
{
	TransformationNode* target_;
	std::vector<KeyPoint*> keypoints_;
	double progress_;
	double duration_;

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

	void add_keypoint(KeyPoint *key_point);
	void build_spline();

	TransformationNode* get_target() const;

	void update(double delta) override;
	void init(RenderingEngine* rendering_engine) override;

	void set_progress(double progress)
	{
		this->progress_ = progress;
	}
	double get_progress() const
	{
		return this->progress_;
	}
};

