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
#include "menus.h"


using namespace std;

class graphics;
extern graphics g;
extern mutex m;
mutex transition;

extern bool is_menu(string s);

enum STAT{
	HP=0,
	ATTACK,
	DEFENSE,
	SPECIAL,
	SPEED,
	SIZE
};

enum direction {
	DOWN,
	UP,
	LEFT,
	RIGHT
};

class location {
public:
	double x;
	double y;
	string level;
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
	power(){ acc = 100; attack = ATTACK; defense = DEFENSE; crit_chance = 1.0f; pow = "0"; pp = 0; defined = false; queue_only = false; }
};

class mon_template {
public:
	string number, name, type1, type2, dex;
	int stats[SIZE];
	int exp_yield, catchrate, height;
	double weight;
	vector<pair<int, string>> learned;
	vector<pair<string, string>> evolution;
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
	string moves[4];
	vector<string> status;
	vector<string> queue;
	string nickname;
	unsigned hp_bar_index, exp_bar_index;
	unsigned hud_index;
	mon() { defined = false; }
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
	string name, rivalname;
	direction dir;
	int wins, losses;
	int money;
	int repel;
	int selected;
	int enemy_selected;
	mon team[6];
	mon enemy_team[6];
	mon storage[20][20];
	location loc;
	std::map<string, unsigned> interaction;
	std::map<string, bool> inactive;
	std::vector<pair<string, int>> inventory;
	std::map<string, int> values;
	player() { wins = 0; losses = 0; money = 0; name = "RED"; rivalname = "BLUE"; repel = 0; selected = 0; enemy_selected = 0; }
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
	direction dir;
	string image;
	string name; // MUST BE UNIQUE
	vector<string> interactions;
	vector<location> force_interactions;
	location loc, origin;
	bool wander;
	bool active;
	character() { wander = false; dir = DOWN; active = true; }
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
	mon team[6];
};

class level {
public:
	string name;
	std::vector<std::vector<int>> data;
	std::vector<std::pair<location, location>> teleport;
	std::vector<character> characters;
	std::vector<int> encounters;
	std::pair<int, int> level_range;
	std::map<string, trainer> trainers;
};

typedef std::map<string, std::map<string, float>>::iterator type_iter;
int min(int a, int b) {
	if (a < b) {
		return a;
	}
	return b;
}
float min(float a, float b) {
	if (a < b) {
		return a;
	}
	return b;
}
double min(double a, double b) {
	if (a < b) {
		return a;
	}
	return b;
}
int max(int a, int b) {
	if (a > b) {
		return a;
	}
	return b;
}
float max(float a, float b) {
	if (a > b) {
		return a;
	}
	return b;
}
double max(double a, double b) {
	if (a > b) {
		return a;
	}
	return b;
}

string get_direction_string(direction dir) {
	switch (dir) {
	case UP:
		return string("up");
	case DOWN:
		return string("down");
	case LEFT:
		return string("left");
	case RIGHT:
		return string("right");
	}
	return string("ERROR");
}


