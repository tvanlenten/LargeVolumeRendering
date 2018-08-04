#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <GL/glew.h>

class FrameBuffer {
public:
	FrameBuffer(int w, int h) {
		ID = 0;
		width = w;
		height = h;
	}
	FrameBuffer(int w, int h, GLuint texID) {
		width = w;
		height = h;
		glGenFramebuffers(1, &ID);
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texID, 0);

		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, w, h);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	/*
	FrameBuffer(int w, int h, GLuint tex0ID, GLuint tex1ID) {
		ID = 0;
		width = w;
		height = h;
		glGenFramebuffers(1, &ID);
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex0ID, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, tex1ID, 0);
	}
	*/
	void use() { 
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
		glViewport(0, 0, width, height);
	}
	void clearToColor(glm::vec4 col) { glClearColor(col.r, col.g, col.b, col.a); glClear(GL_COLOR_BUFFER_BIT); }
	void clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

	~FrameBuffer() { }
private:
	GLuint ID;
	int width;
	int height;
};

#endif //FrameBuffer.h