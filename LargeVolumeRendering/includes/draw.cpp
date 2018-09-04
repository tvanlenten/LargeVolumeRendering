#include "OpenGL\OpenGL.h"
#include <iostream>
#include "SDF/signedDistanceFields.h"
#include <chrono>

//#define width 1280
//#define height 720

#define width 1920
#define height 1080

/*
Excluded from project for now...
CPU rendering of SDF's is very expensive and compute shaders would obviously be better suited
*/

int main() {
	Control control(width, height, "OpenGL Example", false);
	Camera camera(width, height, glm::vec3(-2.0f, 1.0f, -2.0f), 90.0f, 0.1f, 100.0f, 0.1f, 0.1f);
	FrameBuffer screen(width, height);

	
	Texture tex("textures/moon.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, false);

	int N = 16;
	

	auto start = std::chrono::high_resolution_clock::now();

	float *volume_data = (float*)malloc(sizeof(float) * N * N * N);
	int count = genSDFvolume(volume_data, N, N, N, glm::vec3(0.0, -6.8, 0.0), glm::vec3(15.0));

	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << " ms" << std::endl;

	printf("%d active voxels\n", count);
	Texture volume(GL_R32F, GL_RED, GL_FLOAT, N, N, N, GL_CLAMP_TO_EDGE, GL_LINEAR, volume_data, false);
	free(volume_data);

	Shader shader("shaders/posUV3d.vt", "shaders/volumeRender.frag", nullptr, false);
	shader.use();
	shader.setFloat("MAX_DIST", 50.0f);
	shader.setFloat("PRECIS", 0.005f);
	shader.setInt("MAX_RAY_STEPS", 256);
	shader.setInt("tex", 2);
	shader.setInt("volume", 0);

	Mesh cube;
	cube.createCube();
	cube.setInstanceSize(4096);
	
	
	Shader debug("shaders/rect2D.vt", "shaders/testVolume.frag", nullptr, false);
	Mesh rect;
	rect.createRect2D();
	debug.use();
	debug.setInt("volume", 0);
	


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

		/*
		//////////////////////////////////////////////////////////////////////////TIMER
		glGenQueries(2, queryID);
		glQueryCounter(queryID[0], GL_TIMESTAMP);
		//////////////////////////////////////////////////////////////////////////TIMER
		*/

		
		debug.use();
		volume.use(0);
		debug.setFloat("TIME", glfwGetTime());
		rect.draw();
		

		
		control.wireframe(false);

		shader.use();
		shader.setMat4("view", camera.getViewMatrix());
		shader.setMat4("projection", camera.getProjMatrix());
		shader.setVec3("camera", camera.getPosition());
		shader.setBool("wireframe", false);


		tex.use(2);
		volume.use(0);

		cube.draw();
		
		
		/*
		control.wireframe(true);
		shader.use();
		shader.setMat4("view", camera.getViewMatrix());
		shader.setMat4("projection", camera.getProjMatrix());
		shader.setVec3("camera", camera.getPosition());
		shader.setBool("wireframe", true);


		tex.use(0);
		volume.use(1);
		cube.draw();
		*/


		/*
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
		*/


		control.swapBuffers();
	}
	return 0;
}