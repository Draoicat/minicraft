#include "pch.h"
#include "Chunk.h"

void Chunk::generate()
{
	for (float i = 0; i < 10; ++i)
		for (float j = 0; j < 10; ++j)
			cubes.push_back({i - 5, 0, j - 5});
}
