#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
using namespace std;
const int waiting_to_roll = 0;
const int walking = 1;
const int paused = 0;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Vortex;

class Actor : public GraphObject {
public:
	Actor(StudentWorld* world, int imageID, double startX, double startY, int dir, int depth, double size)
		: GraphObject(imageID, startX* SPRITE_WIDTH, startY* SPRITE_HEIGHT, dir, depth, size) {
		m_world = world;
		alive = true;
	}
	StudentWorld* getWorld() {
		return m_world;
	}
	virtual bool isAlive() {
		return true;
	}
	virtual void doSomething() = 0;
	virtual bool isBaddie() {
		return false;
	}
	virtual bool isVortex() {
		return false;
	}
	virtual void teleport(double&x, double& y);
	virtual void setAlive(bool x) {
		alive = x;
	}

private:
	StudentWorld* m_world;
	bool alive;

};

class PlayerActor : public Actor {
public:
	PlayerActor(StudentWorld* world, const int imageID, double startX, double startY, int dir, int depth, double size, int side)
		: Actor(world, imageID, startX, startY, right, 0, 1.0) {
		p_side = side;
		ticks_to_move = 0;
		walk = right;
		coins = 0;
		vortex = 0;
		stars = 0;
		state = waiting_to_roll;
		just_teleported = false;
	}
	void teleport(double& x, double& y);
	void set_teleport(bool val) {
		just_teleported = val;
	}
	void doSomething();
	int get_side() {
		return p_side;
	}
	int getWalk() {
		return walk;
	}
	void setWalk(int x) {
		walk = x;
	}
	void addCoins(int x) {
		coins += x;
	}
	void subtractCoins(int x) {
		coins -= x;
	}
	int getCoins() {
		return coins;
	}
	void addStar(int x) {
		stars += x;
	}
	int getTicks() {
		return ticks_to_move;
	}
	void setTicks(int x) {
		ticks_to_move = x;
	}
	int getState() {
		return state;
	}
	void setState(int x) {
		state = x;
	}
	int getStars() {
		return stars;
	}
	void setStars(int x) {
		stars = x;
	}
	void setCoins(int x) {
		coins = x;
	}
	bool hasVortex() {
		if (vortex == 0) {
			return false;
		}
		return true;
	}

	void giveVortex() {
		vortex = 1;
	}
	void subtractStar(int x) {
		stars-= x;
	}
	
private:
	int p_side;
	int state;
	int ticks_to_move;
	int walk;
	int coins;
	int stars;
	int vortex;
	bool just_teleported;
};

class Peach : public PlayerActor {
public:
	Peach(StudentWorld* world, double startX, double startY) : PlayerActor(world, IID_PEACH, startX, startY, right, 0, 1.00, 1.0) {}

private:
};

class Yoshi : public PlayerActor {
public:
	Yoshi(StudentWorld* world, double startX, double startY) : PlayerActor(world, IID_YOSHI, startX, startY, right, 0, 1.00, 2.0) {}

private:
};


class CoinSquare : public Actor {
public:
	CoinSquare(StudentWorld* world, double startX, double startY, const int imageID) : Actor(world, imageID, startX, startY, right, 1, 1.0) {
		impactable = false;
	}
private:
	//coin squares not impactable by vortexes
	bool impactable;
};

class BlueCoin : public CoinSquare {
public:
	BlueCoin(StudentWorld* world, double startX, double startY) : CoinSquare(world, startX, startY, IID_BLUE_COIN_SQUARE) {
		activated_peach = false;
		activated_yoshi = false;
		alive = true;
	}
	bool isAlive() {
		return alive;
	}
	void doSomething();
private:
	bool activated_peach;
	bool activated_yoshi;
	bool alive;
};

class RedCoin : public CoinSquare {
public:
	RedCoin(StudentWorld* world, double startX, double startY) : CoinSquare(world, startX, startY, IID_RED_COIN_SQUARE) {
		activated_peach = false;
		activated_yoshi = false;
		alive = true;
	}
	void doSomething();
	bool isAlive() {
		return alive;
	}
private:
	bool activated_peach;
	bool activated_yoshi;
	bool alive;
};

