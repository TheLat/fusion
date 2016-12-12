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

string get_special_string(string in) {
	return e.get_special_string(in);
}

vector<int> do_menu(string menu) {
	vector<int> def;
	if (menu == "FIGHT")
		return e.do_move_select();
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
	int l = 49;
	e.make_mon(string("4"), l, e.mc.team[0]);

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
