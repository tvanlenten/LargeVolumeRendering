#ifndef MESH_H_
#define MESH_H_

#include <GL/glew.h>
#include "Buffer.h"
#include <iostream>

float cubeVerts[] = {
	0.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Bottom-left
	1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // bottom-right    
	1.0f,  1.0f, 0.0f,  1.0f, 1.0f, // top-right              
	1.0f,  1.0f, 0.0f,  1.0f, 1.0f, // top-right
	0.0f,  1.0f, 0.0f,  0.0f, 1.0f, // top-left
	0.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom-left                
	// Front face
	0.0f, 0.0f,  1.0f,  0.0f, 0.0f, // bottom-left
	1.0f,  1.0f,  1.0f,  1.0f, 1.0f, // top-right
	1.0f, 0.0f,  1.0f,  1.0f, 0.0f, // bottom-right        
	1.0f,  1.0f,  1.0f,  1.0f, 1.0f, // top-right
	0.0f, 0.0f,  1.0f,  0.0f, 0.0f, // bottom-left
	0.0f,  1.0f,  1.0f,  0.0f, 1.0f, // top-left        
	// Left face
	0.0f,  1.0f,  1.0f,  1.0f, 0.0f, // top-right
	0.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-left
	0.0f,  1.0f, 0.0f,  1.0f, 1.0f, // top-left       
	0.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-left
	0.0f,  1.0f,  1.0f,  1.0f, 0.0f, // top-right
	0.0f, 0.0f,  1.0f,  0.0f, 0.0f, // bottom-right
	// Right face
	1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // top-left
	1.0f,  1.0f, 0.0f,  1.0f, 1.0f, // top-right      
	1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-right          
	1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // bottom-right
	1.0f, 0.0f,  1.0f,  0.0f, 0.0f, // bottom-left
	1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // top-left
	// Bottom face          
	0.0f, 0.0f, 0.0f,  0.0f, 1.0f, // top-right
	1.0f, 0.0f,  1.0f,  1.0f, 0.0f, // bottom-left
	1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top-left        
	1.0f, 0.0f,  1.0f,  1.0f, 0.0f, // bottom-left
	0.0f, 0.0f, 0.0f,  0.0f, 1.0f, // top-right
	0.0f, 0.0f,  1.0f,  0.0f, 0.0f, // bottom-right
	// Top face
	0.0f,  1.0f, 0.0f,  0.0f, 1.0f, // top-left
	1.0f,  1.0f, 0.0f,  1.0f, 1.0f, // top-right
	1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // bottom-right                 
	1.0f,  1.0f,  1.0f,  1.0f, 0.0f, // bottom-right
	0.0f,  1.0f,  1.0f,  0.0f, 0.0f, // bottom-left  
	0.0f,  1.0f, 0.0f,  0.0f, 1.0f  // top-left
};
GLfloat rectVerts[] = {
	1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	1.0f,  1.0f,  1.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,
};



class Mesh {
public:
	Mesh() {
		vertexSize = 0;
		instanceSize = 0;
		attachSize = 0;
		glGenVertexArrays(1, &ID);
	}
	//The vertex array buffer being attach must be bound before this function is called
	void attach(GLenum type, int numberComponents, int stride, int offset, bool instance) {
		glVertexAttribPointer(attachSize, numberComponents, type, GL_FALSE, stride, (GLvoid*)offset);
		glEnableVertexAttribArray(attachSize);
		if(instance)glVertexAttribDivisor(attachSize, 1);
		attachSize++;
	}


	void setVextexSize(int size) { vertexSize = size; }
	void setInstanceSize(int size) { instanceSize = size; }

	void bind() { glBindVertexArray(ID); }

	void draw() { 
		glBindVertexArray(ID);
		if(instanceSize > 0)
			glDrawArraysInstanced(GL_TRIANGLES, 0, vertexSize, instanceSize);
		else
			glDrawArrays(GL_TRIANGLES, 0, vertexSize);
		glBindVertexArray(0);
	}
	void unbind() { glBindVertexArray(0); }
	~Mesh() { glDeleteVertexArrays(1, &ID); }

	void createCube() {
		Buffer meshData(GL_ARRAY_BUFFER, 180 * sizeof(GLfloat), cubeVerts, GL_STATIC_DRAW);
		meshData.bind();
		bind();
		setVextexSize(36);
		attach(GL_FLOAT, 3, 5 * sizeof(GLfloat), 0, false);
		attach(GL_FLOAT, 2, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat), false);
		unbind();
	}
	void createRect2D() {
		Buffer meshData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), rectVerts, GL_STATIC_DRAW);
		meshData.bind();
		bind();
		setVextexSize(6);
		attach(GL_FLOAT, 2, 4 * sizeof(GLfloat), 0, false);
		attach(GL_FLOAT, 2, 4 * sizeof(GLfloat), 2 * sizeof(GLfloat), false);
		unbind();
	}

private:
	GLuint ID;
	unsigned int vertexSize;
	unsigned int instanceSize;
	unsigned int attachSize;
};

#endif //Mesh.h