#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#include "GL/glut.h"
#include <map>
#include <vector>
#include <string>
#include <math.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <fstream>

extern engine e;
class graphics {
public:
	std::map<int, GLuint> tiles;
	std::map<string, GLuint> menu_tex;

	GLuint load_image(string filename) {
		GLuint ret = 0;
		int i;
		unsigned char* image;
		FILE* f = fopen((filename).c_str(), "rb");
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
		glGenTextures(1, &ret);
		glBindTexture(GL_TEXTURE_2D, ret);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		if (step == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		if (step == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		delete[] image;
		return ret;
	}
	void load_tile(string filename, int index) {
		tiles[index] = load_image("../resources/" + filename);
	}

	//Initializes 3D rendering
	void initRendering() {
		//Makes 3D drawing work when something is in front of something else
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ifstream f("../resources/data/level_sprites.dat");
		string tmp;
		std::getline(f, tmp);
		int total = stoi(tmp);
		f.close();
		for (int i = 0; i < total; ++i) {
			tmp = to_string(i);
			if (i < 100)
				tmp = '0' + tmp;
			if (i < 10)
				tmp = '0' + tmp;
			tmp = tmp + ".bmp";
			load_tile(string("level_sprites/") + tmp, i);
		}


		ifstream f2("../resources/data/menu_sprites.dat");
		string line;
		while (f2.is_open()) {
			while (std::getline(f2, line)) {
				menu_tex[line] = load_image("../resources/menu_sprites/" + line);
			}
			f2.close();
		}
	}

	//Called when the window is resized
	void handleResize(int w, int h) {
		//Tell OpenGL how to convert from coordinates to pixel values
		double ratio = 160.0 / 144.0;
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
		//Set the camera perspective
		glLoadIdentity(); //Reset the camera
		gluOrtho2D(-1, 1, -1, 1);
	}
	void draw_level() {
		for (int y = 0; y < e.levels[e.current_level].data.size(); ++y) {
			for (int x = 0; x < e.levels[e.current_level].data[y].size(); ++x) {
				draw_quad(-1.0f + (float(x) / 5.0f) - ((e.mc.loc.x - 4.5f) / 5.0f), (float(-y) / 4.5f) - (0.5f / 4.5f) + (e.mc.loc.y / 4.5f), 1.0f / 5.0f, 1.0f / 4.5f, tiles[e.levels[e.current_level].data[y][x]]);
			}
		}
	}
	void draw_quad(float x, float y, float width, float height, GLuint tex) {
		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(x, y, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(x + width, y, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(x + width, y + height, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(x, y + height, 0.0f);
		glEnd();
	}

	//Draws the 3D scene
	void drawScene() {
		//Clear information from last draw
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
		glLoadIdentity(); //Reset the drawing perspective
		glColor3f(1.0f, 1.0f, 1.0f);
		e.handle_teleport();
		draw_level();
		alert(string("It's super effective!"));
		glutSwapBuffers(); //Send the 3D scene to the screen
		glutPostRedisplay();
	}
	void alert(string s) {
		draw_quad(-1.0f, -0.3f, 0.1f, 0.1f, menu_tex[string("corner-ul.bmp")]);
		draw_quad(0.9f, -0.3f, 0.1f, 0.1f, menu_tex[string("corner-ur.bmp")]);
		draw_quad(-0.9f, -0.3, 1.8f, 0.1f, menu_tex[string("bar-top.bmp")]);
		draw_quad(-1.0f, -0.9f, 0.1f, 0.6f, menu_tex[string("bar-left.bmp")]);
		draw_quad(0.9f, -0.9f, 0.1f, 0.6f, menu_tex[string("bar-right.bmp")]);
		draw_quad(-0.9f, -0.9f, 1.8f, 0.6f, menu_tex[string("space.bmp")]);
		draw_quad(-1.0f, -1.0f, 0.1f, 0.1f, menu_tex[string("corner-bl.bmp")]);
		draw_quad(0.9f, -1.0f, 0.1f, 0.1f, menu_tex[string("corner-br.bmp")]);
		draw_quad(-0.9f, -1.0f, 1.8f, 0.1f, menu_tex[string("bar-bottom.bmp")]);
	}
};

#endif