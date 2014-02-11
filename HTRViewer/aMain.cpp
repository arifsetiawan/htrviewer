
/************************************************************************
 * aMain.cpp
 ************************************************************************/

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "aHtr.h"

aHTRFormat htr;

float ratio;
int h,w;
int font=(int)GLUT_BITMAP_8_BY_13;
int frame,time,timebase=0;
char s[30];
//static GLfloat spin = 0.0;
int cframe = 0;


void InitWindow();

void ChangeSize(int w1, int h1)
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h1 == 0)
		h1 = 1;

	w = w1;
	h = h1;
	ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(100,ratio,0.1,20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0f, 1.0f, 2.0f, 
		      0.0f, 1.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);

}


void InitScene(const char *filename) 
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	
	htr.ImportData(filename);
}

void SetOrthographicProjection() 
{
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, w, 0, h);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -h, 0);
	glMatrixMode(GL_MODELVIEW);
}

void ResetPerspectiveProjection() 
{
	// set the current matrix to GL_PROJECTION
	glMatrixMode(GL_PROJECTION);
	// restore previous settings
	glPopMatrix();
	// get back to GL_MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}

void RenderBitmapString(float x, float y, void *font,char *string)
{
	char *c;
	// set position to start drawing fonts
	glRasterPos2f(x, y);
	// loop all the characters in the string
	for (c=string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(font, *c);
	}
}


void RenderScene(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	glPushMatrix();
		glColor3f(0.0f,0.0f, 0.0f);
		glutSolidCube(0.02);		
		
		glTranslatef(-0.5*2, 0.0, -0.5*2);
		glutSolidCube(0.02);
		
		glTranslatef(1.0*2, 0.0, 0.0);
		glutSolidCube(0.02);
		
		glTranslatef(0.0, 0.0, 1.0*2);
		glutSolidCube(0.02);
		
		glTranslatef(-1.0*2, 0.0, 0.0);
		glutSolidCube(0.02);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.0f,0.0f, 0.0f);
		glTranslatef(0.0, 2.0, 0.0);
		glutSolidCube(0.02);		
		
		glTranslatef(-0.5*2, 0.0, -0.5*2);
		glutSolidCube(0.02);
		
		glTranslatef(1.0*2, 0.0, 0.0);
		glutSolidCube(0.02);
		
		glTranslatef(0.0, 0.0, 1.0*2);
		glutSolidCube(0.02);
		
		glTranslatef(-1.0*2, 0.0, 0.0);
		glutSolidCube(0.02);
	glPopMatrix();
	*/

	printf("Current Frame to draw %d\n", cframe);
	htr.DrawFrame(cframe);

	// display something on view
	glColor3f(0.0f,1.0f, 0.0f);
	SetOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	RenderBitmapString(10,15,(void *)font,"HTR Mocap Data Viewer"); 
	//RenderBitmapString(10,35,(void *)font,"Base Pose Rendering");
	glPopMatrix();
	ResetPerspectiveProjection();
	

	glutSwapBuffers();
}

void RenderSceneAnim()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// increment frame
	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > ((int)1000/htr.NumFPS())) 
	{
		/*
		sprintf(s,"FPS:%4.2f",frame*1000.0/(time-timebase));
		timebase = time;		
		frame = 0;
		*/
		cframe = cframe + 1;
		if(cframe == htr.NumFrames())
			cframe = 0;

		timebase = time;
	}

	// display something on view
	glColor3f(0.0f,1.0f, 0.0f);
	SetOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	RenderBitmapString(10,15,(void *)font,"HTR Mocap Data Viewer"); 
	RenderBitmapString(10,35,(void *)font,"Loaded Animation Data");
	glPopMatrix();
	ResetPerspectiveProjection();
	

	glutPostRedisplay();
//	glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y) 
{
	if (key == 27) 
		exit(0);
}

void Mouse(int button, int state, int x, int y) 
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(RenderSceneAnim);
         break;
      case GLUT_MIDDLE_BUTTON:
      case GLUT_RIGHT_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(NULL);
         break;
      default:
         break;
   }
}

void InitWindow() 
{
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutMouseFunc(Mouse);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		cout << "How to use: HTRViewer htrfile.htr\n";
		return 0;
	}

	string htrfile = argv[1];

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(500,500);
	string windowName = "HTR Viewer " + htrfile;
	glutCreateWindow(windowName.c_str());

	// register all callbacks
	InitWindow();
	InitScene(htrfile.c_str());

	glutMainLoop();

	return(0);
}
