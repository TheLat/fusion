#ifdef _WIN32
#include <Windows.h>
#include "GL/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#endif
#include <sstream>
#include "graphics.h"
#ifdef __SWITCH__
extern EGLDisplay s_display;
extern EGLContext s_context;
extern EGLSurface s_surface;
static GLuint s_vao, s_vbo;

typedef struct
{
	float position[3];
	float texcoord[2];
} Vertex;
#define vertex_list_count (sizeof(vertex_list)/sizeof(vertex_list[0]))
#endif
extern bool safe_getline(ifstream &f, string& s);

extern string safepath;
extern void input_tick(double deltat);
extern int resolution;
double tile_anim_dur = 48000.0;
static GLuint s_program = 0;

#ifdef __SWITCH__
static const char* const vertexShaderSource = R"text(
    #version 320 es
    precision mediump float;

    layout (location = 0) in vec3 inPos;
    layout (location = 1) in vec2 inTexCoord;

    out vec2 vtxTexCoord;

    void main()
    {
        // Calculate position
        vec4 pos = vec4(inPos, 1.0);
        gl_Position = pos;

        // Calculate texcoord
        vtxTexCoord = inTexCoord;
    }
)text";

static const char* const fragmentShaderSource = R"text(
    #version 320 es
    precision mediump float;

    in vec2 vtxTexCoord;

    out vec4 fragColor;

    uniform sampler2D tex_diffuse;

    void main()
    {
        vec4 texDiffuseColor = texture(tex_diffuse, vtxTexCoord);

        fragColor = vec4(texDiffuseColor.rgb, texDiffuseColor.a);
    }
)text";

static const char* const postprocessfragmentShaderSource = R"text(
    #version 320 es
    precision mediump float;

    in vec2 vtxTexCoord;

    out vec4 fragColor;

    uniform sampler2D tex_diffuse;

    void main()
    {
        vec4 texDiffuseColor = texture(tex_diffuse, vtxTexCoord);

        fragColor = vec4(texDiffuseColor.rgb, texDiffuseColor.a);
    }
)text";

static GLuint createAndCompileShader(GLenum type, const char* source)
{
	GLint success;
	GLchar msg[512];

	GLuint handle = glCreateShader(type);
	if (!handle)
	{
		printf("%u: cannot create shader", type);
		return 0;
	}
	glShaderSource(handle, 1, &source, nullptr);
	glCompileShader(handle);
	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(handle, sizeof(msg), nullptr, msg);
		printf("%u: %s\n", type, msg);
		glDeleteShader(handle);
		return 0;
	}

	return handle;
}
#endif

