#ifndef INPUT_HEADER
#define INPUT_HEADER

using namespace std;

class input {
private:
    unsigned char keyMap[16];
    bool keyMapBool[16*16];
    unsigned time_index;
    unsigned char up, down, left, right, confirm, cancel, start, select;
public:
    //void keydown(bool &up, bool &down, bool &left, bool &right, bool &confirm, bool &cancel, bool &start, bool &select);
    //void keypress(bool &up, bool &down, bool &left, bool &right, bool &confirm, bool &cancel, bool &start, bool &select);
    void tick();
	input();
};

#endif