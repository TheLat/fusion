#ifndef CORE_HEADER
#define CORE_HEADER

#include <map>
#include <vector>
#include <string>
#include <math.h>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "menus.h"
#include "anim.h"

using namespace std;

class graphics;
extern graphics g;
extern mutex mut;

extern bool is_menu(string s);

enum STAT {
	HP = 0,
	ATTACK,
	DEFENSE,
	SPECIAL,
	SPEED,
	SIZE
};

enum direction {
	DOWN = 0,
	UP = 1,
	LEFT = 2,
	RIGHT = 3
};

class location {
public:
	double x;
	double y;
	string level;
	bool allow_npcs;
	bool allow_player;
};

class item {
public:
	string name;
	vector<string> use;
	vector<string> effect;
	int price;
	bool defined;
	item() { price = 0; defined = false; }
};

class power {
public:
	string name;
	string type;
	string pow;
	string notes;
	string animation;
	STAT attack, defense;
	int acc;
	int pp;
	float crit_chance;
	string desc;
	vector<string> queue;
	vector<string> additional;
	vector<string> self;
	vector<string> target;
	vector<string> special;
	bool defined;
	bool queue_only;
	power() { acc = 100; attack = ATTACK; defense = DEFENSE; crit_chance = 1.0f; pow = "0"; pp = 0; defined = false; queue_only = false; animation = "";}
};

class mon_template {
public:
	string number, name, type_1, type_2, dex, body;
	int stats[SIZE];
	int exp_yield, catchrate, height;
	double weight;
	vector<pair<int, string> > learned;
	vector<pair<string, string> > evolution;
	std::map<int, bool> TM, HM;
	bool defined;
	mon_template() { defined = false; }
};

class mon {
public:
	string number;
	bool defined;
	bool enemy;
	bool wild;
	int IV[SIZE];
	int EV[SIZE];
	int curr_hp;
	int level;
	int exp;
	int turn_count;
	int pp[4];
	int max_pp[4];
	int disabled_move;
	int last_damage;
	int last_hit_special;
	int last_hit_physical;
	int stored_hp;
	string moves[4];
	vector<string> status;
	vector<string> queue;
	string nickname, last_move;
	unsigned hp_bar_index, exp_bar_index;
	unsigned hud_index;
	unsigned sprite_index;
	vector<int> damage_dealt;
	map<int, bool> fought;
	mon* original;
	mon() { defined = false; disabled_move = -1; last_damage = 0; original = 0; hp_bar_index = 0; }
	mon& operator=(mon& m) {
		for (int i = 0; i < SIZE; ++i) {
			this->IV[i] = m.IV[i];
			this->EV[i] = m.EV[i];
		}
		for (int i = 0; i < 4; ++i) {
			this->pp[i] = m.pp[i];
			this->max_pp[i] = m.max_pp[i];
			this->moves[i] = m.moves[i];
		}
		this->number = m.number;
		this->wild = m.wild;
		this->curr_hp = m.curr_hp;
		this->level = m.level;
		this->exp = m.exp;
		this->turn_count = m.turn_count;
		this->nickname = m.nickname;
		this->status.clear();
		for (unsigned i = 0; i < m.status.size(); ++i) {
			this->status.push_back(m.status[i]);
		}
		this->queue.clear();
		for (unsigned i = 0; i < m.queue.size(); ++i) {
			this->queue.push_back(m.queue[i]);
		}
		this->defined = m.defined;
		this->enemy = true;
		return *this;
	}
};

class player {
public:
	string name, rivalname, movement;
	direction dir;
	int wins, losses;
	int money, coins;
	int repel;
	int selected;
	int enemy_selected;
	int extra_winnings;
	int box_number;
	int frame;
	int game_time;
	mon team[6];
	mon enemy_team[6];
	mon storage[STORAGE_MAX][STORAGE_MAX];
	mon daycare;
	int daycare_bill;
	location loc, last_center, last_non_dungeon, last_loc;
	std::map<string, bool> active;
	std::map<string, unsigned> interaction;
	std::vector<pair<string, int> > inventory;
	std::vector<pair<string, int> > inventory_storage;
	std::map<string, unsigned> values;
	std::map<string, bool> seen;
	std::map<string, bool> caught;
	std::map<string, bool> used_tms;
	std::map<string, bool> visited;
	std::vector<string> to_alert;
	player() { wins = 0; losses = 0; money = 0; name = ""; rivalname = ""; repel = 0; selected = 0; enemy_selected = 0; box_number = 0; movement = string("player"); frame = 0; game_time = 0; daycare_bill = 0; }
};

