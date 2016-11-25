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

using namespace std;
extern mutex m;
class quad {
public:
	float x;
	float y;
	float width;
	float height;
	GLuint tex;
};

class string_lookup_data {
public:
	bool defined;
	string value;
	string_lookup_data() { defined = false; }
};

class graphics {
public:
	std::map<int, GLuint> tiles;
	std::map<string, GLuint> menu_tex;
	std::map<string, string_lookup_data> string_lookup;
	std::map<char, bool> chunk;
	std::vector<quad> draw_list;

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
		while (f2.is_open()) {
			while (std::getline(f2, tmp)) {
				menu_tex[tmp] = load_image("../resources/menu_sprites/" + tmp);
			}
			f2.close();
		}

		ifstream f3("../resources/data/string_lookup.dat");
		string l1, l2;
		while (f3.is_open()) {
			while (std::getline(f3, tmp)) {
				bool first_char;
				unsigned i;
				first_char = true;
				i = 0;
				l1 = "";
				l2 = "";
				while (first_char || tmp[i] != ' ') {
					first_char = false;
					l1 += tmp[i];
					i++;
				}
				i++;
				while (i < tmp.size()) {
					l2 += tmp[i];
					i++;
				}
				string_lookup[l1].defined = true;
				string_lookup[l1].value = l2;
			}
			f3.close();
		}
		chunk[' '] = true;
		chunk['-'] = true;
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
	void push_quad(float x, float y, float width, float height, GLuint tex) {
		quad q;
		q.x = x;
		q.y = y;
		q.width = width;
		q.height = height;
		q.tex = tex;
		draw_list.push_back(q);
	}
	void draw_quad(quad &q) {
		glBindTexture(GL_TEXTURE_2D, q.tex);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(q.x, q.y, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(q.x + q.width, q.y, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(q.x + q.width, q.y + q.height, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(q.x, q.y + q.height, 0.0f);
		glEnd();
	}

	//Draws the 3D scene
	void drawScene() {
		m.lock();
		//Clear information from last draw
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
		glLoadIdentity(); //Reset the drawing perspective
		glColor3f(1.0f, 1.0f, 1.0f);
		alert(string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"));
		for (unsigned i = 0; i < draw_list.size(); i++) {
			draw_quad(draw_list[i]);
		}
		glutSwapBuffers(); //Send the 3D scene to the screen
		glutPostRedisplay();
		m.unlock();
	}
	int next_chunk(string& s, int index) {
		unsigned ret = index + 1;
		while (!chunk[s[ret]] && ret < s.size())
			ret++;
		if (ret == s.size())
			ret--;
		return ret;
	}
	void draw_text(float x, float y, float width, float height, float size, string s) {
		string key;
		float x_curr = x;
		float y_curr = y;
		bool no_chunk_yet = true;
		for (unsigned i = 0; i < s.size(); ++i) {
			if (chunk[s[i]])
				no_chunk_yet = false;
			key = s[i];
			if (s[i] == '{') {
				while (s[i] != '}') {
					i++;
					key = key + s[i];
				}
			}
			key = key + string(".bmp");
			if (string_lookup[key].defined)
				key = string_lookup[key].value;
			push_quad(x_curr, y_curr, size, size, menu_tex[key]);
			x_curr += size;
			if (x_curr >= x + width || (!no_chunk_yet && ((float(next_chunk(s, i) - i))*size + x_curr > 0.9f))) {
				no_chunk_yet = true;
				x_curr = x;
				y_curr -= size*2.0f;
			}
		}
	}
	void draw_box(float xmin, float ymin, float length, float height) {
		push_quad(xmin, ymin + height - 0.1f, 0.1f, 0.1f, menu_tex[string("corner-ul.bmp")]);
		push_quad(xmin + length - 0.1f, ymin + height - 0.1f, 0.1f, 0.1f, menu_tex[string("corner-ur.bmp")]);
		push_quad(xmin + 0.1f, ymin + height - 0.1f, length - 0.2f, 0.1f, menu_tex[string("bar-top.bmp")]);
		push_quad(xmin, ymin + 0.1f, 0.1f, height - 0.2f, menu_tex[string("bar-left.bmp")]);
		push_quad(xmin + length - 0.1f, ymin + 0.1f, 0.1f, height - 0.2f, menu_tex[string("bar-right.bmp")]);
		push_quad(xmin + 0.1f, ymin + 0.1f, length - 0.2f, height - 0.2f, menu_tex[string("space.bmp")]);
		push_quad(xmin, ymin, 0.1f, 0.1f, menu_tex[string("corner-bl.bmp")]);
		push_quad(xmin + length - 0.1f, ymin, 0.1f, 0.1f, menu_tex[string("corner-br.bmp")]);
		push_quad(xmin + 0.1f, ymin, length - 0.2f, 0.1f, menu_tex[string("bar-bottom.bmp")]);
	}
	void alert(string s) {
		draw_box(-1.0f, -1.0f, 2.0f, 0.8f);
		draw_text(-0.9f, -0.5f, 1.8f, 0.4f, 0.1f, s);
	}
};

#endif