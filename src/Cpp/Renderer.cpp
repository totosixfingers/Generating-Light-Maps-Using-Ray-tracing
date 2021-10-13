#include <omp.h>
#include "Renderer.h"
#include "GeometryNode.h"
#include "Tools.h"
#include <algorithm>
#include "ShaderProgram.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "OBJLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include "SDL2/SDL_image.h"
#include <tuple>
#include "Object.h"
#include "glm/gtx/intersect.hpp"
#include <set>
#include <math.h> 
#include "BBox.h"
#include "BVH.h"
#include <stdlib.h> 
//#pragma comment(lib, "irrKlang.lib")
//#include "include/irrKlang.h"
//using namespace irrklang;





// *SoundEngine = createIrrKlangDevice();
// RENDERER
Renderer::Renderer()
{

	m_geometric_object1 = nullptr;
	m_geometric_object2 = nullptr;
	m_geometric_object3 = nullptr;
	m_geometric_object4 = nullptr;

	m_continous_time = 0.0;

	boxlinesVAO = 0, boxlinesVBO = 0;
	skyboxVAO = 0, skyboxVBO = 0;
	cubeVAO = 0, cubeVBO= 0;
	CubemapWidth = 0, CubemapHeight = 0;
	object1Coords = 0, object1Norms = 0, object1Texcoords = 0;
	captured = 0;
	Skybox = 0;
	LightMap = 0;
	SafeLightMap = 0;
	m_screen_height = 0, m_screen_width = 0;

	// initialize the camera parameters
	m_camera_position = glm::vec3(0, 0, 3);
	m_camera_target_position = glm::vec3(0, 0, 0);
	m_camera_up_vector = glm::vec3(0, 1, 0);
}
void printVec(glm::vec3 pri) {
	std::cout << pri.x << "," << pri.y << "," << pri.z << " ";
}
Renderer::~Renderer()
{
	// delete g_buffer
	glDeleteTextures(1, &Skybox);
	glDeleteFramebuffers(1, &captured);

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteVertexArrays(1, &boxlinesVAO);

	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &skyboxVBO);
	glDeleteBuffers(1, &boxlinesVBO);

	delete[] posx;
	delete[] negx;
	delete[] posy;
	delete[] negy;
	delete[] posz;
	delete[] negz;
	for (auto& object : objects)
		delete object;
	objects.clear();
	/*for (auto& bv : pMinMax)
		delete bv;*/
	delete m_geometric_object1;
	delete m_geometric_object2;
	delete m_geometric_object3;
	delete m_geometric_object4;
}

bool Renderer::Init(int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	this->m_screen_width = SCREEN_WIDTH;
	this->m_screen_height = SCREEN_HEIGHT;

	// Initialize OpenGL functions

	//Set clear color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//This enables depth and stencil testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
	// glClearDepth sets the value the depth buffer is set at, when we clear it
	//glEnable(GL_FRAMEBUFFER_SRGB);
	// Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// open the viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); //we set up our viewport

	bool techniques_initialization = InitRenderingTechniques();
	bool meshes_initialization = InitGeometricMeshes();

	//If there was any errors
	if (Tools::CheckGLError() != GL_NO_ERROR)
	{
		printf("Exiting with error at Renderer::Init\n");
		return false;
	}

	m_projection_matrix = glm::perspective(glm::radians(60.f), SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1500.0f);
	m_view_matrix = glm::lookAt(m_camera_position, m_camera_target_position, m_camera_up_vector);
	//initSkyBox();
	CreateObjects();
	//initBoxLines();
	//If everything initialized
	return techniques_initialization && meshes_initialization;
}
void Renderer::initSkyBox() {

	GLfloat skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	GLfloat cubeVertices[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	std::string xpos = "../Data/cubemap/posx.jpg";
	std::string xneg = "../Data/cubemap/negx.jpg";
	std::string ypos = "../Data/cubemap/posy.jpg";
	std::string yneg = "../Data/cubemap/negy.jpg";
	std::string zpos = "../Data/cubemap/posz.jpg";
	std::string zneg = "../Data/cubemap/negz.jpg";
	std::string faces[6];
	faces[0] = xpos;
	faces[1] = xneg;
	faces[2] = ypos;
	faces[3] = yneg;
	faces[4] = zpos;
	faces[5] = zneg;
	glGenTextures(1, &Skybox);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			CubemapHeight = width;
			CubemapWidth = height;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);

		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox);

	posx = new float[3 * CubemapHeight * CubemapWidth];
	glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, GL_FLOAT, posx);
	negx = new float[3 * CubemapHeight * CubemapWidth];
	glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, GL_FLOAT, negx);
	posy = new float[3 * CubemapHeight * CubemapWidth];
	glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, GL_FLOAT, posy);
	negy = new float[3 * CubemapHeight * CubemapWidth];
	glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, GL_FLOAT, negy);
	posz = new float[3 * CubemapHeight * CubemapWidth];
	glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, GL_FLOAT, posz);
	negz = new float[3 * CubemapHeight * CubemapWidth];
	glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, GL_FLOAT, negz);

}


