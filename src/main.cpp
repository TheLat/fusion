#ifdef __SWITCH__
#include <switch.h> 
#include <unistd.h>
#define TRACE(fmt,...) printf("%s: " fmt "\n", __PRETTY_FUNCTION__, ## __VA_ARGS__)
#endif
#include <mutex>
#include <iostream>
#include <fstream>
#include <thread>
#include <ctime>
#include "graphics.h"
#include "core.h"
#include "menus.h"
#include "sound.h"
#include "timer.h"

void log(const char *format, const char *value = 0) {
	if (value) {
		printf(format, value);
	}
	else {
		printf(format);
	}
}

using namespace std;
mutex mut;
engine e;
graphics g;
soundengine se;
timer tim;
extern string safepath;
int resolution;

const char* windowTitle = "Pokemon Fusion";

bool safe_getline(ifstream &f, string& s) {
	bool ret = std::getline(f, s) ? true : false;
	while (s.find(string("\r")) != -1)
		s.erase(s.find(string("\r")), 1);
	while (s.find(string("\n")) != -1)
		s.erase(s.find(string("\n")), 1);
	return ret;
}

int get_storage_use() {
	int ret = 0;
	for (int i = 0; i < STORAGE_MAX; ++i) {
		if (e.mc.storage[e.mc.box_number][i].defined)
			ret++;
		else
			break;
	}
	return ret;
}

int get_team_size() {
	int ret = 0;
	for (int i = 0; i < 6; ++i) {
		if (e.mc.team[i].defined)
			ret++;
		else
			break;
	}
	return ret;
}

std::vector<float> find_map_coords(string this_level) {
	std::vector<float> out;
	std::vector<string> to_traverse;
	std::vector<string> yet_to_traverse;
	std::map<string, bool> traversed;
	yet_to_traverse.push_back(this_level);
	while (yet_to_traverse.size() != 0) {
		to_traverse = yet_to_traverse;
		yet_to_traverse.clear();
		for (unsigned i = 0; i < to_traverse.size(); ++i) {
			traversed[to_traverse[i]] = true;
			if (e.levels[to_traverse[i]].map_order != 0) {
				out.push_back(e.levels[to_traverse[i]].map.x);
				out.push_back(e.levels[to_traverse[i]].map.y);
				return out;
			}
			for (unsigned j = 0; j < e.levels[to_traverse[i]].teleport.size(); ++j) {
				if (!traversed[e.levels[to_traverse[i]].teleport[j].second.level]) {
					yet_to_traverse.push_back(e.levels[to_traverse[i]].teleport[j].second.level);
					traversed[e.levels[to_traverse[i]].teleport[j].second.level] = true;
				}
			}
		}
		to_traverse.clear();
	}
	return out;
}
std::vector<float> find_player_coords() {
	return find_map_coords(e.mc.loc.level);
}
std::vector<float> find_mon_coords(int type) {
	std::vector<float> out;
	std::vector<float> holder;
	std::map<string, level>::iterator it;
	for (it = e.levels.begin(); it != e.levels.end(); it++) {
		if (it->second.nomap)
			continue;
		holder = find_map_coords(it->first);
		if (holder.size() == 0)
			continue;
		for (unsigned i = 0; i < it->second.encounters.size(); ++i) {
			if (it->second.encounters[i] == type && holder.size() != 0) {
				out.push_back(holder[0]);
				out.push_back(holder[1]);
			}
		}
		for (unsigned i = 0; i < it->second.mega_encounters.size(); ++i) {
			if (it->second.mega_encounters[i] == type && holder.size() != 0) {
				out.push_back(holder[0]);
				out.push_back(holder[1]);
			}
		}
		for (unsigned i = 0; i < it->second.water_encounters_old.size(); ++i) {
			if (it->second.water_encounters_old[i] == type && holder.size() != 0) {
				out.push_back(holder[0]);
				out.push_back(holder[1]);
			}
		}
		for (unsigned i = 0; i < it->second.water_encounters_good.size(); ++i) {
			if (it->second.water_encounters_good[i] == type && holder.size() != 0) {
				out.push_back(holder[0]);
				out.push_back(holder[1]);
			}
		}
		for (unsigned i = 0; i < it->second.water_encounters_super.size(); ++i) {
			if (it->second.water_encounters_super[i] == type && holder.size() != 0) {
				out.push_back(holder[0]);
				out.push_back(holder[1]);
			}
		}
	}
	return out;
}

