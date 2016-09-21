#include <map>
#include <vector>
#include <string>

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
	power(){ acc = 100; attack = ATTACK; defense = DEFENSE; crit_chance = 1.0f; pow = ""; pp = 0; defined = false; queue_only = false; }
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
	int pp[4];
	int max_pp[4];
	string moves[4];
	vector<string> status;
	string nickname;
};



// format is always types[ATTACKER][DEFENDER]
std::map<string, std::map<string, float>> types;
std::map<string, bool> special_case;
std::map<string, bool> status;
std::map<string, mon_template> all_mon;
std::map<string, power> moves;
typedef std::map<string, std::map<string, float>>::iterator type_iter;

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
	ifstream f("../resources/data/mon.dat");
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
			while (a != '\r' && a != '\n') {
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
	string line;
	ifstream f("../resources/data/status.dat");
	while (f.is_open()) {
		while (std::getline(f, line)) {
			status[line] = true;
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

