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
	e.make_mon(string("1"), 100, m1);
	e.make_mon(string("7"), 100, m2);
	m1.queue.push_back(m1.moves[0]);
	m2.queue.push_back(m2.moves[3]);
	e.get_order(m1, m2);
	m2.queue.push_back(m2.moves[3]);
	e.get_order(m1, m2);
	m1.queue.push_back(m1.moves[0]);
	m2.queue.push_back(m2.moves[3]);
	e.get_order(m1, m2);
	m2.queue.push_back(m2.moves[3]);
	e.get_order(m1, m2);
 	int a = 0;
}