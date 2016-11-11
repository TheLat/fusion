#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#include "GL/glut.h"

extern engine e;
class graphics {
public:
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
			loadTile(string("level_sprites/") + tmp, i);
		}
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
		gluOrtho2D(-1, 1, -1, 1);
	}
	void DrawLevel() {
		for (int y = 0; y < e.levels[e.current_level].data.size(); ++y) {
			for (int x = 0; x < e.levels[e.current_level].data[y].size(); ++x) {
				glBindTexture(GL_TEXTURE_2D, tiles[e.levels[e.current_level].data[y][x]]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-1.0f + (float(x) / 5.0f) - ((e.mc.loc.x - 4.5f) / 5.0f), (float(-y) / 4.5f) - (0.5f / 4.5f) + (e.mc.loc.y / 4.5f), 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-1.0f + (float(x) / 5.0f) + (1.0f / 5.0f) - ((e.mc.loc.x - 4.5f) / 5.0f), (float(-y) / 4.5f) - (0.5f / 4.5f) + (e.mc.loc.y / 4.5f), 0.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-1.0f + (float(x) / 5.0f) + (1.0f / 5.0f) - ((e.mc.loc.x - 4.5f) / 5.0f), (float(-y) / 4.5f) + (0.5f / 4.5f) + (e.mc.loc.y / 4.5f), 0.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-1.0f + (float(x) / 5.0f) - ((e.mc.loc.x - 4.5f) / 5.0f), (float(-y) / 4.5f) + (0.5f / 4.5f) + (e.mc.loc.y / 4.5f), 0.0f);
				glEnd();
			}
		}
	}

	//Draws the 3D scene
	void drawScene() {
		//Clear information from last draw
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
		glLoadIdentity(); //Reset the drawing perspective
		glColor3f(1.0f, 1.0f, 1.0f);
		for (int i = 0; i < e.levels[e.current_level].teleport.size(); ++i) {
			if (e.mc.loc.x == e.levels[e.current_level].teleport[i].first.x && e.mc.loc.y == e.levels[e.current_level].teleport[i].first.y) {
				e.mc.loc.x = e.levels[e.current_level].teleport[i].second.x;
				e.mc.loc.y = e.levels[e.current_level].teleport[i].second.y;
				e.current_level = e.levels[e.current_level].teleport[i].second.level;
				break;
			}
		}
		DrawLevel();
		glutSwapBuffers(); //Send the 3D scene to the screen
		glutPostRedisplay();
	}
};

#endif