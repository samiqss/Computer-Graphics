#include <math.h>
#include <string>

#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#include <iostream>

// Specify the window dimensions
#define SCREENWIDTH 512
#define SCREENHEIGHT 512

// Coordinates from:
// https://github.com/sgonzalez/OpenGL-Platonic-Solids/blob/master/Classes/Icosahedron.h
static GLfloat gsVertexPositions[] = {0.000000f, -0.525731f, 0.850651f,			// 0
										0.850651f, 0.000000f, 0.525731f,		// 1
										0.850651f, 0.000000f, -0.525731f,		// 2
										-0.850651f, 0.000000f, -0.525731f,		// 3
										-0.850651f, 0.000000f, 0.525731f,		// 4
										-0.525731f, 0.850651f, 0.000000f,		// 5
										0.525731f, 0.850651f, 0.000000f,		// 6
										0.525731f, -0.850651f, 0.000000f,		// 7
										-0.525731f, -0.850651f, 0.000000f,		// 8
										0.000000f, -0.525731f, -0.850651f,		// 9
										0.000000f, 0.525731f, -0.850651f,		// 10
										0.000000f, 0.525731f, 0.850651f    };	// 11

static GLfloat gsVertexNormals[] = {
										0.000000, -0.417775, 0.675974,			// 0
										0.675973, 0.000000, 0.417775,			// 1
										0.675973, -0.000000, -0.417775,			// 2
										-0.675973, 0.000000, -0.417775,			// 3
										-0.675973, -0.000000, 0.417775,			// 4
										-0.417775, 0.675974, 0.000000,			// 5
										0.417775, 0.675973, -0.000000,			// 6
										0.417775, -0.675974, 0.000000,			// 7
										-0.417775, -0.675974, 0.000000,			// 8
										0.000000, -0.417775, -0.675973,			// 9
										0.000000, 0.417775, -0.675974,			// 10
										0.000000, 0.417775, 0.675973,			// 11
};

static GLubyte gsIndices[] = {
		1, 2, 6,
		1, 7, 2,
		3, 4, 5,
		4, 3, 8,
		6, 5, 11,
		5, 6, 10,
		9, 10, 2,
		10, 9, 3,
		7, 8, 9,
		8, 7, 0,
		11, 0, 1,
		0, 11, 4,
		6, 2, 10,
		1, 6, 11,
		3, 5, 10,
		5, 4, 11,
		2, 7, 9,
		7, 1, 0,
		3, 9, 8,
		4, 8, 0 };

// Light colours
GLfloat gsLightSpecularColour[] = {1.0, 1.0, 1.0, 1.0};
GLfloat gsLightDiffuseColour[] = {1.0, 0.0, 0.0, 1.0};
GLfloat gsLightAmbientColour[] = {0.0, 0.0, 0.1, 1.0};
GLfloat gsLightPosition[] = {0.0, 10.0, 10.0, 0.0};

void initializeScene()
{

	// Put light setup code here
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, gsLightSpecularColour);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gsLightDiffuseColour);
	glLightfv(GL_LIGHT0, GL_AMBIENT, gsLightAmbientColour);
	glLightfv(GL_LIGHT0, GL_POSITION, gsLightPosition);	

	glShadeModel(GL_FLAT );
	// glShadeModel( GL_SMOOTH );

  	glEnable(GL_DEPTH_TEST);
  	glEnable(GL_NORMALIZE);
}

// GLUT callback to handle rendering the scene
void renderScene(void)
{
	static float sAccum = 0.0f;
	sAccum += 1.0f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,   // FOV y
					1.0f, 	// Aspect ratio (width/height)
					0.1f, 	// Near distance
					1000.0f);// Far distance



	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt( 0.0f, 0.0f, -5.0f,    // Eye position
				0.0f, 0.0f, 0.0f, 	  // Lookat position
  				0.0f, 1.0f, 0.0f );	  // Up vector

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glPushMatrix();
	{
		glRotatef( sAccum, 0.0f, 1.0f, 0.0f );

		// Put indexed draw calls here
		// Enable the vertex position array and bind it to our vertex positions
		glEnableClientState(GL_NORMAL_ARRAY);				// Enable the normal buffer
		glNormalPointer(GL_FLOAT, 0, gsVertexNormals);		// Supply an array of normals
		
		// Enable the vertex position array and bind it to our vertex positions
		glEnableClientState(GL_VERTEX_ARRAY);				// Enable the vertex buffer
		glVertexPointer(3, GL_FLOAT, 0, gsVertexPositions);	
		// Supply an array of vertices
	        // Draw using the vertex buffers and the supplied ind
		glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_BYTE, gsIndices);
		//
	}
	glPopMatrix();
	glutSwapBuffers();
}



int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_SINGLE|GLUT_RGB);

	glutInitWindowPosition(100,100);
	glutInitWindowSize( SCREENWIDTH, SCREENHEIGHT );
	glutCreateWindow("Texture Loading Example");

	glViewport( 0, 0, SCREENWIDTH, SCREENHEIGHT );

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	initializeScene();

	//  GLUT main loop
	glutMainLoop();

	return(0);
}

