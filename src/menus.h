#ifndef MENU_HEADER
#define MENU_HEADER

class graphics;
class engine;
extern graphics g;
extern engine e;
extern mutex m;
extern string get_special_string(string in);
extern int get_team_size();
extern vector<int> do_menu(string menu);
mutex m2;

class box {
public:
	bool right;
	float xmin, ymin, length, height;
	box() { right = false; }
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
	vector<box> boxes, arrowboxes;
	vector<text> raw, display;
	vector<string> followup;
	int index, step, selection, columns, cursor, selection_cap, offset, cancel_option;
	float cursor_offset_x, cursor_offset_y;
	bool done;
	menu() { step = 0; index = 0; done = false; selection = 0; selection_cap = 0; columns = 1; cursor = -1; offset = 0; cancel_option = -1; cursor_offset_x = 0.0f; cursor_offset_y = 0.0; }
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
		arrowboxes.clear();
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
		boxes.clear();
		arrowboxes.clear();
		raw.clear();
		followup.clear();
		b.xmin = -0.2f;
		b.length = 1.2f;
		b.ymin = -1.0f;
		b.height = 0.6f;
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
		followup.push_back(string("COMBAT_MON"));
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
		selection_cap = 4;
		followup.push_back(string(""));
		raw.push_back(t);
		process_strings();
	}
	void create_stats(int choice) {
		box b;
		text t;
		columns = 1;
		type = "SELECT";
		selection_cap = 1;
		boxes.clear();
		arrowboxes.clear();
		raw.clear();
		followup.clear();
		b.ymin = -1.1f;
		b.height = 2.2f;
		b.xmin = -1.1f;
		b.length = 2.2f;
		boxes.push_back(b);
		b.ymin = -1.0f;
		b.xmin = -1.0f;
		b.length = 1.0f;
		b.height = 1.0f;
		boxes.push_back(b);
		b.xmin = 0.2f;
		b.length = 0.8f;
		b.height = 0.9f;
		arrowboxes.push_back(b);
		b.xmin = -0.2f;
		b.ymin = 0.0f;
		b.length = 1.2f;
		b.height = 0.9f;
		arrowboxes.push_back(b);
		t.height = 0.1f;
		t.length = 1.0f;
		t.ymin = -2.0f; //HACK
		t.xmin = -2.0f;
		t.s = "Next screen.";
		followup.push_back(string("MOVES"));
		raw.push_back(t);
		t.xmin = -0.9f;
		t.length = 0.8f;
		t.ymin = -0.3f;
		t.s = "ATTACK";
		raw.push_back(t);
		t.ymin = -0.5f;
		t.s = "DEFENSE";
		raw.push_back(t);
		t.ymin = -0.7f;
		t.s = "SPEED";
		raw.push_back(t);
		t.ymin = -0.9f;
		t.s = "SPECIAL";
		raw.push_back(t);
		t.xmin = -0.9f;
		t.length = 0.8f;
		t.ymin = -0.4f;
		t.s = string("TEAM_MON_ATTACK:") + to_string(choice);
		raw.push_back(t);
		t.ymin = -0.6f;
		t.s = string("TEAM_MON_DEFENSE:") + to_string(choice);
		raw.push_back(t);
		t.ymin = -0.8f;
		t.s = string("TEAM_MON_SPEED:") + to_string(choice);
		raw.push_back(t);
		t.ymin = -1.0f;
		t.s = string("TEAM_MON_SPECIAL:") + to_string(choice);
		raw.push_back(t);
		t.xmin = -0.1f;
		t.ymin = 0.0f;
		t.s = string("STATUS_BOOL:") + to_string(choice);
		raw.push_back(t);
		t.xmin = 0.4f;
		t.ymin = 0.6f;
		t.s = string("TEAM_MON_LEVEL:") + to_string(choice);
		raw.push_back(t);
		t.xmin = 0.0f;
		t.ymin = -0.2f;
		t.s = string("TYPE1/");
		raw.push_back(t);
		t.xmin = 0.1f;
		t.ymin = -0.3f;
		t.s = string("TEAM_MON_TYPE1:") + to_string(choice);
		raw.push_back(t);
		t.xmin = 0.0f;
		t.ymin = -0.4f;
		t.s = string("TYPE2_IF_EXISTS:") + to_string(choice);
		raw.push_back(t);
		t.xmin = 0.1f;
		t.ymin = -0.5f;
		t.s = string("TEAM_MON_TYPE2:") + to_string(choice);
		raw.push_back(t);
		t.xmin = 0.0f;
		t.ymin = -0.6f;
		t.s = string("ST%ATUS/");
		raw.push_back(t);
		t.xmin = 0.1f;
		t.ymin = -1.1f;
		t.height = 0.5f;
		t.length = 0.9f;
		t.s = string("TEAM_MON_STATUS:") + to_string(choice);
		raw.push_back(t);
		t.xmin = -0.5f;
		t.length = 1.4f;
		t.ymin = 0.7f;
		t.height = 0.1f;
		t.s = string("TEAM_MON_NAME_RIGHT_JUSTIFIED:") + to_string(choice);
		raw.push_back(t);
		t.xmin = 0.2f;
		t.ymin = 0.4f;
		t.length = 0.7f;
		t.s = string("TEAM_MON_FORMATTED_HP:") + to_string(choice);
		raw.push_back(t);
		t.xmin = -0.9f;
		t.length = 0.8f;
		t.ymin = -0.1f;
		t.s = string("TEAM_MON_NUMBER:") + to_string(choice);
		raw.push_back(t);
		process_strings();
	}
	void create_combat_switch_confirm(int choice) {
		box b;
		text t;
		columns = 1;
		type = "SELECT";
		selection_cap = 3;
		cancel_option = 2;
		boxes.clear();
		arrowboxes.clear();
		raw.clear();
		followup.clear();
		// TODO:  Sync formatting with game
		b.xmin = 0.1f;
		b.length = 0.9f;
		b.ymin = -1.0f;
		b.height = 0.7f;
		boxes.push_back(b);
		t.xmin = 0.3f;
		t.length = 0.6f;
		t.ymin = -0.6f;
		t.height = 0.1f;
		t.s = string("SWITCH");
		raw.push_back(t);
		followup.push_back(string(""));
		t.ymin -= 0.2f;
		t.s = string("STATS");
		followup.push_back(string("STATS:") + to_string(choice));
		raw.push_back(t);
		t.ymin -= 0.2f;
		t.s = string("CANCEL");
		followup.push_back(string(""));
		raw.push_back(t);
		process_strings();
	}
	void create_combat_mon_select() {
		box b;
		text t;
		columns = 1;
		type = "SELECT";
		cursor_offset_x = -0.2f;
		cursor_offset_y = -0.05f;
		selection_cap = get_team_size();
		boxes.clear();
		arrowboxes.clear();
		raw.clear();
		followup.clear();
		b.ymin = -1.1f;
		b.height = 2.2f;
		b.xmin = -1.1f;
		b.length = 2.2f;
		boxes.push_back(b);
		b.ymin = -1.0f;
		b.height = 0.8f;
		b.xmin = -1.0f;
		b.length = 2.0f;
		boxes.push_back(b);
		t.xmin = -0.7f;
		t.length = 1.4f;
		t.ymin = 1.0f;
		t.height = 0.1f;
		for (int i = 0; i < 6; ++i) {
			t.ymin -= 0.2f;
			t.s = string("TEAM_MON_NAME:") + to_string(i);
			raw.push_back(t);
			followup.push_back(string("COMBAT_SWITCH_CONFIRM:") + to_string(i));
		}
		t.ymin = 1.0f;
		t.xmin = 0.7f;
		t.length = 0.2f;
		for (int i = 0; i < 6; ++i) {
			t.ymin -= 0.2f;
			t.s = string("TEAM_MON_LEVEL:") + to_string(i);
			raw.push_back(t);
		}
		t.ymin = 0.9f;
		t.length = 1.0f;
		t.xmin = 0.3;
		for (int i = 0; i < 6; ++i) {
			t.ymin -= 0.2f;
			t.s = string("TEAM_MON_FORMATTED_HP:") + to_string(i);
			raw.push_back(t);
		}
		t.xmin = -0.9f;
		t.length = 1.8f;
		t.ymin = -0.6f;
		t.s = string("Choose a POK{e-accent}MON.");
		raw.push_back(t);
		process_strings();
	}
	void create_combat_item_select() {
		box b;
		text t;
		columns = 1;
		type = "SELECT";
		boxes.clear();
		arrowboxes.clear();
		raw.clear();
		followup.clear();
		selection_cap = 5;
		cancel_option = 4; // TODO:  Make this be intentory.size()
		// TODO: Make go off actual inventory and give numbers
		b.xmin = -0.6f;
		b.length = 1.6f;
		b.ymin = -0.5f;
		b.height = 1.3f;
		boxes.push_back(b);
		t.xmin = -0.4f;
		t.length = 1.4f;
		t.ymin = 0.4f;
		t.height = 0.1f;
		t.s = "POK{e-accent} BALL";
		raw.push_back(t);
		t.ymin -= 0.2f;
		t.s = "GREAT BALL";
		raw.push_back(t);
		t.ymin -= 0.2f;
		t.s = "ULTRA BALL";
		raw.push_back(t);
		t.ymin -= 0.2f;
		t.s = "MASTER BALL";
		raw.push_back(t);
		t.ymin -= 0.2f;
		t.s = "CANCEL"; // TODO:  Figure out how to handle this correctly
		raw.push_back(t);


		process_strings();
	}
	void create_combat_move_select() {
		box b;
		text t;
		columns = 1;
		type = "SELECT";
		boxes.clear();
		arrowboxes.clear();
		raw.clear();
		followup.clear();
		b.xmin = -0.6f;
		b.length = 1.6f;
		b.ymin = -1.0f;
		b.height = 0.6f;
		boxes.push_back(b);
		for (int i = 0; i < 4; ++i) {
			t.xmin = -0.4f;
			t.length = 1.4f;
			t.ymin = -0.7f - (0.1f*(float(i)));
			t.height = 0.1f;
			t.s = string("ACTIVE_MON_MOVE:") + to_string(i);
			if (get_special_string(t.s) != "-") {
				selection_cap = i + 1;
			}
			raw.push_back(t);
		}
		// Move description
		b.xmin = -1.0f;
		b.length = 1.1f;
		b.ymin = -0.4f;
		b.height = 0.5f;
		boxes.push_back(b);

		t.xmin = -0.9f;
		t.length = 0.9f;
		t.ymin = -0.2;
		t.height = 0.1f;
		t.s = string("TYPE/");
		raw.push_back(t);
		t.xmin = -0.8f;
		t.length = 0.8f;
		t.ymin = -0.3;
		t.s = string("ACTIVE_MON_MOVE_TYPE:SELECTION");
		raw.push_back(t);
		t.xmin = -0.9f;
		t.length = 0.9f;
		t.ymin = -0.4;
		t.height = 0.1f;
		t.s = string("ACTIVE_MON_MOVE_PP:SELECTION");
		raw.push_back(t);
		process_strings();
	}
	void push_menu() {
		m.lock();
		index = g.draw_list.size();
		for (unsigned i = 0; i < boxes.size(); ++i) {
			g.push_box(boxes[i].xmin, boxes[i].ymin, boxes[i].length, boxes[i].height);
		}
		for (unsigned i = 0; i < arrowboxes.size(); ++i) {
			if (arrowboxes[i].right)
				g.push_arrow_box_right(arrowboxes[i].xmin, arrowboxes[i].ymin, arrowboxes[i].length, arrowboxes[i].height);
			else
				g.push_arrow_box_left(arrowboxes[i].xmin, arrowboxes[i].ymin, arrowboxes[i].length, arrowboxes[i].height);
		}
		for (unsigned i = 0; i < display.size(); ++i) {
			step = g.push_text(display[i].xmin, display[i].ymin, display[i].length, display[i].height, display[i].size, display[i].s);
		}
		m.unlock();
	}
	void pop_menu() {
		m.lock();
		cursor = -1;
		g.draw_list.erase(g.draw_list.begin() + index, g.draw_list.end());
		m.unlock();
	}
	void process_strings() {
		display.clear();
		for (unsigned i = 0; i < raw.size(); ++i) {
			display.push_back(raw[i]);
			if (display[i].s.find("SELECTION") != -1) {
				display[i].s.erase(display[i].s.find("SELECTION"), string("SELECTION").length());
				display[i].s = display[i].s + to_string(selection);
			}
			display[i].s = get_special_string(display[i].s);
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
					selection = selection_cap - (columns - selection);
				}
				else {
					selection -= columns;
				}
			}
			if (down) {
				if (selection >= selection_cap - columns) {
					selection = selection + columns - selection_cap;
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
			process_strings();
			pop_menu();
			push_menu();
		}
		m2.unlock();
	}
	vector<int> main() {
		vector<int> choice;
		while (!done || (done && (selection >= 0) && (selection < (int)followup.size()) && (followup[selection] != ""))) {
			m2.lock();
			if (type == "SELECT" && cursor == -1) {
				cursor = g.draw_list.size();
				g.push_quad(display[0].xmin - 0.1f + cursor_offset_x, display[0].ymin - 0.1f + cursor_offset_y, 0.1f, 0.1f, g.menu_tex[string("cursor-2.bmp")]);
			}
			if ((cursor > 0) && (selection != -1)) {
				g.draw_list[cursor].x = display[selection].xmin - 0.1f + cursor_offset_x;
				g.draw_list[cursor].y = display[selection].ymin + 0.1f + cursor_offset_y;
			}
			m2.unlock();
			if (done && (selection >= 0) && (followup.size() > 0)) {
				vector<int> out;
				if (followup[selection] == "")
					break;
				out = do_menu(followup[selection]);
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
		if ((choice.size() > 0) && (choice[0] == offset + cancel_option))
			choice[0] = -1;
		return choice;
	}
};

#endif