void Renderer::CpuRaster(Object* obv) {
	float offset = 0.00001;

	float* LtMap = new float[4 * obv->LightMapSize * obv->LightMapSize];


	float* coords = new float[4 * obv->LightMapSize * obv->LightMapSize];
	glBindTexture(GL_TEXTURE_2D, obv->ObjectCoords);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, coords);
	float* pixels = new float[4 * obv->LightMapSize * obv->LightMapSize];

	glBindTexture(GL_TEXTURE_2D, obv->ObjectNorms);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels);

	const float PI = 3.14159265359;

	for (int i = 0; i < obv->LightMapSize * obv->LightMapSize; i++) {

		if (pixels[4 * i] != 0 || pixels[4 * i + 1] != 0 || pixels[4 * i + 2] != 0) {
			glm::vec3 irradiance = glm::vec3(0.0);
			glm::vec3 N = glm::vec3(pixels[4 * i], pixels[4 * i + 1], pixels[4 * i + 2]);
			// tangent space calculation from origin point
			glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
			glm::vec3 tangent = glm::vec3(0, 0, 0);
			glm::vec3 bitangent = glm::vec3(0, 0, 0);
			float tang = glm::dot(up, N);
			if (tang == 1 || tang == -1) {
				tangent = glm::vec3(0, 0, 1);
				bitangent = glm::vec3(1, 0, 0);

			}
			else {
				tangent = normalize(cross(up, N));
				bitangent = glm::cross(N, tangent);
			}
			glm::vec3* telos = new glm::vec3[256];
			int sampleDelta = 2;
			float nrSamples = 0.0;
			//int phiMax = int(10 * PI)+1;
			//int thetaMax = int(2.5 * PI)+1;
#pragma omp parallel for
			//for (int phis = 0.0; phis < phiMax * thetaMax; phis++)
			//{
			for (int phis = 0.0; phis < int(20 * PI); phis += sampleDelta)
			{
				for (int thetas = 0.0; thetas < int(5 * PI); thetas += sampleDelta)
				{
					//std::cout << omp_get_num_threads();
					//float phi = float(phis / thetaMax) / 5;
					//float theta = float(phis%thetaMax) / 5;
					float phi = float(phis) / 10;
					float theta = float(thetas) / 10;

					float v = 0;
					float u = 0;
					int index = 0;
					// spherical to cartesian (in tangent space)
					glm::vec3 tangentSample = glm::vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
					// tangent space to world
					glm::vec3 sampleVec = tangentSample.x * tangent + tangentSample.y * bitangent + tangentSample.z * N;
					if (sampleVec.x != 0 || sampleVec.y != 0 || sampleVec.z != 0) {
						bool flag = false;

						for (int j = 0; j < objects.size(); j++) {
							flag = RayTriangleIntersection(glm::vec3(coords[4 * i], coords[4 * i + 1], coords[4 * i + 2]) + glm::vec3(N * offset), sampleVec, objects[j]->Coords);
							if (flag) {
								break;

							}

						}

						if (flag) {
#pragma omp critical
							{
								irradiance += glm::vec3(0, 0, 0);
								nrSamples++;
							}
						}
						else {
							CubeMapSampling(sampleVec.x, sampleVec.y, sampleVec.z, &index, &u, &v);
							int us = int(u * CubemapHeight);
							int vs = int(v * CubemapWidth);
							switch (index) {
							case 0:
#pragma omp critical
							{
								irradiance += glm::vec3(posx[us * vs * 3], posx[us * vs * 3 + 1], posx[us * vs * 3 + 2]) * cos(theta) * sin(theta);
								nrSamples++;
							}
							break;
							case 1:
#pragma omp critical
							{
								irradiance += glm::vec3(negx[us * vs * 3], negx[us * vs * 3 + 1], negx[us * vs * 3 + 2]) * cos(theta) * sin(theta);
								nrSamples++;
							}
							break;
							case 2:
#pragma omp critical
							{
								irradiance += glm::vec3(posy[us * vs * 3], posy[us * vs * 3 + 1], posy[us * vs * 3 + 2]) * cos(theta) * sin(theta);
								nrSamples++;
							}
							break;
							case 3:
#pragma omp critical
							{
								irradiance += glm::vec3(negy[us * vs * 3], negy[us * vs * 3 + 1], negy[us * vs * 3 + 2]) * cos(theta) * sin(theta);
								nrSamples++;
							}
							break;
							case 4:
#pragma omp critical
							{
								irradiance += glm::vec3(posz[us * vs * 3], posz[us * vs * 3 + 1], posz[us * vs * 3 + 2]) * cos(theta) * sin(theta);
								nrSamples++;
							}
							break;
							case 5:
#pragma omp critical
							{
								irradiance += glm::vec3(negz[us * vs * 3], negz[us * vs * 3 + 1], negz[us * vs * 3 + 2]) * cos(theta) * sin(theta);
								nrSamples++;
							}
							break;

							}

						}
					}
				}
			}


			irradiance = (PI * irradiance) * (float(1.0) / float(256));

			LtMap[4 * i] = float(irradiance.x);
			LtMap[4 * i + 1] = float(irradiance.y);
			LtMap[4 * i + 2] = float(irradiance.z);
			LtMap[4 * i + 3] = 1;

		}
		else {
			LtMap[4 * i] = 0;
			LtMap[4 * i + 1] = 0;
			LtMap[4 * i + 2] = 0;
			LtMap[4 * i + 3] = 0;
		}
	}
	float* temp = new float[4 * obv->LightMapSize * obv->LightMapSize];
	for (int h = 0; h < 4 * obv->LightMapSize * obv->LightMapSize; h++) {
		temp[h] = 0;
	}

	int paddingIterations = 3;
	for (int k = 0; k < paddingIterations; k++) {

		Padding(LtMap, temp, obv->LightMapSize);
		std::swap(temp, LtMap);

	}

	glGenTextures(1, &obv->LightMap);
	glBindTexture(GL_TEXTURE_2D, obv->LightMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, obv->LightMapSize, obv->LightMapSize, 0, GL_RGBA, GL_FLOAT, LtMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// LtMap;
	delete[] coords;
	delete[] pixels;
	//delete temp;
	delete[] temp;
	delete[] LtMap;
}
void::Renderer::CubeMapSampling(float x, float y, float z, int* index, float* u, float* v) {

	//std::cout << x << y << z << " " ;
	float absX = fabs(x);
	float absY = fabs(y);
	float absZ = fabs(z);

	int isXPositive = x > 0 ? 1 : 0;
	int isYPositive = y > 0 ? 1 : 0;
	int isZPositive = z > 0 ? 1 : 0;

	float maxAxis, uc, vc;

	// POSITIVE X
	if (isXPositive && absX >= absY && absX >= absZ) {
		// u (0 to 1) goes from +z to -z
		// v (0 to 1) goes from -y to +y
		maxAxis = absX;
		uc = -z;
		vc = y;
		*index = 0;
	}
	// NEGATIVE X
	else if (!isXPositive && absX >= absY && absX >= absZ) {
		// u (0 to 1) goes from -z to +z
		// v (0 to 1) goes from -y to +y
		maxAxis = absX;
		uc = z;
		vc = y;
		*index = 1;
	}
	// POSITIVE Y
	else if (isYPositive && absY >= absX && absY >= absZ) {
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from +z to -z
		maxAxis = absY;
		uc = x;
		vc = -z;
		*index = 2;
	}
	// NEGATIVE Y
	else if (!isYPositive && absY >= absX && absY >= absZ) {
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from -z to +z
		maxAxis = absY;
		uc = x;
		vc = z;
		*index = 3;
	}
	// POSITIVE Z
	else if (isZPositive && absZ >= absX && absZ >= absY) {
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from -y to +y
		maxAxis = absZ;
		uc = x;
		vc = y;
		*index = 4;
	}
	// NEGATIVE Z
	else if (!isZPositive && absZ >= absX && absZ >= absY) {
		// u (0 to 1) goes from +x to -x
		// v (0 to 1) goes from -y to +y
		maxAxis = absZ;
		uc = -x;
		vc = y;
		*index = 5;
	}

	// Convert range from -1 to 1 to 0 to 1
	*u = 0.5f * (uc / maxAxis + 1.0f);
	*v = 0.5f * (vc / maxAxis + 1.0f);

	//	std::cout << *v;
}


