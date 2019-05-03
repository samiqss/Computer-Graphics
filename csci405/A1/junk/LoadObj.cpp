#include <fstream>
#include <string.h>
#include "LoadObj.h"

#pragma warning( disable:4996 )

#define _DEBUGINDICES 0

#if _DEBUGINDICES
#include <iostream>
#endif // _DEBUGINDICES

// Checks to see if the given character is considered
// whitespace or not
bool iswhitespace( char c )
{
	return ((' ' == c) || ('\n' == c) || ('\r' == c) );
}

void trimwhitespace( char* &str )
{
	// Trim the whitespace from the beginning
	while( (0 != str[0]) && iswhitespace( str[0] )) 
		str = &(str[1]);

	// Trim the whitespace from the end
	int len = strlen( str );
	while( (0 > len) && iswhitespace( str[len-1] )) 
	{
		str[len-1] = 0;
		len--;
	}
}

int LoadVector( char *vecStr, int vectorWidth, float *vectorValues )
{
	// Trim any whitespace for the provided string
	trimwhitespace( vecStr );

	char *tok = strtok( vecStr, " " );
	for( int i = 0; i < vectorWidth; i++ )
	{
		vectorValues[i] = (float)atof( tok );
		tok = strtok( NULL, " " );
	}

	return 3;
}

void ValidateIntBuffer( int **IndexBuffer, int *BuffIndex )
{
	if( (NULL == IndexBuffer) || (NULL == BuffIndex) ) return;
	if( 0 == (((*BuffIndex)+1)%32) )
	{
		(*IndexBuffer) = (int*)realloc( *IndexBuffer, sizeof(int) * (((*BuffIndex)+1)+32) );
	} 
}

void LoadFaceData( char *faceStr, 
							int **VertexIndexBuffer, int *VBuffIndex, 
							int **NormalIndexBuffer, int *NBuffIndex, 
							int **UVIndexBuffer, int *UVBuffIndex )
{
	// Trim any whitespace for the provided string
	trimwhitespace( faceStr );

	char *vertextok[4] = {NULL};
	vertextok[0] = strtok( faceStr, " " );
	vertextok[1] = strtok( NULL, " " );
	vertextok[2] = strtok( NULL, " " );
	// vertextok[4] = strtok( NULL, " " );

	char *indtok = NULL;
	int delimoffset = 0; 
	for( int i = 0; i < 3; i++ )
	{
		// indtok = strtok( vertextok[i], "/" );
		indtok = vertextok[i];
		delimoffset = strcspn( indtok, "/" );
		indtok[ delimoffset ] = 0;
		if( 0 < strlen( indtok ) )
		{
			if( (NULL != VertexIndexBuffer) && (NULL != VBuffIndex) )
			{
				ValidateIntBuffer( VertexIndexBuffer, VBuffIndex );
				(*VertexIndexBuffer)[ (*VBuffIndex)++ ] = atoi( indtok );
			}
		}

		// indtok = strtok( NULL, "/" );
		indtok = &(indtok[delimoffset+1]); 
		delimoffset = strcspn( indtok, "/" );
		indtok[ delimoffset ] = 0;
		if( (NULL != indtok) && (0 < strlen( indtok )) )
		{
			if( (NULL != UVIndexBuffer) && (NULL != VBuffIndex) )
			{
				ValidateIntBuffer( UVIndexBuffer, UVBuffIndex );
				(*UVIndexBuffer)[ (*UVBuffIndex)++ ] = atoi( indtok );
			}
		}

		// indtok = strtok( NULL, "/" );
		indtok = &(indtok[delimoffset+1]); 
		delimoffset = strcspn( indtok, "/" );
		indtok[ delimoffset ] = 0;
		if( (NULL != indtok) && (0 < strlen( indtok )) )
		{
			if( (NULL != NormalIndexBuffer) && (NULL != NBuffIndex) )
			{
				ValidateIntBuffer( NormalIndexBuffer, NBuffIndex );
				(*NormalIndexBuffer)[ (*NBuffIndex)++ ] = atoi( indtok );
			}
		}
	}
}

