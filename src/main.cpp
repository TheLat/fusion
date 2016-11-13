#include "core.h"
#include "graphics.h"

using namespace std;
int num = 0;
engine e;
graphics g;

// Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {
	printf("%i\n", int(key));
	num++;
	switch (key) {
	case 27: //Escape key
		exit(0);
		break;
	case 'a':
		if (e.mc.loc.x == 0.0f)
			break;
		if (e.blocking[e.levels[e.current_level].data[int(e.mc.loc.y)][int(e.mc.loc.x - 1)]])
			break;
		e.mc.loc.x -= 1.0f;
		break;
	case 'd':
		if (e.mc.loc.x + 1 >= e.levels[e.current_level].data[int(e.mc.loc.y)].size())
			break;
		if (e.blocking[e.levels[e.current_level].data[int(e.mc.loc.y)][int(e.mc.loc.x + 1)]])
			break;
		e.mc.loc.x += 1.0f;
		break;
	case 's':
		if (e.mc.loc.y + 1 >= e.levels[e.current_level].data.size())
			break;
		if (e.mc.loc.x >= e.levels[e.current_level].data[int(e.mc.loc.y + 1)].size())
			break;
		if (e.blocking[e.levels[e.current_level].data[int(e.mc.loc.y + 1)][int(e.mc.loc.x)]])
			break;
		e.mc.loc.y += 1.0f;
		break;
	case 'w':
		if (e.mc.loc.y == 0)
			break;
		if (e.mc.loc.x >= e.levels[e.current_level].data[int(e.mc.loc.y - 1)].size())
			break;
		if (e.blocking[e.levels[e.current_level].data[int(e.mc.loc.y - 1)][int(e.mc.loc.x)]])
			break;
		e.mc.loc.y -= 1.0f;
		break;
	}
}



void handleResize(int w, int h) {
	g.handleResize(w, h);
}

void drawScene() {
	g.drawScene();
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
	e.make_mon(string("51"), 50, e.mc.team[0]);

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
	glutMainLoop(); //Start the main loop

	return 0;
}
