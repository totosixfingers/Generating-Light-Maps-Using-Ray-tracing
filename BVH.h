
#include "GLEW\glew.h"
#include "glm\glm.hpp"
#include <vector>

class BVH
{
public:
	glm::vec3 pMin, pMax;

	std::vector <glm::vec3> Coords;
	BVH();
	void compute();
};