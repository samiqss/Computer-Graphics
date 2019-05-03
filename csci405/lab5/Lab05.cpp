#include <math.h>
#include <string>

#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#include "mathvector.h"
#include "colour.h"

#include <iostream>

// Specify the window dimensions
#define SCREENWIDTH 512
#define SCREENHEIGHT 512
Using pixel coordinates (x, y):
static CRGB gsPixelBuffer[SCREENWIDTH*SCREENHEIGHT];

void ClearImage()
{
	for( int x = 0; x < SCREENWIDTH; x++ )
	{
		for( int y = 0; y < SCREENHEIGHT; y++ )
		{	
			int bufferInd = y*SCREENWIDTH+x;
			gsPixelBuffer[ bufferInd ] = CRGB(0, 0, 0);
		}
	}
}

void RenderImage()
{
	CRGB WHITE= CRGB(255, 255, 255);
	CRGB BLACK= CRGB(0,0,0);
	for( int x = 0; x < SCREENWIDTH; x++ )
	{
		for( int y = 0; y < SCREENHEIGHT; y++ )
		{	
			int bufferInd = y*SCREENWIDTH+x;			
			float RelX = (float)(x - SCREENWIDTH/2) / (float)(SCREENWIDTH/2);
			float RelY = (float)(y - SCREENHEIGHT/2) / (float)(SCREENHEIGHT/2);

			// Set the pixel buffer here
			
			int bandwidth = 30;		
			int xband = x % (2*bandwidth);
			if( xband < bandwidth ) gsPixelBuffer[xband] = WHITE;
	
			else gsPixelBuffer[xband] = BLACK;

			/*
			int xband = ((x % (2*bandwidth)) < bandwidth)?1:-1;
			int yband = ((y % (2*bandwidth)) < bandwidth)?1:-1;

			int mag = 255*clamp( xband^yband, 0, 1 );
			pixelcolour[x,y] = CRGB(mag, mag, mag);
			*/
		}
	}
}

// GLUT callback to handle rendering the scene
void renderScene(void) 
{
	glDrawPixels( SCREENWIDTH, SCREENHEIGHT,  GL_RGB, GL_UNSIGNED_BYTE, gsPixelBuffer );
	
	glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y) 
{
	if( 'r' == key )
	{
		ClearImage();
		RenderImage();
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);

	glutInitWindowPosition(100,100);
	glutInitWindowSize( SCREENWIDTH, SCREENHEIGHT );
	glutCreateWindow("Ray-Tracing");

	glViewport( 0, 0, SCREENWIDTH, SCREENHEIGHT );

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutKeyboardFunc( keyboard );

	//  GLUT main loop
	glutMainLoop();

	return(0);
}

