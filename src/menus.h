#ifndef MENU_HEADER
#define MENU_HEADER

#include <mutex>
#include <string>
#include "graphics.h"

class graphics;
class engine;
class level;
extern graphics g;
extern engine e;
extern mutex m;
extern string get_special_string(string in);
extern void push_hp_bar_if_exists(float x, float y, int index);
extern void push_storage_hp_bar_if_exists(float x, float y, int index);
extern string get_item_effect(string in);
extern int get_item_count(string in);
extern int get_item_storage_count(string in);
extern int get_item_cost(string in);
extern int get_team_size();
extern int get_storage_use();
extern int get_active_mon_move_size();
extern int get_mon_move_size(int index);
extern int get_storage_mon_move_size(int index);
extern int get_inventory_count(string type);
extern int get_inventory_storage_count(string type);
extern std::vector<float> get_map_coords();
extern std::vector<string> get_map_names();
extern std::map<string, bool> get_seen_table();
extern std::map<string, bool> get_caught_table();
extern bool is_menu(string s);
extern string get_menu(string r);
extern vector<int> do_menu(string menu);
#define STORAGE_MAX 20

enum MENUTYPE {
	NONE=0,
	ALERT,
	SELECT,
	AUTO_FOLLOWUP,
	SCROLL
};

class image {
public:
	string filename;
	float xmin, ymin, length, height;
};

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
private:
    string menuname;
    int anim_index;
    unsigned string_start, string_end;
    int string_stage;
    std::vector<quad> draw_list_copy;
	bool is_map;
public:
	string type;
	string icon;
	MENUTYPE etype;
	vector<image> images;
	vector<box> boxes, arrowboxes;
	vector<text> raw, display, hp_bars, storage_hp_bars;
	vector<string> followup, reserve, reserve_followup;
	int index, step, selection, columns, cursor, selection_cap, offset, cancel_option, scroll, max, replace_cancel;
	float cursor_offset_x, cursor_offset_y;
	bool done, always_cancel, initialized, autoclose, discard_choice;
	menu() { step = 0; index = 0; done = false; selection = 0; selection_cap = 0; columns = 1; cursor = -1; offset = 0; cancel_option = -1; cursor_offset_x = 0.0f; cursor_offset_y = 0.0; scroll = 0; max = 0; replace_cancel = -1; always_cancel = false; initialized = false; etype = NONE; anim_index = -1; autoclose = false; discard_choice = false; icon = string("cursor-2.bmp"); is_map = false; }
	void create_menu(string file, string choice = "", string text_override = "", string followup_override = "");
	void update_reserves();
	void push_menu();
	void pop_menu();
	void process_strings();
	void input(bool up, bool down, bool left, bool right, bool select, bool start, bool confirm, bool cancel);
	vector<int> main();
};

#endif