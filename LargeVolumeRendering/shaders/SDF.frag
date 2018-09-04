#version 430 core

#define ERROR 1.0
#define PRECIS 0.001
#define MAX_RAY_STEPS 128

in vec2 TexCoords;
out vec4 color;

uniform sampler2D tex;

uniform vec3 POS;
uniform vec3 DIR;
uniform vec3 U;
uniform vec3 V;
uniform float TIME;

layout(std430, binding = 3) buffer object_data {
	vec3 data[];
};


///////////////////////////SHADERTOY SDF functions////////////////////////////////////////////////////
float sdPlane(vec3 p, vec4 n ) {
  return dot(p,n.xyz) + n.w;
}
float sdSphere(vec3 p, float r) {
	return length(p) - r;
}
float sdEllipsoid( in vec3 p, in vec3 c, in vec3 r ) {
    return (length( (p-c)/r ) - 1.0) * min(min(r.x,r.y),r.z);
}
//http://research.microsoft.com/en-us/um/people/hoppe/ravg.pdf
float det( vec2 a, vec2 b ) { return a.x*b.y-b.x*a.y; }
vec3 getClosest( vec2 b0, vec2 b1, vec2 b2 ) {
	
  float a =     det(b0,b2);
  float b = 2.0*det(b1,b0);
  float d = 2.0*det(b2,b1);
  float f = b*d - a*a;
  vec2  d21 = b2-b1;
  vec2  d10 = b1-b0;
  vec2  d20 = b2-b0;
  vec2  gf = 2.0*(b*d21+d*d10+a*d20); gf = vec2(gf.y,-gf.x);
  vec2  pp = -f*gf/dot(gf,gf);
  vec2  d0p = b0-pp;
  float ap = det(d0p,d20);
  float bp = 2.0*det(d10,d0p);
  float t = clamp( (ap+bp)/(2.0*a+b+d), 0.0 ,1.0 );
  return vec3( mix(mix(b0,b1,t), mix(b1,b2,t),t), t );
}
vec2 sdBezier( vec3 a, vec3 b, vec3 c, vec3 p, out vec2 pos ) {
	vec3 w = normalize( cross( c-b, a-b ) );
	vec3 u = normalize( c-b );
	vec3 v = normalize( cross( w, u ) );

	vec2 a2 = vec2( dot(a-b,u), dot(a-b,v) );
	vec2 b2 = vec2( 0.0 );
	vec2 c2 = vec2( dot(c-b,u), dot(c-b,v) );
	vec3 p3 = vec3( dot(p-b,u), dot(p-b,v), dot(p-b,w) );

	vec3 cp = getClosest( a2-p3.xy, b2-p3.xy, c2-p3.xy );

    pos = cp.xy;
    
    float z = 0.0;//1.0 - cp.z;
    
	return vec2( sqrt(dot(cp.xy,cp.xy)+p3.z*p3.z) - (z*z)*0.1 - 0.01, cp.z );
}
vec3 getBezier(vec3 a, vec3 b, vec3 c, float t) {
    float s = 1.0 - t;
    return s*(s*a+t*b) + t*(s*b+t*c);;
}
vec4 texCube(in sampler2D tex, in vec3 p, in vec3 n ) {
	vec4 x = texture( tex, p.yz );
	vec4 y = texture( tex, p.zx ); 
	vec4 z = texture( tex, p.xy );
	return x*abs(n.x) + y*abs(n.y) + z*abs(n.z);
}
// smooth min function iq
float smin( float a, float b ) {
     const float k = 0.08;
     float h = clamp( 0.5 + 0.5*(b-a)/k, 0.0, 1.0 );
     return mix( b, a, h ) - k*h*(1.0-h);
}
///////////////////////////SHADERTOY SDF functions////////////////////////////////////////////////////


float map(vec3 p) {
	
	float d = sdPlane(p,vec4(0.0,1.0,0.0,0.0));

	for(int i = 0; i < 32; i+=3) {
		//d = smin(d, sdSphere(p - data[i], 0.1));
		vec2 uv;
		d = smin(d, sdBezier(vec3(0.0), data[i+1]*5.0, data[i+2]*5.0, p, uv).x);
	}

	return d;
}


//////////////////////RAY MARCHING////////////////////////////////////
vec3 calcNormal(in vec3 pos ) {
	vec2 eps = vec2( 0.001, 0.0 );
	vec3 nor = vec3( map(pos+eps.xyy).x - map(pos-eps.xyy).x,
					 map(pos+eps.yxy).x - map(pos-eps.yxy).x,
    				 map(pos+eps.yyx).x - map(pos-eps.yyx).x);
	return normalize(nor);
}
float castRay(in vec3 ro, in vec3 rd, in float tmin, in float tmax) {
    float t = tmin;
    for( int i=0; i<MAX_RAY_STEPS; i++ ) {
	    float tempT = map( ro+rd*t);
        if( tempT<PRECIS || t>tmax ) break;
        t += tempT*ERROR;
    }
	return (t>tmax)? -1.0 : t;
}
//////////////////////RAY MARCHING////////////////////////////////////

void main() {
	vec3 ro = POS;
	vec2 cam = 1.0 - TexCoords*2.0;
	vec3 rd = normalize(DIR + -U*cam.x + V*cam.y);

	vec3 light = normalize(vec3(0.6,0.8,0.0));

	float t = castRay(ro, rd, 0.01, 20.0);
	vec3 col = rd;
	if(t > 0.0){ //simple phong shading
		vec3 hit = ro + rd*t;
		vec3 norm = calcNormal(hit);
        vec3 ambient = texCube(tex, hit, norm).rgb;
        float diff = max(dot(norm, light), 0.0);
        vec3 diffuse = (diff * ambient);
        vec3 refDir = reflect(light, norm);
        float spec = pow(max(dot(rd, refDir),0.0), 32.0);
        vec3 specular = (spec * vec3(0.5));
        col = ambient*0.1 + diffuse*0.9 + specular;
    }

	color = vec4(col, 1.0);
}