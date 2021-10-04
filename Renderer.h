#ifndef BIM_ENGINE_RENDERER_H
#define BIM_ENGINE_RENDERER_H

#include "GLEW\glew.h"
#include "glm\glm.hpp"
#include <vector>
#include "ShaderProgram.h"
#include "SpotlightNode.h"
#include <tuple>


class Renderer
{
public:
	enum RENDERING_MODE
	{
		TRIANGLES,
		LINES,
		POINTS
	};

protected:
	int												m_screen_width, m_screen_height;
	glm::mat4										m_view_matrix;
	glm::mat4										m_projection_matrix;
	glm::vec3										m_camera_position;
	glm::vec3										m_camera_target_position;
	glm::vec3										m_camera_up_vector;
	glm::vec2										m_camera_movement;
	glm::vec2										m_camera_look_angle_destination;
	
	// Geometry Rendering Intermediate Buffer
	GLuint m_fbo;
	GLuint m_fbo_depth_texture;
	GLuint m_fbo_texture;
	
	GLuint m_vao_fbo, m_vbo_fbo_vertices;

	float m_continous_time;

	// Rendering Mode
	RENDERING_MODE m_rendering_mode;

	// Lights
	SpotLightNode m_spotlight_node;

	// Meshes
	class GeometryNode*								m_geometric_object1;
	glm::mat4										m_geometric_object1_transformation_matrix;
	glm::mat4										m_geometric_object1_transformation_normal_matrix;
	class GeometryNode*								m_geometric_object2;
	glm::mat4										m_geometric_object2_transformation_matrix;
	glm::mat4										m_geometric_object2_transformation_normal_matrix;
	class GeometryNode*								m_geometric_object3;
	glm::mat4										m_geometric_object3_transformation_matrix;
	glm::mat4										m_geometric_object3_transformation_normal_matrix;
	class GeometryNode*								m_geometric_object4;
	unsigned int Skybox;
	unsigned int LightMap;
	unsigned int SafeLightMap;
	// Protected Functions
	bool InitRenderingTechniques();
	bool InitIntermediateShaderBuffers();
	bool InitCommonItems();
	bool InitLightSources();
	bool InitGeometricMeshes();

	void DrawGeometryNode(class GeometryNode* node, glm::mat4 model_matrix);
	void DrawGeometryNodeToShadowMap(class GeometryNode* node, glm::mat4 model_matrix, glm::mat4 normal_matrix);

	ShaderProgram								EnviromentShader;
	ShaderProgram								SkyboxShader;
	ShaderProgram								BasicRender;
	ShaderProgram                               lightmap;
	ShaderProgram                               Cpuraster;
	ShaderProgram                               safeguarding;

public:
	Renderer();
	~Renderer();
	bool										Init(int SCREEN_WIDTH, int SCREEN_HEIGHT);
	void CreateObjects();
	void										Update(float dt);
	bool										ResizeBuffers(int SCREEN_WIDTH, int SCREEN_HEIGHT);
	bool										ReloadShaders();
	void										Render();

	void RenderObjects();

	void RenderOnscreen();

	// Passes
	void										RenderShadowMaps();
	void										RenderGeometry();
	void                                        RenderSkyBox();
	void										RenderToOutFB();
	void                                        initSkyBox();
	void Safeguard();
	void initCoords();
	void initCoords(class Object *Obv);
	void CpuRaster();
	bool Traversal(std::vector<class BBox*> *Nodes, class BBox* father, glm::vec3 orig, glm::vec3 dire);
	void CpuRaster(class Object* obv);
	void CubeMapSampling(float x, float y, float z,int *index, float *u, float *v);
	void                                        initLightMap();
	void Padding(float* array,float* temp,int LightMapSize);

	bool RayTriangleIntersection(const glm::vec3 coords,const glm::vec3 normal, std::vector<glm::vec3> NewPos);

	bool Boxintersect(glm::vec3 orig, glm::vec3 dir, glm::vec3 min, glm::vec3 max);

	// Set functions
	void										SetRenderingMode(RENDERING_MODE mode);
	
	// Camera Function
	void										CameraMoveForward(bool enable);
	void										CameraMoveBackWard(bool enable);
	void										CameraMoveLeft(bool enable);
	void										CameraMoveRight(bool enable);
	void										CameraLook(glm::vec2 lookDir);	
};

#endif
