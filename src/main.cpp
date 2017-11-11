#include <mutex>
#include <thread>
#include "graphics.h"
#include "core.h"
#include "menus.h"


using namespace std;
int num = 0;
mutex m;
engine e;
graphics g;

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
std::map<string, bool> get_seen_table() {
	return e.mc.seen;
}
std::map<string, bool> get_caught_table() {
	return e.mc.caught;
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
		ifstream f(string("../resources/menus/") + s + string(".dat"));
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
int get_mon_move_size(int index){
	int ret = 0;
	for (int i = 0; i < 4; ++i) {
		if (e.get_special_string(string("MON_MOVE:") + to_string(index) + string(":") + to_string(i)) != "-")
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

vector<int> do_menu(string menu) {
	vector<int> def;
	string temp;
	if (menu.find(string(":")) != -1) {
		temp = menu;
		menu = menu.erase(menu.find(string(":")), menu.length());
		temp = temp.erase(0, temp.find(":") + 1);
	}
	ifstream f(string("../resources/menus/") + menu + string(".dat"));
	if (!f.good())
		return def;
	f.close();
	return e.do_menu(menu, temp);
}

// Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {
	printf("%i\n", int(key));
	num++;
	switch (key) {
	case 27: //Escape key
		exit(0); // TODO:  Make this not crash on closing.
		break;
	case 'a':
		e.input(false, false, true, false, false, false, false, false);
		break;
	case 'd':
		e.input(false, false, false, true, false, false, false, false);
		break;
	case 's':
		e.input(false, true, false, false, false, false, false, false);
		break;
	case 'w':
		e.input(true, false, false, false, false, false, false, false);
		break;
	case 'j':
		e.input(false, false, false, false, false, false, true, false);
		break;
	case 'k':
		e.input(false, false, false, false, false, false, false, true);
		break;
	case 'm':
		e.input(false, false, false, false, false, true, false, false);
		break;
	case ',':
		e.input(false, false, false, false, true, false, false, false);
		break;
	}
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

int main(int argc, char** argv) {
	srand((unsigned)time(NULL));
	e.init_types();
	e.init_special();
	e.init_exp();
	e.init_status();
	e.init_moves();
	e.init_mon();
	e.init_levels();
	e.init_blocking();
	e.init_jumpdown();
	e.init_swimming();
	e.init_items();
	e.init_hm();
	e.init_tm();

	e.mc.loc.x = 6.0;
	e.mc.loc.y = 4.0;
	e.mc.loc.level = "pallet-town";
	e.mc.money = 3000;
	e.load_game();

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(160*4, 144*4); //Set the window size
	//Create the window
	glutCreateWindow("Pokémon Fusion");
	g.initRendering(); //Initialize renderingg
	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	thread t1(core_main);
	glutMainLoop(); //Start the main loop

	return 0;
}
