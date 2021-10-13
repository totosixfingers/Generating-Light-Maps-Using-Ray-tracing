
#include "GLEW\glew.h"
#include "glm\glm.hpp"
#include <vector>

#ifndef BVH_H
#define BVH_H


class BVH
{
public:
	glm::vec3 pMin, pMax;
	std::vector <glm::vec3> AllBoxVertices;
	std::vector <glm::vec3> Coords;
	BVH();
	~BVH();
	void compute();
	void computeCube();
};
#endif // !BVH_H