std::vector<float> get_map_coords(bool only_include_fly) {
	std::vector<float> out;
	std::map<string, level>::iterator it;
	bool found;
	unsigned total = 0;
	for (it = e.levels.begin(); it != e.levels.end(); it++) {
		if (it->second.map_order >= total)
			total = it->second.map_order + 1;
	}
	for (unsigned i = 1; i < total; ++i) {
		found = false;
		for (it = e.levels.begin(); it != e.levels.end(); it++) {
			if (it->second.map_order == i) {
				found = true;
				if (only_include_fly && (it->second.fly_loc.level == "" || !e.mc.visited[it->first]))
					break;
				out.push_back(it->second.map.x);
				out.push_back(it->second.map.y);
				break;
			}
		}
		if (!found) {
			break;
		}
	}
	return out;
}
std::vector<string> get_map_names(bool only_include_fly, bool data_name) {
	std::vector<string> out;
	std::map<string, level>::iterator it;
	bool found;
	unsigned total = 0;
	for (it = e.levels.begin(); it != e.levels.end(); it++) {
		if (it->second.map_order >= total)
			total = it->second.map_order + 1;
	}
	for (unsigned i = 1; i < total; ++i) {
		found = false;
		for (it = e.levels.begin(); it != e.levels.end(); it++) {
			if (it->second.map_order == i) {
				found = true;
				if (only_include_fly && (it->second.fly_loc.level == "" || !e.mc.visited[it->first]))
					break;
				if (data_name)
					out.push_back(it->first);
				else
					out.push_back(it->second.map.level);
				break;
			}
		}
		if (!found) {
			break;
		}
	}
	return out;
}
std::map<string, bool> get_seen_table() {
	return e.mc.seen;
}
std::map<string, bool> get_caught_table() {
	return e.mc.caught;
}

int get_inventory_storage_count(string type) {
	int ret = 0;
	bool found;
	for (unsigned i = 0; i < e.mc.inventory_storage.size(); ++i) {
		found = false;
		if (type == "ALL")
			found = true;
		for (unsigned j = 0; j < e.items[e.mc.inventory_storage[i].first].use.size(); ++j) {
			if (e.items[e.mc.inventory_storage[i].first].use[j] == type) {
				found = true;
			}
		}
		if (found)
			ret++;
	}
	return ret;
}

int get_inventory_count(string type) {
	int ret = 0;
	bool found;
	for (unsigned i = 0; i < e.mc.inventory.size(); ++i) {
		found = false;
		if (type == "ALL")
			found = true;
		for (unsigned j = 0; j < e.items[e.mc.inventory[i].first].use.size(); ++j) {
			if (e.items[e.mc.inventory[i].first].use[j] == type) {
				found = true;
			}
		}
		if (found)
			ret++;
	}
	return ret;
}

string get_menu(string r) {
	while (r != "") {
		string s = r;
		if (s.find("|") != -1) {
			s.erase(s.find("|"), s.size());
			r.erase(0, r.find("|") + 1);
		}
		else {
			r = "";
		}
		if (s.find(":") != -1)
			s.erase(s.find(":"), s.size());
		ifstream f(safepath + string("menus/") + s + string(".dat"));
		if (f.is_open()) {
			f.close();
			return s;
		}
	}
	return r;
}

bool is_menu(string s) {
	if (get_menu(s) == "")
		return false;
	return true;
}

void push_hp_bar_if_exists(float x, float y, int index) {
	e.push_hp_bar_if_exists(x, y, index);
}

