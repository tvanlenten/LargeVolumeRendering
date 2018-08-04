#ifndef BUFFER_H_
#define BUFFER_H_

#include <GL/glew.h>

/*
	add support for map range instead of bufferSubData
*/

class Buffer {
public:
	Buffer(GLenum buffer_type, unsigned int dataSize, GLvoid* data, GLenum memoryType) {
		bufferType = buffer_type;
		glGenBuffers(1, &ID);
		glBindBuffer(bufferType, ID);
		glBufferData(bufferType, dataSize, data, memoryType);
		glBindBuffer(bufferType, 0);
	}
	void update(int offset, int size, GLvoid* data) {
		glBindBuffer(bufferType, ID);
		glBufferSubData(bufferType, offset, size, data);
		glBindBuffer(bufferType, 0);
	}
	void bind() { glBindBuffer(bufferType, ID); }
	void bindTo(int base) { glBindBufferBase(bufferType, base, ID); }
	void unbind() { glBindBuffer(bufferType, 0); }
	~Buffer() { glDeleteBuffers(1, &ID); }

private:
	GLuint ID;
	GLenum bufferType;
};

#endif //Buffer.h