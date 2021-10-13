#include "GeometricMesh.h"
#include "GLEW\glew.h"
#include "glm\glm.hpp"
#include <vector>
#include "Object.h"
#include "GeometryNode.h"
#include <algorithm>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

Object::Object(GeometryNode* object,glm::mat4 transformation,int size) {
	transformation_matrix = transformation;
	SetObject(object);
	LightMapSize = size;
	//std::cout << m_geometric_object->Pos.size();
	for (int i = 0; i < m_geometric_object->Pos.size(); i++) {
		glm::vec4 vec = transformation_matrix * glm::vec4(m_geometric_object->Pos[i], 1.0);
		Coords.push_back(glm::vec3(vec));
	}
}

 Object::Object() {
	 m_geometric_object = nullptr;
	 LightMapSize = 0;
 }

 Object::~Object() {
	 glDeleteTextures(1, &ObjectCoords);
	 glDeleteTextures(1, &ObjectNorms);
	 glDeleteTextures(1, &LightMap);

	 //delete m_geometric_object;
 }

 void Object::SetObject(GeometryNode* object) {
	 m_geometric_object = object;
 }
/*void Object::initCoords(int LightMapSize,ShaderProgram shader) {
	unsigned int captured;
	
	/*m_geometric_object1_transformation_matrix = glm::mat4(1.f);
	m_geometric_object1_transformation_matrix = glm::translate(glm::mat4(1.f), glm::vec3(1, 1, 0));
	m_geometric_object1_transformation_normal_matrix = glm::mat4(glm::transpose(glm::inverse(glm::mat3(m_geometric_object1_transformation_matrix))));
	glGenFramebuffers(1, &captured);

	glBindFramebuffer(GL_FRAMEBUFFER, captured);

	glGenTextures(1, &ObjectCoords);

	glBindTexture(GL_TEXTURE_2D, ObjectCoords);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, LightMapSize, LightMapSize, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ObjectCoords, 0);

	glGenTextures(1, &ObjectNorms);
	glBindTexture(GL_TEXTURE_2D, ObjectNorms);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, LightMapSize, LightMapSize, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, ObjectNorms, 0);





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
	shader.Bind();
	glViewport(0, 0, LightMapSize, LightMapSize);
	//glUniformMatrix4fv(shader["view"], 1, GL_FALSE, glm::value_ptr(m_view_matrix));
	glUniformMatrix4fv(shader["projection"], 1, GL_FALSE, glm::value_ptr(project));
	glUniformMatrix4fv(shader["model"], 1, GL_FALSE, glm::value_ptr(transformation_matrix));
	glBindVertexArray(object->m_vao);
	for (int j = 0; j <object->parts.size(); j++) {
		//std::cout << (j);
		glDrawArrays(GL_TRIANGLES, object->parts[j].start_offset, object->parts[j].count);

		GLenum error = Tools::CheckGLError();
		if (error != GL_NO_ERROR)
		{
			printf("Reanderer:Draw GL Error\n");
			system("pause");
		}
	}
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	shader.Unbind();
	//glViewport(0, 0, m_screen_width, m_screen_height);



	float * pixels = new float[4 * LightMapSize * LightMapSize];

	glBindTexture(GL_TEXTURE_2D, ObjectNorms);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels);
	for (int i = 0; i < 5; i++) {
		pixels = Padding(pixels,LightMapSize);
	}
	glGenTextures(1, & ObjectNorms);
	glBindTexture(GL_TEXTURE_2D, ObjectNorms);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, LightMapSize, LightMapSize, 0, GL_RGBA, GL_FLOAT, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

/*float* Object::Padding(float* array,int LightMapSize) {
	float* temp = new float[4 * LightMapSize * LightMapSize];
	float* sum = new float[3];

	int c = 0;
	int count = 0;
	for (int i = 0; i < LightMapSize * LightMapSize; i++) {
		if (array[4 * i + 3] != 1) {
			//count++;
			//std::cout << "im here" << " ";
			if (i % LightMapSize != 0) { // if not on the left column
				if (array[4 * i - 1] == 1) { // left neighbor
					sum[0] += array[4 * i - 4];
					sum[1] += array[4 * i - 3];
					sum[2] += array[4 * i - 2];
					c++;
				}
				if (i + LightMapSize <= LightMapSize * LightMapSize) { // if not on the upper row
					if (array[4 * (i + LightMapSize) - 1] == 1) { //upper left neighbor
						sum[0] += array[4 * (i + LightMapSize) - 4];
						sum[1] += array[4 * (i + LightMapSize) - 3];
						sum[2] += array[4 * (i + LightMapSize) - 2];
						c++;
					}

				}
				if (i - LightMapSize >= 0) { // if not on the lower row
					if (array[4 * (i - LightMapSize) - 1] == 1) { //down left neighbor
						sum[0] += array[4 * (i - LightMapSize) - 4];
						sum[1] += array[4 * (i - LightMapSize) - 3];
						sum[2] += array[4 * (i - LightMapSize) - 2];
						c++;
					}

				}
			}

			//right column of neighbors
			if (i % (LightMapSize - 1) != 0) { //if not on right column
				if (array[4 * i + 7] == 1) { //right neighbor
					sum[0] += array[4 * i + 4];
					sum[1] += array[4 * i + 5];
					sum[2] += array[4 * i + 6];
					c++;
					//std::cout << sum[0] << " " << sum[1] << " " << sum[2] << " ";
					//std::cout << array[4 * i + 4] << " " << array[4 * i + 5] << " " << array[4 * i + 6] << " ";
					//std::cout << "right neighbor";
				}
				if (i + LightMapSize <= LightMapSize * LightMapSize) { // if not on the upper row
					if (array[4 * (i + LightMapSize) + 7] == 1) { // upper right neigbor
						sum[0] += array[4 * (i + LightMapSize) + 4];
						sum[1] += array[4 * (i + LightMapSize) + 5];
						sum[2] += array[4 * (i + LightMapSize) + 6];
						c++;
					}

				}
				if (i - LightMapSize >= 0) { // if not on the lower row
					if (array[4 * (i - LightMapSize) + 7] == 1) { // down right neighbor
						sum[0] += array[4 * (i - LightMapSize) + 4];
						sum[1] += array[4 * (i - LightMapSize) + 5];
						sum[2] += array[4 * (i - LightMapSize) + 6];
						c++;
					}

				}
			}
			//center column of neighbors
			if (i + LightMapSize <= LightMapSize * LightMapSize) {
				if (array[4 * (i + LightMapSize) + 3] == 1) { // upper neighbor
					sum[0] += array[4 * (i + LightMapSize)];
					sum[1] += array[4 * (i + LightMapSize) + 1];
					sum[2] += array[4 * (i + LightMapSize) + 2];
					c++;
					//std::cout << array[4 * (i - LightMapSize)] << " " << array[4 * (i - LightMapSize) + 1] << " " << array[4 * (i - LightMapSize) + 2] << " ";
					//std::cout << "right neighbor";
				}
			}

			if (i - LightMapSize >= 0) {
				if (array[4 * (i - LightMapSize) + 3] == 1) { //down  neighbor
					sum[0] += array[4 * (i - LightMapSize)];
					sum[1] += array[4 * (i - LightMapSize) + 1];
					sum[2] += array[4 * (i - LightMapSize) + 2];
					c++;
					//std::cout << array[4 * (i + LightMapSize)] << " " << array[4 * (i + LightMapSize) + 1] << " " << array[4 * (i + LightMapSize) + 2] << " ";
					//std::cout << "right neighbor";
				}

			}


			if (c >= 1) {
				//std::cout << c << " ";
				temp[4 * i] = sum[0] / c;
				temp[4 * i + 1] = sum[1] / c;
				temp[4 * i + 2] = sum[2] / c;
				temp[4 * i + 3] = 1;

			}
			else {
				//std::cout << "im here" << " ";
				//std::cout << temp[4 * i] << " ";
				/*temp[4 * i] = 1;
				temp[4 * i + 1] = 0;
				temp[4 * i + 2] = 0;
				temp[4 * i + 3] = array[4 * i + 3];
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
	temp[4 * 4494] = 1;
	temp[4 * 4494 + 1] = 0;
	temp[4 * 4494 + 2] = 0;
	//std::cout << count << " ";
	return temp;
}*/