class engine {
public: // TODO:  Change back to private
	// format is always types[ATTACKER][DEFENDER]
	std::map<string, std::map<string, float>> types;
	std::map<string, bool> special_case;
	std::vector<int> level_to_exp;
	std::map<string, status_effect> status;
	std::map<string, mon_template> all_mon;
	std::map<string, power> moves;
	std::map<string, level> levels;
	std::map<string, item> items;
	std::map<int, bool> blocking;
	std::map<int, bool> jumpdown;
	std::map<int, bool> water;
	std::vector<menu*> menus;
	std::map<int, string> TM;
	std::map<int, string> HM;
	string alert;
	string encounter;
	int encounter_level;
	string current_level;
	player mc;
	location ahead;
	bool interact;
public:
	void full_heal() {
		for (unsigned i = 0; i < 6; ++i) {
			if (mc.team[i].defined) {
				mc.team[i].curr_hp = get_stat(mc.team[i], HP);
				mc.team[i].status.clear();
				for (unsigned j = 0; j < 4; ++j) {
					if (moves[mc.team[i].moves[j]].defined) {
						mc.team[i].pp[j] = mc.team[i].max_pp[j];
					}
				}
			}
		}
	}
	string get_item_effect(string in) {
		string out = "";
		if (!items[in].defined)
			return string("");
		for (unsigned i = 0; i < items[in].effect.size(); ++i) {
			if (out.size() > 0) {
				out = out + "|";
			}
			out = out + items[in].effect[i];
		}
		return out;
	}
	void push_hp_bar_if_exists(float x, float y, int index) {
		if (mc.team[index].defined)
			g.push_hp_bar(x, y, get_hp_percent(mc.team[index]));
	}
	string get_special_string(string in) {
		string parse;
		string out;
		bool number = false;
		if (in.length() == 0)
			return in;
		if (in.find(":") != -1) {
			string temp = in;
			for (unsigned i = 0; (i < temp.size()) && (temp[i] != ':'); ++i) {
				parse += temp[i];
			}
			temp.erase(0, temp.find(':') + 1);
			for (int i = 0; i <= 9; ++i) {
				if (temp.find(to_string(i)) == 0) {
					number = true;
					break;
				}
			}
			if (number && stoi(temp) == -1)
				return string(""); // TODO:  Better error detection.
			if (parse == "ACTIVE_MON_MOVE") {
				out = mc.team[mc.selected].moves[stoi(temp)];
				if (out == "") {
					out = "-";
				}
				return out;
			}
			else if (parse == "MON_MOVE") {
				int index1 = 0;
				int index2 = 0;
				index1 = stoi(temp);
				temp.erase(0, temp.find(":") + 1);
				index2 = stoi(temp);
				out = mc.team[index1].moves[index2];
				if (out == "") {
					out = "-";
				}
				return out;
			}
			else if (parse == "ACTIVE_MON_MOVE_TYPE") {
				if (!number)
					return in;
				return moves[mc.team[mc.selected].moves[stoi(temp)]].type;
			}
			else if (parse == "ACTIVE_MON_MOVE_PP") {
				if (!number)
					return in;
				int index = stoi(temp);
				out = to_string(mc.team[mc.selected].pp[index]) + string("/") + to_string(mc.team[mc.selected].max_pp[index]);
				while (out.length() < 9)
					out = string(" ") + out;
				return out;
			}
			else if (parse == "MON_MOVE_PP_IF_EXISTS") {
				int index1 = 0;
				int index2 = 0;
				index1 = stoi(temp);
				temp.erase(0, temp.find(":") + 1);
				index2 = stoi(temp);
				if (!moves[mc.team[index1].moves[index2]].defined)
					return string(" ");
				return string("PP");
			}
			else if (parse == "MON_MOVE_PP") {
				int index1 = 0;
				int index2 = 0;
				index1 = stoi(temp);
				temp.erase(0, temp.find(":") + 1);
				index2 = stoi(temp);
				if (!moves[mc.team[index1].moves[index2]].defined)
					return string(" ");
				out = to_string(mc.team[index1].pp[index2]) + string("/") + to_string(mc.team[index1].max_pp[index2]);
				while (out.length() < 9)
					out = string(" ") + out;
				return out;
			}
			else if (parse == "TEAM_MON_NAME"){
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				return get_nickname(mc.team[index]);
			}
			else if (parse == "TEAM_MON_LEVEL"){
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				if (mc.team[index].level == 100)
					return string("{LEVEL}**");
				return string("{LEVEL}") + to_string(mc.team[index].level);
			}
			else if (parse == "TEAM_MON_CAN_LEARN"){
				string temp2 = temp;
				temp2.erase(temp2.find(":"), temp2.size());
				temp.erase(0, temp.find(":") + 1);
				int index = stoi(temp2);
				if (!mc.team[index].defined)
					return string("");
				if (temp.find("TM") != -1) {
					temp.erase(0, temp.find(":") + 1);
					if (all_mon[mc.team[index].number].TM[stoi(temp)])
						return string("ABLE");
					return string("NOT ABLE");
				}
				else if (temp.find("HM") != -1) {
					temp.erase(0, temp.find(":") + 1);
					if (all_mon[mc.team[index].number].HM[stoi(temp)])
						return string("ABLE");
					return string("NOT ABLE");
				}
				else
					return string("ERROR");
			}
			else if (parse == "TEAM_MON_CAN_LEARN_FOLLOW_UP"){
				string temp2 = temp;
				temp2.erase(temp2.find(":"), temp2.size());
				temp.erase(0, temp.find(":") + 1);
				int index = stoi(temp2);
				if (!mc.team[index].defined)
					return string("");
				if (temp.find("TM") != -1) {
					temp.erase(0, temp.find(":") + 1);
					if (all_mon[mc.team[index].number].TM[stoi(temp)])
						return string("");
					return string("ALERT:") + get_nickname(mc.team[index]) + string(" is not compatible with ") + TM[stoi(temp)] + string(".  It can't learn ") + TM[stoi(temp)] + string(".");
				}
				else if (temp.find("HM") != -1) {
					temp.erase(0, temp.find(":") + 1);
					if (all_mon[mc.team[index].number].HM[stoi(temp)])
						return string("");
					return string("ALERT:") + get_nickname(mc.team[index]) + string(" is not compatible with ") + HM[stoi(temp)] + string(".  It can't learn ") + HM[stoi(temp)] + string(".");
				}
				else
					return string("");
			}
			else if (parse == "TEAM_MON_MAX_HP"){
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				return to_string(get_stat(mc.team[index], HP));
			}
			else if (parse == "TEAM_MON_NUMBER"){
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				return string("{NO}.") + mc.team[index].number;
			}
			else if (parse == "TEAM_MON_IMAGE"){
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				return mc.team[index].number + string(".png");
			}
			else if (parse == "TEAM_MON_CURRENT_HP"){
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				return to_string(mc.team[index].curr_hp);
			}
			else if (parse == "TEAM_MON_FORMATTED_HP") {
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				string o = to_string(mc.team[index].curr_hp) + string("/") + to_string(get_stat(mc.team[index], HP));
				while (o.length() < 7)
					o = string(" ") + o;
				return o;
			}
			else if (parse == "TEAM_MON_ATTACK"){
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				string o = to_string(get_stat(mc.team[index], ATTACK, true, true));
				while (o.length() < 8)
					o = string(" ") + o;
				return o;
			}
			else if (parse == "MON_TEMPLATE_NAME"){
				if (!all_mon[temp].defined)
					return string("");
				return all_mon[temp].name;
			}
			else if (parse == "MON_ENTRY"){
				if (!all_mon[temp].defined)
					return string("");
				return all_mon[temp].dex;
			}
			else if (parse == "MON_WEIGHT"){
				if (!all_mon[temp].defined)
					return string("");
				string o = to_string(all_mon[temp].weight);
				string out = "";
				for (unsigned i = 0; i < o.size(); ++i) {
					out = out + o[i];
					if (o[i] == '.') {
						out = out + o[i + 1];
						break;
					}
				}
				return out + string("lb");
			}
			else if (parse == "MON_HEIGHT"){
				if (!all_mon[temp].defined)
					return string("");
				int index = all_mon[temp].height;
				string o = "";
				if (index > 12)
					o = o + to_string(index / 12) + string("{FEET}");
				o = o + to_string(index % 12) + string("{INCHES}");
				return o;
			}
			else if (parse == "TEAM_MON_DEFENSE"){
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				string o = to_string(get_stat(mc.team[index], DEFENSE, true, true));
				while (o.length() < 8)
					o = string(" ") + o;
				return o;
			}
			else if (parse == "TEAM_MON_SPEED"){
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				string o = to_string(get_stat(mc.team[index], SPEED, true, true));
				while (o.length() < 8)
					o = string(" ") + o;
				return o;
			}
			else if (parse == "TEAM_MON_SPECIAL"){
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				string o = to_string(get_stat(mc.team[index], SPECIAL, true, true));
				while (o.length() < 8)
					o = string(" ") + o;
				return o;
			}
			else if (parse == "TEAM_MON_TYPE1") {
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				return all_mon[mc.team[index].number].type1;
			}
			else if (parse == "TEAM_MON_TYPE2") {
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				return all_mon[mc.team[index].number].type2;
			}
			else if (parse == "TYPE2_IF_EXISTS") {
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				if (all_mon[mc.team[index].number].type2 != "")
					return string("TYPE2/");
				return string("");
			}
			else if (parse == "STATUS_BOOL") {
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				if (mc.team[index].curr_hp <= 0)
					return string("STATUS/KO");
				if (mc.team[index].status.size() != 0)
					return string("STATUS/BAD");
				return string("STATUS/OK");
			}
			else if (parse == "TEAM_MON_STATUS") {
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				if (mc.team[index].curr_hp <= 0)
					return string("KO");
				string o = "{SINGLE}";
				for (unsigned i = 0; i < mc.team[index].status.size(); ++i) {
					if (status[mc.team[index].status[i]].defined)
						if (status[mc.team[index].status[i]].nonvolatile)
							if (o.find(mc.team[index].status[i]) == -1)
								o = o + mc.team[index].status[i] + string("\n");
				}
				if (o == "{SINGLE}")
					o = "{SINGLE}OK";
				return o;
			}
			else if (parse == "TEAM_MON_NAME_RIGHT_JUSTIFIED") {
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				string o = get_nickname(mc.team[index]);
				while (o.length() < 14)
					o = string(" ") + o;
				return o;
			}
			else if (parse == "EXP_POINTS") {
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				string o = to_string(mc.team[index].exp);
				while (o.length() < 10)
					o = string(" ") + o;
				return o;
			}
			else if (parse == "LEVEL_UP") {
				int index = stoi(temp);
				if (!mc.team[index].defined)
					return string("");
				int tot = -1;
				for (int i = 0; i <= 100; ++i) {
					if (e.level_to_exp[i] > mc.team[index].exp) {
						tot = e.level_to_exp[i] - mc.team[index].exp;
						break;
					}
				}
				string o = string("");
				if (tot != -1) {
					o = to_string(tot);
				}
				while (o.length() < 10)
					o = string(" ") + o;
				return o;
			}
			else if (parse == "RIGHT_JUSTIFY") {
				unsigned len = stoi(temp);
				temp.erase(0, temp.find(':') + 1);
				temp = get_special_string(temp);
				while (temp.length() < len)
					temp = string(" ") + temp;
				return temp;
			}
			else if (parse == "MOVE_POWER") {
				if (!moves[temp].defined)
					return string("-");
				return moves[temp].pow;
			}
			else if (parse == "MOVE_ACC") {
				if (!moves[temp].defined)
					return string("-");
				return to_string(moves[temp].acc);
			}
			else if (parse == "MOVE_TYPE") {
				if (!moves[temp].defined)
					return string("-");
				return moves[temp].type;
			}
			else if (parse == "MOVE_DEFENSE") {
				if (!moves[temp].defined)
					return string("-");
				switch (moves[temp].defense) {
				case ATTACK:
					return string("ATTACK");
				case DEFENSE:
					return string("DEFENSE");
				case SPECIAL:
					return string("SPECIAL");
				case SPEED:
					return string("SPEED");
				default:
					return string("-");
				}
			}
			else if (parse == "MOVE_CATEGORY") {
				if (!moves[temp].defined)
					return string("-");
				switch (moves[temp].defense) {
				case DEFENSE:
					return string("PHYSICAL");
				case SPECIAL:
					return string("SPECIAL");
				default:
					return string("-");
				}
			}
			else if (parse == "MOVE_NOTES") {
				if (!moves[temp].defined)
					return string("-");
				return moves[temp].notes;
			}
			else if (parse == "ITEM") {
				string type = temp;
				type.erase(type.find(":"), type.length());
				temp.erase(0, temp.find(":") + 1);
				int index = stoi(temp);
				return get_item_name(type, index);
			}
			else if (parse == "ITEM_COUNT") {
				string type = temp;
				type.erase(type.find(":"), type.length());
				temp.erase(0, temp.find(":") + 1);
				int index = stoi(temp);
				return get_item_count(type, index);
			}
		}
		if (in == "{RIVAL_NAME}") {
			return mc.rivalname;
		}
		else if (in == "{PLAYER_NAME}") {
			return mc.name;
		}
		return in;
	}
	bool use_item(string filter, std::vector<int> &choices, string &ret) {
		ret = string("");
		string name = get_item_name(filter, choices[1]);
		string effect = get_item_effect(name);
		string base = effect;
		while (base.size() > 0) {
			effect = base;
			if (base.find("|") != -1) {
				base.erase(0, base.find("|") + 1);
				effect.erase(effect.find("|"), effect.size());
			}
			else {
				base = "";
			}
			if (is_menu(effect)) {
				string menu = effect;
				menu.erase(menu.find(":"), menu.size());
				effect.erase(0, effect.find(":") + 1);
				if (!do_effect(mc.team[choices[2]], effect, choices[choices.size() - 1]))
					return false;
			}
			else if (effect.find("CAPTURE") == 0) {
				ret = effect;
			}
			else if (effect.find("FISH") == 0) {
				// TODO: FISHING
			}
			else if (effect.find("MAP") == 0) {
				// TODO: MAP
			}
			else if (effect.find("TM") == 0) {
				string move = effect;
				move.erase(0, move.find(":") + 1);
				move = TM[stoi(move)];
				do_menu(string("ALERT"), string("Booted up a TM!"));
				do_menu(string("ALERT"), string("It contained ") + move + string("!"));
				choices = do_menu(string("ALERT_YES_NO"), string("Teach ") + move + string(" to a POK{e-accent}MON?"));
				if (choices[0] == 1)
					return false;
				choices = do_menu(string("LEARN_MON_SELECT"), effect);
				if (choices.size() > 1)
					return false;
				if (choices[0] == -1)
					return false;
				return learn_move(mc.team[choices[0]], move);
			}
			else if (effect.find("HM") == 0) {
				string move = effect;
				move.erase(0, move.find(":") + 1);
				move = HM[stoi(move)];
				do_menu(string("ALERT"), string("Booted up an HM!"));
				do_menu(string("ALERT"), string("It contained ") + move + string("!"));
				choices = do_menu(string("ALERT_YES_NO"), string("Teach ") + move + string(" to a POK{e-accent}MON?"));
				if (choices[1] == 1)
					return false;
				choices = do_menu(string("LEARN_MON_SELECT"), effect);
				if (choices.size() > 1)
					return false;
				if (choices[0] == -1)
					return false;
				return learn_move(mc.team[choices[0]], move);
			}
			else if (effect.find("TARGET") == 0) {
				do_effect(mc.enemy_team[mc.enemy_selected], effect);
			}
			else if (effect.find("TEAM") == 0) {
				for (int i = 0; i < 6; ++i) {
					if (mc.team[i].defined)
						do_effect(mc.team[i], effect);
				}
			}
			else if (effect.find("SELF") == 0) {
				do_effect(mc.team[mc.selected], effect);
			}
			else if (effect.find("TELEPORT") == 0) {
				ret = "TELEPORT";
			}
			else if (effect.find("FLEE") == 0) {
				ret = "FLEE";
			}
			else if (effect.find("REPEL") == 0) {
				effect.erase(0, effect.find(":") + 1);
				mc.repel += stoi(effect);
				ret = "TELEPORT";
			}
		}
		if (get_item_effect(name).find("INFINITE") == -1) {
			for (unsigned i = 0; i < mc.inventory.size(); ++i) {
				if (mc.inventory[i].first == name) {
					mc.inventory[i].second--;
					if (mc.inventory[i].second <= 0) {
						mc.inventory.erase(mc.inventory.begin() + i);
					}
					break;
				}
			}
		}
		return true;
	}
	bool do_effect(mon& m, string effect, int extra=-1) {
		if (effect.find("|") != -1)
			effect.erase(effect.find("|"), effect.size());
		if (effect.find("SELF") == 0) {
			effect.erase(0, effect.find(":") + 1);
		}
		if (effect.find("TARGET") == 0) {
			effect.erase(0, effect.find(":") + 1);
		}
		if (effect.find("CLEAR_STATUS") == 0) {
			if (effect.find(":") != -1) {
				effect.erase(0, effect.find(":") + 1);
				remove_status(m, effect, true);
			}
			else {
				while (m.status.size() > 0) {
					remove_status(m, m.status[0], true);
				}
			}
		}
		else if (effect.find("FILL_HP") == 0) {
			effect.erase(0, effect.find(":") + 1);
			heal_damage(m, stoi(effect));
		}
		else if (effect.find("FILL_PP") == 0) {
			effect.erase(0, effect.find(":") + 1);
			m.pp[extra] = min(m.pp[extra] + stoi(effect), m.max_pp[extra]);
		}
		else if (effect.find("FILL_ALL_PP") == 0) {
			effect.erase(0, effect.find(":") + 1);
			for (unsigned i = 0; i < 4; i++) {
				if (moves[m.moves[i]].defined) {
					m.pp[i] = min(m.pp[i] + stoi(effect), m.max_pp[i]);
				}
			}
		}
		else if (effect.find("REVIVE") == 0) {
			m.curr_hp = get_stat(m, HP) / 2;
		}
		else if (effect.find("EV_UP") == 0) {
			STAT s;
			effect.erase(0, effect.find(":") + 1);
			if (effect.find("HP") == 0) {
				s = HP;
			}
			else if (effect.find("ATTACK") == 0) {
				s = ATTACK;
			}
			else if (effect.find("DEFENSE") == 0) {
				s = DEFENSE;
			}
			else if (effect.find("SPEED") == 0) {
				s = SPEED;
			}
			else if (effect.find("SPECIAL") == 0) {
				s = SPECIAL;
			}
			else {
				return false;
			}
			effect.erase(0, effect.find(":") + 1);
			// TODO: Add in-game EV boost limit
			m.EV[s] += stoi(effect);
		}
		else if (effect.find("EVOLVE") == 0) {
			effect.erase(0, effect.find(":") + 1);
			for (unsigned i = 0; i < all_mon[m.number].evolution.size(); ++i) {
				if (all_mon[m.number].evolution[i].second == effect) {
					// TODO:  EVOLUTION SCREEN
					m.number = all_mon[m.number].evolution[i].first;
					break;
				}
			}
		}
		else if (effect.find("PP_UP") == 0) {
			int i = int(double(moves[m.moves[extra]].pp) * 0.2) + m.max_pp[extra];
			int j = int(double(moves[m.moves[extra]].pp) * 1.6);
			i = min(i, j);
			m.max_pp[extra] = i;
			m.pp[extra] = i;
			// TODO:  Don't use up items from maxed out PP.
		}
		else if (effect.find("LEVEL_UP") == 0) {
			// TODO: Fail when level 100
			m.exp = level_to_exp[min(100, m.level + 1)];
			level_up(m, true);
		}
		else if (effect.find("APPLY_STATUS") == 0) {
			effect.erase(0, effect.find(":") + 1);
			apply_status(m, effect);
		}
		else if (effect.find("TM") == 0) {
			effect.erase(0, effect.find(":") + 1);
			if (!learn_move(m, TM[stoi(effect)]))
				return false;
		}
		else if (effect.find("HM") == 0) {
			effect.erase(0, effect.find(":") + 1);
			if (!learn_move(m, HM[stoi(effect)]))
				return false;
		}
		return true;
	}
	string get_item_name(string type, int index) {
		unsigned i = 0, count = 0;
		while (count < e.mc.inventory.size()) {
			bool found = false;
			for (unsigned j = 0; j < e.items[e.mc.inventory[count].first].use.size(); ++j) {
				if (type == "ALL" || type == e.items[e.mc.inventory[count].first].use[j])
					found = true;
			}
			if (!found) {
				count++;
				continue;
			}
			if (i == index) {
				return e.mc.inventory[count].first;
			}
			i++;
			count++;
		}
		return string("NOT FOUND");
	}
	string get_item_count(string type, int index) {
		unsigned i = 0, count = 0;
		while (count < e.mc.inventory.size()) {
			bool found = false;
			for (unsigned j = 0; j < e.items[e.mc.inventory[count].first].use.size(); ++j) {
				if (type == "ALL" || type == e.items[e.mc.inventory[count].first].use[j])
					found = true;
			}
			if (!found) {
				count++;
				continue;
			}
			if (i == index) {
				return string("{TIMES}") + to_string(e.mc.inventory[count].second);
			}
			i++;
			count++;
		}
		return string("NOT FOUND");
	}
	bool gain_item(string& s, int count=1, bool silent=false) {
		bool found = false;
		if (!items[s].defined) {
			do_menu(string("ALERT"), string("Game attempted to give bad item: ") + s + string(" to player."));
			return false;
		}
		for (unsigned i = 0; i < mc.inventory.size(); ++i) {
			if (mc.inventory[i].first == s) {
				found = true;
				mc.inventory[i].second += count;
				break;
			}
		}
		if (!found) {
			pair<string, int> item;
			item.first = s;
			item.second = count;
			mc.inventory.push_back(item);
		}
		if (!silent)
			do_menu(string("ALERT"), string("{PLAYER_NAME} got ") + s + string("!"));
		return true;
	}
	bool run_away(mon& escapee, mon& m, int attempts) {
		double chance = (double(get_stat(escapee, SPEED))*32.0 / double(get_stat(m, SPEED))) + (30.0 * double(attempts));
		return chance > random(0.0, 255.0);
	}
	int in_inventory(string s) {
		for (unsigned i = 0; i < mc.inventory.size(); ++i) {
			if (mc.inventory[i].first == s) {
				return mc.inventory[i].second;
			}
		}
		return 0;
	}
	bool in_status(mon& m, string s) {
		for (unsigned i = 0; i < m.status.size(); ++i) {
			if (m.status[i] == s) {
				return true;
			}
		}
		return false;
	}
	bool in_special(string move, string s) {
		for (unsigned i = 0; i < moves[move].special.size(); ++i) {
			if (moves[move].special[i] == s)
				return true;
		}
		return false;
	}
	void swap_mon(mon& a, mon& b) {
		mon temp;
		temp = a;
		a = b;
		b = temp;
	}
	int attempt_capture(float capture_power, mon& m) {
		int ret = 0;
		double chance, hard_probability;
		chance = 3.0*double(get_stat(m, HP));
		chance -= 2.0*double(m.curr_hp);
		chance *= double(all_mon[m.number].catchrate);
		chance *= capture_power;
		if (in_status(m, string("POISON")))
			chance *= 1.5;
		if (in_status(m, string("TOXIC")))
			chance *= 1.5;
		if (in_status(m, string("BURN")))
			chance *= 1.5;
		if (in_status(m, string("PARALYZE")))
			chance *= 1.5;
		if (in_status(m, string("SLEEP")))
			chance *= 2.0;
		if (in_status(m, string("FREEZE")))
			chance *= 2.0;
		chance /= 3.0*double(get_stat(m, HP));
		chance = 1048560.0 / sqrt(sqrt(16711680.0 / chance));
		hard_probability = pow(chance / 65536.0, 4.0); // Debugging purposes only.
		for (ret = 0; ret < 4; ++ret) {
			if (random(0.0, 65536.0) > chance) {
				break;
			}
		}
		return ret;
	}
	string get_nickname(mon& m) {
		if (m.nickname == "")
			return all_mon[m.number].name;
		return m.nickname;
	}
	void make_mon(string ID, int e_level, mon& out) {
		out.level = 0;
		out.status.clear();
		out.exp = level_to_exp[e_level];
		if (out.exp == -1)
			int lkjawgjkalsgkj = 0;
		for (int x = 0; x < SIZE; x++)
			out.EV[x] = 0;
		for (int x = 0; x < SIZE; x++)
			out.IV[x] = int(random(0.0, 16.0));
		out.number = ID;
		out.wild = true;
		out.defined = true;
		out.turn_count = 0;
		out.nickname = "";
		for (int x = 0; x < 4; ++x)
			out.moves[x] = "";
		level_up(out);
		out.curr_hp = get_stat(out, HP);
		if (out.level == 0) {
			make_mon(ID, e_level, out);
		}
	}
	void gain_exp(mon& winner, mon& loser, int num_fighters) {
		double exp = 1.0;
		if (!loser.wild)
			exp *= 1.5;
		exp *= all_mon[loser.number].exp_yield;
		exp *= loser.level;
		exp /= 5.0;
		exp /= double(num_fighters);
		exp *= pow((2.0*double(loser.level) + 10.0), 2.5);
		exp /= pow((double(winner.level + loser.level) + 10.0), 2.5);
		exp += 1.0;
		double diff = double(max(min(winner.level - loser.level, 5), -10)) / 10.0;
		exp *= (1.0 - diff);
		winner.exp += int(exp);
		for (unsigned i = 0; i < SIZE; ++i) {
			winner.EV[i] += all_mon[loser.number].stats[i];
		}
		resize_exp_bar(winner);
		do_alert(get_nickname(winner) + string(" gained ") + to_string(int(exp)) + string(" EXP. Points!"));
		level_up(winner, true); // TODO:  Switch to true when confirmation boxes are in.
		resize_exp_bar(winner);
	}
	int get_move_count(mon& m) {
		int counter = 0;
		while ((counter < 4) && (m.moves[counter] != ""))
			counter++;
		return counter;
	}
	bool level_up(mon& out, bool confirm_learn=false) {
		if (level_to_exp[out.level + 1] > out.exp)
			return false;
		int counter = get_move_count(out);
		while (out.level < 100 && level_to_exp[out.level + 1] <= out.exp) {
			out.level++;
			if (confirm_learn) {
				do_alert(get_nickname(out) + string(" grew to level ") + to_string(out.level) + "!");
			}
			for (unsigned x = 0; x < all_mon[out.number].learned.size(); ++x) {
				if (all_mon[out.number].learned[x].first == out.level) {
					if (!confirm_learn) { // TODO:  OR Surface confirmation window
						create_move(out, all_mon[out.number].learned[x].second, counter % 4);
						counter++;
					}
					else {
						learn_move(out, all_mon[out.number].learned[x].second);
					}
				}
			}
		}
		return true;
	}
	bool learn_move(mon& m, string move) {
		vector<int> choices;
		if (m.wild) {
			return false;
		}
		int counter = get_move_count(m);
		if (counter < 4) {
			create_move(m, move, counter);
			do_menu("ALERT", get_nickname(m) + string(" learned ") + move + string("!"));
		}
		else {
			do_menu("ALERT", get_nickname(m) + string(" is trying to learn ") + move + string("!"));
			do_menu("ALERT", string("But ") + get_nickname(m) + string(" can't learn more than 4 moves!"));
			choices = do_menu("ALERT_YES_NO", string("Delete an older move to make room for ") + move + string("?"));
			if (choices[choices.size() - 1] == 1 || choices[choices.size() - 1] == -1) {
				choices = do_menu("ALERT_YES_NO", string("Abandon learning ") + move + string("?"));
				if (choices[choices.size() - 1] == 1 || choices[choices.size() - 1] == -1) {
					return learn_move(m, move);
				}
				return false;
			}
			string movelist = "";
			string followuplist = "";
			for (unsigned i = 0; i < 4; i++) {
				movelist = movelist + m.moves[i] + "|";
				followuplist = followuplist + "FORGET_INFO_CANCEL:" + m.moves[i] + "|";
			}
			movelist = movelist + move;
			followuplist = followuplist + "ABANDON_INFO_CANCEL:" + move;
			choices = do_menu("MOVE_FORGET", "", movelist, followuplist);
			if (choices[choices.size() - 1] == -1) {
				choices = do_menu("ALERT_YES_NO", string("Abandon learning ") + move + string("?"));
				if (choices[choices.size() - 1] == 1 || choices[choices.size() - 1] == -1) {
					return learn_move(m, move);
				}
				return false;
			}
			else if (choices[choices.size() - 1] == 0) {
				if (choices[choices.size() - 2] != 4) {
					string oldmove = m.moves[choices[choices.size() - 2]];
					create_move(m, move, choices[choices.size() - 2]);
					do_menu("ALERT", string("1, 2 and... Poof!"));
					do_menu("ALERT", get_nickname(m) + string(" forgot ") + move + string("!"));
					do_menu("ALERT", string("And..."));
					do_menu("ALERT", get_nickname(m) + string(" learned ") + move + string("!"));
					return true;
				}
				else {
					choices = do_menu("ALERT_YES_NO", string("Abandon learning ") + move + string("?"));
					if (choices[choices.size() - 1] == 1 || choices[choices.size() - 1] == -1) {
						return learn_move(m, move);
					}
					return false;
				}
			}
		}
		return false;
	}
	bool create_move(mon& m, string move, int index) {
		if (!moves[move].defined)
			return false;
		m.moves[index] = move;
		m.pp[index] = moves[m.moves[index]].pp;
		m.max_pp[index] = m.pp[index];
		return true;
	}
	void clear_queue(mon& m) {
		while (m.queue.size() > 0) {
			for (int i = 0; i < 4; ++i) {
				if (m.moves[i] != "") {
					if (m.queue[0] == m.moves[i]) {
						m.pp[i]++;
					}
				}
			}
			m.queue.erase(m.queue.begin());
		}
	}
	double random(double min, double max) {
		// range is inclusive.
		double delta = max - min;
		double t = double(rand() % 10000) / double(10000.0);
		return min + t*delta;
	}
	void heal_damage(mon& m, int heal_amount) {
		if (heal_amount < 0) {
			// TODO:  Error logic here
			return;
		}
		m.curr_hp = m.curr_hp + heal_amount;
		if (m.curr_hp > get_stat(m, HP))
			m.curr_hp = get_stat(m, HP);
		resize_hp_bars(m);
		if (m.hud_index)
			rebuild_battle_hud(mc.team[mc.selected], mc.enemy_team[mc.enemy_selected], m.hud_index);
	}
	void deal_damage(mon& m, int damage_amount) {
		if (damage_amount <= 0) {
			// TODO:  Error logic here
			return;
		}
		m.curr_hp = m.curr_hp - damage_amount;
		if (m.curr_hp < 0)
			m.curr_hp = 0;
		resize_hp_bars(m);
		if (m.hud_index)
			rebuild_battle_hud(mc.team[mc.selected], mc.enemy_team[mc.enemy_selected], m.hud_index);
	}
	bool status_immunity(mon& m, string move) {
		for (unsigned i = 0; i < moves[move].special.size(); ++i) {
			if (moves[move].special[i].find(string("STATUS_IMMUNITY")) != -1) {
				if (moves[move].special[i].find(all_mon[m.number].type1) != -1)
					return true;
				if (moves[move].special[i].find(all_mon[m.number].type2) != -1)
					return true;
			}
		}
		return false;
	}
	bool remove_status(mon& m, string s, bool all = false) {
		bool success = false;
		while (m.status.size() > 0 && m.status[0] == s) {
			m.status.erase(m.status.begin());
			if (!all)
				return true;
			success = true;
		}
		for (unsigned i = 1; i < m.status.size(); ++i) {
			if (m.status[i] == s) {
				m.status.erase(m.status.begin() + i);
				if (!all)
					return true;
				success = true;
				i--;
			}
		}
		return success;
	}
	bool apply_status(mon& m, string s) {
		string s2, s3;
		int repeat = 1;
		if (s.find("x2") != -1) {
			s.erase(s.find("x2"), string("x2").length());
			repeat = 2;
		}
		s2 = "";
		s3 = "";
		if (s.find(':') != -1) {
			s2 = s;
			s3 = s;
			s2.erase(s2.find(":"), s2.length());
			s3.erase(0, s3.find(":") + 1);
		}
		else {
			s2 = s;
		}
		if (s3 == "" && status[s2].chance)
			s3 = "100";
		if (!status[s2].defined)
			return false;
		if (s2 == "POISON" && in_status(m, string("TOXIC")))
			return false;
		if (status[s2].singleton) {
			if (in_status(m, s2)) {
				return false;
			}
		}
		if (status[s2].chance) {
			double c = double(stoi(s3));
			if (random(0.0, 100.0) > c) {
				return false;
			}
		}
		if (!status[s2].specialcase) {
			if (s2 == "SLEEP") {
				int max;
				if (s3 == "") {
					max = int(random(1.0, 8.0));
				}
				else {
					max = stoi(s3);
				}
				for (int i = 0; i < max; ++i) {
					m.status.push_back(s2);
				}
			}
			else {
				string s4 = "";
				if (m.wild)
					s4 = string("Wild ");
				else if (m.enemy)
					s4 = string("Enemy ");
				s4 = s4 + get_nickname(m) + string("'s");
				if (s.find("ATTACK") != -1)
					s4 = s4 + " ATTACK ";
				else if (s.find("DEFENSE") != -1)
					s4 = s4 + " DEFENSE ";
				else if (s.find("SPEED") != -1)
					s4 = s4 + " SPEED ";
				else if (s.find("SPECIAL") != -1)
					s4 = s4 + " SPECIAL ";
				else if (s.find("ACCURACY") != -1)
					s4 = s4 + " ACCURACY ";
				else if (s.find("EVASION") != -1)
					s4 = s4 + " EVASION ";
				if (repeat > 1)
					s4 = s4 + "greatly ";
				if (s.find("UP") != -1)
					s4 = s4 + "rose!";
				if (s.find("DOWN") != -1)
					s4 = s4 + "fell!";
				do_alert(s4);
				for (int i = 0; i < repeat; ++i)
					m.status.push_back(s2);
			}
			if (s2 == "TOXIC") {
				remove_status(m, string("POISON"), true);
			}
		}
		else {
			int value = 1;
			if (s3[0] - '0' >= 0 && s3[0] - '0' <= 9)
				value = stoi(s3);
			if (s2 == "CLEAR_STATUS") {
				remove_status(m, s3, true);
			}
			else {
				for (int i = 0; i < value; ++i) {
					m.status.push_back(s2);
				}
			}
		}
		return true;
	}
	void use_status(mon& self, mon& other) {
		for (unsigned i = 0; i < self.status.size(); ++i) {
			if (self.status[i] == "POISON") {
				// TODO: Animation goes here
				if (self.wild)
					do_alert(string("The wild ") + get_nickname(self) + string(" is hurt by the poison!"));
				else
					do_alert(get_nickname(self) + string(" is hurt by the poison!"));
				deal_damage(self, int(max(double(get_stat(self, HP)) / 16.0, 1.0)));
			}
			else if (self.status[i] == "BURN") {
				// TODO: Animation goes here
				if (self.wild)
					do_alert(string("The wild ") + get_nickname(self) + string(" is hurt by its burn!"));
				else
					do_alert(get_nickname(self) + string(" is hurt by its burn!"));
				deal_damage(self, int(max(double(get_stat(self, HP)) / 16.0, 1.0)));
			}
			else if (self.status[i] == "SEED") {
				// TODO: Animation goes here
				if (self.wild)
					do_alert(string("The wild ") + get_nickname(self) + string("'s health was sapped by Leech Seed!"));
				else
					do_alert(get_nickname(self) + string("'s health was sapped by Leech Seed!"));
				deal_damage(self, int(max(double(get_stat(self, HP)) / 16.0, 1.0)));
				heal_damage(other, int(max(double(get_stat(self, HP)) / 16.0, 1.0)));
			}
			else if (self.status[i] == "TOXIC") {
				// TODO: Animation goes here
				if (self.wild)
					do_alert(string("The wild ") + get_nickname(self) + string(" is hurt by the poison!"));
				else
					do_alert(get_nickname(self) + string(" is hurt by the poison!"));
				deal_damage(self, int(self.turn_count * max(double(get_stat(self, HP)) / 16.0, 1.0)));
			}
		}
	}
	bool use_move(mon& attacker, mon& defender, string move, bool skip_accuracy_check = false) {
		double pow = stoi(moves[move].pow);
		bool crit;
		int repeat = 1;
		bool success = false;
		bool miss = false;
		string s2, s3;
		if (in_status(attacker, string("FLINCH"))) {
			do_alert(get_nickname(attacker) + string(" flinched!"));
			remove_status(attacker, string("FLINCH"), true);
			attacker.queue.clear();
			return false;
		}
		do_alert(get_nickname(attacker) + string(" used ") + move + string("!"));
		if (! skip_accuracy_check && moves[move].acc < int(random(0.0, 100.0))) {
			miss = true;
			if (pow == 0) {
				do_alert(string("But, it failed!"));
			}
			else {
				do_alert(get_nickname(attacker) + string("'s attack missed!"));
			}
		}
		if (pow != 0.0) {
			if (moves[move].pow.find(string("x2-5")) != -1) {
				repeat = weightedrand() + 2;
			}
			else if (moves[move].pow.find(string("x2")) != -1) {
				repeat = 2;
			}
		}
		if (!miss) {
			for (int i = 0; i < repeat; ++i) {
				double mul;
				int dam = damage(attacker, defender, move, crit, mul);
				// TODO: Logic for detecting that a move has an x0 modifier.
				deal_damage(defender, dam);
				if (mul == 4.0)
					do_alert("It's extremely effective!");
				else if (mul == 2.0)
					do_alert("It's super effective!");
				else if (mul == 0.0)
					do_alert("It had no effect!");
				else if (mul == 0.5)
					do_alert("It's not very effective...");
				else if (mul == 0.25)
					do_alert("It barely had an effect...");
				if (crit)
					do_alert(string("A critical hit!"));
				if (!status_immunity(defender, move)) {
					for (unsigned j = 0; j < moves[move].target.size(); ++j) {
						success = apply_status(defender, moves[move].target[j]) || success;
						// TODO:  Status announcement
					}
				}
				for (unsigned j = 0; j < moves[move].self.size(); ++j) {
					if (moves[move].self[j].find("RECOIL") != -1) {
						s2 = moves[move].self[j];
						s3 = moves[move].self[j];
						s2.erase(s2.find(":"), s2.length());
						s3.erase(0, s3.find(":") + 1);
						deal_damage(attacker, int(max(double(dam)*(double(stoi(s3)) / 100.0), 1.0)));
					}
					else {
						success = apply_status(attacker, moves[move].self[j]) || success;
					}
				}
			}
			for (unsigned i = 0; i < moves[move].additional.size(); ++i) {
				success = use_move(attacker, defender, moves[move].additional[i], true) || success;
			}
		}
		for (unsigned i = 0; i < moves[move].queue.size(); ++i) {
			if (moves[move].queue[i].find("x0-3") == -1)
				attacker.queue.push_back(moves[move].queue[i]);
			else {
				string temp = moves[move].queue[i];
				temp.erase(temp.find("x0-3"), 4);
				repeat = weightedrand();
				for (int j = 0; j < repeat; ++j)
					attacker.queue.push_back(temp);
			}
		}
		if (repeat != 1) {
			do_alert(string("Hit ") + to_string(repeat) + string(" times!"));
		}
		if (attacker.queue.size() > 0) {
			if (attacker.queue[0] == move) {
				attacker.queue.erase(attacker.queue.begin());
			}
		}
		if (miss && in_special(move, string("CLEAR_QUEUE_ON_FAIL"))) {
			clear_queue(attacker);
		}
		return success;
	}
	bool is_KO(mon& m) {
		if (m.curr_hp <= 0)
			return true;
		return false;
	}
	void do_turn_inner(mon& m1, mon& m2) {
		m1.turn_count++;
		m2.turn_count++;
		if (in_status(m1, string("SLEEP"))) {
			remove_status(m1, string("SLEEP"));
			if (in_status(m1, string("SLEEP"))) {
				do_alert(get_nickname(m1) + string(" is fast asleep."));
				// TODO:  Sleep animation.
			}
			else {
				do_alert(get_nickname(m1) + string(" woke up!"));
			}
		}
		else if (in_status(m1, string("FREEZE"))) {
			if (0.2 > random(0.0, 1.0)) {
				remove_status(m1, string("FREEZE"));
				do_alert(get_nickname(m1) + string(" thawed out!"));
				m1.queue.clear();
			}
			else {
				do_alert(get_nickname(m1) + string(" is frozen solid!"));
				m2.queue.clear();
				// TODO:  Freeze animation
			}
		}
		else if (in_status(m1, string("PARALYZE")) && (0.25 > random(0.0, 1.0))) {
			do_alert(get_nickname(m1) + string(" is paralyzed! It can't move!"));
			m1.queue.clear();
			// TODO:  Paralyze animation
		}
		else if (m1.queue[0] != "") {
			use_move(m1, m2, m1.queue[0]);
			if (is_KO(m2)) {
				// TODO:  Return value
				return;
			}
			use_status(m1, m2);
			if (is_KO(m1)) {
				// TODO:  Return value
				return;
			}
		}
		else {
			m1.queue.erase(m1.queue.begin());
		}
		if (in_status(m2, string("SLEEP"))) {
			remove_status(m2, string("SLEEP"));
			if (in_status(m2, string("SLEEP"))) {
				do_alert(get_nickname(m2) + string(" is fast asleep."));
				// TODO:  Sleep animation
			}
			else {
				do_alert(get_nickname(m2) + string(" woke up!"));
			}
		}
		else if (in_status(m2, string("FREEZE"))) {
			if (0.2 > random(0.0, 1.0)) {
				remove_status(m2, string("FREEZE"));
				do_alert(get_nickname(m2) + string(" thawed out!"));
				m2.queue.clear();
			}
			else {
				do_alert(get_nickname(m2) + string(" is frozen solid!"));
				m2.queue.clear();
				// TODO:  Freeze animation
			}
		}
		else if (in_status(m2, string("PARALYZE")) && (0.25 > random(0.0, 1.0))) {
			do_alert(get_nickname(m2) + string(" is paralyzed! It can't move!"));
			m2.queue.clear();
			// TODO:  Paralyze animation
		}
		else if (m2.queue[0] != "") {
			use_move(m2, m1, m2.queue[0]);
			if (is_KO(m1)) {
				// TODO:  Return value
				return;
			}
			use_status(m2, m1);
			if (is_KO(m2)) {
				// TODO:  Return value
				return;
			}
		}
		else {
			m2.queue.erase(m2.queue.begin());
		}
	}
	void clear_volatile(mon& m) {
		map<string, status_effect>::iterator it;
		if (m.defined) {
			for (it = status.begin(); it != status.end(); it++) {
				if (!it->second.nonvolatile) {
					remove_status(m, it->second.name, true);
				}
			}
		}
	}
	void team_clear_volatile() {
		for (unsigned i = 0; i < 6; ++i) {
			clear_volatile(mc.team[i]);
		}
	}
	float get_hp_percent(mon& m) {
		return float(m.curr_hp) / float(get_stat(m, HP));
	}
	void resize_exp_bar(mon& m) {
		if (m.exp_bar_index) {
			float f = float(m.exp - level_to_exp[m.level]) / float(level_to_exp[m.level + 1] - level_to_exp[m.level]);
			f = fmin(f, 1.0f);
			g.draw_list[m.exp_bar_index].width = 0.85f * f;
			g.draw_list[m.exp_bar_index].x = 0.85f * (1.0 - f);
		}
	}
	void resize_hp_bars(mon& m) {
		float f = get_hp_percent(m);
		g.draw_list[m.hp_bar_index].width = 0.6f * f;
		if (f > 0.66f) {
			g.draw_list[m.hp_bar_index].filename = string("../resources/images/hp-good.bmp");
			g.draw_list[m.hp_bar_index].tex = g.tex[string("../resources/images/hp-good.bmp")];
			g.new_load = true;
		}
		else if (f > 0.33f) {
			g.draw_list[m.hp_bar_index].filename = string("../resources/images/hp-medium.bmp");
			g.draw_list[m.hp_bar_index].tex = g.tex[string("../resources/images/hp-medium.bmp")];
			g.new_load = true;
		}
		else {
			g.draw_list[m.hp_bar_index].filename = string("../resources/images/hp-bad.bmp");
			g.draw_list[m.hp_bar_index].tex = g.tex[string("../resources/images/hp-bad.bmp")];
			g.new_load = true;
		}
	}
	void rebuild_battle_hud(mon& p, mon& e, unsigned clear_point) {
		m.lock();
		g.draw_list.erase(g.draw_list.begin() + clear_point, g.draw_list.end());
		g.push_text(-0.9f, 0.8f, 1.5f, 0.1f, 0.1f, get_nickname(e));
		g.push_text(-0.6f, 0.7f, 0.5f, 0.1f, 0.1f, string("{LEVEL}") + to_string(e.level));
		g.push_text(1.0f - (float(get_nickname(p).length() * 0.1f)), -0.1f, 0.0f, 0.1f, 0.1f, get_nickname(p));
		g.push_text(0.4f, -0.2f, 0.5f, 0.1f, 0.1f, string("{LEVEL}") + to_string(p.level));
		string formatted_hp = "";
		formatted_hp += to_string(p.curr_hp) + string("/") + to_string(get_stat(p, HP));
		g.push_text(0.8f - (float(formatted_hp.length())*0.1f), -0.4f, 0.8f, 0.1f, 0.1f, formatted_hp);
		m.unlock();
	}
	bool battle(player& p, trainer& t) { // trainer battle
		// TODO:  Implement trainer battle
		return true;
	}
	bool battle(player& p, mon& m) { // wild pokemon
		int i;
		int index;
		int escape_attempts = 0;
		vector<int> choices;
		double count;
		int clear_point = g.draw_list.size();
		g.push_box(-1.1f, -1.1f, 2.2f, 2.2f);
		g.push_box(-1.0f, -1.0f, 2.0f, 0.6f);
		mc.selected = -1;
		// TODO:  Initial menu
		for (i = 0; i < 6; ++i) {
			if (p.team[i].defined) {
				if (!is_KO(p.team[i])) {
					mc.selected = i;
					break;
				}
			}
		}
		p.team[mc.selected].queue.clear();
		m.queue.clear();
		unsigned enemy_sprite = g.push_quad_load(0.1f, 0.1f, 0.9f, 0.9f, string("../resources/images/") + m.number + string(".png"));
		m.hp_bar_index = g.push_hp_bar(-0.7f, 0.7f, get_hp_percent(m));
		m.hud_index = 0;
		do_alert(string("Wild ") + get_nickname(m) + string(" appeared!"));
		do_alert(string("Go! ") + get_nickname(p.team[mc.selected]) + string("!"));
		string temp = mc.team[mc.selected].number;
		temp.erase(0, temp.find("-") + 1); // TODO:  Back views
		unsigned team_sprite = g.push_quad_load(-1.0f, -0.422f, 0.9f, 0.9f, string("../resources/images/back/") + temp + string(".png"));
		g.push_arrow_box_left(-0.1f, -0.4f, 1.0f, 0.3f);
		g.push_arrow_box_right(-0.9f, 0.6f, 1.0f, 0.2f);
		unsigned team_hp_sprite = g.push_hp_bar(0.1f, -0.2f, get_hp_percent(mc.team[mc.selected]));
		unsigned exp_bar_index = g.push_quad_load(0.0f, -0.35f, 0.0f, 0.025f, string("../resources/images/exp.bmp"));
		unsigned hud_index = g.draw_list.size();
		m.hud_index = hud_index;
		for (i = 0; i < 6; ++i) {
			mc.team[i].hp_bar_index = team_hp_sprite;
			mc.team[i].hud_index = hud_index;
			mc.team[i].exp_bar_index = 0;
		}
		mc.team[mc.selected].exp_bar_index = exp_bar_index;
		resize_exp_bar(mc.team[mc.selected]);
		rebuild_battle_hud(mc.team[mc.selected], m, hud_index);
		while (true) {
			// TODO: Implement player battle menu
			if (p.team[mc.selected].queue.size() == 0) {
				choices = do_menu(string("COMBAT_SELECT"));
				while (choices.size() == 0 || choices[0] == -1) {
					choices = do_menu(string("COMBAT_SELECT"));
				}
				if (choices[0] == 0) { // Player has selected FIGHT
					escape_attempts = 0;
					p.team[mc.selected].queue.push_back(p.team[mc.selected].moves[choices[1]]);
					p.team[mc.selected].pp[choices[1]]--;
				}
				else if (choices[0] == 1) { // Player has selected Pokemon
					p.team[mc.selected].queue.clear();
					do_alert(get_nickname(p.team[mc.selected]) + string("! That's enough!"));
					clear_volatile(p.team[mc.selected]);
					mc.team[mc.selected].exp_bar_index = 0;
					mc.selected = choices[1];
					mc.team[mc.selected].exp_bar_index = exp_bar_index;
					resize_exp_bar(mc.team[mc.selected]);
					temp = mc.team[mc.selected].number;
					temp.erase(0, temp.find("-") + 1); // TODO:  Back views
					g.draw_list[team_sprite].filename = string("../resources/images/back/") + temp + string(".png");
					g.draw_list[team_sprite].tex = g.tex[string("../resources/images/back/") + temp + string(".png")];
					g.new_load = true;
					resize_hp_bars(mc.team[mc.selected]);
					rebuild_battle_hud(mc.team[mc.selected], m, hud_index);
					do_alert(string("Go, ") + get_nickname(p.team[mc.selected]) + string("!"));
					p.team[mc.selected].queue.clear();
					p.team[mc.selected].queue.push_back(string(""));
				}
				else if (choices[0] == 2) { // Player has selected ITEM
					// TODO:  Implement inventory
					int out = 0;
					string o;
					if (!use_item(string("COMBAT"), choices, o))
						continue;
					p.team[mc.selected].queue.insert(p.team[mc.selected].queue.begin(), string(""));
					if (o.find("CAPTURE") == 0) {
						o.erase(0, o.find(":") + 1);
						out = attempt_capture(stof(o), m);
					}
					// TODO:  Capture messages
					if (out == 4) {
						m.wild = false;
						m.enemy = false;
						// TODO:  Nickname menu
						gain_exp(p.team[mc.selected], m, 1); // TODO:  Implement exp split
						for (int i = 0; i < 6; ++i) {
							if (!p.team[i].defined) {
								do_alert(string("Wild ") + get_nickname(m) + string(" was captured!")); // TODO:  Fact-check string
								p.team[i] = m;
								g.draw_list.erase(g.draw_list.begin() + clear_point, g.draw_list.end());
								return true;
							}
						}
						// TODO:  Implement storage
					}
				}
				else if (choices[0] == 3) { // Player has selected RUN
					escape_attempts++;
					p.team[mc.selected].queue.clear();
					if (run_away(p.team[mc.selected], m, escape_attempts)) {
						do_alert(string("Got away safely!"));
						g.draw_list.erase(g.draw_list.begin() + clear_point, g.draw_list.end());
						team_clear_volatile();
						return false;
					}
					else {
						do_alert(string("Couldn't get away!"));
					}
					p.team[mc.selected].queue.push_back(string(""));
				}
			}
			count = 0.0;
			for (i = 0; i < 4; i++) {
				if (moves[m.moves[i]].defined) { // TODO: Make this factor in power points
					count = count + 1.0;
				}
			}
			index = int(random(0.0, count));
			m.queue.push_back(m.moves[index]);
			m.pp[index]--;
			do_turn(p.team[mc.selected], m);
			remove_status(p.team[mc.selected], string("FLINCH"), true);
			remove_status(m, string("FLINCH"), true);
			if (is_KO(m)) {
				do_alert(string("Enemy ") + get_nickname(m) + string(" fainted!"));
				gain_exp(p.team[mc.selected], m, 1);
				g.draw_list.erase(g.draw_list.begin() + clear_point, g.draw_list.end());
				break;
			}
			if (is_KO(p.team[mc.selected])) {
				// TODO:  Handle selection on KO
				do_alert(get_nickname(p.team[mc.selected]) + string(" fainted!"));
				for (i = 0; i < 6; ++i) {
					if (!is_KO(p.team[i])) {
						mc.team[mc.selected].exp_bar_index = 0;
						mc.selected = i;
						mc.team[mc.selected].exp_bar_index = exp_bar_index;
						resize_exp_bar(mc.team[mc.selected]);
						break;
					}
				}
				if (i == 6) {
					// TODO:  Handle defeat
					do_alert("You lost!");
					g.draw_list.erase(g.draw_list.begin() + clear_point, g.draw_list.end());
					team_clear_volatile();
					return false;
				}
				temp = mc.team[mc.selected].number;
				temp.erase(0, temp.find("-") + 1); // TODO:  Back views
				g.draw_list[team_sprite].filename = string("../resources/images/back/") + temp + string(".png");
				g.draw_list[team_sprite].tex = g.tex[string("../resources/images/back/") + temp + string(".png")];
				g.new_load = true;
				resize_hp_bars(mc.team[mc.selected]);
				rebuild_battle_hud(mc.team[mc.selected], m, hud_index);
			}
		}
		team_clear_volatile();
		return true;
	}
	void do_turn(mon& m1, mon& m2) {
		// TODO:  Expand this to support items, fleeing, and switching.
		// TODO:  Make handle nothing in queue.
		bool m1first = in_special(m1.queue[0], string("FIRST"));
		bool m2first = in_special(m2.queue[0], string("FIRST"));
		if (m1first && !m2first) {
			do_turn_inner(m1, m2);
		}
		else if (!m1first && m2first) {
			do_turn_inner(m2, m1);
		}
		else if (m1first && m2first && (get_stat(m1, SPEED) > get_stat(m2, SPEED))) {
			do_turn_inner(m1, m2);
		}
		else if (m1first && m2first && (get_stat(m1, SPEED) <= get_stat(m2, SPEED))) {
			do_turn_inner(m2, m1);
		}
		else if (get_stat(m1, SPEED) > get_stat(m2, SPEED)) {
			do_turn_inner(m1, m2);
		}
		else {
			do_turn_inner(m2, m1);
		}
	}
	int damage(mon& attacker, mon& defender, string move, bool& crit, double &mul) {
		double pow = stoi(moves[move].pow);
		mul = 1.0;
		crit = false;
		if (pow == 0.0)
			return 0;

		int buff = 0;
		for (unsigned i = 0; i < attacker.status.size(); ++i) {
			if (attacker.status[i] == this->get_buff_name(SPEED)) {
				buff++;
			}
			if (attacker.status[i] == this->get_debuff_name(SPEED)) {
				buff--;
			}
		}
		double chance = get_stat_modifier(buff) * all_mon[attacker.number].stats[SPEED] / 4.0;
		if (in_status(attacker, string("FOCUS")))
			chance *= 4.0;
		chance *= moves[move].crit_chance;
		if (random(0.0, 256.0) < chance) {
			crit = true;
		}
		double damage = 2.0 * attacker.level;
		damage = damage + 10.0;
		damage *= get_stat(attacker, moves[move].attack, false, crit);
		damage *= pow;
		damage /= 250.0;
		damage /= get_stat(defender, moves[move].defense, crit, false);
		damage += 2.0;
		if (all_mon[defender.number].type1 != "") {
			mul *= types[moves[move].type][all_mon[defender.number].type1];
			damage *= types[moves[move].type][all_mon[defender.number].type1];
		}
		if (all_mon[defender.number].type2 != "") {
			mul *= types[moves[move].type][all_mon[defender.number].type2];
			damage *= types[moves[move].type][all_mon[defender.number].type2];
		}
		if ((all_mon[attacker.number].type1 == moves[move].type) || (all_mon[attacker.number].type2 == moves[move].type))
			damage *= 1.5;
		if (crit) {
			damage *= 1.5;
		}
		damage *= random(0.85, 1.0);
		return int(damage);
	}
	string get_buff_name(STAT s) {
		switch (s) {
		case ATTACK:
			return string("ATTACK_UP");
		case DEFENSE:
			return string("DEFENSE_UP");
		case SPEED:
			return string("SPEED_UP");
		case SPECIAL:
			return string("SPECIAL_UP");
		default:
			return string("");
		}
	}
	string get_debuff_name(STAT s) {
		switch (s) {
		case ATTACK:
			return string("ATTACK_DOWN");
		case DEFENSE:
			return string("DEFENSE_DOWN");
		case SPEED:
			return string("SPEED_DOWN");
		case SPECIAL:
			return string("SPECIAL_DOWN");
		default:
			return string("");
		}
	}
	double get_stat_modifier(int buff) {
		if (buff < -6)
			buff = -6;
		if (buff > 6)
			buff = 6;
		switch (buff) {
		case -6:
			return 2.0 / 8.0;
		case -5:
			return 2.0 / 7.0;
		case -4:
			return 2.0 / 6.0;
		case -3:
			return 2.0 / 5.0;
		case -2:
			return 2.0 / 4.0;
		case -1:
			return 2.0 / 3.0;
		case 0:
			return 2.0 / 2.0;
		case 1:
			return 3.0 / 2.0;
		case 2:
			return 4.0 / 2.0;
		case 3:
			return 5.0 / 2.0;
		case 4:
			return 6.0 / 2.0;
		case 5:
			return 7.0 / 2.0;
		case 6:
			return 8.0 / 2.0;
		default:
			return 1.0;
		}
	}
	int get_stat(mon& m, STAT s, bool ignore_buffs=false, bool ignore_debuffs=false) {
		int ret;
		if (s == HP) {
			ret = all_mon[m.number].stats[s] + m.IV[s];
			ret = ret * 2;
			ret = ret + (sqrt(min(65535, m.EV[s])) / 4);
			ret = ret * m.level;
			ret = ret / 100;
			ret = ret + m.level + 10;
		}
		else {
			ret = all_mon[m.number].stats[s] + m.IV[s];
			ret = ret * 2;
			ret = ret + (sqrt(min(65535, m.EV[s])) / 4);
			ret = ret * m.level;
			ret = ret / 100;
			ret = ret + 5;
			if (!ignore_debuffs) {
				if (s == ATTACK && in_status(m, string("BURN")))
					ret = ret / 2;
				if (s == SPEED && in_status(m, string("PARALYZE")))
					ret = ret / 4;
			}
		}
		int buff = 0;
		for (unsigned i = 0; i < m.status.size(); ++i) {
			if (!ignore_buffs) {
				if (m.status[i] == this->get_buff_name(s)) {
					buff++;
				}
			}
			if (!ignore_debuffs) {
				if (m.status[i] == this->get_debuff_name(s)) {
					buff--;
				}
			}
		}
		return int(ret * get_stat_modifier(buff));
	}
	void init_items() {
		ifstream f("../resources/data/items.dat");
		string line;
		string key;
		string temp;
		while (f.is_open()) {
			while (std::getline(f, line)) {
				if (line.find("ITEM:") == 0) {
					line.erase(0, string("ITEM:").length());
					key = line;
					items[key].defined = true;
				}
				else if (line.find("USE:") == 0) {
					line.erase(0, string("USE:").length());
					temp = line;
					while (line.find("|") != -1) {
						line.erase(0, line.find("|") + 1);
						temp.erase(temp.find("|"), temp.length());
						items[key].use.insert(items[key].use.begin(), temp);
						temp = line;
					}
					items[key].use.insert(items[key].use.begin(), temp);
				}
				else if (line.find("EFFECT:") == 0) {
					line.erase(0, string("EFFECT:").length());
					temp = line;
					while (line.find("|") != -1) {
						line.erase(0, line.find("|") + 1);
						temp.erase(temp.find("|"), temp.length());
						items[key].effect.insert(items[key].effect.begin(), temp);
						temp = line;
					}
					items[key].effect.insert(items[key].effect.begin(), temp);
				}
				else if (line.find("PRICE:") == 0) {
					line.erase(0, string("PRICE:").length());
					items[key].price = stoi(line);
				}
			}
			f.close();
		}
	}
	void init_levels() {
		ifstream f("../resources/data/levels.dat");
		string line;
		while (f.is_open()) {
			while (std::getline(f, line))
				init_level(line);
			f.close();
		}
	}
	void init_level(string levelname) {
		ifstream f(("../resources/levels/" + levelname + ".dat").c_str());
		string line;
		std::vector<int> empty;
		unsigned count;
		while (f.is_open()) {
			std::getline(f, line);
			if (line == "NAME") {
				std::getline(f, line);
				levels[levelname].name = line;
				continue;
			}
			if (line == "NPCS") {
				std::getline(f, line);
				while (line != "TELEPORT" && line != "DATA" && line != "ENCOUNTERS" && line != "LEVELS" && line != "TRAINERS") {
					character c;
					string s;
					levels[levelname].characters.push_back(c);
					s = line;
					s.erase(s.find(" "), s.length());
					line.erase(0, line.find(" ") + 1);
					levels[levelname].characters[levels[levelname].characters.size() - 1].name = s;
					s = line;
					s.erase(s.find(" "), s.length());
					line.erase(0, line.find(" ") + 1);
					levels[levelname].characters[levels[levelname].characters.size() - 1].image = s;
					s = line;
					s.erase(s.find(" "), s.length());
					line.erase(0, line.find(" ") + 1);
					levels[levelname].characters[levels[levelname].characters.size() - 1].origin.x = stoi(s);
					s = line;
					if (s.find(" ") != -1)
						s.erase(s.find(" "), s.length());
					if (line.find(" ") != -1)
						line.erase(0, line.find(" ") + 1);
					levels[levelname].characters[levels[levelname].characters.size() - 1].origin.y = stoi(s);
					levels[levelname].characters[levels[levelname].characters.size() - 1].origin.level = levelname;
					levels[levelname].characters[levels[levelname].characters.size() - 1].loc = levels[levelname].characters[levels[levelname].characters.size() - 1].origin;
					s = line;
					if (s.find("INACTIVE") != -1) {
						levels[levelname].characters[levels[levelname].characters.size() - 1].active = false;
					}
					if (s == "WANDER") {
						levels[levelname].characters[levels[levelname].characters.size() - 1].wander = true;
						s.erase(0, string("WANDER").length() + 1);
					}
 					else {
						levels[levelname].characters[levels[levelname].characters.size() - 1].wander = false;
						if (s.find(" ") != -1) {
							s.erase(0, s.find(" ") + 1);
						}
						else {
							s = "";
						}
					}
					if (s != "") {
						if (s == "LEFT")
							levels[levelname].characters[levels[levelname].characters.size() - 1].dir = LEFT;
						else if (s == "RIGHT")
							levels[levelname].characters[levels[levelname].characters.size() - 1].dir = RIGHT;
						else if (s == "UP")
							levels[levelname].characters[levels[levelname].characters.size() - 1].dir = UP;
						else if (s == "DOWN")
							levels[levelname].characters[levels[levelname].characters.size() - 1].dir = DOWN;
					}
					// TODO:  LOAD FROM CHARACTER SAVE
					mc.interaction[levels[levelname].characters[levels[levelname].characters.size() - 1].name] = 0;
					std::getline(f, line);
					if (line == "FORCE_INTERACTION") {
						std::getline(f, line);
						while (line != "END") {
							location l;
							s = line;
							s.erase(s.find(" "), s.size());
							line.erase(0, line.find(" ") + 1);
							l.x = double(stoi(s));
							l.y = double(stoi(line));
							levels[levelname].characters[levels[levelname].characters.size() - 1].force_interactions.push_back(l);
							std::getline(f, line);
						}
						std::getline(f, line);
					}
					if (line == "INTERACTIONS") {
						std::getline(f, line);
						while (line != "END") {
							while (line.find(string("{NEWLINE}")) != -1) {
								int i;
								i = line.find(string("{NEWLINE}"));
								line.erase(i, string("{NEWLINE}").length());
								line.insert(i, string("\n"));
							}
							levels[levelname].characters[levels[levelname].characters.size() - 1].interactions.push_back(line);
							std::getline(f, line);
						}
						std::getline(f, line);
					}
				}
			}
			if (line == "TRAINERS") {
				std::getline(f, line);
				trainer d;
				while (line != "DATA" && line != "ENCOUNTERS" && line != "LEVELS" && line != "TELEPORT" && line != "END") {
					string s, s2, s3;
					unsigned i = 1;
					unsigned count = 1;
					s = line;
					s2 = s;
					s.erase(0, s.find(" ") + 1);
					s2.erase(s2.find(" "), s2.length());
					d.name = s2;
					s2 = s;
					s.erase(0, s.find(" ") + 1);
					s2.erase(s2.find(" "), s2.length());
					d.payout = stoi(s2);
					s2 = s;
					s.erase(0, s.find(" ") + 1);
					s2.erase(s2.find(" "), s2.length());
					d.skill = stof(s2);
					s2 = s;
					s.erase(0, s.find(" ") + 1);
					s2.erase(s2.find(" "), s2.length());
					d.knowledge = stof(s2);
					s2 = s;
					s.erase(0, s.find(" ") + 1);
					s2.erase(s2.find(" "), s2.length());
					d.display_name = s2;
					s2 = s;
					s.erase(0, s.find(" ") + 1);
					s2.erase(s2.find(" "), s2.length());
					d.image = s2;
					s2 = s;
					while (count > 0) {
						if (s2[i] == '{')
							count++;
						if (s2[i] == '}')
							count--;
						++i;
					}
					s2.erase(i-1, s2.length());
					s2.erase(0, 1);
					d.win_message = s2;
					s.erase(0, i);
					s2 = s;
					i = 1;
					count = 1;
					while (count > 0) {
						if (s2[i] == '{')
							count++;
						if (s2[i] == '}')
							count--;
						++i;
					}
					s2.erase(i - 1, s2.length());
					s2.erase(0, 1);
					d.lose_message = s2;
					s.erase(0, i + 1);
					i = 0;
					for (unsigned j = 0; j < 6; ++j) {
						d.team[j].defined = false;
					}
					while (s.find(":") != -1) {
						if (s.find("|") == 0)
							s.erase(0, 1);
						s2 = s;
						s2.erase(s2.find(":"), s2.length());
						s.erase(0, s.find(":") + 1);
						s3 = s;
						if (s3.find(":") != -1) {
							s3.erase(s3.find(":"), s3.length());
						}
						if (s3.find(" ") != -1) {
							s3.erase(s3.find(" "), s3.length());
						}
						count = stoi(s3);
						s.erase(0, s3.length());
						make_mon(s2, count, d.team[i]);
						d.team[i].wild = false;
						if (s.find(":") == 0) {
							s.erase(0, 1);
							count = 0;
							while (s != "" && s.find("|") != 0) {
								s2 = s;
								if (s2.find("|") != -1) {
									s2.erase(s2.find("|"), s2.length());
								}
								if (s2.find(",") != -1) {
									s2.erase(s2.find(","), s2.length());
								}
								create_move(d.team[i], s2, count);
								s.erase(0, s2.length());
								if (s.find(",") == 0) {
									s.erase(0, 1);
								}
								count++;
							}
						}
						i = i + 1;
					}
					levels[levelname].trainers[d.name] = d;
					std::getline(f, line);
				}
				std::getline(f, line);
			}
			if (line == "TELEPORT")
			{
				std::getline(f, line);
				std::pair<location, location> temp;
				while (line != "DATA" && line != "ENCOUNTERS" && line != "LEVELS") {
					temp.first.level = levelname;
					temp.first.x = double(stoi(line));
					line.erase(0, line.find(" ") + 1);
					temp.first.y = double(stoi(line));
					line.erase(0, line.find(" ") + 1);
					temp.second.level = "";
					while (line[0] != ' ') {
						temp.second.level += line[0];
						line.erase(0, 1);
					}
					line.erase(0, 1);
					temp.second.x = double(stoi(line));
					line.erase(0, line.find(" ") + 1);
					temp.second.y = double(stoi(line));
					levels[levelname].teleport.push_back(temp);
					std::getline(f, line);
				}
			}
			if (line == "ENCOUNTERS") {
				std::getline(f, line);
				while (line.size() > 0) {
					levels[levelname].encounters.push_back(stoi(line));
					if (line.find(' ') == -1)
						break;
					line.erase(0, line.find(' ') + 1);
					while (line.find(' ') == 0)
						line.erase(0, 1);
				}
				std::getline(f, line);
			}
			if (line == "LEVELS") {
				std::getline(f, line);
				levels[levelname].level_range.first = stoi(line);
				line.erase(0, line.find('-') + 1);
				levels[levelname].level_range.second = stoi(line);
				std::getline(f, line);
			}
			if (line == "DATA") {
				while (std::getline(f, line)) {
					levels[levelname].data.push_back(empty);
					while (line.length() > 0) {
						levels[levelname].data[levels[levelname].data.size() - 1].push_back(stoi(line));
						count = 0;
						while (line[count] != '\n' && line[count] != ' ' && count < line.length())
							count++;
						while (line[count] == ' ')
							count++;
						line.erase(0, count);
					}
				}
			}
			f.close();
		}
	}
	void init_moves() {
		string line, key;
		ifstream f("../resources/data/moves.dat");
		char a = f.get();
		while (a != EOF && f.is_open()) {
			line = "";
			while (a != ':') {
				line = line + a;
				a = f.get();
			}
			while (a == ':' || a == ' ')
				a = f.get();
			if (line == "MOVE") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				key = line;
				moves[key].defined = true;
				moves[key].name = key;
			}
			else if (line == "TYPE") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				moves[key].type = line;
			}
			else if (line == "POWER") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				moves[key].pow = line;
			}
			else if (line == "DEFENSE") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				if (line == "SPECIAL") {
					moves[key].attack = SPECIAL;
					moves[key].defense = SPECIAL;
				}
			}
			else if (line == "CRITICAL") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				if (line == "HIGH") {
					moves[key].crit_chance = 8.0f;
				}
				else if (line == "NORMAL") {
					moves[key].crit_chance = 1.0f;
				}
				else if (line == "NONE") {
					moves[key].crit_chance = 0.0f;
				}
			}
			else if (line == "QUEUE_ONLY") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				if (line == "YES") {
					moves[key].queue_only = true;
				}
			}
			else if (line == "ACC") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				moves[key].acc = stoi(line);
			}
			else if (line == "PP") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				moves[key].pp = stoi(line);
			}
			else if (line == "SELF") {
				while (a != '\r' && a != '\n' && a != EOF) {
					line = "";
					while (a != ' ' && a != '\r' && a != '\n' && a != EOF) {
						line = line + a;
						a = f.get();
					}
					while (a == ' ')
						a = f.get();
					moves[key].self.push_back(line);
				}
			}
			else if (line == "TARGET") {
				while (a != '\r' && a != '\n' && a != EOF) {
					line = "";
					while (a != ' ' && a != '\r' && a != '\n' && a != EOF) {
						line = line + a;
						a = f.get();
					}
					while (a == ' ')
						a = f.get();
					moves[key].target.push_back(line);
				}
			}
			else if (line == "QUEUE") {
				while (a != '\r' && a != '\n' && a != EOF) {
					line = "";
					while (a != ' ' && a != '\r' && a != '\n' && a != EOF) {
						line = line + a;
						a = f.get();
					}
					while (a == ' ')
						a = f.get();
					moves[key].queue.push_back(line);
				}
			}
			else if (line == "SPECIAL") {
				while (a != '\r' && a != '\n' && a != EOF) {
					line = "";
					while (a != ' ' && a != '\r' && a != '\n' && a != EOF) {
						line = line + a;
						a = f.get();
					}
					while (a == ' ')
						a = f.get();
					moves[key].special.push_back(line);
				}
			}
			else if (line == "ADDITIONAL") {
				while (a != '\r' && a != '\n' && a != EOF) {
					line = "";
					while (a != ' ' && a != '\r' && a != '\n' && a != EOF) {
						line = line + a;
						a = f.get();
					}
					while (a == ' ')
						a = f.get();
					moves[key].additional.push_back(line);
				}
			}
			else if (line == "DESC") {
				line = "";
				while (a != '\r' && a != '\n' && a != EOF) {
					line = line + a;
					a = f.get();
				}
				moves[key].desc = line;
			}
			else if (line == "NOTES") {
				line = "";
				while (a != '\r' && a != '\n' && a != EOF) {
					line = line + a;
					a = f.get();
				}
				moves[key].notes = line;
			}

			while (a != '\n' && a != EOF)
				a = f.get();
			while (a == '\n' && a != EOF)
				a = f.get();
			if (a == EOF) {
				f.close();
				break;
			}
		}
	}
	void init_tm() {
		string line, index;
		ifstream f("../resources/data/TM.dat");
		while (std::getline(f, line)) {
			index = line;
			index.erase(index.find(":"), index.size());
			line.erase(0, line.find(":") + 1);
			TM[stoi(index)] = line;
		}
		f.close();
	}
	void init_hm() {
		string line, index;
		ifstream f("../resources/data/HM.dat");
		while (std::getline(f, line)) {
			index = line;
			index.erase(index.find(":"), index.size());
			line.erase(0, line.find(":") + 1);
			HM[stoi(index)] = line;
		}
		f.close();
	}
	void init_mon() {
		string line, key, temp;
		ifstream f("../resources/data/mon.dat");
		char a = 1;
		int level = 0;
		while (f.is_open() && !f.eof()) {
			line = "";
			std::getline(f, temp);
			if (temp.find(":") != -1) {
				line = temp;
				line.erase(line.find(":"), line.length());
				temp.erase(0, temp.find(":") + 1);
				while (temp.find(" ") == 0)
					temp.erase(0, 1);
			}
			if (line == "NUMBER") {
				key = temp;
				all_mon[key].number = temp;
				all_mon[key].defined = true;
			}
			else if (line == "NAME") {
				all_mon[key].name = temp;
			}
			else if (line == "SURNAME") {
				all_mon[key].name = all_mon[key].name + temp;
			}
			else if (line == "SWITCH") {
			}
			else if (line == "TYPE1") {
				all_mon[key].type1 = temp;
			}
			else if (line == "TYPE2") {
				all_mon[key].type2 = temp;
			}
			else if (line == "DEX") {
				all_mon[key].dex = temp;
			}
			else if (line == "DEX1") {
				all_mon[key].dex = temp;
			}
			else if (line == "DEX2") {
				all_mon[key].dex = all_mon[key].dex + string(" ") + temp;
			}
			else if (line == "EXP_YIELD") {
				all_mon[key].exp_yield = stoi(temp);
			}
			else if (line == "CATCHRATE") {
				all_mon[key].catchrate = stoi(temp);
			}
			else if (line == "HEIGHT") {
				all_mon[key].height = stoi(temp);
			}
			else if (line == "WEIGHT") {
				all_mon[key].weight = stof(temp);
			}
			else if (line == "HP") {
				all_mon[key].stats[HP] = stoi(temp);
			}
			else if (line == "ATTACK") {
				all_mon[key].stats[ATTACK] = stoi(temp);
			}
			else if (line == "DEFENSE") {
				all_mon[key].stats[DEFENSE] = stoi(temp);
			}
			else if (line == "SPECIAL") {
				all_mon[key].stats[SPECIAL] = stoi(temp);
			}
			else if (line == "SPEED") {
				all_mon[key].stats[SPEED] = stoi(temp);
			}
			else if (line == "MOVES") {
				pair<int, string> tempmove;
				std::getline(f, temp);
				while (temp.find(":") == -1) {
					line = temp;
					line.erase(line.find(" "), line.length());
					temp.erase(0, temp.find(" ") + 1);
					tempmove.first = stoi(line);
					tempmove.second = temp;
					all_mon[key].learned.push_back(tempmove);
					std::getline(f, temp);
				}
				line = temp;
				line.erase(line.find(":"), line.length());
				temp.erase(0, temp.find(":") + 1);
			}
			if (line == "EVOLUTION") {
				pair<string, string> tempev;
				std::getline(f, temp);
				while (temp.find("TM") != 0) {
					pair<string, string> tempev;
					line = temp;
					line.erase(line.find(":"), line.length());
					temp.erase(0, temp.find(":") + 1);
					tempev.first = line;
					tempev.second = temp;
					all_mon[key].evolution.push_back(tempev);
					std::getline(f, temp);
				}
				line = temp;
				line.erase(line.find(":"), line.length());
				temp.erase(0, temp.find(":") + 1);
			}
			if (line == "TM") {
				while (temp.find(" ") == 0)
					temp.erase(0, 1);
				while (temp != "") {
					line = temp;
					if (temp.find(" ") != -1) {
						line.erase(line.find(" "), line.length());
						temp.erase(0, temp.find(" ") + 1);
					}
					else
						temp = "";
					all_mon[key].TM[stoi(line)] = true;
				}
			}
			if (line == "HM") {
				while (temp.find(" ") == 0)
					temp.erase(0, 1);
				while (temp != "") {
					line = temp;
					if (temp.find(" ") != -1) {
						line.erase(line.find(" "), line.length());
						temp.erase(0, temp.find(" ") + 1);
					}
					else
						temp = "";
					all_mon[key].HM[stoi(line)] = true;
				}
			}
		}
	}
	void init_status(){
		string line, key;
		ifstream f("../resources/data/status.dat");
		char a = f.get();
		while (f.is_open()) {
			line = "";
			while (a == '\r' || a == '\n')
				a = f.get();
			while (a != ':' && a != '\n') {
				line = line + a;
				a = f.get();
				if (a == EOF) {
					f.close();
					return;
				}
			}
			key = line;
			status[key].defined = true;
			status[key].name = line;
			while (true) {
				if (a == '\n')
					break;
				while (a == ' ' || a == ':') {
					a = f.get();
				}
				line = "";
				while (a != ' ' && a != ':' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				if (line == "NONVOLATILE")
					status[key].nonvolatile = true;
				else if (line == "CHANCE")
					status[key].chance = true;
				else if (line == "SINGLETON")
					status[key].singleton = true;
				else if (line == "SPECIALCASE")
					status[key].specialcase = true;
			}
			while (a != '\n') {
				a = f.get();
			}
		}
		f.close();
	}
	void init_blocking() {
		string line;
		ifstream f("../resources/data/blocking-tiles.dat");
		while (f.is_open()) {
			while (std::getline(f, line)) {
				blocking[stoi(line)] = true;
			}
			f.close();
		}
	}
	void init_jumpdown() {
		string line;
		ifstream f("../resources/data/jumpdown-tiles.dat");
		while (f.is_open()) {
			while (std::getline(f, line)) {
				jumpdown[stoi(line)] = true;
			}
			f.close();
		}
	}
	void init_swimming() {
		string line;
		ifstream f("../resources/data/water-tiles.dat");
		while (f.is_open()) {
			while (std::getline(f, line)) {
				water[stoi(line)] = true;
			}
			f.close();
		}
	}
	void init_exp() {
		string line;
		ifstream f("../resources/data/exp.dat");
		while (f.is_open()) {
			while (std::getline(f, line)) {
				level_to_exp.push_back(stoi(line));
			}
			f.close();
		}
	}
	void init_special(){
		string line;
		ifstream f("../resources/data/special.dat");
		while (f.is_open()) {
			while (std::getline(f, line)) {
				special_case[line] = true;
			}
			f.close();
		}
	}
	void init_types(){
		string line;
		ifstream f("../resources/data/types.dat", ios::binary);
		char a = 1;
		while (f.is_open()){
			while (a && a != EOF) {
				a = f.get();
				if (a != ':')
					line = line + a;
				if (a == ':') {
					types[line];
					while (a != '\n' && a != 0) {
						a = f.get();
					}
					line = "";
					continue;
				}
			}
			f.close();
		}

		for (type_iter it1 = types.begin(); it1 != types.end(); it1++) {
			for (type_iter it2 = types.begin(); it2 != types.end(); it2++){
				types[it1->first][it2->first] = 1.0f;
			}
		}

		ifstream f2("../resources/data/types.dat", ios::binary);
		a = 1;
		string attack, defense, mul;
		while (f2.is_open()){
			while (a && a != EOF) {
				a = f2.get();
				if (a == '\r')
					a = f2.get();
				if (a == '\n')
					a = f2.get();
				if (a != ':')
					attack = attack + a;
				if (a == ':') {
					while (a == ':' || a == ' ') {
						a = f2.get();
					}
					while (true) {
						defense = "";
						mul = "";
						while (a != 'x'){
							defense = defense + a;
							a = f2.get();
						}
						a = f2.get();
						while (a != ' ' && a != '\r' && a != '\n') {
							mul = mul + a;
							a = f2.get();
						}
						if (mul == "2") {
							types[attack][defense] = 2.0f;
						}
						else if (mul == "0.5") {
							types[attack][defense] = 0.5f;
						}
						else if (mul == "0") {
							types[attack][defense] = 0.0f;
						}
						while (a == ' ')
							a = f2.get();
						if (a == '\n' || a == '\r')
							break;
					}
					attack = "";
					continue;
				}
			}
			f2.close();
		}
	}
	int weightedrand() {
		int i = int(random(0.0, 8.0));
		switch (i) {
		case 0:
		case 1:
		case 2:
			return 0;
		case 3:
		case 4:
		case 5:
			return 1;
		case 6:
			return 2;
		case 7:
			return 3;
		default:
			return 0;
		}
	}
	void handle_teleport() {
		for (unsigned i = 0; i < levels[current_level].teleport.size(); ++i) {
			if (mc.loc.x == levels[current_level].teleport[i].first.x && mc.loc.y == levels[current_level].teleport[i].first.y) {
				transition.lock();
				mc.loc.x = levels[current_level].teleport[i].second.x;
				mc.loc.y = levels[current_level].teleport[i].second.y;
				current_level = levels[current_level].teleport[i].second.level;
				transition.unlock();
				break;
			}
		}
	}
	int get_tile(double y, double x) {
		return levels[current_level].data[int(y)][int(x)];
	}
	void input(bool up, bool down, bool left, bool right, bool select, bool start, bool confirm, bool cancel) {
		if (menus.size() == 0) {
			// TODO:  Animations
			location l = mc.loc;
			ahead = l;
			switch (mc.dir) {
			case UP:
				ahead.y -= 1.0;
				break;
			case DOWN:
				ahead.y += 1.0;
				break;
			case LEFT:
				ahead.x -= 1.0;
				break;
			case RIGHT:
				ahead.x += 1.0;
				break;
			}
			if (left) {
				l.x -= 1.0;
				mc.dir = LEFT;
			}
			if (right) {
				l.x += 1.0;
				mc.dir = RIGHT;
			}
			if (up) {
				l.y -= 1.0;
				mc.dir = UP;
			}
			if (down) {
				l.y += 1.0;
				mc.dir = DOWN;
			}
			if (l.y == -1.0)
				return;
			if (l.y >= levels[current_level].data.size())
				return;
			if (l.x == -1.0)
				return;
			if (l.x >= levels[current_level].data[int(l.y)].size())
				return;
			if (blocking[get_tile(l.y, l.x)])
				return;
			for (unsigned i = 0; i < levels[current_level].characters.size(); ++i) {
				if (!levels[current_level].characters[i].active)
					continue;
				if (l.x == levels[current_level].characters[i].loc.x && l.y == levels[current_level].characters[i].loc.y)
					return;
			}
			if (confirm) {
				interact = true;
			}
			mc.loc = l;
			update_level();
			if (get_tile(mc.loc.y, mc.loc.x) == 4) {
				if (levels[current_level].encounters.size() > 0) {
					if (random(0.0, 187.5) < 8.5) {
						int choice = int(random(0.0, double(levels[current_level].encounters.size())));
						int choice2 = int(random(0.0, double(levels[current_level].encounters.size())));
						// TODO:  Switch this over to fusions
						encounter = std::to_string(levels[current_level].encounters[choice]) + string("-") + std::to_string(levels[current_level].encounters[choice2]);
						int l = int(random(levels[current_level].level_range.first, levels[current_level].level_range.second + 1));
						encounter_level = l;
					}
				}
			}
		}
		else {
			menus[menus.size() - 1]->input(up, down, left, right, select, start, confirm, cancel);
		}
	}
	void create_menu(string s, string choice = "", string text_override = "", string followup_override = "") {
		menu* m = new menu;
		menus.push_back(m);
		menus[menus.size() - 1]->create_menu(s, choice, text_override, followup_override);
		menus[menus.size() - 1]->push_menu();
	}
	vector<int> do_alert(string s) {
		return do_menu(string("ALERT"), s);
	}
	vector<int> do_menu(string menu, string choice = "", string text_override = "", string followup_override = "") {
		vector<int> out;
		create_menu(menu, choice, text_override, followup_override);
		out = menus[menus.size() - 1]->main();
		delete menus[menus.size() - 1];
		menus.erase(menus.end() - 1);
		return out;
	}
	void update_level() {
		m.lock();
		g.draw_list.clear();
		draw_level();
		draw_characters();
		m.unlock();
	}
	void draw_level() {
		for (unsigned y = 0; y < levels[current_level].data.size(); ++y) {
			for (unsigned x = 0; x < levels[current_level].data[y].size(); ++x) {
				g.push_quad(-1.0f + (float(x) / 5.0f) - ((mc.loc.x - 4.5f) / 5.0f), 
					(-float(y) / 4.5f) - (0.5f / 4.5f) + (mc.loc.y / 4.5f), 
					1.0f / 5.0f, 1.0f / 4.5f, g.tiles[levels[current_level].data[y][x]]);
			}
		}
	}
	void draw_characters() {
		for (unsigned i = 0; i < levels[current_level].characters.size(); ++i) {
			if (!levels[current_level].characters[i].active)
				continue;
			g.push_quad((levels[current_level].characters[i].loc.x - (mc.loc.x + 0.5))/5.0,
				(-0.5 - levels[current_level].characters[i].loc.y + mc.loc.y)/ 4.5f + 0.055,
				1.0 / 5.0f, 1.0 / 4.5f, g.tex[levels[current_level].characters[i].image + string("-") + get_direction_string(levels[current_level].characters[i].dir) + string("-0.bmp")]);
		}
		g.push_quad(-0.1, -0.5 / 4.5 + 0.055, 1.0 / 5.0, 1.0 / 4.5, g.tex[string("player-") + get_direction_string(mc.dir) + string("-0.bmp")]);
	}
	void main() {
		while (true) {
			handle_teleport();
			transition.lock();
			for (unsigned i = 0; i < levels[current_level].characters.size(); ++i) {
				if (!levels[current_level].characters[i].active)
					continue;
				for (unsigned j = 0; j < levels[current_level].characters[i].force_interactions.size(); ++j) {
					if (mc.loc.x == levels[current_level].characters[i].force_interactions[j].x + levels[current_level].characters[i].loc.x &&
						mc.loc.y == levels[current_level].characters[i].force_interactions[j].y + levels[current_level].characters[i].loc.y) {
						if (levels[current_level].characters[i].loc.x > mc.loc.x)
							mc.dir = RIGHT;
						else if (levels[current_level].characters[i].loc.x < mc.loc.x)
							mc.dir = LEFT;
						else if (levels[current_level].characters[i].loc.y > mc.loc.y)
							mc.dir = DOWN;
						else if (levels[current_level].characters[i].loc.y < mc.loc.y)
							mc.dir = UP;
						ahead.x = levels[current_level].characters[i].loc.x;
						ahead.y = levels[current_level].characters[i].loc.y;
						interact = true;
					}
				}
			}
			update_level();
			if (interact) {
				vector<int> choices;
				interact = false;
				for (unsigned i = 0; i < levels[current_level].characters.size(); ++i) {
					if (!levels[current_level].characters[i].active)
						continue;
					if (ahead.x == levels[current_level].characters[i].loc.x && ahead.y == levels[current_level].characters[i].loc.y) {
						if (levels[current_level].characters[i].loc.x < mc.loc.x)
							levels[current_level].characters[i].dir = RIGHT;
						else if (levels[current_level].characters[i].loc.x > mc.loc.x)
							levels[current_level].characters[i].dir = LEFT;
						else if (levels[current_level].characters[i].loc.y > mc.loc.y)
							levels[current_level].characters[i].dir = UP;
						else if (levels[current_level].characters[i].loc.y < mc.loc.y)
							levels[current_level].characters[i].dir = DOWN;
						string s = levels[current_level].characters[i].interactions[mc.interaction[levels[current_level].characters[i].name]];
						string s2, s3;
						choices.clear();
						bool advance;
						advance = true;
						while (s != "") {
							if (s.find("MENU") == 0) {
								s.erase(0, s.find(":") + 1);
								s2 = s;
								s3 = s;
								s2.erase(s2.find(":"), s2.length());
								s3.erase(0, s3.find(":") + 1);
								if (s3.find("|") != -1)
									s3.erase(s3.find("|"), s3.length());
								choices = do_menu(s2, s3);
								s.erase(0, s2.length() + s3.length() + 1);
							}
							else if (s.find("NO_ADVANCE") == 0) {
								advance = false;
								s.erase(0, string("NO_ADVANCE").length() + 1);
								break;
							}
							else if (s.find("SET:") == 0) {
								s.erase(0, string("SET:").length());
								s2 = s;
								s.erase(0, s.find(":") + 1);
								s2.erase(s2.find(":"), s2.length());
								s3 = s;
								if (s.find("|") != -1) {
									s.erase(0, s.find("|"));
									s3.erase(s3.find("|"), s3.length());
								}
								else {
									s = "";
								}
								mc.values[s2] = stoi(s3);
							}
							else if (s.find("GET:") == 0) {
								s.erase(0, string("GET:").length());
								s2 = s;
								if (s.find("|") != -1) {
									s.erase(0, s.find("|"));
									s2.erase(s2.find("|"), s2.length());
								}
								else {
									s = "";
								}
								choices.clear();
								choices.push_back(e.mc.values[s2]);
							}
							else if (s.find("ADVANCE") == 0) {
								s.erase(0, string("ADVANCE:").length());
								s2 = s;
								if (s2.find("|") != -1) {
									s2.erase(s2.find("|"));
									mc.interaction[s2]++;
								}
								else {
									mc.interaction[s2]++;
									s = "";
								}
							}
							else if (s.find("GIVE_MON") == 0) {
								s.erase(0, string("GIVE_MON:").length());
								s2 = s;
								s.erase(0, s.find(":") + 1);
								s2.erase(s2.find(":"), s2.length());
								int l = stoi(s2);
								s2 = s;
								if (s2.find("|") != -1) {
									s2.erase(s2.find("|"), s2.length());
								}
								for (unsigned i = 0; i < 6; ++i) {
									if (!mc.team[i].defined) {
										make_mon(s2, l, mc.team[i]);
										mc.team[i].wild = false;
										mc.team[i].enemy = false;
										// TODO: Nickname menu
										do_menu(string("ALERT"), mc.name + string(" got ") + all_mon[mc.team[i].number].name + string("!"));
										break;
									}
								}
							}
							else if (s.find("SET_FACE:") == 0) {
								s.erase(0, s.find(":") + 1);
								s2 = s;
								s.erase(0, s.find(":") + 1);
								s2.erase(s2.find(":"), s2.length());
								direction dir = DOWN;
								if (s.find("LEFT") == 0) {
									dir = LEFT;
								}
								else if (s.find("RIGHT") == 0) {
									dir = RIGHT;
								}
								else if (s.find("UP") == 0) {
									dir = UP;
								}
								else if (s.find("DOWN") == 0) {
									dir = DOWN;
								}
								if (s2 == "PLAYER") {
									mc.dir = dir;
								}
								else {
									map<string, level>::iterator it;
									for (it = levels.begin(); it != levels.end(); it++) {
										for (unsigned k = 0; k < it->second.characters.size(); ++k) {
											if (it->second.characters[k].name == s2) {
												it->second.characters[k].dir = dir;
											}
										}
									}
								}
								update_level();
							}
							else if (s.find("FACE") == 0) {
								s.erase(0, s.find(":") + 1);
								if (s.find("LEFT") == 0) {
									levels[current_level].characters[i].dir = LEFT;
									s.erase(0, string("LEFT").length());
								}
								else if (s.find("RIGHT") == 0) {
									levels[current_level].characters[i].dir = RIGHT;
									s.erase(0, string("RIGHT").length());
								}
								else if (s.find("UP") == 0) {
									levels[current_level].characters[i].dir = UP;
									s.erase(0, string("UP").length());
								}
								else if (s.find("DOWN") == 0) {
									levels[current_level].characters[i].dir = DOWN;
									s.erase(0, string("DOWN").length());
								}
								update_level();
							}
							else if (s.find("GIVE_ITEM:") == 0) {
								s.erase(0, string("GIVE_ITEM:").length());
								s2 = s;
								if (s2.find("|") != -1) {
									s2.erase(s2.find("|"), s2.length());
								}
								gain_item(s2);
								s.erase(0, s2.length());
							}
							else if (s.find("MOVE:") == 0) {
								direction dir;
								s.erase(0, string("MOVE:").length());
								s2 = s;
								s.erase(0, s.find(":") + 1);
								s2.erase(s2.find(":"), s2.length());
								s3 = s;
								s.erase(0, s.find(",") + 1);
								s3.erase(s3.find(","), s3.length());
								int deltax = stoi(s3);
								int deltay = 0;
								s3 = s;
								if (s.find("|") != -1) {
									s.erase(0, s.find("|"));
									s3.erase(s3.find("|"), s3.length());
								}
								else {
									s = "";
								}
								deltay = stoi(s3);
								if (deltax < 0)
									dir = LEFT;
								else if (deltax > 0)
									dir = RIGHT;
								else if (deltay > 0)
									dir = DOWN;
								else
									dir = UP;
								if (s2.find("PLAYER") == 0) {
									mc.loc.x += double(deltax);
									mc.loc.y += double(deltay);
									mc.dir = dir;
								}
								else {
									map<string, level>::iterator it;
									for (it = levels.begin(); it != levels.end(); it++) {
										for (unsigned k = 0; k < it->second.characters.size(); ++k) {
											if (it->second.characters[k].name == s2) {
												it->second.characters[k].loc.x += double(deltax);
												it->second.characters[k].loc.y += double(deltay);
												it->second.characters[k].dir = dir;
											}
										}
									}
								}
								update_level();
							}
							else if (s.find("{") == 0) {
								int counter;
								vector<string> options;
								unsigned x = 0;
								counter = 1;
								s2 = "";
								for (x = 1; x < s.size(); ++x) {
									if (s[x] == '{') {
										counter++;
										if (counter == 1) {
											continue;
										}
									}
									if (s[x] == '}') {
										counter--;
										if (counter == 0) {
											options.push_back(s2);
											s2 = "";
											continue;
										}
									}
									if (counter == 0 && s[x] == '|')
										break;
									s2 += s[x];
								}
								s.erase(0, x);
								s = options[choices[choices.size() - 1]] + s;
								continue;
							}
							else if (s.find("FULL_HEAL") == 0) {
								full_heal();
							}
							else if (s.find("DEACTIVATE:") == 0) {
								s2 = s;
								s2.erase(0, string("DEACTIVATE:").length());
								if (s2.find("|") != -1) {
									s2.erase(s2.find("|"), s2.length());
								}
								else
									s = "";
								map<string, level>::iterator it;
								for (it = levels.begin(); it != levels.end(); it++) {
									for (unsigned k = 0; k < it->second.characters.size(); ++k) {
										if (it->second.characters[k].name == s2) {
											it->second.characters[k].active = false;
										}
									}
								}
								update_level();
							}
							else if (s.find("ACTIVATE:") == 0) {
								s2 = s;
								s2.erase(0, string("ACTIVATE:").length());
								if (s2.find("|") != -1) {
									s2.erase(s2.find("|"), s2.length());
								}
								else
									s = "";
								map<string, level>::iterator it;
								for (it = levels.begin(); it != levels.end(); it++) {
									for (unsigned k = 0; k < it->second.characters.size(); ++k) {
										if (it->second.characters[k].name == s2) {
											it->second.characters[k].active = true;
										}
									}
								}
								update_level();
							}
							else if (s.find("!:") == 0) {
								int clear_point = g.draw_list.size();
								s2 = s;
								s2.erase(0, string("!:").length());
								if (s2.find("|") != -1) {
									s2.erase(s2.find("|"), s2.length());
								}
								else
									s = "";
								if (s2 == "PLAYER") {
									g.push_quad_load(-0.1f, (0.5f / 4.5f) + 0.0416f, 0.2f, 1.0f / 4.5f, string("../resources/images/speechbubble.png"));
								}
								else {
									map<string, level>::iterator it;
									for (it = levels.begin(); it != levels.end(); it++) {
										for (unsigned k = 0; k < it->second.characters.size(); ++k) {
											if (it->second.characters[k].name == s2) {
												g.push_quad_load((it->second.characters[k].loc.x - (mc.loc.x + 0.5)) / 5.0f, (0.5 - it->second.characters[k].loc.y + mc.loc.y) / 4.5f + 0.0416f, 0.2f, 1.0f / 4.5f, string("../resources/images/speechbubble.png"));
											}
										}
									}
								}
								do_menu("BLANK", "BLANK");
								g.draw_list.erase(g.draw_list.begin() + clear_point, g.draw_list.end());
							}
							else if (s.find("SOUND:") == 0) {
								// TODO: Implement sound playing
								if (s.find("|") != -1) {
									s.erase(0, s.find("|") + 1);
								}
								else {
									s = "";
								}
							}
							else if (s.find("MAP") == 0) {
								// TODO: Implement MAP
								if (s.find("|") != -1) {
									s.erase(0, s.find("|") + 1);
								}
								else {
									s = "";
								}
							}
							if (s.find("|") != -1)
								s.erase(0, s.find("|") + 1);
							if (!advance)
								break;
						}
						if (advance) {
							if (mc.interaction[levels[current_level].characters[i].name] < levels[current_level].characters[i].interactions.size() - 1) {
								mc.interaction[levels[current_level].characters[i].name]++;
							}
						}
					}
				}
			}
			transition.unlock();
			if (alert != "") {
				do_menu(string("ALERT"), alert);
				alert = "";
			}
			if (encounter != "") {
				mc.enemy_selected = 0;
				make_mon(encounter, encounter_level, mc.enemy_team[0]);
				battle(mc, mc.enemy_team[0]);
				encounter = "";
			}
			if (menus.size() > 0) {
				menus[0]->main();
				menus.erase(menus.begin());
			}
		}
	}
};

#endif