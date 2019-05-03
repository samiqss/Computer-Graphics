#include <math.h>
#include <string>

#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glut.h"

#include "mathvector.h"
#include "bitmap.h"

#include <iostream>

// Specify the window dimensions
#define SCREENWIDTH 512
#define SCREENHEIGHT 512

static GLuint textureHandle = 0;

GLuint LoadImage(const std::string &ImageName, const std::string &ImagePath)
{
	std::string FileNameNPath = ImagePath + ImageName;
	CBitmap *Bitmap = new CBitmap(FileNameNPath.c_str());

	GLuint img = 0;
	if (NULL != Bitmap)
	{
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);

		glGenTextures(1, &img);
		glBindTexture(GL_TEXTURE_2D, img);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

		int imageWidth = Bitmap->GetWidth();
		int imageHeight = Bitmap->GetHeight();
		void *imageData = Bitmap->GetBits();

		GLenum errEnum = glGetError();

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		errEnum = glGetError();
		if (GL_NO_ERROR != errEnum)
		{
			printf("An error occured: %x\n", errEnum);
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		delete Bitmap;
	}

	return img;
}

void initializeScene()
{	
		textureHandle = LoadImage("LAND.BMP", "");
}

// GLUT callback to handle rendering the scene
void renderScene(void) 
{
	glPushMatrix();
	{
		// glActiveTexture(GL_TEXTURE0); // activate the first slot
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

		glBegin(GL_QUADS);
                glTexCoord2f(0,0);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord2f(1,0);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glTexCoord2f(1,1);
		glVertex3f(1.0f, 1.0f, 0.0f);		
		glTexCoord2f(0,1);
		glVertex3f(-1.0f, 1.0f, 0.0f);


		glEnd();
	}
	glPopMatrix();
	glutSwapBuffers();
}



int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);

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