void Renderer::initCoords(Object* Obv) {


	glGenFramebuffers(1, &captured);

	glBindFramebuffer(GL_FRAMEBUFFER, captured);

	glGenTextures(1, &Obv->ObjectCoords);

	glBindTexture(GL_TEXTURE_2D, Obv->ObjectCoords);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Obv->LightMapSize, Obv->LightMapSize, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Obv->ObjectCoords, 0);

	glGenTextures(1, &Obv->ObjectNorms);
	glBindTexture(GL_TEXTURE_2D, Obv->ObjectNorms);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Obv->LightMapSize, Obv->LightMapSize, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, Obv->ObjectNorms, 0);





	GLenum buffers_to_render[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers_to_render);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);

	}

	glm::mat4 project = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 10.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, captured);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Cpuraster.Bind();
	glViewport(0, 0, Obv->LightMapSize, Obv->LightMapSize);
	glUniformMatrix4fv(Cpuraster["view"], 1, GL_FALSE, glm::value_ptr(m_view_matrix));
	glUniformMatrix4fv(Cpuraster["projection"], 1, GL_FALSE, glm::value_ptr(project));
	glUniformMatrix4fv(Cpuraster["model"], 1, GL_FALSE, glm::value_ptr(Obv->transformation_matrix));
	glBindVertexArray(Obv->m_geometric_object->m_vao);
	for (int j = 0; j < Obv->m_geometric_object->parts.size(); j++) {
		glDrawArrays(GL_TRIANGLES, Obv->m_geometric_object->parts[j].start_offset, Obv->m_geometric_object->parts[j].count);
		GLenum error = Tools::CheckGLError();
		if (error != GL_NO_ERROR)
		{
			printf("Reandererlkjlkj:Draw GL Error\n");
			system("pause");
		}
	}
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Cpuraster.Unbind();
	glViewport(0, 0, m_screen_width, m_screen_height);

}
bool InBounds(int i, int LightMapSize) {

	return (!(i % LightMapSize == 0)
		&& !(i % LightMapSize == LightMapSize - 1)
		&& (i >= 0 && i <= LightMapSize * LightMapSize));
}


