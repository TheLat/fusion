#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#include "GL/glut.h"
#include "PNG/png.h"
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
	string filename;
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
	std::map<string, GLuint> tex;
	std::map<string, string_lookup_data> string_lookup;
	std::map<char, bool> chunk;
	std::vector<quad> draw_list;
	bool new_load;
	graphics() { new_load = false; }

	GLuint load_image(string filename) {
		GLuint ret = 0;
		int i;
		unsigned char* image;
		FILE* f = fopen((filename).c_str(), "rb");
		if (f == 0)
			return 0;
		int height = 0;
		int width = 0;
		int step = 0;
		int size = 0;
		if (filename.find(".bmp") != -1) {
			unsigned char info[54];
			fread(info, sizeof(unsigned char), 54, f);
			i = 0;
			width = *(int*)&info[18];
			height = *(int*)&info[22];
			step = info[28] / 8;
			size = step * width * height;
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
		}
		else if (filename.find(".png") != -1) {
			char header[8];
			fread(header, 1, 8, f);
			png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			png_infop info_ptr = png_create_info_struct(png_ptr);
			setjmp(png_jmpbuf(png_ptr));
			png_init_io(png_ptr, f);
			png_set_sig_bytes(png_ptr, 8);
			png_read_info(png_ptr, info_ptr);
			width = png_get_image_width(png_ptr, info_ptr);
			height = png_get_image_height(png_ptr, info_ptr);
			png_byte color_type = png_get_color_type(png_ptr, info_ptr);
			png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
			int number_of_passes = png_set_interlace_handling(png_ptr);
			if (color_type == PNG_COLOR_TYPE_PALETTE)
				png_set_palette_to_rgb(png_ptr);
			if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
				png_set_expand(png_ptr);
			if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
				png_set_expand(png_ptr);
			if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
				png_set_tRNS_to_alpha(png_ptr);
			if (bit_depth == 16)
				png_set_strip_16(png_ptr);
			if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
				png_set_gray_to_rgb(png_ptr);
			png_read_update_info(png_ptr, info_ptr);
			setjmp(png_jmpbuf(png_ptr));
			png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
			for (int y = 0; y < height; y++) {
				row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
			}
			png_read_image(png_ptr, row_pointers);
			step = (int)png_get_channels(png_ptr, info_ptr);
			size = height * width* step;
			image = new unsigned char[size]; // allocate 4 bytes per pixel
			int i = 0;
			for (int y = height-1; y >= 0; --y) {
				for (int x = 0; x < width*step; ++x) {
					image[i] = row_pointers[y][x];
					i = i + 1;
				}
				free(row_pointers[y]);
			}
			free(row_pointers);
		}
		fclose(f);
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
				tex[tmp] = load_image("../resources/menu_sprites/" + tmp);
			}
			f2.close();
		}


		ifstream f4("../resources/data/character_sprites.dat");
		while (f4.is_open()) {
			while (std::getline(f4, tmp)) {
				tex[tmp] = load_image("../resources/characters/" + tmp);
			}
			f4.close();
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
		chunk['\n'] = true;
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
	unsigned push_quad(float x, float y, float width, float height, GLuint texture, string filename = string("")) {
		quad q;
		q.x = x;
		q.y = y;
		q.width = width;
		q.height = height;
		q.tex = texture;
		if (texture == 0)
			q.filename = filename;
		draw_list.push_back(q);
		return draw_list.size() - 1;
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
		if (new_load) {
			new_load = false;
			std::map<string, GLuint>::iterator it;
			for (it = tex.begin(); it != tex.end(); it++) {
				if (it->second == 0)
					it->second = load_image(it->first);
			}
			for (unsigned i = 0; i < draw_list.size(); i++) {
				if (draw_list[i].tex == 0)
					draw_list[i].tex = tex[draw_list[i].filename];
			}
		}
		//Clear information from last draw
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
		glLoadIdentity(); //Reset the drawing perspective
		glColor3f(1.0f, 1.0f, 1.0f);
		for (unsigned i = 0; i < draw_list.size(); i++) {
			draw_quad(draw_list[i]);
		}
		glutSwapBuffers(); //Send the 3D scene to the screen
		glutPostRedisplay();
		m.unlock();
	}
	int next_chunk(string& s, int index) {
		unsigned ret = index + 1;
		unsigned skip = 0;
		while (!chunk[s[ret + skip]] && ret + skip < s.size()) {
			if (s[ret + skip] == '{') {
				while (s[ret + skip] != '}') {
					skip++;
				}
			}
			ret++;
		}
		ret--;
		return ret;
	}
	unsigned push_text(float x, float y, float width, float height, float size, string s) {
		string key;
		float x_curr = x;
		float y_curr = y + height;
		bool no_chunk_yet = true;
		bool double_spaced = true;
		if (s.find("{SINGLE}") == 0) {
			double_spaced = false;
			s.erase(0, string("{SINGLE}").length());
		}
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
			if (key != string("\n.bmp")) {
				if (key == string("space.bmp") && x_curr == x && y_curr != y + height) {
				}
				else {
					push_quad(x_curr, y_curr, size, size, tex[key]);
					x_curr += size;
				}
			}
			if ((key == string("\n.bmp")) || ((height != size) && (x_curr >= x + width || (!no_chunk_yet && ((float(next_chunk(s, i) - i))*size + x_curr > (x + width + 0.00001)))))) {
				no_chunk_yet = true;
				x_curr = x;
				y_curr -= size;
				if (double_spaced)
					y_curr -= size;
			}
			if (y_curr < y)
				return i + 1;
		}
		return s.size();
	}
	void push_box(float xmin, float ymin, float length, float height) {
		push_quad(xmin, ymin + height - 0.1f, 0.1f, 0.1f, tex[string("corner-ul.bmp")]);
		push_quad(xmin + length - 0.1f, ymin + height - 0.1f, 0.1f, 0.1f, tex[string("corner-ur.bmp")]);
		push_quad(xmin + 0.1f, ymin + height - 0.1f, length - 0.2f, 0.1f, tex[string("bar-top.bmp")]);
		push_quad(xmin, ymin + 0.1f, 0.1f, height - 0.2f, tex[string("bar-left.bmp")]);
		push_quad(xmin + length - 0.1f, ymin + 0.1f, 0.1f, height - 0.2f, tex[string("bar-right.bmp")]);
		push_quad(xmin + 0.1f, ymin + 0.1f, length - 0.2f, height - 0.2f, tex[string("space.bmp")]);
		push_quad(xmin, ymin, 0.1f, 0.1f, tex[string("corner-bl.bmp")]);
		push_quad(xmin + length - 0.1f, ymin, 0.1f, 0.1f, tex[string("corner-br.bmp")]);
		push_quad(xmin + 0.1f, ymin, length - 0.2f, 0.1f, tex[string("bar-bottom.bmp")]);
	}
	void push_arrow_box_left(float xmin, float ymin, float length, float height) {
		push_quad(xmin + length - 0.1f, ymin + 0.1f, 0.1f, height - 0.1f, tex[string("arrow-bar-vertical-right.bmp")]);
		push_quad(xmin, ymin, 0.1f, 0.1f, tex[string("arrow-left.bmp")]);
		push_quad(xmin + length - 0.1f, ymin, 0.1f, 0.1f, tex[string("arrow-bar-corner-ur.bmp")]);
		push_quad(xmin + 0.1f, ymin, length - 0.2f, 0.1f, tex[string("arrow-bar-horizontal.bmp")]);
	}
	void push_arrow_box_right(float xmin, float ymin, float length, float height) {
		push_quad(xmin, ymin + 0.1f, 0.1f, height - 0.1f, tex[string("arrow-bar-vertical-left.bmp")]);
		push_quad(xmin, ymin, 0.1f, 0.1f, tex[string("arrow-bar-corner-ul.bmp")]);
		push_quad(xmin + length - 0.1f, ymin, 0.1f, 0.1f, tex[string("arrow-right.bmp")]);
		push_quad(xmin + 0.1f, ymin, length - 0.2f, 0.1f, tex[string("arrow-bar-horizontal.bmp")]);
	}
	unsigned push_hp_bar(float xmin, float ymin, float hp) {
		unsigned ret;
		if (hp > 0.66f)
			ret = push_quad_load(xmin + 0.1f, ymin + 0.025f, 0.6f * hp, 0.05f, string("../resources/images/hp-good.bmp"));
		else if (hp > 0.33f)
			ret = push_quad_load(xmin + 0.1f, ymin + 0.025f, 0.6f * hp, 0.05f, string("../resources/images/hp-medium.bmp"));
		else
			ret = push_quad_load(xmin + 0.1f, ymin + 0.025f, 0.6f * hp, 0.05f, string("../resources/images/hp-bad.bmp"));
		push_quad_load(xmin - 0.1f, ymin, 0.2f, 0.1f, string("../resources/images/hp.png"));
		push_quad_load(xmin + 0.1f, ymin, 0.6f, 0.1f, string("../resources/images/hpbar-middle.png"));
		push_quad_load(xmin, ymin, 0.1f, 0.1f, string("../resources/images/hpbar-left.png"));
		push_quad_load(xmin + 0.7f, ymin, 0.1f, 0.1f, string("../resources/images/hpbar-right.png"));
		return ret;
	}
	unsigned push_quad_load(float x, float y, float width, float height, string filename) {
		if (!tex[filename]) {
			tex[filename] = load_image(filename);
			new_load = true;
		}
		return push_quad(x, y, width, height, tex[filename], filename);
	}
	void alert(string s) {
		push_box(-1.0f, -1.0f, 2.0f, 0.8f);
		unsigned x = 0;
		x = push_text(-0.9f, -0.9f, 1.8f, 0.4f, 0.1f, s);
		x = x + 1;
	}
};

#endif