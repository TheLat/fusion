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



using namespace std;
mutex m;
engine e;
graphics g;
soundengine se;
timer tim;
extern string safepath;

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

int main(int argc, char** argv) {
	srand((unsigned)time(NULL));
	e.mc.values[string("SFXVOLUME")] = 4;
	e.mc.values[string("MUSICVOLUME")] = 4;
	printf("Loading types...");
	e.init_types();
	printf("Done!\n");
	printf("Loading special effects...");
	e.init_special();
	printf("Done!\n");
	printf("Loading EXP table...");
	e.init_exp();
	printf("Done!\n");
	printf("Loading status table...");
	e.init_status();
	printf("Done!\n");
	printf("Loading move database...");
	e.init_moves();
	printf("Done!\n");
	printf("Loading blocking tiles...");
	e.init_blocking();
	e.init_npc_blocking();
	printf("Done!\n");
	printf("Loading animating tiles...");
	e.init_animating();
	printf("Done!\n");
	printf("Loading jumping tiles...");
	e.init_jumpdown();
	e.init_jumpleft();
	e.init_jumpright();
	printf("Done!\n");
	printf("Loading slide tiles...");
	e.init_slide();
	printf("Done!\n");
	printf("Loading encounter tiles...");
	e.init_encounter_tiles();
	printf("Done!\n");
	printf("Loading grass tiles...");
	e.init_grass();
	printf("Done!\n");
	printf("Loading draw over tiles...");
	e.init_draw_over();
	printf("Done!\n");
	printf("Loading swimming tiles...");
	e.init_swimming();
	printf("Done!\n");
	printf("Loading items...");
	e.init_items();
	printf("Done!\n");
	printf("Loading HM and TM definitions...");
	e.init_hm();
	e.init_tm();
	printf("Done!\n");
	printf("Loading mon database...");
	e.init_mon();
	printf("Done!\n");
	printf("Loading levels...");
	e.init_levels();
	printf("Done!\n");
	printf("Loading sounds...");
	se.init_sounds();
	printf("Done!\n");
	printf("Initializing game clock...");
	e.init_game_timer();
	printf("Done!\n");



	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(160*4, 144*4); //Set the window size
	//Create the window
	glutCreateWindow("Pokemon Fusion");
	g.initRendering(); //Initialize rendering
	e.init_characters();
	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutReshapeFunc(handleResize);
	thread t1(core_main);
	thread t2(animate);
	glutMainLoop(); //Start the main loop

	return 0;
}