void Renderer::Padding(float* array, float* temp, int LightMapSize) {
	float sum[3] = { 0, 0, 0 };

	int c = 0;
	for (int i = 0; i < LightMapSize * LightMapSize; i++) {
		if (array[4 * i + 3] != 1) {
			for (int j = i - 1; j <= i + 1; j += 2) { // left and right neighbor
				if (InBounds(j, LightMapSize))
					if (array[4 * j + 3] == 1) {
						sum[0] += array[4 * j];
						sum[1] += array[4 * j + 1];
						sum[2] += array[4 * j + 2];
						c++;
					}

			}
			for (int j = i + LightMapSize - 1; j <= i + LightMapSize + 1; j++) { //UPPER left, center and right neighbor | Texture (0,0) is bottom left corner
				if (InBounds(j, LightMapSize))
					if (array[4 * j + 3] == 1) {
						sum[0] += array[4 * j];
						sum[1] += array[4 * j + 1];
						sum[2] += array[4 * j + 2];
						c++;
					}
			}
			for (int j = i - LightMapSize - 1; j <= i - LightMapSize + 1; j++) { //DOWN left, center and right neighbor | Texture (0,0) is bottom left corner
				if (InBounds(j, LightMapSize))
					if (array[4 * j + 3] == 1) {
						sum[0] += array[4 * j];
						sum[1] += array[4 * j + 1];
						sum[2] += array[4 * j + 2];
						c++;
					}
			}

			if (c >= 1) {
				temp[4 * i] = sum[0] / c;
				temp[4 * i + 1] = sum[1] / c;
				temp[4 * i + 2] = sum[2] / c;
				temp[4 * i + 3] = 1;

			}
			else {
				temp[4 * i] = array[4 * i];
				temp[4 * i + 1] = array[4 * i + 1];
				temp[4 * i + 2] = array[4 * i + 2];
				temp[4 * i + 3] = array[4 * i + 3];
			}
		}
		else {
			temp[4 * i] = array[4 * i];
			temp[4 * i + 1] = array[4 * i + 1];
			temp[4 * i + 2] = array[4 * i + 2];
			temp[4 * i + 3] = array[4 * i + 3];
		}

		c = 0;
		sum[0] = 0;
		sum[1] = 0;
		sum[2] = 0;
	}

}

