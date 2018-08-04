#version 430 core

in vec2 TexCoords;
in float Val;
out vec4 color;

uniform int sel;

void main() {

	if(sel == 1) {
		color = vec4(1.0,0.0,0.0,1.0);
	} else {
		color = vec4(TexCoords,0.0,1.0);
	}
}