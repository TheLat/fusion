
/*#include "core.h"
#include "GL/glut.h"
using namespace std;

engine e;

int
main(int argc, char **argv)
{
	srand((unsigned)time(NULL));
	e.init_types();
	e.init_special();
	e.init_exp();
	e.init_status();
	e.init_moves();
	e.init_mon();
 	int a = 0;
}
*/
#include "core.h"
#include <iostream>
#include <stdlib.h>
#include "GL/glut.h"

using namespace std;
int num = 0;
unsigned char* image;
engine e;
//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {
	printf("%i\n", int(key));
	switch (key) {
	case 27: //Escape key
		exit(0);
	}
}
//Initializes 3D rendering
void initRendering() {
	//Makes 3D drawing work when something is in front of something else
	glDisable(GL_DEPTH_TEST); 
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	int i;
	FILE* f = fopen("../resources/characters/player-down-0.bmp", "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f);
	i = 0;
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];
	int step = info[28]/8;
	int size = step * width * height;
	image = new unsigned char[size]; // allocate 3 bytes per pixel
	if (step == 4)
		fread(image, sizeof(unsigned char), 84, f); // skip ahead to image
	fread(image, sizeof(unsigned char), size, f); // read the rest of the data at once
	fclose(f);

	
	for (i = 0; i < size; i += step)
	{
		if (step == 3) {
			unsigned char tmp = image[i];
			image[i] = image[i + 2];
			image[i + 2] = tmp;
		}
		else if (step == 4) {
			unsigned char tmp = image[i];
			image[i] = image[i + 3];
			image[i + 3] = tmp;
			tmp = image[i + 1];
			image[i + 1] = image[i + 2];
			image[i + 2] = tmp;

		}
	}
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}
//Called when the window is resized
void handleResize(int w, int h) {
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0,                  //The camera angle
		(double)w / (double)h, //The width-to-height ratio
		0.1,                   //The near z clipping coordinate
		200.0);                //The far z clipping coordinate
}
//Draws the 3D scene
void drawScene() {
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective
	glBegin(GL_QUADS); //Begin quadrilateral coordinates
	glBindTexture(GL_TEXTURE_2D, 0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 0.0f, -5.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0f, 0.0f, -5.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0f, 1.0f, -5.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0f, 1.0f, -5.0f);
	glEnd(); //End quadrilateral coordinates
	glutSwapBuffers(); //Send the 3D scene to the screen
	glutPostRedisplay();
}
int main(int argc, char** argv) {
	srand((unsigned)time(NULL));
	e.init_types();
	e.init_special();
	e.init_exp();
	e.init_status();
	e.init_moves();
	e.init_mon();

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400); //Set the window size
	//Create the window
	glutCreateWindow("Basic Shapes - programming-technique.blogspot.com");
	initRendering(); //Initialize rendering
	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMainLoop(); //Start the main loop

	return 0;
}
