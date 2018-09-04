#ifndef OPENGL_H_
#define OPENGL_H_

#include "Control.h"
#include "Camera.h"
#include "Mesh.h"
#include "Buffer.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "Text.h"
#include "Slider.h"

/*
	By Tyler Van Lenten
	please note that many aspects of this program were taken from various openGL tutorials
	this is my attempt to organize openGL for my purposes and likely will not be useful for much else
	last updated: 12/03/17

	has nearly all basic openGL support including
	-FBOs(FrameBuffers)
	-buffers of all types
	-Mesh(VAO w/ extra features)
	-Text(simple SD field text that is uniformly spaced)
	-support for all 4 types of shaders
	-Has a standard 3D camera that can also be used for ray casting
	-Also has support for keyboard, mouse movement, and mouse scroll

	TO DO:
	-add support for mouse buttons
	
*/


//EXAMPLE PROGRAM//
/*
	#include "OpenGL\OpenGL.h"
	#include <iostream>

	#define width 1280
	#define height 720

	int main() {
		Control control(width, height, "OpenGL Example", false);
		Camera camera(width, height, glm::vec3(-2.0f, 1.0f, -2.0f), 90.0f, 0.1f, 100.0f, 0.1f, 0.1f);
		FrameBuffer screen(width,height);
		Text text(width, height);


		Mesh rect;
		rect.createRect2D();
		Shader shader("shaders/texture2D.vt", "shaders/texture2D.frag", nullptr, false);
		Texture tex("textures/test.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, true);
		shader.setInt("text", 0);

		//control.depthTest(true);
		//control.cull(GL_BACK);
		//control.wireframe(true);

		while (!control.shouldQuit()) {
			control.update();
			camera.update();

			screen.use();
			screen.clear();

			shader.use();
			tex.use(0);
			rect.draw();

			std::string example = "OpenGL Example";
			text.draw(example.c_str(), example.length(), glm::vec2(0.0,0.0), 0.1, 0.5, glm::vec3(1.0,0.0,0.0));

			control.swapBuffers();
		}
		return 0;
	}
*/

#endif //OpenGL.h