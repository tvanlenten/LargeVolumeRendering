#include "OpenGL\OpenGL.h"
#include <iostream>
#include "SDF/signedDistanceFields.h"
#include <chrono>

//#define width 1280
//#define height 720

#define width 1920
#define height 1080

int main() {
	Control control(width, height, "LargeVolumeRendering", false);
	Camera camera(width, height, glm::vec3(-2.0f, 1.0f, -2.0f), 90.0f, 0.1f, 100.0f, 0.1f, 0.1f);
	FrameBuffer screen(width, height);

	

	Shader shader("shaders/rect2D.vt", "shaders/SDF.frag", nullptr, false);

	const int N = 3000;
	float* data = (float*)malloc(N*sizeof(float));
	for (int i = 0; i < N; i++) {
		data[i] = float(rand() % 10000) / 10000.0f;
	}

	Buffer SSBO(GL_SHADER_STORAGE_BUFFER, N*sizeof(float), data, GL_STATIC_READ);
	SSBO.bind();
	SSBO.bindTo(3);
	SSBO.unbind();

	free(data);


	Texture tex("textures/test.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, true);
	shader.use();
	shader.setInt("tex", 0);
	
	Mesh rect;
	rect.createRect2D();

	//control.depthTest(true);
	//control.cull(GL_BACK);
	//control.wireframe(true);
	control.showMouse(false);
	while (!control.shouldQuit()) {
		control.update();
		camera.update();
		screen.use();
		screen.clear();

		GLuint64 startTime, stopTime;
		unsigned int queryID[2];

		
		//////////////////////////////////////////////////////////////////////////TIMER
		glGenQueries(2, queryID);
		glQueryCounter(queryID[0], GL_TIMESTAMP);
		//////////////////////////////////////////////////////////////////////////TIMER
		

		shader.use();
		shader.setVec3("POS", camera.getPosition());
		shader.setVec3("DIR", camera.getDir());
		shader.setVec3("U", camera.getU());
		shader.setVec3("V", camera.getV());
		shader.setFloat("TIME", control.getTime());

		tex.use(0);
		rect.draw();

		
		//////////////////////////////////////////////////////////////////////////TIMER
		glQueryCounter(queryID[1], GL_TIMESTAMP);
		GLint stopTimerAvailable = 0;
		while (!stopTimerAvailable) {
		glGetQueryObjectiv(queryID[1],
		GL_QUERY_RESULT_AVAILABLE,
		&stopTimerAvailable);
		}
		glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &startTime);
		glGetQueryObjectui64v(queryID[1], GL_QUERY_RESULT, &stopTime);
		GLfloat deltaTime = (stopTime - startTime) / 1000000.0;
		GLfloat FPS = 1000.0 / deltaTime;
		printf("Time spent on the GPU: %f ms  FPS: %f \n", deltaTime, FPS);
		//////////////////////////////////////////////////////////////////////////TIMER
		
		control.swapBuffers();
	}
	return 0;
}