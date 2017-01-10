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

string get_special_string(string in) {
	return e.get_special_string(in);
}

vector<int> do_menu(string menu) {
	vector<int> def;
	int choice = 0;
	if (menu.find(string(":")) != -1) {
		string temp = menu;
		menu = menu.erase(menu.find(string(":")), menu.length());
		temp = temp.erase(0, temp.find(":") + 1);
		choice = stoi(temp);
	}
	if (menu == "FIGHT")
		return e.do_move_select();
	else if (menu == "COMBAT_ITEM")
		return e.do_combat_item_select();
	else if (menu == "COMBAT_MON")
		return e.do_combat_mon_select();
	else if (menu == "COMBAT_SWITCH_CONFIRM")
		return e.do_combat_switch_confirm(choice);
	else if (menu == "STATS")
		return e.do_stats(choice);
	return def;
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
	int l = 29;
	e.make_mon(string("6"), l, e.mc.team[0]);
	e.mc.team[0].wild = false;
	e.apply_status(e.mc.team[0], string("BURN"));
	e.apply_status(e.mc.team[0], string("POISON"));
	e.apply_status(e.mc.team[0], string("FREEZE"));
	e.apply_status(e.mc.team[0], string("PARALYZE"));

	e.mc.loc.x = 2.0;
	e.mc.loc.y = 2.0;
	e.current_level = "pallet-town";

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
