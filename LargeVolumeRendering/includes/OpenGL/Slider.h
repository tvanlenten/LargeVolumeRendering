#ifndef SLIDER_H_
#define SLIDER_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "text.h"
#include "Mesh.h"
#include "Shader.h"
#include <iostream>
#include <vector>
#include "Input.h"


/*
uses the text class to add support for sliders in a super basic GUI
NOT a proper solution... just useful for quickly testing out different settings
*/

class Sliders {
public:
	Sliders(float width, float height, float size, float space) {
		shader.use();
		shader.setFloat("aspectRatio", height / width);
		shader.setVec2("Pos", glm::vec2(0.1,0.1));
		shader.setFloat("size", size);
		shader.setFloat("space", space);
		w = width;
		h = height;
		GLfloat textVerts[] = {
			1.0f, 0.0f,  1.0f, 0.0f,
			0.0f, 0.0f,  0.0f, 0.0f,
			0.0f,  1.0f,  0.0f, 1.0f,

			1.0f,  1.0f,  1.0f, 1.0f,
			1.0f, 0.0f,  1.0f, 0.0f,
			0.0f,  1.0f,  0.0f, 1.0f,
		};
		Buffer meshData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), textVerts, GL_STATIC_DRAW);
		meshData.bind();
		mesh.bind();
		mesh.setVextexSize(6);
		mesh.attach(GL_FLOAT, 2, 4 * sizeof(GLfloat), 0, false);
		mesh.attach(GL_FLOAT, 2, 4 * sizeof(GLfloat), 2 * sizeof(GLfloat), false);
		buffer.bind();
		mesh.attach(GL_FLOAT, 1, sizeof(GLfloat), 0, true);
		mesh.setInstanceSize(0);
		mesh.unbind();
	}

	void add(float* ptr) {
		val = ptr;

		pos = glm::vec4(0.0,0.0,0.5,0.5);

		mesh.setInstanceSize(1);
	}
	void update() {
		glm::vec2 m = glm::vec2(mouse.x, mouse.y) / glm::vec2(w, h);
		if (m.x > pos.x && m.x < pos.x + pos.z && m.y > pos.y && m.y < pos.y + pos.w) {
			*val = m.x / pos.z;
			//set select to slider
			shader.use();
			shader.setInt("sel", 1);
		}

		shader.use();
		shader.setInt("sel", 0);

		float data[256];

		data[0] = *val;

		buffer.update(0, 256, data);
	}
	void draw() {
		shader.use();
		mesh.draw();
	}
	~Sliders() {}
private:
	//std::vector<float*> val;
	//std::vector<glm::vec4> pos;

	float* val;
	glm::vec4 pos;

	float w;
	float h;
	Shader shader{ "includes/OpenGL/slide.vt", "includes/OpenGL/slide.frag", nullptr, false };
	Mesh mesh;
	Buffer buffer{ GL_ARRAY_BUFFER, 256 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW }; //max of 256 sliders for no particular reason
};


#endif //Slider.h