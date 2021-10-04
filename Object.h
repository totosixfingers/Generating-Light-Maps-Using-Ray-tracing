#include "ObjLoader.h"
#include "GeometricMesh.h"
#include "GLEW\glew.h"
#include "glm\glm.hpp"
#include <vector>
#include "ShaderProgram.h"
#include "GeometryNode.h"

class Object
{

public:
	class GeometryNode* m_geometric_object;
	std::vector<glm::vec3> Coords;
	int number;
	glm::mat4										transformation_matrix;
	glm::mat4										transformation_normal_matrix;
	int LightMapSize;
	unsigned int LightMap;
	unsigned int ObjectCoords;
	unsigned int ObjectNorms;

	//void initCoords(int LightMapSize, ShaderProgram shader);
	//float* Padding(float* array,int LightMapSize);

	Object();
	~Object();
	void SetObject(GeometryNode * object);
	Object::Object(GeometryNode *bject, glm::mat4 transformation,int size);
};
