#ifndef INPUT_HEADER
#define INPUT_HEADER

using namespace std;

class input {
private:
    unsigned char keyMap[16*2];
    bool key_down[16*16*2];
    bool key_press[16*16*2];
	int controller[164];
    unsigned time_index;
	unsigned char i_up, i_down, i_left, i_right, i_confirm, i_cancel, i_start, i_select;
	unsigned char u_up, u_down, u_left, u_right, u_confirm, u_cancel, u_start, u_select;
	int c_up, c_down, c_left, c_right, c_confirm, c_cancel, c_start, c_select;
public:
	unsigned char get_pressed_key(bool& pressed);
	bool set_key(unsigned char key, int mapping);
	bool get_and_set_key(int mapping);
    void keydown(bool &up, bool &down, bool &left, bool &right, bool &confirm, bool &cancel, bool &start, bool &select);
    void keypress(bool &up, bool &down, bool &left, bool &right, bool &confirm, bool &cancel, bool &start, bool &select);
    void tick();
	void save_bindings();
	void init_controller();
	input();
	~input();
};

#endif