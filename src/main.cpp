#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "core.h"
using namespace std;

engine e;
void main() {
	srand((unsigned)time(NULL));
	e.init_types();
	e.init_special();
	e.init_status();
	e.init_moves();
	e.init_mon();
	mon m1, m2, m3, m4;
	bool crit = false;
	e.make_mon(string("1"), 1, m1);
	e.make_mon(string("1"), 50, m2);
	e.make_mon(string("1"), 100, m3);
	e.make_mon(string("76"), 100, m4);
	e.use_move(m3, m4, string("SPORE"));
	e.use_move(m3, m4, string("POISON POWDER"));
	e.use_move(m3, m4, string("EMBER"));
	e.use_move(m3, m4, string("EMBER"));
	e.use_move(m3, m4, string("EMBER"));
	e.use_status(m4, m3);
 	int a = 0;
}