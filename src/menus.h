#ifndef MENU_HEADER
#define MENU_HEADER

class graphics;
class engine;
extern graphics g;
extern engine e;
extern mutex m;

class box {
public:
	float xmin, ymin, length, height;
};

class text {
public:
	float xmin, ymin, length, height, size;
	string s;
	text() { size = 0.1f; }
};

class menu{
public:
	vector<box> boxes;
	vector<text> texts;
	int index, step;
	menu() { step = 0; index = 0; }
	void push_menu() {
		m.lock();
		index = g.draw_list.size();
		for (unsigned i = 0; i < boxes.size(); ++i) {
			g.push_box(boxes[i].xmin, boxes[i].ymin, boxes[i].length, boxes[i].height);
		}
		for (unsigned i = 0; i < texts.size(); ++i) {
			g.push_text(texts[i].xmin, texts[i].ymin, texts[i].length, texts[i].height, texts[i].size, texts[i].s);
		}
		m.unlock();
	}
	void pop_menu() {
		m.lock();
		g.draw_list.erase(g.draw_list.begin() + index, g.draw_list.end());
		m.unlock();
	}
};

#endif