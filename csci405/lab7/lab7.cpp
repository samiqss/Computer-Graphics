#include <math.h>
#include <string>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/*
GLfloat vertices[] = {-0.5f, -0.5f, 0.0f,
                           0.5f, -0.5f, 0.0f,
                           0.5f,  0.5f, 0.0f,
                           0.5f,  0.5f, 0.0f,
                          -0.5f,  0.5f, 0.0f,
                          -0.5f, -0.5f, 0.0f};
*/

static GLfloat time = 0; 

void keyboard(unsigned char c, int x, int y){
	
	time=0;
	if (c == '0'){
}
	
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
 	glOrtho(-1,1,-1,1,-1,1);

 
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();
 
	// Render stuff here
	glBegin(GL_POINTS);
	time = time+0.1;
	glPointSize(50);
	glColor3f(0.0,1.0,0.0);
	for (int i = 0; i < 10000; i++){
		GLfloat x=i+time;
		GLfloat y=i+time;
	    GLfloat z=i+time;
		glVertex3f(x, y, z);
	}
	glEnd();



/*
    glBegin(GL_POINTS);
		glColor3f(0.0,1.0,0.0);
		glVertex3fv(position);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(0,0);
		glColor3f(0.0,1.0,0.0);
        glVertex2f(-0.2,0);
		glColor3f(1.0,0.0,0.0);
        glVertex2f(-0.2,-0.1);
        glVertex2f(0,-0.1);
	glEnd();


    glutSolidTeapot(0.5);
*/
	glutSwapBuffers();
}



int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA|GLUT_MULTISAMPLE);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(1000,1000);
	glutCreateWindow("Graphics Hello World");

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(keyboard);

	//  GLUT main loop
	glutMainLoop();

        

	return(0);

}
