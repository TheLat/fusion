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

enum STAT{
	HP=0,
	ATTACK,
	DEFENSE,
	SPECIAL,
	SPEED,
	SIZE
};

class location {
public:
	double x;
	double y;
	string level;
};

class power {
public:
	string name;
	string type;
	string pow;
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
	string number, name, type1, type2;
	int stats[SIZE];
	int exp_yield, catchrate;
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
		return *this;
	}
};

class player {
public:
	string name;
	int wins, losses;
	int money;
	mon team[6];
	mon storage[20][20];
	location loc;
	player() { wins = 0; losses = 0; money = 0; }
	// TODO:  INVENTORY
};

class trainer {
public:
	string name;
	int money_yield;
	mon team[6];
	trainer() { name = "DEFAULT_TRAINER_NAME"; money_yield = 0; }
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

class level {
public:
	string name;
	std::vector<std::vector<int>> data;
	std::vector<std::pair<location, location>> teleport;
	std::vector<int> encounters;
	std::pair<int, int> level_range;
};

typedef std::map<string, std::map<string, float>>::iterator type_iter;
int min(int a, int b) {
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
	std::map<int, bool> blocking;
	std::vector<menu*> menus;
	string alert;
	string encounter;
	int encounter_level;
	int selected;
	string current_level;
	player mc;
public:
	string get_special_string(string in) {
		string parse;
		string out;
		if (in.find(":") != -1) {
			string temp = in;
			for (unsigned i = 0; (i < temp.size()) && (temp[i] != ':'); ++i) {
				parse += temp[i];
			}
			temp.erase(0, temp.find(':') + 1);
			if (parse == "ACTIVE_POKEMON_MOVE") {
				out = mc.team[selected].moves[stoi(temp)];
				if (out == "") {
					out = "-";
				}
				return out;
			}
			else if (parse == "ACTIVE_POKEMON_MOVE_TYPE") {
				return moves[mc.team[selected].moves[stoi(temp)]].type;
			}
			else if (parse == "ACTIVE_POKEMON_MOVE_PP") {
				int index = stoi(temp);
				out = to_string(mc.team[selected].pp[index]) + string("/") + to_string(mc.team[selected].max_pp[index]);
				while (out.length() < 9)
					out = string(" ") + out;
				return out;
			}
		}
		return in;
	}
	bool run_away(mon& escapee, mon& m, int attempts) {
		double chance = (double(get_stat(escapee, SPEED))*32.0 / double(get_stat(m, SPEED))) + (30.0 * double(attempts));
		return chance > random(0.0, 255.0);
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
	void make_mon(string ID, int& e_level, mon& out) {
		out.level = 0;
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
		out.nickname = all_mon[ID].name;
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
		level_up(winner, false); // TODO:  Switch to true when confirmation boxes are in.
	}
	bool level_up(mon& out, bool confirm_learn=false) {
		if (level_to_exp[out.level + 1] > out.exp)
			return false;
		int counter = 0;
		while ((counter < 4) && (out.moves[counter] != ""))
			counter++;
		while (out.level < 100 && level_to_exp[out.level + 1] <= out.exp) {
			out.level++;
			for (unsigned x = 0; x < all_mon[out.number].learned.size(); ++x) {
				if (all_mon[out.number].learned[x].first == out.level) {
					if (!confirm_learn) { // TODO:  OR Surface confirmation window
						out.moves[counter % 4] = all_mon[out.number].learned[x].second;
						out.pp[counter % 4] = moves[out.moves[counter % 4]].pp;
						out.max_pp[counter % 4] = out.pp[counter % 4];
						counter++;
					}
				}
			}
		}
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
	}
	void deal_damage(mon& m, int damage_amount) {
		if (damage_amount < 0) {
			// TODO:  Error logic here
			return;
		}
		m.curr_hp = m.curr_hp - damage_amount;
		if (m.curr_hp < 0)
			m.curr_hp = 0;
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
		bool first = true;
		s2 = "";
		s3 = "";
		if (s.find(':') != -1) {
			for (unsigned i = 0; i < s.size(); ++i) {
				if (s[i] == ':') {
					first = false;
					continue;
				}
				if (first)
					s2 += s[i];
				else
					s3 += s[i];
			}
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
				m.status.push_back(s2);
			}
			if (s2 == "TOXIC") {
				remove_status(m, string("POISON"), true);
			}
		}
		return true;
	}
	void use_status(mon& self, mon& other) {
		for (unsigned i = 0; i < self.status.size(); ++i) {
			if (self.status[i] == "POISON") {
				// TODO: Animation goes here
				deal_damage(self, int(double(get_stat(self, HP)) / 16.0));
			}
			else if (self.status[i] == "BURN") {
				// TODO: Animation goes here
				deal_damage(self, int(double(get_stat(self, HP)) / 16.0));
			}
			else if (self.status[i] == "SEED") {
				// TODO: Animation goes here
				deal_damage(self, int(double(get_stat(self, HP)) / 16.0));
				heal_damage(other, int(double(get_stat(self, HP)) / 16.0));
			}
			else if (self.status[i] == "TOXIC") {
				// TODO: Animation goes here
				deal_damage(self, int(self.turn_count * double(get_stat(self, HP)) / 16.0));
			}
		}
	}
	bool use_move(mon& attacker, mon& defender, string move) {
		double pow = stoi(moves[move].pow);
		bool crit;
		int repeat = 1;
		bool success = false;
		bool miss = false;
		do_alert(attacker.nickname + string(" used ") + move + string("!"));
		if (moves[move].acc < int(random(0.0, 100.0))) {
			miss = true;
			if (pow == 0) {
				do_alert(string("But, it failed!"));
			}
			else {
				do_alert(attacker.nickname + string("'s attack missed!"));
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
				int dam = damage(attacker, defender, move, crit);
				// TODO: Logic for detecting that a move has an x0 modifier.
				deal_damage(defender, dam);
				if (crit)
					do_alert(string("A critical hit!"));
				if (!status_immunity(defender, move)) {
					for (unsigned j = 0; j < moves[move].target.size(); ++j) {
						success = apply_status(defender, moves[move].target[j]) || success;
						// TODO:  Status announcement
					}
				}
				for (unsigned j = 0; j < moves[move].self.size(); ++j) {
					success = apply_status(attacker, moves[move].self[j]) || success;
				}
			}
			for (unsigned i = 0; i < moves[move].additional.size(); ++i) {
				success = use_move(attacker, defender, moves[move].additional[i]) || success;
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
				do_alert(m1.nickname + string(" is fast asleep."));
				// TODO:  Sleep animation.
			}
			else {
				do_alert(m1.nickname + string(" woke up!"));
			}
		}
		else if (in_status(m1, string("FREEZE"))) {
			if (0.2 > random(0.0, 1.0)) {
				remove_status(m1, string("FREEZE"));
				do_alert(m1.nickname + string(" thawed out!"));
			}
			else {
				do_alert(m1.nickname + string(" is frozen solid!"));
				// TODO:  Freeze animation
			}
		}
		else if (in_status(m1, string("PARALYZE")) && (0.25 > random(0.0, 1.0))) {
			do_alert(m1.nickname + string(" is paralyzed! It can't move!"));
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
				do_alert(m2.nickname + string(" is fast asleep."));
				// TODO:  Sleep animation
			}
			else {
				do_alert(m2.nickname + string(" woke up!"));
			}
		}
		else if (in_status(m2, string("FREEZE"))) {
			if (0.2 > random(0.0, 1.0)) {
				remove_status(m2, string("FREEZE"));
				do_alert(m2.nickname + string(" thawed out!"));
			}
			else {
				do_alert(m2.nickname + string(" is frozen solid!"));
				// TODO:  Freeze animation
			}
		}
		else if (in_status(m2, string("PARALYZE")) && (0.25 > random(0.0, 1.0))) {
			do_alert(m2.nickname + string(" is paralyzed! It can't move!"));
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
	void battle(player& p, trainer& t) { // trainer battle
		// TODO:  Implement trainer battle
	}
	void battle(player& p, mon& m) { // wild pokemon
		int i;
		int index;
		int escape_attempts = 0;
		vector<int> choices;
		double count;
		selected = -1;
		// TODO:  Initial menu
		for (i = 0; i < 6; ++i) {
			if (p.team[i].defined) {
				if (!is_KO(p.team[i])) {
					selected = i;
					break;
				}
			}
		}
		p.team[selected].queue.clear();
		m.queue.clear();
		do_alert(string("Wild ") + m.nickname + string(" appeared!"));
		do_alert(string("Go! ") + p.team[selected].nickname + string("!"));
		while (true) {
			// TODO: Implement player battle menu
			if (p.team[selected].queue.size() == 0) {
				choices = do_combat_select();
				while (choices.size() == 0 || choices[0] == -1) {
					choices = do_combat_select();
				}
				if (choices[0] == 0) { // Player has selected FIGHT
					escape_attempts = 0;
					p.team[selected].queue.push_back(p.team[selected].moves[choices[1]]);
					p.team[selected].pp[choices[1]]--;
				}
				else if (choices[0] == 2) { // Player has selected ITEM
					// TODO:  Implement inventory
					int out = 0;
					p.team[selected].queue.insert(p.team[selected].queue.begin(), string(""));
					if (choices[1] == 0) {
						out = attempt_capture(1.0, m); // Pokeball
					}
					else if (choices[1] == 1) {
						out = attempt_capture(1.5, m); // Great Ball
					}
					else if (choices[1] == 2) {
						out = attempt_capture(2.0, m); // Ultra Ball
					}
					else if (choices[1] == 3) {
						out = attempt_capture(255.0, m); // Master Ball
					}
					// TODO:  Capture messages
					if (out == 4) {
						m.wild = false;
						// TODO:  Nickname menu
						gain_exp(p.team[selected], m, 1); // TODO:  Implement exp split
						for (int i = 0; i < 6; ++i) {
							if (!p.team[i].defined) {
								do_alert(string("Wild ") + m.nickname + string(" was captured!")); // TODO:  Fact-check string
								p.team[i] = m;
								return;
							}
						}
						// TODO:  Implement storage
					}
				}
				else if (choices[0] == 2) { // Player has selected Pokemon
					p.team[selected].queue.clear();
					selected = choices[1];
					p.team[selected].queue.clear();
					p.team[selected].queue.push_back(string(""));
				}
				else if (choices[0] == 3) { // Player has selected RUN
					escape_attempts++;
					p.team[selected].queue.clear();
					if (run_away(p.team[selected], m, escape_attempts)) {
						do_alert(string("Got away safely!"));
						break;
					}
					else {
						do_alert(string("Couldn't get away!"));
					}
					p.team[selected].queue.push_back(string(""));
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
			do_turn(p.team[selected], m);
			if (is_KO(m)) {
				do_alert(string("Enemy ") + m.nickname + string(" fainted!"));
				gain_exp(p.team[selected], m, 1);
				break;
			}
			if (is_KO(p.team[selected])) {
				// TODO:  Handle selection on KO
				do_alert(p.team[selected].nickname + string(" fainted!"));
				for (i = 0; i < 6; ++i) {
					if (!is_KO(p.team[i])) {
						selected = i;
						if (i == 5)
							i--;
						break;
					}
				}
				if (i == 6) {
					// TODO:  Handle defeat
					break;
				}
			}
		}
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
	int damage(mon& attacker, mon& defender, string move, bool& crit) {
		double pow = stoi(moves[move].pow);
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
		if (all_mon[defender.number].type1 != "")
			damage *= types[moves[move].type][all_mon[defender.number].type1];
		if (all_mon[defender.number].type2 != "")
			damage *= types[moves[move].type][all_mon[defender.number].type2];
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
			if (s == ATTACK && in_status(m, string("BURN")))
				ret = ret / 2;
			if (s == SPEED && in_status(m, string("PARALYZE")))
				ret = ret / 4;
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
	void init_mon() {
		string line, key;
		ifstream f("../resources/original/pokemon.dat");//f("../resources/data/mon.dat");
		char a = 1;
		int level = 0;
		a = f.get();
		while (f.is_open()) {
			line = "";
			while (a == '\r' || a == '\n')
				a = f.get();
			while (a != ':') {
				line = line + a;
				a = f.get();
				if (a == EOF) {
					f.close();
					return;
				}
			}
			while (a == ' ' || a == ':')
				a = f.get();
			if (line == "NUMBER") {
				key = "";
				while (a != '\r' && a != '\n') {
					key = key + a;
					a = f.get();
				}
				all_mon[key].number = key;
				all_mon[key].defined = true;
			}
			else if (line == "NAME") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				all_mon[key].name = line;
			}
			else if (line == "SURNAME") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				all_mon[key].name = all_mon[key].name + line;
			}
			else if (line == "SWITCH") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
			}
			else if (line == "TYPE1") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				all_mon[key].type1 = line;
			}
			else if (line == "TYPE2") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				all_mon[key].type2 = line;
			}
			else if (line == "EXP_YIELD") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				all_mon[key].exp_yield = stoi(line);
			}
			else if (line == "CATCHRATE") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				all_mon[key].catchrate = stoi(line);
			}
			else if (line == "HP") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				all_mon[key].stats[HP] = stoi(line);
			}
			else if (line == "ATTACK") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				all_mon[key].stats[ATTACK] = stoi(line);
			}
			else if (line == "DEFENSE") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				all_mon[key].stats[DEFENSE] = stoi(line);
			}
			else if (line == "SPECIAL") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				all_mon[key].stats[SPECIAL] = stoi(line);
			}
			else if (line == "SPEED") {
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				all_mon[key].stats[SPEED] = stoi(line);
			}
			else if (line == "MOVES") {
				pair<int, string> tempmove;
				while (true) {
					while (a == '\r' || a == '\n')
						a = f.get();
					if (a == 'E' || a == 'T') {
						line = "";
						while (a != ':') {
							line = line + a;
							a = f.get();
						}
						while (a == ':' || a == ' ')
							a = f.get();
						break;
					}
					line = "";
					while (a != ' ') {
						line = line + a;
						a = f.get();
					}
					level = stoi(line);
					line = "";
					a = f.get();
					while (a != '\r' && a != '\n') {
						line = line + a;
						a = f.get();
					}
					tempmove.first = level;
					tempmove.second = line;
					all_mon[key].learned.push_back(tempmove);
				}
			}
			if (line == "EVOLUTION") {
				while (a == '\r' || a == '\n')
					a = f.get();
				while (true) {
					pair<string, string> tempev;
					line = "";
					while (a != ':') {
						line = line + a;
						a = f.get();
					}
					a = f.get();
					tempev.first = line;
					line = "";
					while (a != '\r' && a != '\n') {
						line = line + a;
						a = f.get();
					}
					tempev.second = line;
					all_mon[key].evolution.push_back(tempev);
					while (a == '\r' || a == '\n') {
						a = f.get();
					}
					if (a == 'T') {
						line = "";
						while (a != ':') {
							line = line + a;
							a = f.get();
						}
						while (a != ' ' && a != '\r' && a != '\n')
							a = f.get();
						break;
					}
				}
			}
			if (line == "TM") {
				while (a != '\r' && a != '\n') {
					line = "";
					while (a == ' ')
						a = f.get();
					while (a != ' ' && a != '\n') {
						line = line + a;
						a = f.get();
					}
					all_mon[key].TM[stoi(line)] = true;
				}
			}
			if (line == "HM") {
				while (a != '\r' && a != '\n' && a != EOF) {
					line = "";
					while (a == ' ')
						a = f.get();
					while (a != ' ' && a != '\n') {
						line = line + a;
						a = f.get();
					}
					all_mon[key].HM[stoi(line)] = true;
					a = f.get();
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
				mc.loc.x = levels[current_level].teleport[i].second.x;
				mc.loc.y = levels[current_level].teleport[i].second.y;
				current_level = levels[current_level].teleport[i].second.level;
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
			if (left) {
				if (mc.loc.x == 0.0f)
					return;
				if (blocking[get_tile(mc.loc.y, mc.loc.x - 1.0)])
					return;
				mc.loc.x -= 1.0f;
			}
			else if (right) {
				if (mc.loc.x + 1 >= levels[current_level].data[int(mc.loc.y)].size())
					return;
				if (blocking[get_tile(mc.loc.y, mc.loc.x + 1.0)])
					return;
				mc.loc.x += 1.0f;
			}
			else if (down) {
				if (mc.loc.y + 1 >= levels[current_level].data.size())
					return;
				if (mc.loc.x >= levels[current_level].data[int(mc.loc.y + 1)].size())
					return;
				if (blocking[get_tile(mc.loc.y + 1.0, mc.loc.x)])
					return;
				mc.loc.y += 1.0f;
			}
			else if (up) {
				if (mc.loc.y == 0)
					return;
				if (mc.loc.x >= levels[current_level].data[int(mc.loc.y - 1)].size())
					return;
				if (blocking[get_tile(mc.loc.y - 1.0, mc.loc.x)])
					return;
				mc.loc.y -= 1.0f;
			}
			update_level();
			if (get_tile(mc.loc.y, mc.loc.x) == 4) {
				if (levels[current_level].encounters.size() > 0) {
					if (random(0.0, 187.5) < 8.5) {
						int choice = int(random(0.0, double(levels[current_level].encounters.size())));
						// TODO:  Switch this over to fusions
						encounter = std::to_string(levels[current_level].encounters[choice]);
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
	void create_alert(string s) {
		menu* m = new menu;
		menus.push_back(m);
		menus[menus.size() - 1]->create_alert(s);
		menus[menus.size() - 1]->push_menu();
	}
	void create_combat_select() {
		menu* m = new menu;
		menus.push_back(m);
		menus[menus.size() - 1]->create_combat_select();
		menus[menus.size() - 1]->push_menu();
	}
	void create_combat_item_select() {
		menu* m = new menu;
		menus.push_back(m);
		menus[menus.size() - 1]->create_combat_item_select();
		menus[menus.size() - 1]->push_menu();
	}
	void create_combat_move_select() {
		menu* m = new menu;
		menus.push_back(m);
		menus[menus.size() - 1]->create_combat_move_select();
		menus[menus.size() - 1]->push_menu();
	}
	void do_alert(string s) {
		create_alert(s);
		menus[menus.size() - 1]->main();
		delete menus[menus.size() - 1];
		menus.erase(menus.end() - 1);
	}
	vector<int> do_combat_select() {
		vector<int> out;
		create_combat_select();
		out = menus[menus.size() - 1]->main();
		delete menus[menus.size() - 1];
		menus.erase(menus.end() - 1);
		return out;
	}
	vector<int> do_combat_item_select() {
		vector<int> out;
		create_combat_item_select();
		out = menus[menus.size() - 1]->main();
		delete menus[menus.size() - 1];
		menus.erase(menus.end() - 1);
		return out;
	}
	vector<int> do_move_select() {
		vector<int> out;
		create_combat_move_select();
		out = menus[menus.size() - 1]->main();
		delete menus[menus.size() - 1];
		menus.erase(menus.end() - 1);
		return out;
	}
	void update_level() {
		m.lock();
		g.draw_list.clear();
		handle_teleport();
		draw_level();
		m.unlock();
	}
	void draw_level() {
		for (unsigned y = 0; y < levels[current_level].data.size(); ++y) {
			for (unsigned x = 0; x < levels[current_level].data[y].size(); ++x) {
				g.push_quad(-1.0f + (float(x) / 5.0f) - ((mc.loc.x - 4.5f) / 5.0f), (-float(y) / 4.5f) - (0.5f / 4.5f) + (mc.loc.y / 4.5f), 1.0f / 5.0f, 1.0f / 4.5f, g.tiles[levels[current_level].data[y][x]]);
			}
		}
	}
	void main() {
		update_level();
		while (true) {
			if (alert != "") {
				create_alert(alert);
				alert = "";
			}
			if (encounter != "") {
				mon m;
				make_mon(encounter, encounter_level, m);
				battle(mc, m);
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