void push_storage_hp_bar_if_exists(float x, float y, int index) {
	e.push_storage_hp_bar_if_exists(x, y, index);
}
int get_mon_move_size(int index){
	int ret = 0;
	for (int i = 0; i < 4; ++i) {
		if (e.get_special_string(string("MON_MOVE:") + to_string(index) + string(":") + to_string(i)) != "-")
			ret++;
	}
	return ret;
}
int get_storage_mon_move_size(int index){
	int ret = 0;
	for (int i = 0; i < 4; ++i) {
		if (e.get_special_string(string("STORAGE_MON_MOVE:") + to_string(index) + string(":") + to_string(i)) != "-")
			ret++;
	}
	return ret;
}

int get_active_mon_move_size() {
	int ret = 0;
	for (int i = 0; i < 4; ++i) {
		if (e.get_special_string(string("ACTIVE_MON_MOVE:") + to_string(i)) != "-")
			ret++;
	}
	return ret;
}

string get_special_string(string in) {
	return e.get_special_string(in);
}

string get_item_effect(string in) {
	return e.get_item_effect(in);
}


int get_item_count(string in) {
	return e.get_inventory_count(in);
}
int get_item_storage_count(string in) {
	return e.get_inventory_storage_count(in);
}
int get_item_cost(string in) {
	return e.get_item_cost(in);
}

vector<int> do_menu(string menu) {
	vector<int> def;
	string temp;
	if (menu.find(string(":")) != -1) {
		temp = menu;
		menu = menu.erase(menu.find(string(":")), menu.length());
		temp = temp.erase(0, temp.find(":") + 1);
	}
	ifstream f(safepath + string("menus/") + menu + string(".dat"));
	if (!f.good())
		return def;
	f.close();
	return e.do_menu(menu, temp);
}

void handleResize(int w, int h) {
	g.handleResize(w, h);
}

void drawScene() {
	g.drawScene();
}

void core_main() {
	while (true) {
		e.main();
	}
}

void animate() {
	while (true) {
		g.animate();
	}
}

#ifdef __SWITCH__
static void setMesaConfig()
{
	// Uncomment below to disable error checking and save CPU time (useful for production):
	//setenv("MESA_NO_ERROR", "1", 1);

	// Uncomment below to enable Mesa logging:
	//setenv("EGL_LOG_LEVEL", "debug", 1);
	//setenv("MESA_VERBOSE", "all", 1);
	//setenv("NOUVEAU_MESA_DEBUG", "1", 1);

	// Uncomment below to enable shader debugging in Nouveau:
	//setenv("NV50_PROG_OPTIMIZE", "0", 1);
	//setenv("NV50_PROG_DEBUG", "1", 1);
	//setenv("NV50_PROG_CHIPSET", "0x120", 1);
}


EGLDisplay s_display;
EGLContext s_context;
EGLSurface s_surface;

