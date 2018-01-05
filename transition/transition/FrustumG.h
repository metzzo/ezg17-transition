/* ------------------------------------------------------

View Frustum Culling - Lighthouse3D
GEOMETRIC METHOD by Lighthouse3d.com
Source: http://www.lighthouse3d.com/tutorials/view-frustum-culling/
Requested on 12th June 2016

-----------------------------------------------------*/

#pragma once
#include "glm/glm.hpp"

class Plane
{

public:

	glm::vec3 normal, point;
	float d;


	Plane::Plane(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3);
	Plane::Plane(void);
	Plane::~Plane();

	void set3Points(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3);
	float distance(glm::vec3 &p);
};


class FrustumG
{
private:

	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};


public:

	static enum { OUTSIDE, INTERSECT, INSIDE };

	Plane pl[6];


	glm::vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	float nearD, farD, ratio, angle, tang;
	float nw, nh, fw, fh;

	FrustumG::FrustumG();
	FrustumG::~FrustumG();

	void FrustumG::setCamInternals(float angle, float ratio, float nearD, float farD);
	void FrustumG::setCamDef(glm::vec3 &p, glm::vec3 &l, glm::vec3 &u);
	int FrustumG::pointInFrustum(glm::vec3 &p);
	int FrustumG::sphereInFrustum(glm::vec3 &p, float raio);
};