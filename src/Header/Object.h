#include "GeometricMesh.h"
#include "GLEW\glew.h"
#include "glm\glm.hpp"
#include <vector>
#include "GeometryNode.h"

#ifndef OBJECT_H
#define OBJECT_H


class Object
{
	
public:
	class GeometryNode* m_geometric_object;
	std::vector<glm::vec3> Coords;
	glm::mat4										transformation_matrix;
	glm::mat4										transformation_normal_matrix;
	int LightMapSize;
	GLuint LightMap;
	GLuint ObjectCoords;
	GLuint ObjectNorms;

	//void initCoords(int LightMapSize, ShaderProgram shader);
	//float* Padding(float* array,int LightMapSize);

	Object();
	~Object();
	void SetObject(GeometryNode * object);
	Object(GeometryNode *object, glm::mat4 transformation,int size);
};
#endif 