class StarSquare : public Actor {
public:
	StarSquare(StudentWorld* world, double startX, double startY) : Actor(world, IID_STAR_SQUARE, startX, startY, right, 1, 1.0) {
		impactable = false;
		activated_peach = false;
		activated_yoshi = false;
	}
	void doSomething();
private:
	bool impactable;
	bool activated_peach;
	bool activated_yoshi;
};

class DirectionalSquare : public Actor {
public:
	DirectionalSquare(StudentWorld* world, double startX, double startY, int sprite_dir) : Actor(world, IID_DIR_SQUARE, startX, startY, sprite_dir, 1, 1.0) {
		impactable = false;
	}
	void doSomething();
private:
	bool impactable;
};

class BankSquare : public Actor {
public:
	BankSquare(StudentWorld* world, double startX, double startY) : Actor(world, IID_BANK_SQUARE, startX, startY, 0, 1, 1.0) {
		impactable = false;
		activated_peach_overlap = false;
		activated_yoshi_overlap = false;
		activated_peach_intersection = false;
		activated_yoshi_intersection = false;

	}
	void doSomething();
private:
	bool impactable;
	bool activated_peach_overlap;
	bool activated_yoshi_overlap;
	bool activated_peach_intersection;
	bool activated_yoshi_intersection;
};

class EventSquare : public Actor {
public:
	EventSquare(StudentWorld* world, double startX, double startY) : Actor(world, IID_EVENT_SQUARE, startX, startY, 0, 1, 1.0) {
		impactable = false;
		activated_peach = false;
		activated_yoshi = false;
		peach_swap = false;
		yoshi_swap = false;
	}
	void doSomething();
private:
	bool impactable;
	bool activated_peach;
	bool activated_yoshi;
	bool peach_swap;
	bool yoshi_swap;
};

class DroppingSquare : public Actor {
public:
	DroppingSquare(StudentWorld* world, double startX, double startY) : Actor(world, IID_DROPPING_SQUARE, startX, startY, 0, 1, 1.0) {
		impactable = false;
		activated_peach = false;
		activated_yoshi = false;
	}
	void doSomething();
private:
	bool impactable;
	bool activated_peach;
	bool activated_yoshi;
};

class Vortex : public Actor {
public:
	Vortex(StudentWorld* world, double startX, double startY, int fir_dir) : Actor(world, IID_VORTEX, startX, startY, 0, 0, 1.0) {
		alive = true;
		impactable = false;
		firing_direction = fir_dir;
	}
	void doSomething();
	bool isAlive() {
		return alive;
	}
	bool isVortex() {
		return true;
	}
	void setAlive(bool x) {
		alive = x;
	}
private:
	int firing_direction;
	bool alive;
	bool impactable;
};

class Baddie : public Actor {
public:
	Baddie(StudentWorld* world, const int imageID, double startX, double startY, int dir, int depth, double size)
		: Actor(world, imageID, startX, startY, right, 0, 1.0) {
		impactable = true;
	}
	bool isBaddie() {
		return true;
	}
private:
	bool impactable;

};

class Bowser : public Baddie {
public:
	Bowser(StudentWorld* world, double startX, double startY) : Baddie(world, IID_BOWSER, startX, startY, right, 0, 1.0) {
		state = paused;
		pause_counter = 180;
		walk = right;
		ticks_to_move = 0;
		activated_peach = false;
		activated_yoshi = false;
	}
	void doSomething();
	int getWalk() {
		return walk;
	}
private:
	int state;
	int pause_counter;
	int walk;
	int ticks_to_move;
	bool activated_peach;
	bool activated_yoshi;
};

class Boo : public Baddie {
public:
	Boo(StudentWorld* world, double startX, double startY) : Baddie(world, IID_BOO, startX, startY, right, 0, 1.0) {
		state = paused;
		pause_counter = 180;
		walk = right;
		ticks_to_move = 0;
		activated_peach = false;
		activated_yoshi = false;
	}
	int getWalk() {
		return walk;
	}
	void doSomething();
private:
	int state;
	int pause_counter;
	int walk;
	int ticks_to_move;
	bool activated_peach;
	bool activated_yoshi;
};

#endif // ACTOR_H_