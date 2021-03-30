#ifndef GRAPHICS_HEADER
#define GRAPHICS_HEADER

#ifdef __SWITCH__
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <glad/glad.h>
#else
#include "GL/glut.h"
#endif
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
extern mutex mut;
extern timer tim;
class quad {
public:
	double x;
	double y;
	double width;
	double height;
	bool half;
	GLuint tex;
	string filename;
	bool animated;
	string anim_filename;
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
	unsigned wobble_index;
	double wobble_counter;
	GLuint fbo;
	GLuint r_tex;
	GLuint VertexArrayID;
	GLuint FragmentShaderID;
	GLuint PostProgram;
	GLuint PreProgram;
	unsigned frame_anim_holder, fast_frame_anim_holder;
public:
	int frame, fast_frame;
	quad r_quad;
	quad r_effects;
	animation_engine ae;
	std::vector<GLuint> to_delete_textures;
	std::vector<GLuint> to_delete_buffers;
	std::vector<GLuint> to_delete_frames;
	std::vector<GLuint> to_delete_vertex_arrays;
	std::map<int, GLuint> tiles;
	std::map<string, GLuint> tex;
	std::map<string, string_lookup_data> string_lookup;
	std::map<string, bool> offset_letters;
	std::map<char, bool> chunk;
	std::vector<quad> draw_list;
	bool new_load;
	graphics() { new_load = false; }

	GLuint load_image(string filename);
	void cleanup();
	void load_tile(string filename, int index);

	//Initializes 3D rendering
	void initRendering();

	//Called when the window is resized
	void handleResize(int w, int h);
	unsigned push_quad(float x, float y, float width, float height, GLuint texture, string filename = string(""), bool animated = false);
	unsigned push_quad_half(float x, float y, float width, float height, GLuint texture, string filename = string(""));
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
	unsigned push_quad_load(float x, float y, float width, float height, string filename, bool animated = false);
	void alert(string s);
};

#endif