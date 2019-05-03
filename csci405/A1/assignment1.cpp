//Sami Al-Qusus 
//Assignment 1 CSCI405 (Oct 26, 2018)
//Credits: Mike Krazanowski (Lab 1-4 + Lecture code)
//         https://en.wikipedia.org/wiki/Rotation_matrix
//	   https://braintrekking.wordpress.com/2012/08/21/tutorial-of-arcball-without-quaternions/ (took ideas from here)
//	   https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball

#include <math.h>
#include <string>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "bitmap.h"
#include "LoadObj.h"
#include <iostream>
//#include "mathvector.h"
using namespace std;

//store texture name from main arg
string texture;

// Specify the window dimensions
#define SCREENWIDTH 600
#define SCREENHEIGHT 600

static GLuint textureHandle = 0;

//Loads Image 
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
  //variables used to Load Object
	int NumVertexPositions;
	int NumVertexNormals;
	int NumVertexUVs;
	int *NumIndices;
	float *VertexPositions;
	float *VertexNormals;
	float *VertexUVs;
	//int *Indices=NULL;
	
	//process texture name from command line
	string proccessText(char* &img){
		string textName(img);
		return textName;
	}
        
        //process object file name from command line
	void proccessData(char* &pathObj){
	  std::string filename(pathObj);
	  bool res = LoadOBJ( filename, 
				 "", 
				 VertexPositions, 
				 VertexNormals, 
				 VertexUVs, 
				 NULL,                   
				 NumVertexPositions, 
				 NumVertexNormals, 
				 NumVertexUVs, 
				 NumIndices );
	  
}

//glutKeyboardFunc(keyboard)
void keyboard(unsigned char c, int x, int y){
	
	
	if (c == '0'){
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glEnable(GL_LIGHT0);
	}
	if (c == '1'){
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glEnable(GL_LIGHT1);
	}
	if (c == '2'){
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT3);
		glEnable(GL_LIGHT2);
	}
	if (c == '3'){
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glEnable(GL_LIGHT3);
	}
	
}

//variables used for arcball
typedef float GLMatrix[16];
float fov;
int fovStartY;
int fovCurrentY;

float transX, transY;
float currentTransX, currentTransY;
float startTransX, startTransY;

GLMatrix startMatrix;
GLMatrix currentMatrix;
vector3 startRotationVector;
vector3 currentRotationVector;
bool isRotating;
float ballRadius;
double residualSpin;

//static const float MINIMAL_FOV;

const float INITIAL_FOV = 20;
const float TRANSLATION_FACTOR = 0.01f;

int width, height;

void reset()
{  fov = INITIAL_FOV;
   // reset matrix
   memset(startMatrix, 0, sizeof(startMatrix));
   startMatrix[0] = 1;
   startMatrix[1] = 0;
   startMatrix[2] = 0;
   startMatrix[3] = 0;
   startMatrix[4] = 0;
   startMatrix[5] = 1;
   startMatrix[6] = 0;
   startMatrix[7] = 0;
   startMatrix[8] = 0;
   startMatrix[9] = 0;
   startMatrix[10] = 1;
   startMatrix[11] = 0;
   startMatrix[12] = 0;
   startMatrix[13] = 0;
   startMatrix[14] = 0;
   startMatrix[15] = 1;

   transX = transY = 0;
   startTransX = startTransY = currentTransX = currentTransY = 0;
}

//arcball initialization
void Arcball()
{  ballRadius= 300;
   isRotating= false;
   width=height= 0;
   reset();
}

//arcball functions
void setWidthHeight(int w, int h)
{  width=w;
   height=h;
   ballRadius = min((int)(w/2), (int)(h/2));
}

void setRadius(float newRadius)
{  ballRadius = newRadius;
}

vector3 convertXY(int x, int y)
{

   int d = x*x+y*y;
   float radiusSquared = ballRadius*ballRadius;
   if (d > radiusSquared)
   {  return vector3((float)x,(float)y, 0 );
   }
   else
   {  return vector3((float)x,(float)y, sqrt(radiusSquared - d));
   }
}

void startRotation(int _x, int _y)
{  int x = ( (_x)-(width/2) );
   int y = ((height/2)-_y);

   startRotationVector = convertXY(x,y);
   startRotationVector.Normalize();

   currentRotationVector=  startRotationVector;
   isRotating = true;
}

void updateRotation(int _x, int _y)
{  int x = ( (_x)-(width/2) );
   int y = ((height/2)-_y);

   currentRotationVector = convertXY(x,y);

   currentRotationVector.Normalize();
}

void applyTranslationMatrix(bool reverse)
{  float factor = (reverse?-1.0f:1.0f);
   float tx = transX + (currentTransX - startTransX)*TRANSLATION_FACTOR;
   float ty = transY + (currentTransY - startTransY)*TRANSLATION_FACTOR;
   glTranslatef(factor*tx,  factor*(-ty), 0);
}

void applyRotationMatrix()
{  if (isRotating)
   {  // Do some rotation according to start and current rotation vectors
      //cerr << currentRotationVector.transpose() << " " << startRotationVector.transpose() << endl;
      if ( ( currentRotationVector - startRotationVector).Length() > 1E-6 )
      {  
		 const vector3 currRotationVector = currentRotationVector;
		 const vector3 strtRotationVector = startRotationVector;
		 vector3 rotationAxis = vector3::CrossProduct(currRotationVector,strtRotationVector);
                 rotationAxis.Normalize();

                 double val= vector3::DotProduct(currRotationVector,strtRotationVector);
                 val > (1-1E-10) ? val=1.0 : val=val ;
                 double rotationAngle = acos(val) * 180.0f/(float)3.15;

                 // rotate around the current position
                 applyTranslationMatrix(true);
                 glRotatef(rotationAngle * 2, -1*rotationAxis.X(),-1*rotationAxis.Y(),-1*rotationAxis.Z());

                 applyTranslationMatrix(false);
      }
   }
   glMultMatrixf(startMatrix);
}