static bool initEgl(NWindow* win)
{
	// Connect to the EGL default display
	s_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (!s_display)
	{
		TRACE("Could not connect to display! error: %d", eglGetError());
		goto _fail0;
	}

	// Initialize the EGL display connection
	eglInitialize(s_display, nullptr, nullptr);

	// Select OpenGL (Core) as the desired graphics API
	if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
	{
		TRACE("Could not set API! error: %d", eglGetError());
		goto _fail1;
	}

	// Get an appropriate EGL framebuffer configuration
	EGLConfig config;
	EGLint numConfigs;
	static const EGLint framebufferAttributeList[] =
	{
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_RED_SIZE,     8,
		EGL_GREEN_SIZE,   8,
		EGL_BLUE_SIZE,    8,
		EGL_ALPHA_SIZE,   8,
		EGL_DEPTH_SIZE,   24,
		EGL_STENCIL_SIZE, 8,
		EGL_NONE
	};
	eglChooseConfig(s_display, framebufferAttributeList, &config, 1, &numConfigs);
	if (numConfigs == 0)
	{
		TRACE("No config found! error: %d", eglGetError());
		goto _fail1;
	}

	// Create an EGL window surface
	s_surface = eglCreateWindowSurface(s_display, config, win, nullptr);
	if (!s_surface)
	{
		TRACE("Surface creation failed! error: %d", eglGetError());
		goto _fail1;
	}

	// Create an EGL rendering context
	static const EGLint contextAttributeList[] =
	{
		EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
		EGL_CONTEXT_MAJOR_VERSION_KHR, 4,
		EGL_CONTEXT_MINOR_VERSION_KHR, 3,
		EGL_NONE
	};
	s_context = eglCreateContext(s_display, config, EGL_NO_CONTEXT, contextAttributeList);
	if (!s_context)
	{
		TRACE("Context creation failed! error: %d", eglGetError());
		goto _fail2;
	}

	// Connect the context to the surface
	eglMakeCurrent(s_display, s_surface, s_surface, s_context);
	return true;

_fail2:
	eglDestroySurface(s_display, s_surface);
	s_surface = nullptr;
_fail1:
	eglTerminate(s_display);
	s_display = nullptr;
_fail0:
	return false;
}
#endif

int main(int argc, char *argv[])
{
#ifdef __SWITCH__
	socketInitializeDefault();
	nxlinkStdio();
	setMesaConfig();
	if (!initEgl(nwindowGetDefault()))
		return EXIT_FAILURE;
	gladLoadGL();
	Result rc = romfsInit();
	if (R_FAILED(rc))
		log("romfsInit: %08X\n", to_string(rc).c_str());
#endif
	e.mc.values[string("SFXVOLUME")] = 4;
	e.mc.values[string("MUSICVOLUME")] = 4;
	log("Loading types...");
	e.init_types();
	log("Done!\n");
	log("Loading special effects...");
	e.init_special();
	log("Done!\n");
	log("Loading EXP table...");
	e.init_exp();
	log("Done!\n");
	log("Loading status table...");
	e.init_status();
	log("Done!\n");
	log("Loading move database...");
	e.init_moves();
	log("Done!\n");
	log("Loading blocking tiles...");
	e.init_blocking();
	e.init_npc_blocking();
	log("Done!\n");
	log("Loading animating tiles...");
	e.init_animating();
	log("Done!\n");
	log("Loading jumping tiles...");
	e.init_jumpdown();
	e.init_jumpleft();
	e.init_jumpright();
	log("Done!\n");
	log("Loading slide tiles...");
	e.init_slide();
	log("Done!\n");
	log("Loading encounter tiles...");
	e.init_encounter_tiles();
	log("Done!\n");
	log("Loading grass tiles...");
	e.init_grass();
	log("Done!\n");
	log("Loading draw over tiles...");
	e.init_draw_over();
	log("Done!\n");
	log("Loading swimming tiles...");
	e.init_swimming();
	log("Done!\n");
	log("Loading items...");
	e.init_items();
	log("Done!\n");
	log("Loading HM and TM definitions...");
	e.init_hm();
	e.init_tm();
	log("Done!\n");
	log("Loading mon database...");
	e.init_mon();
	log("Done!\n");
	log("Loading levels...");
	e.init_levels();
	log("Done!\n");
	log("Loading sounds...");
	se.init_sounds();
	log("Done!\n");
	log("Initializing game clock...");
	e.init_game_timer();
	log("Done!\n");
	resolution = e.get_resolution();


#ifdef __SWITCH__
	g.initRendering(); //Initialize rendering
	e.init_characters();
	thread t1(core_main);
	thread t2(animate);
	while (appletMainLoop())
	{
		drawScene();
	}
#else
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(resolution, (resolution*9)/10); //Set the window size
	//Create the window
	glutCreateWindow(windowTitle);
	g.initRendering(); //Initialize rendering
	e.init_characters();
	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutReshapeFunc(handleResize);
	thread t1(core_main);
	thread t2(animate);
	glutMainLoop(); //Start the main loop
#endif

	return 0;
}
