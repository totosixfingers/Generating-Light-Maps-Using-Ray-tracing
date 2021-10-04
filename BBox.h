#pragma once
#include "GLEW\glew.h"
#include "glm\glm.hpp"
#include <vector>
class BBox
{
public: glm::vec3 pMin, pMax;
	  int Depth, Position;
	  BBox* child[8];
	  std::vector <glm::vec3> Triangles;
	  bool isLeaf;
	  bool HasTriang;
	  BBox(glm::vec3 pMin, glm::vec3 pMax, int Pos, int Dpth);
	  BBox(int Depth, glm::vec3 pMin, glm::vec3 pMax);
	  BBox( glm::vec3 pMin, glm::vec3 pMax,std::vector<glm::vec3> Triagles);
	 // ~BBox() {
		//  for (int i = 0; i < 8; ++i) if (child[i] != NULL) delete child[i];
	 // };
	  void CreateChildren(int Pos, int Dpth);
};