void stopRotation()
{

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   applyRotationMatrix();
   // set the current matrix as the permanent one
   glGetFloatv(GL_MODELVIEW_MATRIX, startMatrix);
   isRotating = false;
}



void mouse(int button, int state, int x, int y){
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
            startRotation(x,y);
        else
            stopRotation();
        
	glutPostRedisplay();
 }




void motion(int x, int y) {
   updateRotation(x,y);
   glutPostRedisplay();
}


//lights variables
// Light 0 colours
GLfloat gsLightSpecularColour0[] = {1.0, 1.0, 1.0, 1.0};
GLfloat gsLightDiffuseColour0[] = {1.0, 1.0, 1.0, 1.0};
GLfloat gsLightAmbientColour0[] = {0.0, 0.0, 0.1, 1.0};
GLfloat gsLightPosition0[] = {0.0, 10.0, 10.0, 0.0};
// Light 1 colours
GLfloat gsLightSpecularColour1[] = {1.0, 1.0, 1.0, 1.0};
GLfloat gsLightDiffuseColour1[] = {1.0, 0.0, 1.0, 0.0};
GLfloat gsLightAmbientColour1[] = {1.0, 1.0, 1.0, 1.0};
GLfloat gsLightPosition1[] = {20.0,20.0, 20.0, 20.0};
// Light 2 colours
GLfloat gsLightSpecularColour2[] = {1.0, 1.0, 1.0, 1.0};
GLfloat gsLightDiffuseColour2[] = {0.1, 2.0, 0.1, 2.0};
GLfloat gsLightAmbientColour2[] = {0.0, 1.0, 1.0, 1.0};
GLfloat gsLightPosition2[] = {0.0, 10.0, 0.0, 10.0};
// Light 3 colours
GLfloat gsLightSpecularColour3[] = {1.0, 1.0, 1.0, 1.0};
GLfloat gsLightDiffuseColour3[] = {1.0, 0.0, 0.0, 1.0};
GLfloat gsLightAmbientColour3[] = {1.0, 0.5, 0.0, 1.0};
GLfloat gsLightPosition3[] = {10.0, 0.0, 0.0, 10.0};

void initializeScene()
{
	// light setup code
	glEnable(GL_LIGHTING);
	//light 0
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, gsLightSpecularColour0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gsLightDiffuseColour0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, gsLightAmbientColour0);
	glLightfv(GL_LIGHT0, GL_POSITION, gsLightPosition0);
        //light 1
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, gsLightSpecularColour1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, gsLightDiffuseColour1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, gsLightAmbientColour1);
	glLightfv(GL_LIGHT1, GL_POSITION, gsLightPosition1);
	//light 2
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT2, GL_SPECULAR, gsLightSpecularColour2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, gsLightDiffuseColour2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, gsLightAmbientColour2);
	glLightfv(GL_LIGHT2, GL_POSITION, gsLightPosition2);
	//light 3
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT3, GL_SPECULAR, gsLightSpecularColour3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, gsLightDiffuseColour3);
	glLightfv(GL_LIGHT3, GL_AMBIENT, gsLightAmbientColour3);
	glLightfv(GL_LIGHT3, GL_POSITION, gsLightPosition3);
	glShadeModel(GL_FLAT );
	//glShadeModel( GL_SMOOTH );
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	textureHandle = LoadImage(texture,"");



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
		       1.0f,   // Aspect ratio (width/height)
	               0.1f,   // Near distance
		       1000.0f);// Far distance
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt( 0.0f, 0.0f, -5.0f,    // Eye position
			0.0f, 0.0f, 0.0f,         // Lookat position
			0.0f, 1.0f, 0.0f );       // Up vector
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glPushMatrix();
	{
		glActiveTexture(GL_TEXTURE0); // activate the first slot
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		//glRotatef( sAccum, 0.0f, 1.0f, 1.0f );
		applyRotationMatrix();

		// Put indexed draw calls here
		// Enable the vertex normal array and bind it to our vertex Normals
		glEnableClientState(GL_NORMAL_ARRAY);                           // Enable the normal buffer
		glNormalPointer(GL_FLOAT, 0, VertexNormals);          // Supply an array of normals

		// Enable the vertex position array and bind it to our vertex positions
		glEnableClientState(GL_VERTEX_ARRAY);                           // Enable the vertex buffer
		glVertexPointer(3, GL_FLOAT, 0, VertexPositions);
		// Enable the vertex texture array and bind it to our vertex texture
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);  // Enable the uvs  buffer
		glTexCoordPointer(2, GL_FLOAT, 0, VertexUVs);   //supply an array of textures
		// Supply an array of vertices
		// Draw using the vertex buffers and the supplied index buffer
		//glDrawElements(GL_TRIANGLES, 60, GL_UNglutKeyboardFunc(keyboard);SIGNED_BYTE, gsIndices); //using vertex position 
		glDrawArrays(GL_TRIANGLES, 0, NumVertexPositions);
	}
	glPopMatrix();
	glutSwapBuffers();
}

int main(int argc, char **argv) {
	// get file name
	char *pathObj=argv[1];
	
	char *pathImg=argv[2];
	//pass file name
	proccessData(pathObj);
	texture = proccessText(pathImg);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize( SCREENWIDTH, SCREENHEIGHT );
	glutCreateWindow("assignment1");
	
        Arcball();
	glViewport( 0, 0, SCREENWIDTH, SCREENHEIGHT );

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	initializeScene();

	//  GLUT main loop
	glutMainLoop();

	return(0);

}