bool LoadPosition( char *posStr, float* VertexPositions )
{
	float Position[3] = { 0.0f, 0.0f, 0.0f };

	if( (NULL != VertexPositions) && LoadVector( posStr, 3, Position ) )
	{
		VertexPositions[0] = Position[0];
		VertexPositions[1] = Position[1];
		VertexPositions[2] = Position[2];
		return true;
	}

	return false;
} 

bool LoadNormal( char *normStr, float* VertexNormals )
{
	float Normal[3] = { 0.0f, 0.0f, 0.0f };

	if( (NULL != VertexNormals) && LoadVector( normStr, 3, Normal ) )
	{
		VertexNormals[0] = Normal[0];
		VertexNormals[1] = Normal[1];
		VertexNormals[2] = Normal[2];
		return true;
	}

	return false;
} 

bool LoadTextureCoordinates( char *texStr, float* VertexTexCoords )
{
	float UVs[2] = { 0.0f, 0.0f };

	if( (NULL != VertexTexCoords) && LoadVector( texStr, 2, UVs ) )
	{
		VertexTexCoords[0] = UVs[0];
		VertexTexCoords[1] = UVs[1];
		return true;
	}
    
	return false;
} 

bool LoadOBJTriangles( const std::string &Name, const std::string &Path, float* &VertexPositions, float* &VertexNormals, float* &VertexUVs, 
									int &NumVertexPositions, int &NumVertexNormals, int &NumVertexUVs )
{
	int VIndex = 0;
	int NIndex = 0;
	int UVIndex = 0;

	float *VertexBuffer = (float*)malloc( sizeof( float ) * 3 * 32 );
	float *NormalBuffer = (float*)malloc( sizeof( float ) * 3 * 32 );
	float *UVBuffer = (float*)malloc( sizeof( float ) * 2 * 32 );

	int VBuffIndex = 0;
	int NBuffIndex = 0;
	int UVBuffIndex = 0;

	int *VertexIndexBuffer = (int*)malloc( sizeof( int ) * 32 );
	int *NormalIndexBuffer = (int*)malloc( sizeof( int ) * 32 );
	int *UVIndexBuffer = (int*)malloc( sizeof( int ) * 32 );

	int numEmptyLines = 0;
    	std::string Data;
	char LineBuff[1024];

	std::string FileNameNPath = Path + Name;

	std::ifstream File( FileNameNPath.c_str() );
    	while ( !File.eof() )
    	{
        	getline(File, Data);
		strcpy( LineBuff, Data.c_str() );

		if( 0 == strlen( LineBuff ) )  numEmptyLines++;
		if( 16 <= numEmptyLines ) break;

		if ( ('v' == Data[0]) && ('t' != Data[1]) && ('n' != Data[1]) )
        	{
			VIndex++;
			if( 0 == (VIndex % 32) ) 
			{
				VertexBuffer = (float*)realloc( VertexBuffer, sizeof(float) * 3 * (VIndex+32) );
			}
            		LoadPosition( &(LineBuff[1]), &(VertexBuffer[3*(VIndex-1)]) ); 
       	}
        	else if ( ('v' == Data[0]) && ('n' == Data[1]) )
        	{
			NIndex++;
			if( 0 == (NIndex % 32) ) 
			{
				NormalBuffer = (float*)realloc( NormalBuffer, sizeof(float) * 3 * (NIndex+32) );
			}
        		LoadNormal( &(LineBuff[2]), &(NormalBuffer[3*(NIndex-1)]) );
        	}
        	else if ( ('v' == Data[0]) && ('t' == Data[1]) )
        	{
			UVIndex++;
			if( 0 == (UVIndex % 32) ) 
			{
				UVBuffer = (float*)realloc( UVBuffer, sizeof(float) * 2 * (UVIndex+32) );
			}
			LoadTextureCoordinates( &(LineBuff[2]), &(UVBuffer[2*(UVIndex-1)]) ); 
        	}
        	else if ( 'f' == Data[0] )
        	{
			LoadFaceData( &(LineBuff[1]), 
							&VertexIndexBuffer, &VBuffIndex, 
							&NormalIndexBuffer, &NBuffIndex, 
							&UVIndexBuffer, &UVBuffIndex );
        	}
    	}

	// Must have at least vertices
	if( 0 < VBuffIndex )
	{
		NumVertexPositions = VBuffIndex;
		float *PosArr = new float[ NumVertexPositions*3 ]; 
		VertexPositions = PosArr;

		VertexNormals = NULL;
		NumVertexNormals = NBuffIndex;		
		if( 0 < NumVertexNormals )
		{
			float *NormArr = new float [ NumVertexNormals*3 ];
			VertexNormals = NormArr;
		}

		VertexUVs = NULL;
		NumVertexUVs = UVBuffIndex;
		if( 0 < NumVertexUVs )
		{
			float *UVArr = new float [ NumVertexUVs*2 ];
			VertexUVs = UVArr;
		}
	}

	#if _DEBUGINDICES
	for( int i = 0; i < VBuffIndex; i++ )
	{
		std::cout << "vi-" << i << ": " << VertexIndexBuffer[ i ] - 1 << std::endl;
		std::cout << "ni-" << i << ": " << NormalIndexBuffer[ i ] - 1 << std::endl;
		std::cout << "ti-" << i << ": " << UVIndexBuffer[ i ] - 1 << std::endl;
	}
	#endif // _DEBUGINDICES

	for( int i = 0; i < VBuffIndex; i++ )
	{
		int ind = VertexIndexBuffer[ i ] - 1;
		// Indices are 1-based instead of 0-based
		// subtract 1 to get the buffer index
		VertexPositions[3*i+0] = VertexBuffer[ 3*ind+0 ];
		VertexPositions[3*i+1] = VertexBuffer[ 3*ind+1 ];
		VertexPositions[3*i+2] = VertexBuffer[ 3*ind+2 ];

		if( (0 < NumVertexNormals) && (i < NumVertexNormals) )
		{
			ind = NormalIndexBuffer[ i ] - 1;
        		VertexNormals[3*i+0] = NormalBuffer[ 3*ind+0 ];
			VertexNormals[3*i+1] = NormalBuffer[ 3*ind+1 ];
			VertexNormals[3*i+2] = NormalBuffer[ 3*ind+2 ];

		}
        
		if( 0 < NumVertexUVs )
		{
        		ind = UVIndexBuffer[ i ] - 1;
        		VertexUVs[2*i+0] = UVBuffer[ 2*ind+0 ];
			VertexUVs[2*i+1] = UVBuffer[ 2*ind+1 ];
		}	
	}
	
	return true;
}

bool LoadOBJPrimitives( const std::string &Name, const std::string &Path, float* &VertexPositions, float* &VertexNormals, float* &VertexUVs, int** Indices, 
									int &NumVertexPositions, int &NumVertexNormals, int &NumVertexUVs, int *NumIndices )
{
	return true;
}

bool LoadOBJ( const std::string &Name, const std::string &Path, float* &VertexPositions, float* &VertexNormals, float* &VertexUVs, int** Indices, 
							int &NumVertexPositions, int &NumVertexNormals, int &NumVertexUVs, int *NumIndices )
{
    if( NULL == Indices )
	{
		return LoadOBJTriangles( Name, Path, VertexPositions, VertexNormals, VertexUVs,
						NumVertexPositions, NumVertexNormals, NumVertexUVs );
	}
	else 
	{
		return LoadOBJPrimitives( Name, Path, VertexPositions, VertexNormals, VertexUVs, Indices,
						NumVertexPositions, NumVertexNormals, NumVertexUVs, NumIndices );
	}
	return false;
}
