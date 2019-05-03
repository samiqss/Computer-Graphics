// Sami Al-Qusus
// Dec 21, 2018
// CSCI405

#include <math.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

string mygrammar="";

void draw(float len, int g){
	glLineWidth(g);
	glBegin(GL_LINES);
	glColor3f(0.3f, 0.1f, 0.1f);
    	glVertex3f(0, 0.0f, 0);
	glVertex3f(0, len, 0);
        glEnd(); 
}

int depth=1;      //depth of recursion
int gir=1;        //thickness parameter to relate plant girth to the resulting branch depth
void renderScene(void) {
	
	//open file and read grammar
	string line;
	ifstream myfile (mygrammar);
	int lineNum=0;
	string s, rule;
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			if(lineNum==0) 	s = line, lineNum++;
			else rule=line;

		}
		myfile.close();
	}
	else cout << "Unable to open file";

	//set up for rotating the tree
	static float treeAccum = 0.0f;
	treeAccum += 5.0f;

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	glOrtho(-1,1,-1,1,-1,1);
  	
	gluPerspective(45.0f,   // FOV y
  		       1.0f, 	// Aspect ratio (width/height)
         	       50.0f, 	// Near distance
  	 	       170.0f); // Far distance

	
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();

        gluLookAt( 0.0f,-50.0f, -70.0f,    // Eye position	
	           0.0f, 0.0f, -20.0f, 	   // Lookat position
  		   0.0f, 1.0f, 0.0f );	   // Up vector
	
	//set up for tree
	float len=2.0;    //length of starting line
	float theta = 30; //angle of rotation
	string next="";
	
	int recurse=0; //counter for recursion
	while(recurse<depth){
		for(int i=0; i<s.length(); i++){
			char c= s[i];
			if(c=='F') next.append(rule);
			else if (c=='+') next.append("+");
			else if (c=='-') next.append("-");
			else if (c==']') next.append("]");
			else if (c=='[') next.append("[");
		}
		s= next;
		recurse++;
		cout<<next<<endl;
	}
	
	int girth=gir;

	//rotate whole tree around its y-axis
        glRotatef(treeAccum,0.0f,1.0f,0.0f);

	for(int i=0; i<next.length(); i++){
		char c= next[i];
		if(c=='F'){
			draw(len,girth);
			glRotatef(40.0f,0.0f,1.0f,0.0f);
			glTranslatef(0,len,0);
		}
		else if(c=='+'){
			glRotatef(theta,1.0f,1.0f,1.0f);
		}
		else if(c=='-'){
			glRotatef(-theta,1.0f,1.0f,1.0f);
		}
		else if(c=='['){
			glPushMatrix();
			len=len/1.4;
			girth=girth/3;
		}
		else if(c==']'){
			glPopMatrix();
			len=len*1.4;
			girth=girth*3;
		}
	}
	
	glFinish();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
	
	if(key == 's' && depth!=5) depth=5;
	else if(key == 's' && depth==5) depth=1;

	if(key == 'g' && gir==1) gir=8;
	else if(key == 'g' && gir!=1) gir=1;
}

int main(int argc, char **argv) {

	mygrammar = argv[1];
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1000,1000);
	glutCreateWindow("Graphics Hello World");

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glEnable(GL_BLEND);
	glutKeyboardFunc(keyboard);
	
	//  GLUT main loop
	glutMainLoop();

	return(0);

}
