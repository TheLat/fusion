#include <map>
#include <vector>
#include <string>

using namespace std;

// format is always types[ATTACKER][DEFENDER]
std::map<string, std::map<string, float>> types;
std::map<string, bool> special_case;
std::map<string, bool> status;
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
};

class pokemon_template {
public:
	string number, name, type1, type2;
        int stats[STAT.SIZE];
        vector<pair<int, string>> learned;
	vector<pair<string, string>> evolution;
};

class pokemon : public pokemon_template {
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