bool Renderer::RayTriangleIntersection(glm::vec3 coords, glm::vec3 normal, std::vector<glm::vec3> NewPos) {

	for (int i = 0; i < NewPos.size(); i = i + 3)
	{

		glm::vec3 barycoord;

		if (glm::intersectRayTriangle(coords, normal, NewPos[i], NewPos[i + 1], NewPos[i + 2], barycoord))
		{

			return true;
		}
	}
	return false;
}


bool LessEqual(glm::vec3 min, glm::vec3 coords) {
	//bool lol = min.x <= coords.x && min.y <= coords.y && min.z <= coords.z;
	//std::cout << lol << " " ;
	if (min.x <= coords.x && min.y <= coords.y && min.z <= coords.z) {
		return true;
	}
	return false;
}

void findpMINMAX(std::vector<glm::vec3> Coords) {
	glm::vec3 pMax, pMin;
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

	pMax = glm::vec3(max_x, max_y, max_z);
	pMin = glm::vec3(min_x, min_y, min_z);
	//BBox a = BBox(pMin, pMax, Coords);
	//pMinMax.push_back(pMax);
	//pMinMax.push_back(pMin);
	//pMinMax.push_back(a);
}


bool Renderer::Boxintersect(glm::vec3 orig, glm::vec3 dir, glm::vec3 min, glm::vec3 max)

{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	glm::vec3 invdir = glm::vec3(1 / dir.x, 1 / dir.y, 1 / dir.z);

	if (invdir.x >= 0) {
		tmin = (min.x - orig.x) * invdir.x;
		tmax = (max.x - orig.x) * invdir.x;
	}
	else {
		tmin = (max.x - orig.x) * invdir.x;
		tmax = (min.x - orig.x) * invdir.x;
	}

	if (invdir.y >= 0) {
		tymin = (min.y - orig.y) * invdir.y;
		tymax = (max.y - orig.y) * invdir.y;
	}
	else {
		tymin = (max.y - orig.y) * invdir.y;
		tymax = (min.y - orig.y) * invdir.y;
	}

	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	if (invdir.z >= 0) {
		tzmin = (min.z - orig.z) * invdir.z;
		tzmax = (max.z - orig.z) * invdir.z;
	}
	else {
		tzmin = (max.z - orig.z) * invdir.z;
		tzmax = (min.z - orig.z) * invdir.z;
	}

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	return true;
}

void Renderer::Clusters(glm::vec3 pmin, glm::vec3 pmax, std::vector<glm::vec3> Coords) {
	std::vector<glm::vec3> Triangles;
	for (int i = 0; i < Coords.size(); i += 3) {

		glm::vec3 center = (Coords[i] + Coords[i + 1] + Coords[i + 2]) / 3.0f;
		if (LessEqual(pmin, center) && LessEqual(center, pmax)) {
			Triangles.push_back(Coords[i]);
			Triangles.push_back(Coords[i + 1]);
			Triangles.push_back(Coords[i + 2]);
		}
	}
	BVH a;
	a.Coords = Triangles;
	a.compute();
	a.computeCube();
	pMinMax.push_back(a);
}

