#include "core.h"
#include <iostream>
#include <stdlib.h>
#include "GL/glut.h"
#include <string>

using namespace std;
int num = 0;
engine e;

// Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {
	printf("%i\n", int(key));
	num++;
	switch (key) {
	case 27: //Escape key
		exit(0);
	}
}
std::map<int, GLuint> tiles;
void loadTile(string filename, int index) {
	int i;
	unsigned char* image;
	FILE* f = fopen(("../resources/" + filename).c_str(), "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f);
	i = 0;
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];
	int step = info[28] / 8;
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
	glGenTextures(1, &(tiles[index]));
	glBindTexture(GL_TEXTURE_2D, tiles[index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	delete[] image;
}


//Initializes 3D rendering
void initRendering() {
	//Makes 3D drawing work when something is in front of something else
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	string tmp;
	for (int i = 0; i < 81; ++i) {
		tmp = to_string(i);
		if (i < 100)
			tmp = '0' + tmp;
		if (i < 10)
			tmp = '0' + tmp;
		tmp = tmp + ".bmp";
		loadTile(string("level_sprites/") + tmp, i);
	}

	e.init_levels();
}
//Called when the window is resized
void handleResize(int w, int h) {
	//Tell OpenGL how to convert from coordinates to pixel values
	double ratio = 160.0 / 144.0;
	h = (int)(double(w) / ratio);
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
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int y = 0; y < e.levels[string("pallet-town")].data.size(); ++y) {
		for (int x = 0; x < e.levels[string("pallet-town")].data[y].size(); ++x) {
			glBindTexture(GL_TEXTURE_2D, tiles[e.levels[string("pallet-town")].data[y][x]]);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.0f + (float(x) / 16.0f), (float(-y) / 16.0f), -2.5f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-1.0f + (float(x) / 16.0f) + (1.0f / 16.0f), (float(-y) / 16.0f), -2.5f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-1.0f + (float(x) / 16.0f) + (1.0f / 16.0f), (float(-y) / 16.0f) + (1.0f / 16.0f), -2.5f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-1.0f + (float(x) / 16.0f), (float(-y) / 16.0f) + (1.0f / 16.0f), -2.5f);
			glEnd();
		}
	}
	/*
	glBindTexture(GL_TEXTURE_2D, tiles[num]);
	glBegin(GL_QUADS); //Begin quadrilateral coordinates
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0f, -1.0f, -2.5f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0f, -1.0f, -2.5f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0f, 1.0f, -2.5f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0f, 1.0f, -2.5f);
	glEnd(); //End quadrilateral coordinates
	*/
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
	glutInitWindowSize(160*4, 144*4); //Set the window size
	//Create the window
	glutCreateWindow("Pokémon Fusion");
	initRendering(); //Initialize rendering
	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMainLoop(); //Start the main loop

	return 0;
}
