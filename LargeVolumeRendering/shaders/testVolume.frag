#version 430 core

in vec2 TexCoords;
out vec4 color;

uniform sampler3D volume;

uniform float TIME;

void main() {
	
	float val = texture(volume, vec3(TexCoords,sin(TIME*0.1))).r;



	float val2 = sin(val*200.0 + TIME*5.0);

	color = (val < 0.0)? vec4(val2,0.0,0.0,1.0) : vec4(val2,val2,val2,1.0);
}