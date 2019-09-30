#pragma once

enum Key {
	KEY_W,
	KEY_S,
	KEY_A,
	KEY_D,
	KEY_Q,
	KEY_E
};

class KeyPressListener {
public:
	void virtual handleKeyPress(Key key) = 0;
};