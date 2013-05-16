#ifndef KEYSTATE_H
#define KEYSTATE_H

#include <SFML/Network.hpp>

class KeyState
{
	public:
		KeyState();
		KeyState(bool up, bool down, bool left, bool right);
		virtual ~KeyState();

		bool up;
		bool down;
		bool left;
		bool right;

		bool operator == (const KeyState & other){
			return up == other.up && down == other.down && left == other.left && right == other.right;
		}
		bool operator != (const KeyState & other){
			return (up != other.up || down != other.down || left != other.left || right != other.right);
		}

	protected:
	private:
		friend sf::Packet & operator << (sf::Packet & pack, const KeyState & keyState){
			return pack << keyState.up << keyState.down << keyState.left << keyState.right;
		}
		friend sf::Packet & operator >> (sf::Packet & pack, KeyState & keyState){
			return pack >> keyState.up >> keyState.down >> keyState.left >> keyState.right;
		}
		friend std::ostream & operator << (std::ostream & out, const KeyState & keyState){
			return out << "(u:" << keyState.up << ",d:" << keyState.down << ",l:" << keyState.left << ",r:" << keyState.right <<")";
		}
};

#endif // KEYSTATE_H
