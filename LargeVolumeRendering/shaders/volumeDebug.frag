#version 430 core

#define ERROR 1.0

in vec2 TexCoords;
out vec4 color;


uniform vec3 POS;
uniform vec3 U;
uniform vec3 V;
unifrom vec3 DIR;

uniform float MAX_DIST;
uniform float PRECIS;
uniform int MAX_RAY_STEPS;

uniform sampler3D volume;


float min3(vec3 x) {
	return min(min(x.x,x.y),x.z);
}

float max3(vec3 x) {
	return max(max(x.x,x.y),x.z);
}

float map(vec3 p) {
	return texture(volume, p).r - 0.4;
}

float castRay( vec3 ro, vec3 rd, float tmin, float tmax) {
    float t = tmin;
    for( int i=0; i < MAX_RAY_STEPS; i++ ) {

		//float p = PRECIS*t*t;

	    float tempT = map( ro+rd*t );
        if( tempT < PRECIS || t > tmax ) break;
        t += tempT * ERROR;
    }

	return (t>tmax)? -1.0 : t;
}

vec3 calcNormal( in vec3 pos )
{
	vec2 eps = vec2( 0.001, 0.0 );

	vec3 nor = vec3( map(pos+eps.xyy).x - map(pos-eps.xyy).x,
					 map(pos+eps.yxy).x - map(pos-eps.yxy).x,
    				 map(pos+eps.yyx).x - map(pos-eps.yyx).x);
	return normalize(nor);
}


void main() {
	vec3 ro = POS;
	vec2 cam = 2.0 * TexCoords - 1.0;
	vec3 rd = normalize(DIR + U*cam.x + V*cam.y);

	float t = castRay(ro, rd, 0.0, MAX_DIST);
	
	color = vec4(rd, 1.0);
}