GLuint graphics::load_image(string filename) {
	GLuint ret = 0;
	int i;
	unsigned char* image = 0;
	FILE* f;
#ifdef __APPLE__
    f = fopen((filename).c_str(), "rb");
#endif
#ifdef __SWITCH__
	f = fopen((filename).c_str(), "rb");
#endif
#ifdef _WIN32
	fopen_s(&f, (filename).c_str(), "rb");
#endif
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
		i = 0;
		for (int y = height - 1; y >= 0; --y) {
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
#ifdef __SWITCH__
	glActiveTexture(GL_TEXTURE0);
#endif
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

void graphics::load_tile(string filename, int index) {
	tiles[index] = load_image(safepath + filename);
}

void graphics::initRendering() {
	//Makes 3D drawing work when something is in front of something else
	frame = 0;
	fast_frame = 0;
	frame_anim_holder = ae.create_animi(&frame, 0, 64000, tile_anim_dur);
	fast_frame_anim_holder = ae.create_animi(&fast_frame, 0, 64000, tile_anim_dur/2.5);
	time_index = tim.create();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ifstream f((safepath + string("data/level_sprites.dat")).c_str());
	string tmp;
	safe_getline(f, tmp);
	int total = stoi(tmp);
	f.close();
	for (int i = 0; i < total; ++i) {
		tmp = to_string(i);
		if (i < 100)
			tmp = '0' + tmp;
		if (i < 10)
			tmp = '0' + tmp;
		tmp = tmp + ".png";
		load_tile(string("level_sprites/") + tmp, i);
	}


	ifstream f2((safepath + string("data/menu_sprites.dat")).c_str());
	while (f2.is_open()) {
		while (safe_getline(f2, tmp)) {
			tex[tmp] = load_image((safepath + string("menu_sprites/") + tmp));
		}
		f2.close();
	}


	ifstream f4((safepath + string("data/character_sprites.dat")).c_str());
	while (f4.is_open()) {
		while (safe_getline(f4, tmp)) {
			tex[tmp] = load_image((safepath + string("characters/") + tmp));
		}
		f4.close();
	}

	ifstream f3((safepath + string("data/string_lookup.dat")).c_str());
	string l1, l2;
	while (f3.is_open()) {
		while (safe_getline(f3, tmp)) {
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

    ifstream f5((safepath + string("data/offset_letters.dat")).c_str());
	while (f5.is_open()) {
		while (safe_getline(f5, tmp)) {
			offset_letters[tmp] = true;
		}
		f5.close();
	}
	chunk[' '] = true;
	chunk['-'] = true;
	chunk['\n'] = true;
	// create frame buffer texture
#ifdef __APPLE__
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
#endif
#ifdef __SWITCH__
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
#endif
#ifdef _WIN32
	glewInit();
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER, fbo);
#endif
	glGenTextures(1, &r_tex);
#ifdef __SWITCH__
	glActiveTexture(GL_TEXTURE0);
#endif
    glBindTexture(GL_TEXTURE_2D, r_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, resolution,(resolution*9)/10, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef __APPLE__
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, r_tex, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
#ifdef __SWITCH__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, r_tex, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
#ifdef _WIN32
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, r_tex, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
#endif
#ifdef __SWITCH__
	r_quad.x = 0.625;
	r_quad.width = 1.25;
#else
    r_quad.x = -1.0;
	r_quad.width = 2.0;
#endif
    r_quad.y = -1.0;
    r_quad.height = 2.0;
    r_quad.tex = r_tex;
#ifdef __SWITCH__
	GLint vsh = createAndCompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLint fsh = createAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	s_program = glCreateProgram();
	glAttachShader(s_program, vsh);
	glAttachShader(s_program, fsh);
	glLinkProgram(s_program);
	//glDeleteShader(vsh);
	//glDeleteShader(fsh);
#endif

	GLint Result = GL_FALSE;
	int InfoLogLength;
    FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    string FragmentShaderCode;
	ifstream FragmentShaderStream((safepath + string("shaders/post.shader")).c_str(), std::ios::in);
	if(FragmentShaderStream.is_open()){
		stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}
	printf("Compiling shader : %s\n", (safepath + string("shaders/post.shader")).c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	printf("Linking program\n");
	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	glDetachShader(ProgramID, FragmentShaderID);
	glDeleteShader(FragmentShaderID);

	wobble_index = tim.create();
#ifdef __SWITCH__
	glGenVertexArrays(1, &s_vao);
	glGenBuffers(1, &s_vbo);
	glBindVertexArray(s_vao);
	glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
#endif
}

void graphics::handleResize(int w, int h) {
#ifdef __SWITCH__
#else
	//Tell OpenGL how to convert from coordinates to pixel values
	double ratio = 160.0 / 144.0;
	glViewport(0, 0, w, h);
	//glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
								 //Set the camera perspective
	//glLoadIdentity(); //Reset the camera
	gluOrtho2D(-1, 1, -1, 1);
#endif
}

unsigned graphics::push_quad(float x, float y, float width, float height, GLuint texture, string filename, bool animated) {
	quad q;
	q.x = x;
	q.y = y;
	q.width = width;
	q.height = height;
	q.tex = texture;
	q.half = false;
	q.animated = animated;
	if (texture == 0)
		q.filename = filename;
	if (animated) {
		q.anim_filename = filename;
		q.filename = filename;
		q.filename.insert(q.filename.find("-.") + 1, to_string(fast_frame % 8));
	}
	draw_list.push_back(q);
	return draw_list.size() - 1;
}

unsigned graphics::push_quad_half(float x, float y, float width, float height, GLuint texture, string filename) {
	quad q;
	q.x = x;
	q.y = y;
	q.width = width;
	q.height = height;
	q.tex = texture;
	q.half = true;
	q.animated = false;
	if (texture == 0)
		q.filename = filename;
	draw_list.push_back(q);
	return draw_list.size() - 1;
}

void graphics::draw_quad(quad &q) {
	glBindTexture(GL_TEXTURE_2D, q.tex);
#ifdef __SWITCH__
	GLuint text_loc = glGetUniformLocation(s_program, "tex_diffuse");
	glUniform1i(text_loc, q.tex);
	if (!q.half) {
		/*
		GLfloat squareVertices[] = {
			float(q.x), float(q.y), 0.0f,
			float(q.x + q.width), float(q.y), 0.0f,
			float(q.x + q.width), float(q.y + q.height), 0.0f,
			float(q.x),  float(q.y + q.height), 0.0f,
		};
		GLfloat textureVertices[] = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
		};
		*/
		Vertex vertex_list[] =
		{
			// First triangle
			{ { float(q.x),				float(q.y),				+0.5f },{ 0.0f, 0.0f } },
			{ { float(q.x + q.width),	float(q.y),				+0.5f },{ 1.0f, 0.0f } },
			{ { float(q.x + q.width),	float(q.y + q.height),	+0.5f },{ 1.0f, 1.0f } },
			// Second triangle
			{ { float(q.x + q.width),	float(q.y + q.height),	+0.5f },{ 1.0f, 1.0f } },
			{ { float(q.x),				float(q.y + q.height),	+0.5f },{ 0.0f, 1.0f } },
			{ { float(q.x),				float(q.y),				+0.5f },{ 0.0f, 0.0f } },
		};
		glBindVertexArray(s_vao);
		glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_list), vertex_list, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, vertex_list_count);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else {
		/*
		GLfloat squareVertices[] = {
			float(q.x), float(q.y + (q.height / 2.0)), 0.0f,
			float(q.x + q.width), float(q.y + (q.height/2.0)), 0.0f,
			float(q.x + q.width), float(q.y + q.height), 0.0f,
			float(q.x),  float(q.y + q.height), 0.0f,
		};
		GLfloat textureVertices[] = {
			0.0f, 0.5f,
			1.0f, 0.5f,
			1.0f, 1.0f,
			0.0f, 1.0f,
		};
		*/
	}
#else
	glBegin(GL_QUADS);
	if (!q.half) {
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(q.x, q.y, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(q.x + q.width, q.y, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(q.x + q.width, q.y + q.height, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(q.x, q.y + q.height, 0.0f);
	}
	else {
		glTexCoord2f(0.0f, 0.5f);
		glVertex3f(q.x, q.y + (q.height / 2.0), 0.0f);
		glTexCoord2f(1.0f, 0.5f);
		glVertex3f(q.x + q.width, q.y + (q.height / 2.0), 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(q.x + q.width, q.y + q.height, 0.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(q.x, q.y + q.height, 0.0f);
	}
	glEnd();
#endif
}

void graphics::animate() {
	double deltat = tim.delta(time_index);
	while (deltat < 1.0/120.0)
		deltat = tim.delta(time_index);
	tim.update(time_index);
	ae.tick(deltat);
	input_tick(deltat);
}

void graphics::drawScene() {
	tim.update(wobble_index);
	if (ae.is_donei(frame_anim_holder)) {
	    frame_anim_holder = ae.create_animi(&frame, 0, 64000, tile_anim_dur);
	}
	if (ae.is_donei(fast_frame_anim_holder)) {
		fast_frame_anim_holder = ae.create_animi(&fast_frame, 0, 64000, tile_anim_dur / 2.5);
	}
	while  (tim.delta(wobble_index) < 1.0/120.0) {
	}
	wobble_counter += tim.delta(wobble_index);
	std::vector<quad> draw_list_copy;
	mut.lock();
	for (unsigned i = 0; i < draw_list.size(); ++i) {
		if (draw_list[i].animated) {
			draw_list[i].filename = draw_list[i].anim_filename;
			draw_list[i].filename.insert(draw_list[i].filename.find("-.") + 1, to_string(fast_frame % 8));
			if (!tex[draw_list[i].filename]) {
				new_load = true;
			}
			draw_list[i].tex = tex[draw_list[i].filename];
		}
	}
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
	draw_list_copy = draw_list;
	mut.unlock();
	//Clear information from last draw
#ifdef __APPLE__
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
#endif
#ifdef __SWITCH__
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
#endif
#ifdef _WIN32
	glBindFramebufferEXT(GL_FRAMEBUFFER, fbo);
#endif
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	//glLoadIdentity(); //Reset the drawing perspective
	//glViewport(0,0,resolution,(resolution*9)/10);
	//glColor3f(1.0f, 1.0f, 1.0f);
	for (unsigned i = 0; i < draw_list_copy.size(); i++) {
		draw_quad(draw_list_copy[i]);
	}
	//glutSwapBuffers(); //Send the 3D scene to the screen
	//glutPostRedisplay();

#ifdef __APPLE__
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
#ifdef __SWITCH__
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
#ifdef _WIN32
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
#endif
    glUseProgram(ProgramID);
#ifdef __SWITCH__
	glUseProgram(s_program);
#endif
    GLuint invert_loc = glGetUniformLocation(ProgramID, "invert");
    glUniform1f(invert_loc, r_effects.x);
    GLuint contrast_loc = glGetUniformLocation(ProgramID, "contrast");
    glUniform1f(contrast_loc, r_effects.y+1.0);
    GLuint brightness_loc = glGetUniformLocation(ProgramID, "brightness");
    glUniform1f(brightness_loc, r_effects.width);
    GLuint offset_loc = glGetUniformLocation(ProgramID, "offset");
    glUniform1f(offset_loc, wobble_counter*50.0);
    GLuint wobble_loc = glGetUniformLocation(ProgramID, "wobble");
    glUniform1f(wobble_loc, 0.025 * r_effects.height);
    if (r_effects.height == 0.0)
        wobble_counter = 0.0;
	if (r_effects.x > 0.5)
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	else
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
#ifdef __SWITCH__
#else
	glClear(GL_COLOR_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	draw_quad(r_quad); // render screen texture to screen
#endif
#ifdef __SWITCH__
	eglSwapBuffers(s_display, s_surface);
#else
	glutSwapBuffers();
	glutPostRedisplay();
#endif
    glUseProgram(s_program);
}

int graphics::next_chunk(string& s, int index) {
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

unsigned graphics::push_text(float x, float y, float width, float height, float size, string s) {
	string key;
	float x_curr = x;
	float y_curr = y + height;
	bool no_chunk_yet = true;
	bool double_spaced = true;
	if (s.find("{SINGLE}") == 0) {
		double_spaced = false;
		s.erase(0, string("{SINGLE}").length());
	}
	bool newline = false;
	for (unsigned i = 0; i < s.size(); ++i) {
		if (newline) {
			while (i < s.size() && s[i] == ' ') {
				++i;
			}
			newline = false;
			if (i >= s.size()) {
				return i;
			}
		}
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
		if (key != string("{NEWLINE}.bmp")) {
			if (key == string("white.bmp") && x_curr == x && y_curr != y + height) {
			}
			else {
			    if (offset_letters[key])
    				push_quad(x_curr, y_curr-(2.0/144.0), size, size, tex[key]);
			    else
    				push_quad(x_curr, y_curr, size, size, tex[key]);
				x_curr += size;
			}
		}
		if ((key == string("{NEWLINE}.bmp")) || ((height != size) && (x_curr >= x + width || (!no_chunk_yet && ((float(next_chunk(s, i) - i))*size + x_curr > (x + width + 0.00001)))))) {
			no_chunk_yet = true;
			x_curr = x;
			y_curr -= size;
			if (double_spaced)
				y_curr -= size;
			newline = true;
		}
		if (y_curr < y)
			return i + 1;
	}
	return s.size();
}

void graphics::push_box(float xmin, float ymin, float length, float height) {
	push_quad(xmin, ymin + height - 0.1f, 0.1f, 0.1f, tex[string("corner-ul.bmp")]);
	push_quad(xmin + length - 0.1f, ymin + height - 0.1f, 0.1f, 0.1f, tex[string("corner-ur.bmp")]);
	push_quad(xmin + 0.1f, ymin + height - 0.1f, length - 0.2f, 0.1f, tex[string("bar-top.bmp")]);
	push_quad(xmin, ymin + 0.1f, 0.1f, height - 0.2f, tex[string("bar-left.bmp")]);
	push_quad(xmin + length - 0.1f, ymin + 0.1f, 0.1f, height - 0.2f, tex[string("bar-right.bmp")]);
	push_quad(xmin + 0.1f, ymin + 0.1f, length - 0.2f, height - 0.2f, tex[string("white.bmp")]);
	push_quad(xmin, ymin, 0.1f, 0.1f, tex[string("corner-bl.bmp")]);
	push_quad(xmin + length - 0.1f, ymin, 0.1f, 0.1f, tex[string("corner-br.bmp")]);
	push_quad(xmin + 0.1f, ymin, length - 0.2f, 0.1f, tex[string("bar-bottom.bmp")]);
}

void graphics::push_arrow_box_left(float xmin, float ymin, float length, float height) {
	push_quad(xmin + length - 0.1f, ymin + 0.1f, 0.1f, height - 0.1f, tex[string("arrow-bar-vertical-right.bmp")]);
	push_quad(xmin, ymin, 0.1f, 0.1f, tex[string("arrow-left.bmp")]);
	push_quad(xmin + length - 0.1f, ymin, 0.1f, 0.1f, tex[string("arrow-bar-corner-ur.bmp")]);
	push_quad(xmin + 0.1f, ymin, length - 0.2f, 0.1f, tex[string("arrow-bar-horizontal.bmp")]);
}

void graphics::push_arrow_box_right(float xmin, float ymin, float length, float height) {
	push_quad(xmin, ymin + 0.1f, 0.1f, height - 0.1f, tex[string("arrow-bar-vertical-left.bmp")]);
	push_quad(xmin, ymin, 0.1f, 0.1f, tex[string("arrow-bar-corner-ul.bmp")]);
	push_quad(xmin + length - 0.1f, ymin, 0.1f, 0.1f, tex[string("arrow-right.bmp")]);
	push_quad(xmin + 0.1f, ymin, length - 0.2f, 0.1f, tex[string("arrow-bar-horizontal.bmp")]);
}

unsigned graphics::push_hp_bar(float xmin, float ymin, float hp) {
	unsigned ret;
	if (hp > 0.66f)
		ret = push_quad_load(xmin + 0.1f, ymin + 0.025f, 0.6f * hp, 0.05f, safepath + string("images/hp-good.bmp"));
	else if (hp > 0.33f)
		ret = push_quad_load(xmin + 0.1f, ymin + 0.025f, 0.6f * hp, 0.05f, safepath + string("images/hp-medium.bmp"));
	else
		ret = push_quad_load(xmin + 0.1f, ymin + 0.025f, 0.6f * hp, 0.05f, safepath + string("images/hp-bad.bmp"));
	push_quad_load(xmin - 0.1f, ymin, 0.2f, 0.1f, safepath + string("images/hp.png"));
	push_quad_load(xmin + 0.1f, ymin, 0.6f, 0.1f, safepath + string("images/hpbar-middle.png"));
	push_quad_load(xmin, ymin, 0.1f, 0.1f, safepath + string("images/hpbar-left.png"));
	push_quad_load(xmin + 0.7f, ymin, 0.1f, 0.1f, safepath + string("images/hpbar-right.png"));
	return ret;
}

unsigned graphics::push_quad_load(float x, float y, float width, float height, string filename, bool animated) {
	string temp_filename = filename;
	if (animated) {
		temp_filename.insert(temp_filename.find("-.") + 1, to_string(fast_frame % 8));
	}
	if (!tex[temp_filename]) {
#ifdef __APPLE__
        tex[temp_filename] = 0;
#endif
#ifdef __SWITCH__
		tex[temp_filename] = 0;
#endif
#ifdef _WIN32
		tex[temp_filename] = load_image(temp_filename);
#endif
		new_load = true;
	}
	return push_quad(x, y, width, height, tex[temp_filename], filename, animated);
}

void graphics::alert(string s) {
	push_box(-1.0f, -1.0f, 2.0f, 0.8f);
	unsigned x = 0;
	x = push_text(-0.9f, -0.9f, 1.8f, 0.4f, 0.1f, s);
	x = x + 1;
}
