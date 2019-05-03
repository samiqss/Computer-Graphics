#ifndef OBJLOADER_H__
#define OBJLOADER_H__

#include <stdlib.h>
#include <string>
#include "mathvector.h"

bool LoadOBJ( const std::string &Name, const std::string &Path, float* &VertexPositions, float* &VertexNormals, float* &VertexUVs, int** Indices, 
				int &NumVertexPositions, int &NumVertexNormals, int &NumVertexUVs, int *NumIndices );

#endif // OBJLOADER_H__
