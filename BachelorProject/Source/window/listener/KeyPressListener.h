#pragma once

enum Key {
	KEY_W,
	KEY_S,
	KEY_A,
	KEY_D,
	KEY_Q,
	KEY_E,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_9
};

class KeyPressListener {
public:
	void virtual handleKeyPress(Key key) = 0;
};