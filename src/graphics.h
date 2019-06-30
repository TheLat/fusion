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
#include <mutex>
#include "timer.h"
#include "anim.h"

using namespace std;
extern mutex m;
extern timer tim;
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
private:
	unsigned time_index;
	GLuint fbo;
	GLuint r_tex;
	GLuint VertexArrayID;
	GLuint FragmentShaderID;
	GLuint ProgramID;
public:
	quad r_quad;
	quad r_effects;
	animation_engine ae;
	std::map<int, GLuint> tiles;
	std::map<string, GLuint> tex;
	std::map<string, string_lookup_data> string_lookup;
	std::map<char, bool> chunk;
	std::vector<quad> draw_list;
	bool new_load;
	graphics() { new_load = false; }

	GLuint load_image(string filename);
	void load_tile(string filename, int index);

	//Initializes 3D rendering
	void initRendering();

	//Called when the window is resized
	void handleResize(int w, int h);
	unsigned push_quad(float x, float y, float width, float height, GLuint texture, string filename = string(""));
	void draw_quad(quad &q);

	//Draws the scene
	void animate();
	void drawScene();
	int next_chunk(string& s, int index);
	unsigned push_text(float x, float y, float width, float height, float size, string s);
	void push_box(float xmin, float ymin, float length, float height);
	void push_arrow_box_left(float xmin, float ymin, float length, float height);
	void push_arrow_box_right(float xmin, float ymin, float length, float height);
	unsigned push_hp_bar(float xmin, float ymin, float hp);
	unsigned push_quad_load(float x, float y, float width, float height, string filename);
	void alert(string s);
};

#endif