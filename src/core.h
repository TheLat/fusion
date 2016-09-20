#include <map>
#include <vector>
#include <string>

using namespace std;

// format is always types[ATTACKER][DEFENDER]
std::map<string, std::map<string, float>> types;
std::map<string, bool> special_case;
std::map<string, bool> status;
std::map<string, mon_template> all_mon;
typedef std::map<string, std::map<string, float>>::iterator type_iter;

enum STAT{
	HP=0,
	ATTACK,
	DEFENSE,
	SPECIAL,
	SPEED,
	SIZE
};

class move {
public:
	string name;
	string type;
	int power;
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
};

class mon_template {
public:
	string number, name, type1, type2;
        int stats[STAT.SIZE];
        vector<pair<int, string>> learned;
	vector<pair<string, string>> evolution;
	std::map<int, bool> TM, HM;
	bool defined;
};

class mon : public mon_template {
public:
	int IV[STAT.SIZE];
	int EV[STAT.SIZE];
	int curr_hp;
	int pp[4];
	int max_pp[4];
	string moves[4];
	vector<string> status;
	string nickname;
};
void init_mon() {
	string line, line2;
	ifstream f("../resources/data/mon.dat");
	char a = 1;
	level = 0;
	string key;
	while (f.is_open()) {
		line = "";
		while (a == '\r' || a == '\n')
			a = f.get();
		while (a != ':') {
			a = f.get();
			line = line + a;
		}
		while (a == ' ' || a == ':')
			a = f.get();
		if (line == "NUMBER") {
			key = "";
			while (a != '\r' && a != '\n') {
				key = key + a;
				a = f.get();
			}
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
			all_mon[key].stats[STAT.HP] = stoi(line);
		}
		else if (line == "ATTACK") {
			line = "";
			while (a != '\r' && a != '\n') {
				line = line + a;
				a = f.get();
			}
			all_mon[key].stats[STAT.ATTACK] = stoi(line);
		}
		else if (line == "DEFENSE") {
			line = "";
			while (a != '\r' && a != '\n') {
				line = line + a;
				a = f.get();
			}
			all_mon[key].stats[STAT.DEFENSE] = stoi(line);
		}
		else if (line == "SPECIAL") {
			line = "";
			while (a != '\r' && a != '\n') {
				line = line + a;
				a = f.get();
			}
			all_mon[key].stats[STAT.SPECIAL] = stoi(line);
		}
		else if (line == "SPEED") {
			line = "";
			while (a != '\r' && a != '\n') {
				line = line + a;
				a = f.get();
			}
			all_mon[key].stats[STAT.SPEED] = stoi(line);
		}
		else if (line == "MOVES") {
			while (true) {
				while (a == '\r' || a == '\n')
					a = f.get();
				if (a == 'E' || a == 'T') {
					while (a != ':") {
						line = line + a;
						a = f.get();
					}
					while (a == ':' || a == ' ')
						a = f.get();
					break;
				}
				while (a != ' ') {
					line = line + a;
					a = f.get();
				}
				level = stoi(line);
				line = "";
				while (a != '\r' && a != '\n') {
					a = f.get();
					line = line + 1;
				}
				all_mon[key].learned.first = level;
				all_mon[key].learned.second = line;
			}
		if (line == "EVOLUTION") {
			while (a == '\r' || a == '\n') {
				a = f.get();
			while (true) {
				line = "";
				while (a != ':') {
					line = line + a;
					a = f.get();
				}
				a = f.get();
				line2 = line;
				line = "";
				while (a != '\r' && a != '\n') {
					line = line + a;
					a = f.get();
				}
				//TODO:  Make pair of line and line2 and push to evolution
				while (a == '\r' || a == '\n') {
					a = f.get();
				}
				if (a == 'T') {
					line = "";
					while (a != ' ') {
						line = line + 1;
						a = f.get();
					}
					break;
				}
			}
		}
		if (line == "TM") {
			while (a == '\r' || a == '\n') {
				line = "";
				while (a != ' ') {
					line = line + a;
					a = f.get();
				}
				all_mon[key].TM[stoi(line)] = true;
			}
		}
		if (line == "HM") {
			while (a == '\r' || a == '\n') {
				line = "";
				while (a != ' ') {
					line = line + a;
					a = f.get();
				}
				all_mon[key].HM[stoi(line)] = true;
			}
		}
	}
}
void init_status() {
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

