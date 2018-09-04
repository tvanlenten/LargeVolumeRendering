#pragma once

#ifndef SIGNED_DISTANCE_FIELDS_H_
#define SIGNED_DISTANCE_FIELDS_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


struct Chunk {
	size_t res;
	glm::vec3 chunkCoord;
	glm::vec3 memCoord;
	float *data;
};

void destroy_chunk(Chunk *chunk) {
	delete [] chunk->data;
	delete chunk;
}

float hash12(glm::vec2 p) {
	float h = glm::dot(p, glm::vec2(127.1f, 311.7f));
	return glm::fract(sin(h) * 437.545f);
}

float noise(glm::vec3 p) {
	glm::vec3 i = floor(p);
	glm::vec3 f = fract(p);
	glm::vec3 u = f*f*(glm::vec3(3.0f) - glm::vec3(2.0f) * f);
	glm::vec2 ii = glm::vec2(i.x, i.y) + i.z * glm::vec2(5.0f);
	float a = hash12(ii + glm::vec2(0.0f, 0.0f));
	float b = hash12(ii + glm::vec2(1.0f, 0.0f));
	float c = hash12(ii + glm::vec2(0.0f, 1.0f));
	float d = hash12(ii + glm::vec2(1.0f, 1.0f));
	float v1 = glm::mix(glm::mix(a, b, u.x), glm::mix(c, d, u.x), u.y);
	ii += glm::vec2(5.0f);
	a = hash12(ii + glm::vec2(0.0f, 0.0f));
	b = hash12(ii + glm::vec2(1.0f, 0.0f));
	c = hash12(ii + glm::vec2(0.0f, 1.0f));
	d = hash12(ii + glm::vec2(1.0f, 1.0f));
	float v2 = glm::mix(glm::mix(a, b, u.x), glm::mix(c, d, u.x), u.y);
	return glm::max(glm::mix(v1, v2, u.z), 0.0f);
}

float sdSphere(glm::vec3 p, float s) {
	return length(p) - s;
}

float opU(float a, float b) {
	return glm::min(a, b);
}


float dis(glm::vec3 ws) {
	glm::vec3 ws2 = ws;
	ws += noise(ws*0.4f) * 4.0f;

	ws2 += noise(ws*0.4f) * 8.0f;

	float density = 0.0f;

	density += noise(ws*4.01f)*0.25f;
	if (density > 0.25)return density;
	density += noise(ws*1.96f)*0.50f;
	density += noise(ws*1.01f)*1.00f;
	density += noise(ws*0.56f)*2.00f;
	density += noise(ws*0.25f)*4.00f;
	density += noise(ws*0.12f)*8.00f;
	density += noise(ws*0.08f)*16.00f;

	return density;
}

float map(glm::vec3 p) {
	return p.y*3.0f + dis(p);
}


/*
	Must be given the malloced memory to write in
	TODO: add function pointer for SDF genorator
*/
int genSDFvolume(float *volume, int x_size, int y_size, int z_size, glm::vec3 p, glm::vec3 w) {
	int size = x_size*y_size*z_size;
	w = glm::vec3(w.x / float(x_size), w.y / float(y_size), w.z / float(z_size));

	int count = 0;
	for (int id = 0; id < size; id++) {
		glm::vec3 coord = glm::vec3(id%x_size, (id / x_size) % y_size, id / (x_size*y_size));
		coord = p + coord*w + w*0.5f;
		volume[id] = map(coord);
		if (volume[id] < 3.0)
			count++;
	}

	return count;
}





std::vector<Chunk> genChunkPool(int chunk_pool_size, glm::vec3 p, glm::vec3 w) {

	int size = chunk_pool_size * chunk_pool_size * chunk_pool_size;

	float T = 3.0f;

	std::vector<Chunk> chunk_pool;

	for (int id = 0; id < size; id++) {
		glm::vec3 chunkCoord = glm::vec3(id % chunk_pool_size, (id / chunk_pool_size) % chunk_pool_size, id / (chunk_pool_size*chunk_pool_size))*w + p;
		float sdf = map(chunkCoord + glm::vec3(w*0.5f));
		if (sdf < T) {
			Chunk chunk;
			chunk.res = 34;
			chunk.data = (float*)malloc(sizeof(float)*34*34*34);
			chunk.chunkCoord = chunkCoord;

			glm::vec3 chunkSize = w / float(chunk_pool_size);
			glm::vec3 realVoxelSize = chunkSize / 32.0f; //offset for tri-linear interpolation


			//fix! must be offset by buffer
			chunk.memCoord = glm::vec3(chunk_pool.size() % chunk_pool_size, (chunk_pool.size() / chunk_pool_size) % chunk_pool_size, chunk_pool.size() / (chunk_pool_size*chunk_pool_size))*w;
			int count = genSDFvolume(chunk.data, 34, 34, 34, chunkCoord - realVoxelSize, glm::vec3(chunkSize) + 2.0f * realVoxelSize);
			chunk_pool.push_back(chunk);
		}
	}

	return chunk_pool;
}






#endif //SignedDistanceFields.h