#ifndef SIGNED_DISTANCE_FIELDS_H_
#define SIGNED_DISTANCE_FIELDS_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float hash12(glm::vec2 p) {
	float h = glm::dot(p, glm::vec2(127.1, 311.7));
	return glm::fract(sin(h) * 437.545);
}

float noise(glm::vec3 p) {
	glm::vec3 i = floor(p);
	glm::vec3 f = fract(p);
	glm::vec3 u = f*f*(glm::vec3(3.0) - glm::vec3(2.0) * f);
	glm::vec2 ii = glm::vec2(i.x, i.y) + i.z * glm::vec2(5.0);
	float a = hash12(ii + glm::vec2(0.0, 0.0));
	float b = hash12(ii + glm::vec2(1.0, 0.0));
	float c = hash12(ii + glm::vec2(0.0, 1.0));
	float d = hash12(ii + glm::vec2(1.0, 1.0));
	float v1 = glm::mix(glm::mix(a, b, u.x), glm::mix(c, d, u.x), u.y);
	ii += glm::vec2(5.0);
	a = hash12(ii + glm::vec2(0.0, 0.0));
	b = hash12(ii + glm::vec2(1.0, 0.0));
	c = hash12(ii + glm::vec2(0.0, 1.0));
	d = hash12(ii + glm::vec2(1.0, 1.0));
	float v2 = glm::mix(glm::mix(a, b, u.x), glm::mix(c, d, u.x), u.y);
	return glm::max(glm::mix(v1, v2, u.z), 0.0f);
}

float sdSphere(glm::vec3 p, float s) {
	return length(p) - s;
}

float opU(float a, float b) {
	return glm::min(a, b);
}


#endif //SignedDistanceFields.h