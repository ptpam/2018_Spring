// Includes
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Function Prototypes
void display();
void specialKeys();
void makeCube(float x,float y,float z);
// Global Variables
double rotate_y=-30; 
double rotate_x=-30;

int main(int argc, char* argv[]){
 
	//  Initialize GLUT and process user parameters
	glutInit(&argc,argv);
	 
	//  Request double buffered true color window with Z-buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);


	// Create window
	glutCreateWindow("Awesome Cube");

	//  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);

	// Callback functions
	glutDisplayFunc(display);
	//glutSpecialFunc(specialKeys);
	//  Pass control to GLUT for events
	glutMainLoop();
	 
	//  Return to OS
	return 0;
 
}

void display(){
 
	//  Clear screen and Z-buffer
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// Multi-colored side - FRONT
	glBegin(GL_POLYGON);
	 
	// Vertices will be added in the next step
	 
	glEnd();

	//on uclu
	makeCube(0.2,0,0);
	makeCube(0.4,0,0);
	// en sol
	makeCube(0,0,0);
	
	// orta
	makeCube(0.2,0,0.2);
	makeCube(0.4,0,0.2);
	makeCube(0,0,0.2);

	// arka uclu
	makeCube(0.2,0,0.4);
	makeCube(0.4,0,0.4);
	makeCube(0,0,0.4);

	//arka ust
	makeCube(0.4,0.2,0.4);
	makeCube(0,0.2,0.4);

	//arka alt
	makeCube(0.4,-0.2,0.4);
	makeCube(0,-0.2,0.4);

	// on alt
	makeCube(0.4,-0.2,0);
	makeCube(0.0,-0.2,0);
	 
	glFlush();
	glutSwapBuffers();
 
}

void makeCube(float x, float y, float z){
		// Multi-colored side - FRONT
	glBegin(GL_POLYGON);
	 
	glVertex3f( -0.1, -0.1, -0.1);       // P1
	glVertex3f( -0.1,  0.1, -0.1);       // P2
	glVertex3f(  0.1,  0.1, -0.1);       // P3
	glVertex3f(  0.1, -0.1, -0.1);       // P4
	 
	glEnd();

	// Reset transformations
	glLoadIdentity();
	 
	// Rotate when user changes rotate_x and rotate_y
	
	glRotatef( rotate_x, 1.0, 0.0, 0.0 );
	glRotatef( rotate_y, 0.0, 1.0, 0.0 );
	glTranslatef( x, y, z);
	/*
	glRotatef( 60, 1.0, 0.0, 0.0 );
	glRotatef( 60, 0.0, 1.0, 0.0 );
	*/
	//Multi-colored side - FRONT
	glBegin(GL_POLYGON);
	 
	glColor3f( 1.0, 0.0, 0.0 );     glVertex3f(  0.1, -0.1, -0.1 );      // P1 is red
	glColor3f( 0.0, 1.0, 0.0 );     glVertex3f(  0.1,  0.1, -0.1 );      // P2 is green
	glColor3f( 0.0, 0.0, 1.0 );     glVertex3f( -0.1,  0.1, -0.1 );      // P3 is blue
	glColor3f( 1.0, 0.0, 1.0 );     glVertex3f( -0.1, -0.1, -0.1 );      // P4 is purple
	 
	glEnd();

	// White side - BACK
	glBegin(GL_POLYGON);
	glColor3f(   1.0,  1.0, 1.0 );
	glVertex3f(  0.1, -0.1, 0.1 );
	glVertex3f(  0.1,  0.1, 0.1 );
	glVertex3f( -0.1,  0.1, 0.1 );
	glVertex3f( -0.1, -0.1, 0.1 );
	glEnd();
	 
	// Purple side - RIGHT
	glBegin(GL_POLYGON);
	glColor3f(  1.0,  0.0,  1.0 );
	glVertex3f( 0.1, -0.1, -0.1 );
	glVertex3f( 0.1,  0.1, -0.1 );
	glVertex3f( 0.1,  0.1,  0.1 );
	glVertex3f( 0.1, -0.1,  0.1 );
	glEnd();
	 
	// Green side - LEFT
	glBegin(GL_POLYGON);
	glColor3f(   0.0,  1.0,  0.0 );
	glVertex3f( -0.1, -0.1,  0.1 );
	glVertex3f( -0.1,  0.1,  0.1 );
	glVertex3f( -0.1,  0.1, -0.1 );
	glVertex3f( -0.1, -0.1, -0.1 );
	glEnd();
	 
	// Blue side - TOP
	glBegin(GL_POLYGON);
	glColor3f(   0.0,  0.0,  1.0 );
	glVertex3f(  0.1,  0.1,  0.1 );
	glVertex3f(  0.1,  0.1, -0.1 );
	glVertex3f( -0.1,  0.1, -0.1 );
	glVertex3f( -0.1,  0.1,  0.1 );
	glEnd();
	 
	// Red side - BOTTOM
	glBegin(GL_POLYGON);
	glColor3f(   1.0,  0.0,  0.0 );
	glVertex3f(  0.1, -0.1, -0.1 );
	glVertex3f(  0.1, -0.1,  0.1 );
	glVertex3f( -0.1, -0.1,  0.1 );
	glVertex3f( -0.1, -0.1, -0.1 );
	glEnd();
}

void specialKeys( int key, int x, int y ) {
 
	//  Right arrow - increase rotation by 5 degree
	if (key == GLUT_KEY_RIGHT)
	  rotate_y += 5;
	 
	//  Left arrow - decrease rotation by 5 degree
	else if (key == GLUT_KEY_LEFT)
	  rotate_y -= 5;
	 
	else if (key == GLUT_KEY_UP)
	  rotate_x += 5;
	 
	else if (key == GLUT_KEY_DOWN)
	  rotate_x -= 5;
	 
	//  Request display update
	glutPostRedisplay();
 
}


 
/*
// Other transformations
glTranslatef( 0.1, 0.0, 0.0 );
glRotatef( 180, 0.0, 1.0, 0.0 );
glScalef( 2.0, 2.0, 0.0 );
*/