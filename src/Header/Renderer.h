#ifndef BIM_ENGINE_RENDERER_H
#define BIM_ENGINE_RENDERER_H

#include "GLEW\glew.h"
#include "glm\glm.hpp"
#include <vector>
#include "ShaderProgram.h"
#include "SpotlightNode.h"
#include <tuple>
#include "BVH.h"
#include "Object.h"

class Renderer
{

protected:
	int												m_screen_width, m_screen_height;
	glm::mat4										m_view_matrix;
	glm::mat4										m_projection_matrix;
	glm::vec3										m_camera_position;
	glm::vec3										m_camera_target_position;
	glm::vec3										m_camera_up_vector;
	glm::vec2										m_camera_movement;
	glm::vec2										m_camera_look_angle_destination;

	std::vector<glm::vec3> boxesVertices;
	std::vector<Object*> objects;
	std::vector<BVH> pMinMax;

	GLuint boxlinesVAO, boxlinesVBO;
	GLuint skyboxVAO, skyboxVBO;
	GLuint cubeVAO, cubeVBO;
	GLuint CubemapWidth, CubemapHeight;
	GLuint object1Coords, object1Norms, object1Texcoords;
	GLuint captured;

	GLuint Skybox;
	GLuint LightMap;
	GLuint SafeLightMap;

	float* posx;
	float* negx;
	float* posy;
	float* negy;
	float* posz;
	float* negz;
	// Geometry Rendering Intermediate Buffer

	float m_continous_time;

	// Rendering Mode
	//BBox* Root;

	// Lights
	

	// Meshes
	class GeometryNode* m_geometric_object1;
	glm::mat4										m_geometric_object1_transformation_matrix;
	glm::mat4										m_geometric_object1_transformation_normal_matrix;
	class GeometryNode* m_geometric_object2;
	glm::mat4										m_geometric_object2_transformation_matrix;
	glm::mat4										m_geometric_object2_transformation_normal_matrix;
	class GeometryNode* m_geometric_object3;
	glm::mat4										m_geometric_object3_transformation_matrix;
	glm::mat4										m_geometric_object3_transformation_normal_matrix;
	class GeometryNode* m_geometric_object4;
	
	// Protected Functions
	bool InitRenderingTechniques();
	bool InitGeometricMeshes();
	void DrawGeometryNode(class GeometryNode* node, glm::mat4 model_matrix);

	ShaderProgram								SkyboxShader;
	ShaderProgram								BasicRender;
	ShaderProgram                               Cpuraster;
	ShaderProgram                               safeguarding;
	ShaderProgram								BoxShader;

public:
	Renderer();
	~Renderer();
	bool										Init(int SCREEN_WIDTH, int SCREEN_HEIGHT);
	void CreateObjects();
	void										Update(float dt);
	bool										ResizeBuffers(int SCREEN_WIDTH, int SCREEN_HEIGHT);
	bool										ReloadShaders();
	void										Render();

	void										RenderObjects();
	void										RenderBoxes();
	void										RenderOnscreen();

	// Passes
	void										RenderGeometry();
	void                                        RenderSkyBox();
	void                                        initSkyBox();
	void										initCoords(class Object* Obv);
	void										CpuRaster(class Object* obv);
	void										CubeMapSampling(float x, float y, float z, int* index, float* u, float* v);
	void                                        initLightMap();
	void										Padding(float* array, float* temp, int LightMapSize);

	bool										RayTriangleIntersection(const glm::vec3 coords, const glm::vec3 normal, std::vector<glm::vec3> NewPos);

	bool										Boxintersect(glm::vec3 orig, glm::vec3 dir, glm::vec3 min, glm::vec3 max);
	void										Clusters(glm::vec3 pmin, glm::vec3 pmax, std::vector<glm::vec3> Coords);
	void										BVh(std::vector<glm::vec3> Coords);
	void										initBoxLines();
	// Camera Function
	void										CameraMoveForward(bool enable);
	void										CameraMoveBackWard(bool enable);
	void										CameraMoveLeft(bool enable);
	void										CameraMoveRight(bool enable);
	void										CameraLook(glm::vec2 lookDir);
};

#endif
