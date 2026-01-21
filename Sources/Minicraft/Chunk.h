#ifndef CHUNK_H
#define CHUNK_H

#include "Minicraft/Cube.h"

class Chunk
{
public:
	int const CHUNK_SIZE{ 16 };
	int const CHINK_HEIGHT{ 384 };

	Chunk() = default;

	void generate();

private:
	std::vector<Vector3> cubes;

};

#endif
