#ifndef TEXT_H_
#define TEXT_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Mesh.h"
#include "Buffer.h"
#include "Shader.h"
#include "Texture.h"
#include <iostream>

/*
	This is a very simple text renderer
	supports only basic uniform spaced text
	only really useful for displaying info like FPS and other debug stuff

	not efficient since it requires an update everytime it is drawn!

*/


class Text {
public:
	Text(float width, float height){
		shader.use();
		shader.setFloat("aspectRatio", height / width);

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
	

	//use with snprintf() to print variables
	void draw(const char* text, int textLength, glm::vec2 pos, float size, float space, glm::vec3 color) {
		mesh.setInstanceSize(textLength);
		float data[256];
		for (int i = 0; i < textLength; i++) {
			data[i] = int(text[i]);
		}
		buffer.update(0, 256, data);

		shader.use();
		shader.setVec2("textPos", pos);
		shader.setFloat("size", size);
		shader.setFloat("space", space);
		shader.setVec3("texCol", color);
		font.use(0);
		mesh.draw();
	}
	
	~Text() {}
private:
	Shader shader{ "includes/OpenGL/text.vt", "includes/OpenGL/text.frag", nullptr, false };
	Mesh mesh;
	Buffer buffer{ GL_ARRAY_BUFFER, 256 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW };
	Texture font{ "includes/OpenGL/fontSD.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, true };
};


#endif //Text.h