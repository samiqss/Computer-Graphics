#include <math.h>
#include <string>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void renderScene(void) {

	static float gsCameraAccum = 0.0f;
	static float gsTeapotAccum = 0.0f;

	gsCameraAccum += 0.01f;
	gsTeapotAccum += 0.1f;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	glOrtho(-1,1,-1,1,-1,1);
  	gluPerspective(45.0f,   // FOV y
  					1.0f, 	// Aspect ratio (width/height)
  					1.0f, 	// Near distance
  					100.0f);// Far distance


  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();


  	gluLookAt( 0.0f,cos(gsCameraAccum), -10.0f,    // Eye position
         			0.0f, 0.0f, 0.0f, 	  // Lookat position
  				0.0f, 1.0f, 0.0f );	  // Up vector

	glPushMatrix();
	glRotatef(gsTeapotAccum,0.0f,1.0f,0.0f);
	glutSolidTeapot(1.0f);
	glPopMatrix();

	glutSwapBuffers();
}



int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(512,512);
	glutCreateWindow("Graphics Hello World");

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	//  GLUT main loop
	glutMainLoop();

	return(0);

}
