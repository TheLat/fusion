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
	string type;
	vector<box> boxes;
	vector<text> raw, display;
	int index, step;
	bool done;
	menu() { step = 0; index = 0; done = false; }
	void create_alert(string s) {
		box b;
		text t;
		type = "ALERT";
		b.xmin = -1.0f;
		b.length = 2.0f;
		b.ymin = -1.0f;
		b.height = 0.8f;
		t.xmin = -0.9f;
		t.length = 1.8f;
		t.ymin = -0.9f;
		t.height = 0.4f;
		t.s = s;
		boxes.clear();
		raw.clear();
		boxes.push_back(b);
		raw.push_back(t);
		process_strings();
	}
	void push_menu() {
		m.lock();
		index = g.draw_list.size();
		for (unsigned i = 0; i < boxes.size(); ++i) {
			g.push_box(boxes[i].xmin, boxes[i].ymin, boxes[i].length, boxes[i].height);
		}
		for (unsigned i = 0; i < display.size(); ++i) {
			step = g.push_text(display[i].xmin, display[i].ymin, display[i].length, display[i].height, display[i].size, display[i].s);
		}
		m.unlock();
	}
	void pop_menu() {
		m.lock();
		g.draw_list.erase(g.draw_list.begin() + index, g.draw_list.end());
		m.unlock();
	}
	void process_strings() {
		display.clear();
		for (unsigned i = 0; i < raw.size(); ++i) {
			display.push_back(raw[i]); // TODO:  Process special keys
		}
	}
	void input(bool up, bool down, bool left, bool right, bool select, bool start, bool confirm, bool cancel) {
		if (type == "ALERT") {
			if (start | select | confirm | cancel) {
				if (step == raw[0].s.size()) {
					done = true;
				}
				else {
					raw[0].s.erase(0, step);
					process_strings();
					pop_menu();
					push_menu();
				}
			}
		}
	}
	void main() {
		while (!done) {
			int a = 0;
		}
		pop_menu();
	}
};

#endif