#include "OpenGL\OpenGL.h"
#include <iostream>

//#define width 1280
//#define height 720

#define width 1920
#define height 1080

int main() {
	Control control(width, height, "OpenGL Example", false);
	Camera camera(width, height, glm::vec3(-2.0f, 1.0f, -2.0f), 90.0f, 0.1f, 100.0f, 0.1f, 0.1f);
	FrameBuffer screen(width, height);


	Texture tex("textures/moon.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, true);
	Shader shader("shaders/posUV3d.vt", "shaders/rayRasterBackface.frag", nullptr, false);
	shader.use();
	shader.setFloat("MAX_DIST", 50.0f);
	shader.setFloat("PRECIS", 0.005f);
	shader.setInt("MAX_RAY_STEPS", 20);
	shader.setInt("tex", 0);

	Mesh cube;
	cube.createCube();
	cube.setInstanceSize(4096);


	control.depthTest(true);
	control.cull(GL_BACK);
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

		control.wireframe(false);
		shader.use();
		shader.setMat4("view", camera.getViewMatrix());
		shader.setMat4("projection", camera.getProjMatrix());
		shader.setVec3("camera", camera.getPosition());
		shader.setBool("wireframe", false);

		tex.use(0);
		cube.draw();

		
		/*
		control.wireframe(true);
		shader.use();
		shader.setMat4("view", camera.getViewMatrix());
		shader.setMat4("projection", camera.getProjMatrix());
		shader.setVec3("camera", camera.getPosition());
		shader.setBool("wireframe", true);


		tex.use(0);
		cube.draw();
		*/


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