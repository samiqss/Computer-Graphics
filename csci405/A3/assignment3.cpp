//Sami Al-Qusus
//Nov 30, 2018
//csci405 asn 3
//Bezier Curve
#include <math.h>
#include <string>
#include <time.h>
#include <cstdlib>
#include <stdio.h>
#include "mathvector.h"
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include<iostream>

//Bezier Curve
//q0 and q1 are points on p0->p1 and p1->p2
//q0=(1-t)p0+tp1
//q1=(1-t)p1+tp2
//c(t) is a point on the bezier curve on q0->q1
//c(t)=(1-t)q0+tq1
//c(t)=(1-t)^2p0+2t(1-t)p1+t^2p2
//where t is between 0 and 1

using namespace std;

class particles {
public: 
	double x;
	double y;

	float red;
	float green;
	float blue;

	float life;
	
	float speedy;
	float speedx;
};

particles particle[1000];

float visibility_r =1;
float visibility_g =1;
float visibility_b =1;

float seed = 0;
float r = 0;
float g = 0;
float b = 0;
float speeder = 0;

double cpy;
double cpx;
float t=0.001;
float speedxx=30.0f;
float speedyy=30.0f;
int irand = 0;
int life = 2;

void resize (int w, int h){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,w-1,h-1,0);
	glPopMatrix();	
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT);
	t = t+speeder;
	int i = 0;
	
		glColor3f(visibility_r, visibility_g, visibility_r);
		
		//draw loop
		glBegin(GL_LINES);
		{
			float f=0;
			while(f<=2){
				if (f<1.0){
				float xx= (1-f)*(1-f)*-1.0+2*f*(1-f)*0+f*f*1.0;
				float yy= (1-f)*(1-f)*0.0+2*f*(1-f)*1+f*f*0.0;
				glVertex2f(xx, yy);
				f = f+0.001;
				xx= (1-f)*(1-f)*-1.0+2*f*(1-f)*0+f*f*1.0;
				yy= (1-f)*(1-f)*0.0+2*f*(1-f)*1+f*f*0.0;
				glVertex2f(xx, yy);
				f = f+0.001;
				}else if(f<2.0 && f>=1){
					float tb = f-1;
					float xx= (1-tb)*(1-tb)*1.0+2*tb*(1-tb)*0+tb*tb*-1.0;
					float yy= (1-tb)*(1-tb)*0.0+2*tb*(1-tb)*-1+tb*tb*0.0;
					glVertex2f(xx, yy);
					f = f+0.001;
					tb = f-1;
					xx= (1-tb)*(1-tb)*1.0+2*tb*(1-tb)*0+tb*tb*-1.0;
					yy= (1-tb)*(1-tb)*0.0+2*tb*(1-tb)*-1+tb*tb*0.0;
					glVertex2f(xx, yy);
					f = f+0.001;
				}

			}
		}
		glEnd();
		
		//render emitter
		do{
			srand(seed);
			seed += 0.1;
			r += 0.1;
			b += 0.002;
			g += 0.009;
			if (r>1.0) r= float(rand()%2)-0.7;
			if (g>1.0) g= float(rand()%1)-0.3;			
			if (b>1.0) b= 0;
			if (seed>1.0) seed =0;
			if (irand > 100) irand = -50;
	
			glColor4f(particle[i].red, particle[i].green, particle[i].blue, particle[i].life);
			glBegin(GL_TRIANGLE_STRIP);
				float divider = particle[i].life;
				if (divider<0.01) divider=divider+0.01; 
				float resize=0.00005/ divider;
				glVertex2f(particle[i].x+resize, particle[i].y+resize);
				glVertex2f(particle[i].x-resize, particle[i].y+resize);
				glVertex2f(particle[i].x+resize, particle[i].y-resize);
				glVertex2f(particle[i].x-resize, particle[i].y-resize);
			glEnd();
		
		
	
		if (t<1.0){
			cpx= (1-t)*(1-t)*-1.0+2*t*(1-t)*0+t*t*1.0;
			cpy= (1-t)*(1-t)*0.0+2*t*(1-t)*1+t*t*0.0;
		}else if(t<2.0 && t>=1){
			float tb = t-1;
			cpx= (1-tb)*(1-tb)*1.0+2*tb*(1-tb)*0+tb*tb*-1.0;
			cpy= (1-tb)*(1-tb)*0.0+2*tb*(1-tb)*-1+tb*tb*0.0;
		}else
			t=0;

		
		if (particle[i].life <= 0.0){
			life =2.5;
			srand((seed+r+g+b)*500);

			particle[i].x = cpx;
			//cout<<"x= "<<cpx<<endl;
			//cout<<"y= "<<cpy<<endl;
			particle[i].y = cpy;
			particle[i].life = float(rand()%life);
			particle[i].speedy = float((rand()%50)/speedyy);
			particle[i].speedx = float((rand()%50)/speedxx);
			particle[i].red = r;
			particle[i].green = g;
			particle[i].blue = b;
			irand += rand() % 50;
		}

		particle[i].life -= 0.03;
		
		particle[i].y += particle[i].speedx/2000;
		particle[i].x += particle[i].speedy/2000;
			//20000;

		++i;

	}while (i<1000);
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
	if(key == '1') speedyy = 20.0f, speedxx = 20.0f;
	if(key == '2') speedyy = 9.0f, speedxx = 9.0f;
	if(key == '3') speedyy = 2.0f, speedxx = 2.0f;

	if(key == '-' ) t=t/4 ;
	if(key == '+' && speeder<0.1)  speeder += 0.04;
	
	if(key == 'd') {
		if(visibility_r ==0) visibility_r =1;
		else visibility_r =0;
		if (visibility_g ==0) visibility_g =1;
		else visibility_g =0;
		if (visibility_b ==0) visibility_b =1;
		else visibility_b =0;
	}
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(1000,1000);
	glutCreateWindow("Graphics Hello World");
	
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glEnable(GL_BLEND);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	
	//  GLUT main loop
	glutMainLoop();

	return(0);

}
