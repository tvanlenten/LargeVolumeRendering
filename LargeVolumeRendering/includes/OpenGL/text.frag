#version 430 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D tex;

uniform vec3 texCol;

void main() {
	
	float min = 0.50;
	float max = 0.51;

	
	float dist = texture(tex, TexCoords).r;
	float val = smoothstep(min,max,dist);
	if(dist > max)
		discard;
	else
		color = vec4(texCol,1.0);
	
}