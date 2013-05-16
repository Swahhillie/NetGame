#include "KeyState.hpp"

KeyState::KeyState()
{
	//ctor
	up = false;
	down = false;
	left = false;
	right = false;
}

KeyState::KeyState(bool aUp, bool aDown, bool aLeft, bool aRight):
	up(aUp),
	down(aDown),
	left(aLeft),
	right(aRight)
{
	//ctor
}

KeyState::~KeyState()
{
	//dtor
}