void Renderer::BVh(std::vector<glm::vec3> Coords) {
	std::vector<float> X;
	std::vector<float> Y;
	std::vector<float> Z;
	float minmax[3][5];
	int slide = Coords.size() / 4;
	for (int i = 0; i < Coords.size(); i++) {
		X.push_back(Coords[i].x);
		Y.push_back(Coords[i].y);
		Z.push_back(Coords[i].z);
	}
	sort(X.begin(), X.begin() + Coords.size());
	sort(Y.begin(), Y.begin() + Coords.size());
	sort(Z.begin(), Z.begin() + Coords.size());

	minmax[0][0] = X[0];
	minmax[0][1] = X[slide];
	minmax[0][2] = X[2.f * slide];
	minmax[0][3] = X[3.f * slide];
	minmax[0][4] = X[Coords.size() - 1];
	minmax[1][0] = Y[0];
	minmax[1][1] = Y[slide];
	minmax[1][2] = Y[2.f * slide];
	minmax[1][3] = Y[3.f * slide];
	minmax[1][4] = Y[Coords.size() - 1];
	minmax[2][0] = Z[0];
	minmax[2][1] = Z[slide];
	minmax[2][2] = Z[2.f * slide];
	minmax[2][3] = Z[3.f * slide];
	minmax[2][4] = Z[Coords.size() - 1];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			Clusters(glm::vec3(minmax[0][0], minmax[1][i], minmax[2][j]), glm::vec3(minmax[0][1], minmax[1][i + 1], minmax[2][j + 1]), Coords);
			Clusters(glm::vec3(minmax[0][1], minmax[1][i], minmax[2][j]), glm::vec3(minmax[0][2], minmax[1][i + 1], minmax[2][j + 1]), Coords);
			Clusters(glm::vec3(minmax[0][2], minmax[1][i], minmax[2][j]), glm::vec3(minmax[0][3], minmax[1][i + 1], minmax[2][j + 1]), Coords);
			Clusters(glm::vec3(minmax[0][3], minmax[1][i], minmax[2][j]), glm::vec3(minmax[0][4], minmax[1][i + 1], minmax[2][j + 1]), Coords);

		}
	}
}

void Renderer::CreateObjects() {

	//SoundEngine->play2D("C://Users/riko/Desktop/ptyxiakh/ptuxiaki/3rdparty/lib/x64/irrKlang/media/ElevatorMusic.mp3", true);

	glm::mat4 trans = glm::translate(glm::mat4(1.f), glm::vec3(1, 1, 0));

	Object* a = new Object(m_geometric_object4, trans, 256);

	objects.push_back(a);

	//trans = glm::translate(glm::mat4(1.f), glm::vec3(2, 5, 0));
	//Object* b = new Object(m_geometric_object4, trans, 256);
	//b->transformation_matrix = trans;
	//objects.push_back(b);

	//trans = glm::translate(glm::mat4(1.f), glm::vec3(2, 7, 0));
	//Object* c = new Object(m_geometric_object4, trans, 256);
	//c->transformation_matrix = trans;
	//objects.push_back(c);

	//trans = glm::translate(glm::mat4(1.f), glm::vec3(1, 3, 0));
	//Object* d = new Object(m_geometric_object4, trans, 256);
	////c->transformation_matrix = trans;
	//objects.push_back(d);
	std::vector<glm::vec3> Cords;

	for (int j = 0; j < objects.size(); j++) {
		for (int h = 0; h < objects[j]->Coords.size(); h++) {
			Cords.push_back(objects[j]->Coords[h]);
		}
	}
	BVh(Cords);
	//FindBounds(Cords);
	for (int i = 0; i < objects.size(); i++) {
		std::cout << "hi";

		initCoords(objects[i]);

		std::cout << "hi"<< objects[i]->Coords.size();

		//CpuRaster(objects[i]);

		std::cout << "hi";
	}

	//SoundEngine->play2D("C://Users/riko/Desktop/ptyxiakh/ptuxiaki/3rdparty/lib/x64/irrKlang/media/ElevatorMusic.mp3", false);
}




void Renderer::Update(float dt)
{
	float movement_speed = 25.0f;
	// compute the direction of the camera
	glm::vec3 direction = glm::normalize(m_camera_target_position - m_camera_position);

	// move the camera towards the direction of the camera
	m_camera_position += m_camera_movement.x * movement_speed * direction * dt;
	m_camera_target_position += m_camera_movement.x * movement_speed * direction * dt;

	// move the camera sideways
	glm::vec3 right = glm::normalize(glm::cross(direction, m_camera_up_vector));
	m_camera_position += m_camera_movement.y * movement_speed * right * dt;
	m_camera_target_position += m_camera_movement.y * movement_speed * right * dt;

	glm::mat4 rotation = glm::mat4(1.0f);
	float angular_speed = glm::pi<float>() * 0.0025f;

	// compute the rotation of the camera based on the mouse movement
	rotation *= glm::rotate(glm::mat4(1.0), m_camera_look_angle_destination.y * angular_speed, right);
	rotation *= glm::rotate(glm::mat4(1.0), -m_camera_look_angle_destination.x * angular_speed, m_camera_up_vector);
	m_camera_look_angle_destination = glm::vec2(0);

	// rotate the camera direction
	direction = rotation * glm::vec4(direction, 0);
	float dist = glm::distance(m_camera_position, m_camera_target_position);
	m_camera_target_position = m_camera_position + direction * dist;

	// compute the view matrix
	m_view_matrix = glm::lookAt(m_camera_position, m_camera_target_position, m_camera_up_vector);

	m_continous_time += dt;

}