class status_effect {
public:
	bool defined;
	bool chance;
	bool nonvolatile;
	bool singleton;
	bool specialcase;
	string name;
	status_effect() { defined = false; chance = false; nonvolatile = false; name = string(""); singleton = false; specialcase = false; }
};

class character {
public:
	direction dir, origin_dir;
	string image;
	string name; // MUST BE UNIQUE
	vector<string> interactions;
	vector<location> force_interactions;
	location loc, origin, anim_offset;
	double height, width;
	int frame;
	GLuint images[16];
	bool wander, incorporeal, no_force, nolook, far, no_offset, pushable, teleportable, assigned_facing, no_fall, wide;
	double delay;
	character() { wander = false; dir = DOWN; incorporeal = false; no_force = false; nolook = false; far = false; no_offset = false; pushable = false; teleportable = false; frame = 0; anim_offset.x = 0.0; anim_offset.y = 0.0; assigned_facing = false; delay = 0.0; width = 1.0 / 5.0; height = 1.0 / 4.5f; wide = false; }
};

class trainer {
public:
	string display_name;
	string name;
	string image;
	int payout;
	float skill, knowledge;
	string lose_message;
	string win_message;
	bool skip_accuracy_check;
	bool overswitch;
	bool tutorial;
	int no_switch;
	bool bossfight;
	bool finalboss;
	bool max_ev;
	mon team[6];
};

class neighbor {
public:
	string level;
	double x, y;
};

class level {
public:
	string name;
	std::vector<std::vector<int> > data;
	std::vector<std::pair<location, location> > teleport;
	std::vector<neighbor> neighbors;
	std::vector<character> characters;
	std::vector<int> encounters, water_encounters_old, water_encounters_good, water_encounters_super, mega_encounters;
	std::pair<int, int> level_range, mega_level_range;
	std::map<string, trainer> trainers;
	string music, leavesound;
	location map, fly_loc;
	int map_order;
	bool dungeon, dark, inside, nomap;
};

