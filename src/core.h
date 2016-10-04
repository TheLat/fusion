#include <map>
#include <vector>
#include <string>
#include <math.h>
#include <ctime>

using namespace std;

enum STAT{
	HP=0,
	ATTACK,
	DEFENSE,
	SPECIAL,
	SPEED,
	SIZE
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
	vector<pair<int, string>> learned;
	vector<pair<string, string>> evolution;
	std::map<int, bool> TM, HM;
	bool defined;
	mon_template() { defined = false; }
};

class mon : public mon_template {
public:
	int IV[SIZE];
	int EV[SIZE];
	int curr_hp;
	int level;
	int turn_count;
	int pp[4];
	int max_pp[4];
	string moves[4];
	vector<string> status;
	vector<string> queue;
	string nickname;
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
private:
	// format is always types[ATTACKER][DEFENDER]
	std::map<string, std::map<string, float>> types;
	std::map<string, bool> special_case;
	std::map<string, status_effect> status;
	std::map<string, mon_template> all_mon;
	std::map<string, power> moves;
public:
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
	void make_mon(string ID, int level, mon& out) {
		out.level = level;
		for (int x = 0; x < SIZE; x++)
			out.EV[x] = 0;
		for (int x = 0; x < SIZE; x++)
			out.IV[x] = int(random(0.0, 15.999));
		for (int x = 0; x < SIZE; x++)
			out.stats[x] = all_mon[ID].stats[x];
		out.number = ID;
		out.name = all_mon[ID].name;
		out.type1 = all_mon[ID].type1;
		out.type2 = all_mon[ID].type2;
		out.learned = all_mon[ID].learned;
		out.evolution = all_mon[ID].evolution;
		out.TM = all_mon[ID].TM;
		out.curr_hp = get_stat(out, HP);
		out.HM = all_mon[ID].HM;
		out.turn_count = 0;
		out.nickname = all_mon[ID].name;
		for (unsigned x = 0; x < all_mon[ID].learned.size(); ++x) {
			// TODO:  Apply randomness with priority to forget the oldest move, but also be able to forget other
			out.moves[x % 4] = all_mon[ID].learned[x].second;
			out.pp[x % 4] = moves[all_mon[ID].learned[x].second].pp;
			out.max_pp[x % 4] = moves[all_mon[ID].learned[x].second].pp;
		}
	}
	double random(double min, double max) {
		// range is inclusive.
		double delta = max - min;
		double t = double(rand()) / double(RAND_MAX);
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
				if (moves[move].special[i].find(m.type1) != -1)
					return true;
				if (moves[move].special[i].find(m.type2) != -1)
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
					max = int(random(1.0, 7.999));
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
		if (moves[move].acc < int(random(0.0, 100.0))) {
			return false;
		}
		if (pow != 0.0) {
			if (moves[move].pow.find(string("x2-5")) != -1) {
				repeat = weightedrand() + 2;
			}
			else if (moves[move].pow.find(string("x2")) != -1) {
				repeat = 2;
			}
		}
		for (int i = 0; i < repeat; ++i) {
			int dam = damage(attacker, defender, move, crit);
			// TODO: Logic for detecting that a move has an x0 modifier.
			deal_damage(defender, dam);
			if (!status_immunity(defender, move)) {
				for (unsigned j = 0; j < moves[move].target.size(); ++j) {
					success = apply_status(defender, moves[move].target[j]) || success;
				}
			}
			for (unsigned j = 0; j < moves[move].self.size(); ++j) {
				success = apply_status(attacker, moves[move].self[j]) || success;
			}
		}
		for (unsigned i = 0; i < moves[move].additional.size(); ++i) {
			success = use_move(attacker, defender, moves[move].additional[i]) || success;
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
		return success;
	}
	void do_turn_inner(mon& m1, mon& m2) {
		m1.turn_count++;
		m2.turn_count++;
		if (in_status(m1, string("SLEEP"))) {
			remove_status(m1, string("SLEEP"));
			// TODO:  Sleep message
		}
		else if (in_status(m1, string("FREEZE"))) {
			if (0.2 > random(0.0, 1.0)) {
				remove_status(m1, string("FREEZE"));
			}
			// TODO:  Freeze message
		}
		else if (in_status(m1, string("PARALYZE")) && (0.25 > random(0.0, 1.0))) {
			// TODO:  Paralyze message
		}
		else if (m1.queue[0] != "") {
			use_move(m1, m2, m1.queue[0]);
			m1.queue.erase(m1.queue.begin());
			use_status(m1, m2);
			// TODO:  Move announcement and KO-logic.
		}
		if (in_status(m2, string("SLEEP"))) {
			remove_status(m2, string("SLEEP"));
			// TODO:  Sleep message
		}
		else if (in_status(m2, string("FREEZE"))) {
			if (0.2 > random(0.0, 1.0)) {
				remove_status(m2, string("FREEZE"));
			}
			// TODO:  Freeze message
		}
		else if (in_status(m2, string("PARALYZE")) && (0.25 > random(0.0, 1.0))) {
			// TODO:  Paralyze message
		}
		if (m2.queue[0] != "") {
			use_move(m2, m1, m2.queue[0]);
			m2.queue.erase(m2.queue.begin());
			use_status(m2, m1);
			// TODO:  Move announcement and KO-logic.
		}
	}
	void do_turn(mon& m1, mon& m2) {
		// TODO:  Expand this to support items, fleeing, and switching.
		// TODO:  Make handle nothing in queue.
		if (in_special(m1.queue[0], string("FIRST")) && !in_special(m2.queue[0], string("FIRST"))) {
			do_turn_inner(m1, m2);
		}
		else if (!in_special(m1.queue[0], string("FIRST")) && in_special(m2.queue[0], string("FIRST"))) {
			do_turn_inner(m2, m1);
		}
		else if (in_special(m1.queue[0], string("FIRST")) && in_special(m2.queue[0], string("FIRST")) && (get_stat(m1, SPEED) > get_stat(m2, SPEED))) {
			do_turn_inner(m1, m2);
		}
		else if (in_special(m1.queue[0], string("FIRST")) && in_special(m2.queue[0], string("FIRST")) && (get_stat(m1, SPEED) <= get_stat(m2, SPEED))) {
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
		if (pow == 0.0)
			return 0;
		double chance = attacker.stats[SPEED] / 4.0;
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
		if (defender.type1 != "")
			damage *= types[moves[move].type][defender.type1];
		if (defender.type2 != "")
			damage *= types[moves[move].type][defender.type2];
		if ((attacker.type1 == moves[move].type) || (attacker.type2 == moves[move].type))
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
			ret = m.stats[s] + m.IV[s];
			ret = ret * 2;
			ret = ret + (sqrt(min(65535, m.EV[s])) / 4);
			ret = ret * m.level;
			ret = ret / 100;
			ret = ret + m.level + 10;
		}
		else {
			ret = m.stats[s] + m.IV[s];
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
		int i = int(random(0.0, 7.999));
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
};