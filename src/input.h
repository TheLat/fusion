#ifndef INPUT_HEADER
#define INPUT_HEADER

using namespace std;

class input {
private:
    unsigned char keyMap[16];
    bool key_down[16*16];
    bool key_press[16*16];
    unsigned time_index;
    unsigned char i_up, i_down, i_left, i_right, i_confirm, i_cancel, i_start, i_select;
public:
    void keydown(bool &up, bool &down, bool &left, bool &right, bool &confirm, bool &cancel, bool &start, bool &select);
    void keypress(bool &up, bool &down, bool &left, bool &right, bool &confirm, bool &cancel, bool &start, bool &select);
    void tick();
	input();
};

#endif