class engine {
public:
	std::map<string, std::map<string, float> > types;
	std::map<string, bool> special_case;
	std::vector<int> level_to_exp;
	std::map<string, status_effect> status;
	std::map<string, mon_template> all_mon;
	std::map<string, power> moves;
	std::map<string, level> levels;
	std::map<string, item> items;
	std::map<int, bool> blocking;
	std::map<int, bool> animating;
	std::map<int, bool> npc_blocking;
	std::map<int, bool> jumpdown;
	std::map<int, bool> jumpright;
	std::map<int, bool> jumpleft;
	std::map<int, bool> slidedown;
	std::map<int, bool> slideup;
	std::map<int, bool> slideleft;
	std::map<int, bool> slideright;
	std::map<int, bool> slidestop;
	std::map<int, bool> encounter_tile;
	std::map<int, bool> water;
	std::map<int, bool> water_render;
	std::map<int, bool> grass_render;
	std::map<int, bool> draw_over;
	std::vector<menu*> menus;
	std::map<int, string> TM;
	std::map<int, string> HM;
	string alert;
	string encounter;
	int encounter_level;
	player mc;
	location ahead, ahead2;
	bool interact, open_menu;
	unsigned game_start_timer;
public:
	void npc_wander(double deltat);
	void full_heal();
	string get_item_effect(string in);
	void push_hp_bar_if_exists(float x, float y, int index);
	void push_storage_hp_bar_if_exists(float x, float y, int index);
	string get_special_string(string in);
	bool can_use_item(string filter, string name);
	bool use_item(string filter, std::vector<int> &choices, string &ret);
	bool do_effect(mon& m, string effect, int extra = -1);
	string get_item_name(string type, int index);
	string get_item_storage_name(string type, int index);
	int get_item_cost(string in);
	int get_inventory_count(string in);
	int get_inventory_storage_count(string in);
	string get_item_count(string type, int index);
	string get_item_storage_count(string type, int index);
	bool withdraw_item(string s, int count);
	bool withdraw_item(string s);
	bool remove_item(string s, int count);
	bool remove_item(string s);
	bool store_item(string s, int count = 1);
	bool gain_item(string s, int count = 1, bool silent = false);
	bool run_away(mon& escapee, mon& m, int attempts);
	int has_move_in_party(string s);
	int in_inventory(string s);
	bool in_status(mon& m, string s);
	bool in_special(string move, string s);
	bool in_move_target(string move, string effect);
	bool in_move_self(string move, string effect);
	bool in_move_special(string move, string effect);
	void swap_mon(mon& a, mon& b);
	int attempt_capture(float capture_power, mon& m);
	string get_nickname(mon& m);
	void make_mon(string ID, int e_level, mon& out);
	void gain_exp(mon& winner, mon& loser, int num_fighters, double scale = 1.0);
	int get_move_count(mon& m);
	bool level_up(mon& out, bool confirm_learn = false);
	bool learn_move(mon& m, string move);
	bool create_move(mon& m, string move, int index, bool overrule = false);
	void clear_queue(mon& m);
	double random(double min, double max);
	void heal_damage(mon& m, int heal_amount, bool silent = false);
	void deal_damage(mon& m, int damage_amount, bool silent = false);
	bool status_immunity(mon& m, string move);
	bool remove_status_wildcard(mon& m, string s, bool all = false);
	bool remove_status(mon& m, string s, bool all = false);
	bool apply_status(mon& m, string s, bool skip_chance = false, bool silent = false);
	void use_status(mon& self, mon& other);
	bool use_move(mon& attacker, mon& defender, string move, bool skip_accuracy_check = false);
	bool can_evolve(string& ID, string& effect);
	bool is_KO(mon& m);
	bool team_KO();
	bool has_move(mon& m, string& move);
	void KO_move_clear(mon& m);
	void do_turn_inner(mon& m1, mon& m2);
	void clear_volatile(mon& m);
	void team_clear_volatile();
	float get_hp_percent(mon& m);
	bool is_valid_move(mon& m, unsigned i);
	double get_smart_damage(mon& attacker, mon& defender, string move, trainer& t, int future = 0);
	double get_smart_healing(mon& attacker, mon& defender, string move, trainer& t, int future = 0);
	int get_smart_move(mon& attacker, mon& defender, trainer& t, bool skip_recurse, int hp_offset, int& self_turns_to_live, int& opponent_turns_to_live, bool no_depth, double& future_damage);
	void resize_exp_bar(mon& m);
	void resize_hp_bars(mon& m);
	vector<int> remove_cancels(vector<int> v);
	void rebuild_battle_hud(mon& p, mon& e);
	bool battle(trainer& t);
	bool battle();
	void do_turn(mon& m1, mon& m2);
	int damage(mon& attacker, mon& defender, string move, bool& crit, double &mul, bool &non_zero, bool force_no_crit = false, bool force_crit = false, bool skip_damage_range = false);
	string get_buff_name(STAT s);
	string get_debuff_name(STAT s);
	double get_accuracy_modifier(mon& m);
	double get_evasion_modifier(mon& m);
	double get_stat_modifier(int buff);
	string get_type_1(mon& m);
	string get_type_2(mon& m);
	int get_stat(mon& m, STAT s, bool ignore_buffs = false, bool ignore_debuffs = false);
	void init_items();
	void init_characters();
	void init_levels();
	void init_level(string levelname);
	void init_moves();
	void init_tm();
	void init_hm();
	void init_mon();
	void init_status();
	void init_blocking();
	void init_animating();
	void init_npc_blocking();
	void init_slide();
	void init_jumpdown();
	void init_jumpright();
	void init_jumpleft();
	void init_encounter_tiles();
	void init_swimming();
	void init_grass();
	void init_draw_over();
	void init_exp();
	void init_special();
	void init_types();
	void init_game_timer();
	int get_resolution();
	int weightedrand();
	void handle_teleport();
	int get_tile(double y, double x);
	void input(double deltat);
	void collision();
	void play_level_music();
	void player_input(bool up, bool down, bool left, bool right, bool select, bool start, bool confirm, bool cancel);
	void create_menu(string s, string choice = "", string text_override = "", string followup_override = "");
	vector<int> do_alert(string s);
	vector<int> do_menu(string menu, string choice = "", string text_override = "", string followup_override = "");
	void do_move_animation(mon& attacker, mon& defender, string move);
	void update_level();
	void push_tile(double xp, double yp, double xl, double yl, int ID);
	void draw_level();
	void draw_special();
	void draw_characters();
	void do_interaction(character& npc);
	void main();
	void pack_team();
	void pack_storage();
	void save_game();
	void save_mon(ofstream& f, mon& m);
	void load_game();
	void load_mon(string s, mon& m);
	string get_input_string();
};

#endif