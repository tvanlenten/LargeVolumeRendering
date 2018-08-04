#ifndef CONTROL_H_
#define CONTROL_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Input.h"


class Control {
public:
	//This should be implimented to throw a exception if it cannont be initialized
	//Creates a GLFW window and initializes the keyboard and mouse
	Control(unsigned int w, unsigned int h, std::string name, bool fullscreen) {
		//INIT GLFW
		width = w;
		height = h;
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


		//Create GLFW window
		window = (fullscreen) ?
			glfwCreateWindow(width, height, name.c_str(), glfwGetPrimaryMonitor(), nullptr) :
			glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

		if (window == nullptr) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}
		glfwMakeContextCurrent(window);

		//INIT GLEW
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			std::cout << "Failed to initialize GLEW" << std::endl;
		}

		prevX = float(width) / 2.0;
		prevY = float(height) / 2.0;

		//INIT input
		initInput(window);
	}

	bool shouldQuit() { return glfwWindowShouldClose(window); }
	void update() { glfwPollEvents(); }
	void swapBuffers() { glfwSwapBuffers(window); }
	void showMouse(bool state) {
		if (state)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	void depthTest(bool state) { 
		if (state)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}
	//only counter clockwise
	//use GL_FRONT or GL_BACK or false
	void cull(int state) {
		switch (state) {
		case GL_FRONT:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glFrontFace(GL_CCW);
			break;
		case GL_BACK:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			break;
		default:
			glDisable(GL_CULL_FACE);
			break;
		}
	}
	void wireframe(bool state) {
		if (state)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
	}

	~Control() { glfwTerminate(); }
private:
	GLFWwindow* window;
	unsigned int width;
	unsigned int height;
};

#endif // !CONTROL_H_