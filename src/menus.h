#ifndef MENU_HEADER
#define MENU_HEADER

class graphics;
class engine;
extern graphics g;
extern engine e;
extern mutex m;
extern string get_special_string(string in);
extern vector<int> do_menu(string menu);
mutex m2;

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
	vector<string> followup;
	int index, step, selection, columns, cursor;
	bool done;
	menu() { step = 0; index = 0; done = false; selection = 0; columns = 1; cursor = -1; }
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
	void create_combat_select() {
		box b;
		text t;
		columns = 2;
		type = "SELECT";
		b.xmin = -0.2f;
		b.length = 1.2f;
		b.ymin = -1.0f;
		b.height = 0.6f;
		boxes.clear();
		raw.clear();
		followup.clear();
		boxes.push_back(b);
		t.xmin = 0.0f;
		t.length = 0.6f;
		t.ymin = -0.8f;
		t.height = 0.1f;
		t.s = "FIGHT";
		raw.push_back(t);
		followup.push_back(string("FIGHT"));
		t.xmin = 0.6f;
		t.length = 0.3f;
		t.s = "{PK}{MN}";
		followup.push_back(string("POKEMON"));
		raw.push_back(t);
		t.xmin = 0.0f;
		t.length = 0.6f;
		t.ymin = -1.0f;
		t.height = 0.1f;
		t.s = "ITEM";
		followup.push_back(string("COMBAT_ITEM"));
		raw.push_back(t);
		t.xmin = 0.6f;
		t.length = 0.3f;
		t.s = "RUN";
		followup.push_back(string(""));
		raw.push_back(t);
		process_strings();
	}
	void create_move_select() {
		box b;
		text t;
		columns = 1;
		type = "SELECT";
		b.xmin = -0.6f;
		b.length = 1.6f;
		b.ymin = -1.0f;
		b.height = 0.6f;
		boxes.clear();
		raw.clear();
		followup.clear();
		boxes.push_back(b);
		for (int i = 0; i < 4; ++i) {
			t.xmin = -0.4f;
			t.length = 1.4f;
			t.ymin = -0.7f - (0.1f*(float(i)));
			t.height = 0.1f;
			t.s = get_special_string(string("ACTIVE_POKEMON_MOVE:") + to_string(i));
			raw.push_back(t);
		}
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
		m2.lock();
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
		if (type == "SELECT") {
			if (up) {
				if (selection < columns) {
					selection = raw.size() - (columns - selection);
				}
				else {
					selection -= columns;
				}
			}
			if (down) {
				if (selection >= int(raw.size()) - columns) {
					selection = selection + columns - raw.size();
				}
				else {
					selection += columns;
				}
			}
			if (right) {
				if (columns > 1) {
					if (selection % columns < columns - 1) {
						selection++;
					}
					else {
						selection--;
					}
				}
			}
			if (left) {
				if (columns > 1) {
					if (selection % columns == 0) {
						selection += columns - 1;
					}
					else {
						selection--;
					}
				}
			}
			if (confirm | start) {
				done = true;
			}
			if (cancel) {
				selection = -1;
				done = true;
			}
		}
		m2.unlock();
	}
	vector<int> main() {
		vector<int> choice;
		while (!done || (done && (selection < followup.size()) && (followup[selection] != ""))) {
			m2.lock();
			if (type == "SELECT" && cursor == -1) {
				cursor = g.draw_list.size();
				g.push_quad(display[0].xmin - 0.1f, display[0].ymin - 0.1f, 0.1f, 0.1f, g.menu_tex[string("cursor-2.bmp")]);
			}
			if ((cursor > 0) && (selection != -1)) {
				g.draw_list[cursor].x = display[selection].xmin - 0.1f;
				g.draw_list[cursor].y = display[selection].ymin + 0.1f;
			}
			m2.unlock();
			if (done && (followup.size() > 0) && (followup[selection] == "FIGHT")) {
				vector<int> out;
				out = do_menu(string("FIGHT"));
				if ((out.size() == 0) || (out[out.size() - 1] == -1)) {
					done = false;
				}
				else {
					choice = out;
					break;
				}
			}
			int a = 0;
		}
		pop_menu();
		choice.insert(choice.begin(), selection);
		return choice;
	}
};

#endif