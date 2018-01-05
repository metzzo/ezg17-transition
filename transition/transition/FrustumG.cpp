/* ------------------------------------------------------

View Frustum Culling - Lighthouse3D
GEOMETRIC METHOD by Lighthouse3d.com
Source: http://www.lighthouse3d.com/tutorials/view-frustum-culling/
Requested on 12th June 2016

-----------------------------------------------------*/

#include "FrustumG.h"
#include <math.h>

Plane::Plane(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3) {

	set3Points(v1, v2, v3);
}


Plane::Plane() {}

Plane::~Plane() {}


void Plane::set3Points(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3) {


	glm::vec3 aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = glm::cross(aux2, aux1);
	auto normal2 = glm::cross(aux1, aux2);

	normal = glm::normalize(normal);
	point.x = v2.x; point.y = v2.y; point.z = v2.z;
	d = -(glm::dot(normal, point));
}


float Plane::distance(glm::vec3 &p) {
	auto asdf = (d + glm::dot(normal, p));
	return (d + glm::dot(normal, p));
}

FrustumG::FrustumG() {}

FrustumG::~FrustumG() {}

void FrustumG::setCamInternals(float angle, float ratio, float nearD, float farD) {

	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	tang = (float)tan(angle * 0.5);
	nh = nearD * tang;
	nw = nh * ratio;
	fh = farD  * tang;
	fw = fh * ratio;


}


void FrustumG::setCamDef(glm::vec3 &p, glm::vec3 &l, glm::vec3 &u) {

	glm::vec3 dir, nc, fc, X, Y, Z;

	Z = p - l;
	Z = glm::normalize(Z);

	X = glm::cross(u, Z);
	X = glm::normalize(X);

	Y = glm::cross(Z, X);

	nc = p - (Z * nearD);
	fc = p - (Z * farD);

	ntl = nc + (Y * nh) - (X * nw);
	ntr = nc + (Y * nh) + (X * nw);
	nbl = nc - (Y * nh) - (X * nw);
	nbr = nc - (Y * nh) + (X * nw);

	ftl = fc + (Y * fh) - (X * fw);
	ftr = fc + (Y * fh) + (X * fw);
	fbl = fc - (Y * fh) - (X * fw);
	fbr = fc - (Y * fh) + (X * fw);

	pl[TOP].set3Points(ntr, ntl, ftl);
	pl[BOTTOM].set3Points(nbl, nbr, fbr);
	pl[LEFT].set3Points(ntl, nbl, fbl);
	pl[RIGHT].set3Points(nbr, ntr, fbr);
	pl[NEARP].set3Points(ntl, ntr, nbr);
	pl[FARP].set3Points(ftr, ftl, fbl);
}


int FrustumG::pointInFrustum(glm::vec3 &p) {

	int result = INSIDE;
	for (int i = 0; i < 6; i++) {

		if (pl[i].distance(p) < 0)
			return OUTSIDE;
	}
	return(result);

}


int FrustumG::sphereInFrustum(glm::vec3 &p, float raio) {

	int result = INSIDE;
	float distance;

	for (int i = 0; i < 6; i++) {
		distance = pl[i].distance(p);
		if (distance < -raio)
			return OUTSIDE;
		else if (distance < raio)
			result = INTERSECT;
	}
	return(result);

}