bool Renderer::InitRenderingTechniques()
{
	bool initialized = true;


	// Create and Compile Particle Shader
	std::string vertex_shader_path = "../Data/Shaders/sky.vert";
	std::string fragment_shader_path = "../Data/Shaders/sky.frag";
	SkyboxShader.LoadVertexShaderFromFile(vertex_shader_path.c_str());
	SkyboxShader.LoadFragmentShaderFromFile(fragment_shader_path.c_str());
	initialized = initialized && SkyboxShader.CreateProgram();
	SkyboxShader.LoadUniform("projection");
	SkyboxShader.LoadUniform("view");
	SkyboxShader.LoadUniform("skybox");

	vertex_shader_path = "../Data/Shaders/render.vert";
	fragment_shader_path = "../Data/Shaders/render.frag";
	BasicRender.LoadVertexShaderFromFile(vertex_shader_path.c_str());
	BasicRender.LoadFragmentShaderFromFile(fragment_shader_path.c_str());
	initialized = initialized && BasicRender.CreateProgram();
	BasicRender.LoadUniform("projection");
	BasicRender.LoadUniform("view");
	BasicRender.LoadUniform("model");
	BasicRender.LoadUniform("iradtexture");
	

	vertex_shader_path = "../Data/Shaders/cpuraster.vert";
	fragment_shader_path = "../Data/Shaders/cpuraster.frag";
	Cpuraster.LoadVertexShaderFromFile(vertex_shader_path.c_str());
	Cpuraster.LoadFragmentShaderFromFile(fragment_shader_path.c_str());
	initialized = initialized && Cpuraster.CreateProgram();
	Cpuraster.LoadUniform("projection");
	Cpuraster.LoadUniform("view");
	Cpuraster.LoadUniform("model");

	vertex_shader_path = "../Data/Shaders/safeguarding.vert";
	fragment_shader_path = "../Data/Shaders/safeguarding.frag";
	safeguarding.LoadVertexShaderFromFile(vertex_shader_path.c_str());
	safeguarding.LoadFragmentShaderFromFile(fragment_shader_path.c_str());
	initialized = initialized && safeguarding.CreateProgram();
	safeguarding.LoadUniform("projection");
	safeguarding.LoadUniform("Sampler");
	safeguarding.LoadUniform("invScreenSize");

	vertex_shader_path = "../Data/Shaders/drawboxes.vert";
	fragment_shader_path = "../Data/Shaders/drawboxes.frag";
	BoxShader.LoadVertexShaderFromFile(vertex_shader_path.c_str());
	BoxShader.LoadFragmentShaderFromFile(fragment_shader_path.c_str());
	initialized = initialized && BoxShader.CreateProgram();
	BoxShader.LoadUniform("projection");
	BoxShader.LoadUniform("view");
	return initialized;
}

// Load Geometric Meshes
bool Renderer::InitGeometricMeshes()
{
	bool initialized = true;

	OBJLoader loader;
	// load geometric object 1
	auto mesh = loader.load("../Data/apartment_atlas.obj");
	if (mesh != nullptr)
	{
		m_geometric_object1 = new GeometryNode();
		m_geometric_object1->Init(mesh);
	}
	else
		initialized = false;


	// load geometric object 1
	mesh = loader.load("../Data/storageroom_atlas.obj");
	if (mesh != nullptr)
	{
		m_geometric_object2 = new GeometryNode();
		m_geometric_object2->Init(mesh);
	}
	else
		initialized = false;

	mesh = loader.load("../Data/wheelbarrow_atlas.obj");
	if (mesh != nullptr)
	{
		m_geometric_object3 = new GeometryNode();
		m_geometric_object3->Init(mesh);
	}
	else
		initialized = false;

	mesh = loader.load("../Data/fire-hydrant/fire-hydrant-main.obj");
	if (mesh != nullptr)
	{
		m_geometric_object4 = new GeometryNode();
		m_geometric_object4->Init(mesh);
	}
	else
		initialized = false;

	delete mesh;

	return initialized;
}

