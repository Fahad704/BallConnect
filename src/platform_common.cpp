enum {
	BUTTON_A,
	BUTTON_B,
	LEFT_CLICK,
	BUTTON_COUNT
};
struct Button_state {
	bool is_down;
	bool changed;
};
struct Input {
	Button_state buttons[BUTTON_COUNT];
};