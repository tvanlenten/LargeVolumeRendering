#version 430 core

#define ERROR 0.01

in vec2 uv;
in vec3 uvw;
in vec4 ChunkCoord;
in vec4 MemoryCoord;
out vec4 color;


uniform vec3 camera;
uniform float MAX_DIST;
uniform float PRECIS;
uniform int MAX_RAY_STEPS;
uniform bool wireframe;

uniform sampler2D tex;
uniform sampler3D volume;


float min3(vec3 x) {
	return min(min(x.x,x.y),x.z);
}

float max3(vec3 x) {
	return max(max(x.x,x.y),x.z);
}

float sdSphere(vec3 p, float r) {
	return length(p) - r;
}

float sdTorus( vec3 p, vec2 t )
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

float map(vec3 p) {
	return texture(volume, p).r;//min( sdTorus(p - vec3(0.5), vec2(0.3,0.1)), sdSphere(p - vec3(0.5), 0.05));
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

vec4 texCube(in vec3 p, in vec3 n )
{
	vec4 x = texture( tex, p.yz );
	vec4 y = texture( tex, p.zx ); 
	vec4 z = texture( tex, p.xy );
	return x*abs(n.x) + y*abs(n.y) + z*abs(n.z);
}

void main() {
	//SET UP RAY

	float scale = MemoryCoord.w / ChunkCoord.w;

	vec3 ro = camera;
	vec3 rd = normalize(uvw*ChunkCoord.w + ChunkCoord.xyz - camera);
	vec3 rs = sign(rd);
	vec3 ri = 1.0 / rd;
	vec3 entry = -rs * 0.5 + 0.5;
	float tMin = max( max3( (entry*ChunkCoord.w + ChunkCoord.xyz - ro) * ri), 0.0);
	if(tMin > MAX_DIST)discard;
	vec3 exit = rs * 0.5 + 0.5;
	float tMax = min3( (exit*ChunkCoord.w + ChunkCoord.xyz - ro) * ri);
	

	//Convert to Chunk Space

	//local ro,tMin,tMax
	ro = (camera - ChunkCoord.xyz) * scale + MemoryCoord.xyz;
	tMin = tMin * scale;
	tMax = tMax * scale;


	//VISUAL REMOVE
	//if(map(MemoryCoord.xyz + MemoryCoord.w*0.5) < 0.05) {
		if(!wireframe){
			//CAST RAY
			float t = castRay(ro, rd, tMin, tMax);
			if(t > 0.0) {
		
				vec3 hit = ro + rd*t;
				vec3 norm = calcNormal(hit);

				vec3 light = normalize(vec3(0.1243,0.10123,-0.2482));
				float diff = max( dot(light,norm), 0.0 );

				float val = map(MemoryCoord.xyz + vec3(MemoryCoord.w*0.5));
				val = (val < 3.0)? 1.0 : 0.0;


				vec3 col = vec3(val) + 0.1;//texCube(hit,norm).rgb;
				//vec3 col = sin(ChunkCoord.xyz*5000.0) * 0.5 + 0.5;
				//col = mix(col, texCube(hit,norm).rgb, 0.5);
				col = col*diff*0.8 + col*0.2;

				

				color = vec4(col, 1.0);


			} else {
				discard;
			}
		} else {
			color = vec4(1.0);
		}
	//} else {
		//discard;
	
	//}
}