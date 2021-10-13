#include "Tools.h"
#include "BVH.h"
#include <iostream>

BVH::BVH() {

}
BVH::~BVH() {}
void BVH::compute() {
	float min_x = 100000;
	float min_y = 100000;
	float min_z = 100000;
	float max_x = -100000;
	float max_y = -100000;
	float max_z = -100000;

	for (int i = 0; i < Coords.size(); i++) {

		if (Coords[i].x < min_x) min_x = Coords[i].x;
		if (Coords[i].y < min_y) min_y = Coords[i].y;
		if (Coords[i].z < min_z) min_z = Coords[i].z;
		if (Coords[i].x > max_x) max_x = Coords[i].x;
		if (Coords[i].y > max_y) max_y = Coords[i].y;
		if (Coords[i].z > max_z) max_z = Coords[i].z;
	}

	this->pMax = glm::vec3(max_x, max_y, max_z);
	this->pMin = glm::vec3(min_x, min_y, min_z);

}
void BVH::computeCube() {
		// bottom square lines
	AllBoxVertices.push_back(glm::vec3(pMin.x, pMin.y, pMin.z));
	AllBoxVertices.push_back(glm::vec3(pMin.x, pMin.y, pMax.z));

	AllBoxVertices.push_back(glm::vec3(pMin.x, pMin.y, pMax.z));
	AllBoxVertices.push_back(glm::vec3(pMax.x, pMin.y, pMax.z));

	AllBoxVertices.push_back(glm::vec3(pMax.x, pMin.y, pMax.z));
	AllBoxVertices.push_back(glm::vec3(pMax.x, pMin.y, pMin.z));

	AllBoxVertices.push_back(glm::vec3(pMax.x, pMin.y, pMin.z));
	AllBoxVertices.push_back(glm::vec3(pMin.x, pMin.y, pMin.z));

	//perpandicular lines
	AllBoxVertices.push_back(glm::vec3(pMin.x, pMin.y, pMin.z));
	AllBoxVertices.push_back(glm::vec3(pMin.x, pMax.y, pMin.z));

	AllBoxVertices.push_back(glm::vec3(pMin.x, pMin.y, pMax.z));
	AllBoxVertices.push_back(glm::vec3(pMin.x, pMax.y, pMax.z));

	AllBoxVertices.push_back(glm::vec3(pMax.x, pMin.y, pMax.z));
	AllBoxVertices.push_back(glm::vec3(pMax.x, pMax.y, pMax.z));

	AllBoxVertices.push_back(glm::vec3(pMax.x, pMin.y, pMin.z));
	AllBoxVertices.push_back(glm::vec3(pMax.x, pMax.y, pMin.z));

	//upper square lines

	AllBoxVertices.push_back(glm::vec3(pMin.x, pMax.y, pMin.z));
	AllBoxVertices.push_back(glm::vec3(pMin.x, pMax.y, pMax.z));

	AllBoxVertices.push_back(glm::vec3(pMin.x, pMax.y, pMax.z));
	AllBoxVertices.push_back(glm::vec3(pMax.x, pMax.y, pMax.z));

	AllBoxVertices.push_back(glm::vec3(pMax.x, pMax.y, pMax.z));
	AllBoxVertices.push_back(glm::vec3(pMax.x, pMax.y, pMin.z));

	AllBoxVertices.push_back(glm::vec3(pMax.x, pMax.y, pMin.z));
	AllBoxVertices.push_back(glm::vec3(pMin.x, pMax.y, pMin.z));
}