void Renderer::initBoxLines() {


	for (int i = 0; i < pMinMax.size(); i++) {
		for (int j = 0; j < pMinMax[i].AllBoxVertices.size(); j++) {
			boxesVertices.push_back(pMinMax[i].AllBoxVertices[j]);
		}

	}
	glGenVertexArrays(1, &boxlinesVAO);
	glGenBuffers(1, &boxlinesVBO);
	glBindVertexArray(boxlinesVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boxlinesVBO);
	glBufferData(GL_ARRAY_BUFFER, boxesVertices.size() * sizeof(glm::vec3), &boxesVertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


}

// Render the Scene
void Renderer::Render()
{
	// Draw the geometry
	RenderObjects();
	//RenderSkyBox();
	//RenderBoxes();
	//RenderOnscreen();

	GLenum error = Tools::CheckGLError();
	if (error != GL_NO_ERROR)
	{
		printf("Reandererror:Draw GL Error\n");
		system("pause");
	}
}

void Renderer::RenderBoxes() {

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDepthFunc(GL_ALWAYS);
	
	BoxShader.Bind();
	glBindVertexArray(boxlinesVAO);
	glUniformMatrix4fv(BoxShader["view"], 1, GL_FALSE, glm::value_ptr(m_view_matrix));
	glUniformMatrix4fv(BoxShader["projection"], 1, GL_FALSE, glm::value_ptr(m_projection_matrix));
	glDrawArrays(GL_LINES, 0, boxesVertices.size());

	BoxShader.Unbind();
	glBindVertexArray(0);
	//glDepthFunc(GL_LEQUAL);
}
void Renderer::RenderObjects()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	BasicRender.Bind();

	for (int i = 0; i < objects.size(); i++) {

		glUniformMatrix4fv(BasicRender["view"], 1, GL_FALSE, glm::value_ptr(m_view_matrix));
		glUniformMatrix4fv(BasicRender["projection"], 1, GL_FALSE, glm::value_ptr(m_projection_matrix));
		glUniform1i(BasicRender["iradtexture"], 0);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, objects[i]->LightMap);
		DrawGeometryNode(objects[i]->m_geometric_object, objects[i]->transformation_matrix);

	}
	glBindVertexArray(0);
	BasicRender.Unbind();
}

void Renderer::RenderOnscreen() {

	glm::mat4 project = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 10.0f);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	safeguarding.Bind();
	glUniform1i(safeguarding["Sampler"], 0);

	glUniformMatrix4fv(safeguarding["projection"], 1, GL_FALSE, glm::value_ptr(project));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, objects[0]->LightMap);



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(m_geometric_object4->m_vao);
	for (int j = 0; j < m_geometric_object4->parts.size(); j++) {

		glDrawArrays(GL_TRIANGLES, m_geometric_object4->parts[j].start_offset, m_geometric_object4->parts[j].count);

	}
	glBindVertexArray(0);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	safeguarding.Unbind();
	glViewport(0, 0, m_screen_width, m_screen_height);


}

void Renderer::RenderSkyBox() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	SkyboxShader.Bind();
	glUniform1i(SkyboxShader["skybox"], 0);
	glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(m_camera_position, m_camera_target_position, m_camera_up_vector)));
	glUniformMatrix4fv(SkyboxShader["view"], 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(SkyboxShader["projection"], 1, GL_FALSE, glm::value_ptr(m_projection_matrix));

	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	SkyboxShader.Unbind();
	glDepthFunc(GL_LESS);
}


void Renderer::DrawGeometryNode(GeometryNode* node, glm::mat4 model_matrix)
{
	glBindVertexArray(node->m_vao);
	glUniformMatrix4fv(BasicRender["model"], 1, GL_FALSE, glm::value_ptr(model_matrix));
	for (int j = 0; j < node->parts.size(); j++)
	{
		glDrawArrays(GL_TRIANGLES, node->parts[j].start_offset, node->parts[j].count);
	}
}



void Renderer::CameraMoveForward(bool enable)
{
	m_camera_movement.x = (enable) ? 1 : 0;
}
void Renderer::CameraMoveBackWard(bool enable)
{
	m_camera_movement.x = (enable) ? -1 : 0;
}

void Renderer::CameraMoveLeft(bool enable)
{
	m_camera_movement.y = (enable) ? -1 : 0;
}
void Renderer::CameraMoveRight(bool enable)
{
	m_camera_movement.y = (enable) ? 1 : 0;
}

void Renderer::CameraLook(glm::vec2 lookDir)
{
	m_camera_look_angle_destination = glm::vec2(1, -1) * lookDir;
}
