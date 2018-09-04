#pragma once

//#include "SDF/signedDistanceFields.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct Chunk {
	size_t res;
	glm::vec3 chunkCoord;
	glm::vec3 memCoord;
	float *data;
};

