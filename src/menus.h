#ifndef MENU_HEADER
#define MENU_HEADER

class graphics;
class engine;
extern graphics g;
extern engine e;
extern mutex m;
extern string get_special_string(string in);
extern int get_team_size();
extern int get_active_mon_move_size();
extern int get_mon_move_size(int index);
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
	void create_menu(string file, int choice = 0, string str_choice = "") {
		box b;
		text t;
		boxes.clear();
		arrowboxes.clear();
		raw.clear();
		followup.clear();
		ifstream f(string("../resources/menus/") + file + string(".dat"));
		string line, temp1, temp2;
		while (f.is_open()) {
			while (std::getline(f, line)){
				temp1 = line;
				temp2 = line;
				if (temp1.find(":") != -1) {
					temp1.erase(temp1.find(":"), temp1.length());
					temp2.erase(0, temp2.find(":") + 1);
					if (temp1 == "TYPE") {
						type = temp2;
					}
					else if (temp1 == "COLUMNS") {
						columns = stoi(temp2);
					}
					else if (temp1 == "CANCEL_OPTION") {
						cancel_option = stoi(temp2);
					}
					else if (temp1 == "CURSOR_OFFSET_X") {
						cursor_offset_x = stof(temp2);
					}
					else if (temp1 == "CURSOR_OFFSET_Y") {
						cursor_offset_y = stof(temp2);
					}
					else if (temp1 == "SELECTION_CAP") {
						if (temp2.find("{CHOICE}") != -1) {
							temp2.insert(temp2.find("{CHOICE}"), to_string(choice));
							temp2.erase(temp2.find("{CHOICE}"), string("{CHOICE}").length());
						}
						if (temp2 == "{ACTIVE_MON_MOVE_SIZE}") {
							selection_cap = get_active_mon_move_size();
						}
						else if (temp2 == "{TEAM_SIZE}") {
							selection_cap = get_team_size();
						}
						else if (temp2.find("{MON_MOVE_COUNT:") == 0) {
							temp2.erase(temp2.find("{MON_MOVE_COUNT:"), string("{MON_MOVE_COUNT:").length());
							selection_cap = get_mon_move_size(stoi(temp2));
						}
						else
							selection_cap = stoi(temp2);
					}
					else if (temp1 == "BOXES") {
						std::getline(f, line);
						while (line != "END") {
							temp1 = line;
							temp1.erase(temp1.find(" "), temp1.length());
							line.erase(0, line.find(" ") + 1);
							b.xmin = stof(temp1);
							temp1 = line;
							temp1.erase(temp1.find(" "), temp1.length());
							line.erase(0, line.find(" ") + 1);
							b.ymin = stof(temp1);
							temp1 = line;
							temp1.erase(temp1.find(" "), temp1.length());
							line.erase(0, line.find(" ") + 1);
							b.length = stof(temp1);
							temp1 = line;
							b.height = stof(temp1);
							boxes.push_back(b);
							std::getline(f, line);
						}
					}
					else if (temp1 == "ARROWBOXES") {
						std::getline(f, line);
						while (line != "END") {
							temp1 = line;
							temp1.erase(temp1.find(" "), temp1.length());
							line.erase(0, line.find(" ") + 1);
							b.xmin = stof(temp1);
							temp1 = line;
							temp1.erase(temp1.find(" "), temp1.length());
							line.erase(0, line.find(" ") + 1);
							b.ymin = stof(temp1);
							temp1 = line;
							temp1.erase(temp1.find(" "), temp1.length());
							line.erase(0, line.find(" ") + 1);
							b.length = stof(temp1);
							temp1 = line;
							b.height = stof(temp1);
							arrowboxes.push_back(b);
							std::getline(f, line);
						}
					}
					else if (temp1 == "TEXT") {
						std::getline(f, line);
						while (line != "END") {
							temp1 = line;
							temp1.erase(temp1.find(" "), temp1.length());
							line.erase(0, line.find(" ") + 1);
							t.xmin = stof(temp1);
							temp1 = line;
							temp1.erase(temp1.find(" "), temp1.length());
							line.erase(0, line.find(" ") + 1);
							t.ymin = stof(temp1);
							temp1 = line;
							temp1.erase(temp1.find(" "), temp1.length());
							line.erase(0, line.find(" ") + 1);
							t.length = stof(temp1);
							temp1 = line;
							temp1.erase(temp1.find(" "), temp1.length());
							line.erase(0, line.find(" ") + 1);
							t.height = stof(temp1);
							t.s = line;
							if (t.s.find("{CHOICE}") != -1) {
								t.s.insert(t.s.find("{CHOICE}"), to_string(choice));
								t.s.erase(t.s.find("{CHOICE}"), string("{CHOICE}").length());
							}
							raw.push_back(t);
							std::getline(f, line);
						}
					}
					else if (temp1 == "FOLLOW_UP") {
						std::getline(f, line);
						while (line != "END") {
							if (line.find("{CHOICE}") != -1) {
								line.insert(line.find("{CHOICE}"), to_string(choice));
								line.erase(line.find("{CHOICE}"), string("{CHOICE}").length());
							}
							if (line.find(":") != -1) {
								string s1 = line;
								string s2 = line;
								s1.erase(line.find(":"), s1.length());
								s2.erase(0, line.find(":") + 1);
								s2 = get_special_string(s2);
								line = s1 + string(":") + s2;
							}
							followup.push_back(line);
							std::getline(f, line);
						}
						break;
					}
				}
			}
			f.close();
		}
		process_strings();
	}
	void create_moves(int choice) {
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
		b.length = 2.0f;
		b.height = 1.0f;
		boxes.push_back(b);
		b.xmin = -0.2f;
		b.ymin = 0.0f;
		b.length = 1.2f;
		b.height = 0.9f;
		arrowboxes.push_back(b);
		t.height = 0.1f;
		t.length = 1.8f;
		t.xmin = -0.8f;
		t.ymin = -0.1f;
		for (int i = 0; i < 4; ++i) {
			t.ymin -= 0.2f;
			t.s = string("MON_MOVE:") + to_string(choice) + string(":") + to_string(i);
			followup.push_back(string("MOVE_DEFINITION:") + get_special_string(string("MON_MOVE:") + to_string(choice) + string(":") + to_string(i)));
			raw.push_back(t);
			if (get_special_string(t.s) != "-")
				selection_cap = i + 1;
		}
		t.xmin = -0.1f;
		t.ymin = -0.2f;
		for (int i = 0; i < 4; ++i) {
			t.ymin -= 0.2f;
			t.s = string("MON_MOVE_PP:") + to_string(choice) + string(":") + to_string(i);
			raw.push_back(t);
		}
		t.xmin = 0.0f;
		t.ymin = -0.2f;
		for (int i = 0; i < 4; ++i) {
			t.ymin -= 0.2f;
			t.s = string("MON_MOVE_PP_IF_EXISTS:") + to_string(choice) + string(":") + to_string(i);
			raw.push_back(t);
		}
		t.xmin = -0.5f;
		t.length = 1.4f;
		t.ymin = 0.7f;
		t.height = 0.1f;
		t.s = string("TEAM_MON_NAME_RIGHT_JUSTIFIED:") + to_string(choice);
		raw.push_back(t);
		t.xmin = -0.9f;
		t.length = 0.8f;
		t.ymin = -0.1f;
		t.s = string("TEAM_MON_NUMBER:") + to_string(choice);
		raw.push_back(t);
		t.xmin = -0.1f;
		t.length = 1.4f;
		t.ymin = 0.3f;
		t.height = 0.1f;
		t.s = string("EXP POINTS");
		raw.push_back(t);
		t.ymin = 0.2f;
		t.s = string("EXP_POINTS:") + to_string(choice);
		raw.push_back(t);
		t.ymin = 0.1f;
		t.s = string("LEVEL_UP");
		raw.push_back(t);
		t.ymin = 0.0f;
		t.s = string("LEVEL_UP:") + to_string(choice);
		raw.push_back(t);
		t.xmin = 0.2f;
		t.ymin = 0.4f;
		t.length = 0.7f;
		t.s = string("TEAM_MON_FORMATTED_HP:") + to_string(choice);
		raw.push_back(t);
		t.xmin = 0.4f;
		t.ymin = 0.6f;
		t.s = string("TEAM_MON_LEVEL:") + to_string(choice);
		raw.push_back(t);
		process_strings();
	}

	void create_move_definition(string in) {
		box b;
		text t;
		columns = 1;
		cancel_option = 0;
		type = "SELECT";
		selection_cap = 1;
		boxes.clear();
		arrowboxes.clear();
		raw.clear();
		followup.clear();
		b.ymin = -1.0f;
		b.xmin = -1.0f;
		b.length = 1.5f;
		b.height = 2.0f;
		boxes.push_back(b);
		t.height = 0.1f;
		t.length = 1.0f;
		t.ymin = -2.0f;
		t.xmin = -2.0f;
		t.s = "Done.";
		raw.push_back(t);
		t.height = 0.1f;
		t.length = 1.3f;
		t.xmin = -0.9f;
		t.ymin = 0.7f;
		t.s = "MOVE";
		raw.push_back(t);
		t.ymin = 0.6f;
		t.s = string("RIGHT_JUSTIFY:13:") + in;
		raw.push_back(t);
		t.ymin = 0.5f;
		t.s = "TYPE";
		raw.push_back(t);
		t.ymin = 0.4f;
		t.s = string("RIGHT_JUSTIFY:13:") + get_special_string(string("MOVE_TYPE:") + in);
		raw.push_back(t);
		t.ymin = 0.3f;
		t.s = "POWER";
		raw.push_back(t);
		t.ymin = 0.2f;
		t.s = string("RIGHT_JUSTIFY:13:") + get_special_string(string("MOVE_POWER:") + in);
		raw.push_back(t);
		t.ymin = 0.1f;
		t.s = "ACCURACY";
		raw.push_back(t);
		t.ymin = 0.0f;
		t.s = string("RIGHT_JUSTIFY:13:") + get_special_string(string("MOVE_ACC:") + in);
		raw.push_back(t);
		t.ymin = -0.1f;
		t.s = "DEFENSE";
		raw.push_back(t);
		t.ymin = -0.2f;
		t.s = string("RIGHT_JUSTIFY:13:") + get_special_string(string("MOVE_DEFENSE:") + in);
		raw.push_back(t);
		t.ymin = -0.3f;
		t.s = "NOTES";
		raw.push_back(t);
		t.ymin = -0.9f;
		t.height = 0.6f;
		t.s = string("{SINGLE}") + get_special_string(string("MOVE_NOTES:") + in);
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