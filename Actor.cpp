#include "Actor.h"
#include "StudentWorld.h"
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


void PlayerActor::doSomething() {
	if (state == waiting_to_roll) {
		int action = getWorld()->getAction(get_side());
		if (action == ACTION_ROLL) {
			int die_roll = randInt(1, 10);
			ticks_to_move = die_roll * 8;
			state = walking;
		}
		else if (action == ACTION_FIRE) {
			if (hasVortex()) {
				int a1, b1;
				getPositionInThisDirection(getWalk(), 16, a1, b1);
				getWorld()->addVortex(a1 / 16, b1 / 16, getWalk());
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				vortex = 0;
			}
			else {
				return;
			}
		}
		else {
			return;
		}
	}
	else {
		int a, b;
		//let player move by themselves
		getPositionInThisDirection(getWalk(), 16, a, b);
		//making sure it's not a fork
		int temp_count = 0;
		for (int i = 0; i <= 270; i += 90) {
			int temp_x1, temp_y1;
			getPositionInThisDirection(i, 16, temp_x1, temp_y1);
			if (getWorld()->isValidPos(temp_x1, temp_y1)) {
				temp_count++;
			}
		}
		if ((a % 16 == 0 && b % 16 == 0) && !(getWorld()->isValidPos(a, b)) && temp_count == 2) {
			if (walk == right || walk == left) {
				int x, y;
				getPositionInThisDirection(up, 16, x, y);
				if (getWorld()->isValidPos(x, y)) {
					setDirection(right);
					walk = up;
				}
				else {
					int c, d;
					getPositionInThisDirection(down, 16, c, d);
					if (getWorld()->isValidPos(c, d)) {
						setDirection(right);
						walk = down;
					}
					else {
						if (walk == right) {
							setDirection(left);
							walk = left;
						}
						else {
							setDirection(right);
							walk = right;
						}
					}
				}
			}
			else {
				int h1, h2;
				getPositionInThisDirection(right, 16, h1, h2);
				if (getWorld()->isValidPos(h1, h2)) {
					setDirection(right);
					walk = right;
				}
				else {
					int g1, g2;
					getPositionInThisDirection(left, 16, g1, g2);
					if (getWorld()->isValidPos(g1, g2)) {
						setDirection(left);
						walk = left;
					}
					else {
						if (walk == up) {
							setDirection(right);
							walk = down;
						}
						else {
							setDirection(right);
							walk = up;

						}
					}
				}
			}

		}

		//check if directional square
		if ((getX() % 16 == 0) && (getY() % 16 == 0)) {
			int temp;
			if (getWorld()->isDirectional(getX(), getY(), temp)) {
				walk = temp;
				if (walk == left) {
					setDirection(left);
				}
				else {
					setDirection(right);
				}
			}
			//check if avatar is on fork and make sure IT'S NOT ON EVENT SQUARE uNLESS JUST TELEPORTED
			else {
				//loop to check direction
				if ((!(getWorld()->isEventSquare(getX(), getY()))) || just_teleported == true || ticks_to_move >= 2) {
					int curr_dir = getWalk();
					int count = 0;
					for (int i = 0; i <= 270; i += 90) {
						int temp_x, temp_y;
						getPositionInThisDirection(i, 16, temp_x, temp_y);
						if (getWorld()->isValidPos(temp_x, temp_y)) {
							count++;
						}
					}
					if (count >= 3) {
						int user_choice = getWorld()->getAction(get_side());
						switch (user_choice) {
						case ACTION_LEFT:
							if (curr_dir == right) {
								return;
							}
							int x1, y1;
							getPositionInThisDirection(left, 16, x1, y1);
							if (getWorld()->isValidPos(x1, y1)) {
								setDirection(left);
								walk = left;
							}
							else {
								return;
							}
							break;
						case ACTION_RIGHT:
							if (curr_dir == left) {
								return;
							}
							int x2, y2;
							getPositionInThisDirection(right, 16, x2, y2);
							if (getWorld()->isValidPos(x2, y2)) {
								walk = right;
								setDirection(right);
							}
							else {
								return;
							}
							break;
						case ACTION_UP:
							if (curr_dir == down) {
								return;
							}
							int x3, y3;
							getPositionInThisDirection(up, 16, x3, y3);
							if (getWorld()->isValidPos(x3, y3)) {
								walk = up;
								setDirection(right);
							}
							else {
								return;
							}
							break;
						case ACTION_DOWN:
							if (curr_dir == up) {
								return;
							}
							int x4, y4;
							getPositionInThisDirection(down, 16, x4, y4);
							if (getWorld()->isValidPos(x4, y4)) {
								walk = down;
								setDirection(right);
							}
							else {
								return;
							}
							break;
						default:
							return;
						}
					}
				}

			}
		}
		moveAtAngle(getWalk(), 2);
		ticks_to_move -= 1;
		if (ticks_to_move == 0) {
			state = waiting_to_roll;
		}
		return;
	}
}

void BlueCoin::doSomething() {
	//if coin is dead
	if (!alive) {
		return;
	}
	//if peach is not on square
	if (!(getWorld()->intersection(this, getWorld()->getPeach())) || getWorld()->getPeach()->getState() == walking) {
		activated_peach = false;
	}
	else {
		//do nothing if already activated
		if (activated_peach) {
			return;
		}
		//otherwise
		getWorld()->getPeach()->addCoins(3);
		getWorld()->playSound(SOUND_GIVE_COIN);
		activated_peach = true;
	}
	if (!(getWorld()->intersection(this, getWorld()->getYoshi())) || getWorld()->getYoshi()->getState() == walking) {
		activated_yoshi = false;
	}
	else {
		//do nothing if already activated
		if (activated_yoshi) {
			return;
		}
		//otherwise
		getWorld()->getYoshi()->addCoins(3);
		getWorld()->playSound(SOUND_GIVE_COIN);
		double x, y;
		activated_yoshi = true;
	}
}

void RedCoin::doSomething() {
	//if coin is dead
	if (!alive) {
		return;
	}
	//if peach is not on square
	if (!(getWorld()->intersection(this, getWorld()->getPeach())) || getWorld()->getPeach()->getState() == walking) {
		activated_peach = false;
	}
	else {
		//do nothing if already activated
		if (activated_peach) {
			return;
		}
		//otherwise
		if (getWorld()->getPeach()->getCoins() < 3) {
			getWorld()->getPeach()->subtractCoins(getWorld()->getPeach()->getCoins());
			getWorld()->playSound(SOUND_TAKE_COIN);
		}
		else {
			getWorld()->getPeach()->subtractCoins(3);
			getWorld()->playSound(SOUND_TAKE_COIN);
		}
		activated_peach = true;
	}
	if (!(getWorld()->intersection(this, getWorld()->getYoshi()))) {
		activated_yoshi = false;
	}
	else {
		//do nothing if already activated
		if (activated_yoshi) {
			return;
		}
		//otherwise
		if (getWorld()->getYoshi()->getCoins() < 3) {
			getWorld()->getYoshi()->subtractCoins(getWorld()->getYoshi()->getCoins());
			getWorld()->playSound(SOUND_TAKE_COIN);
		}
		else {
			getWorld()->getYoshi()->subtractCoins(3);
			getWorld()->playSound(SOUND_TAKE_COIN);
		}
		activated_yoshi = true;
	}
}

void StarSquare::doSomething() {
	if (!(getWorld()->overlap(this, getWorld()->getPeach()))) {
		activated_peach = false;
	}
	else {
		//do nothing if already activated
		if (activated_peach) {
			return;
		}
		//otherwise
		if (getWorld()->getPeach()->getCoins() < 20) {
			return;
		}
		else {
			getWorld()->getPeach()->subtractCoins(20);
			getWorld()->getPeach()->addStar(1);
			getWorld()->playSound(SOUND_GIVE_STAR);
			activated_peach = true;
		}
	}
	if (!(getWorld()->overlap(this, getWorld()->getYoshi()))) {
		activated_yoshi = false;
	}
	else {
		//do nothing if already activated
		if (activated_yoshi) {
			return;
		}
		//otherwise
		if (getWorld()->getYoshi()->getCoins() < 20) {
			return;
		}
		else {
			getWorld()->getYoshi()->subtractCoins(20);
			getWorld()->getYoshi()->addStar(1);
			getWorld()->playSound(SOUND_GIVE_STAR);
			activated_yoshi = true;
		}
	}
}

void DirectionalSquare::doSomething() {
	return;
}

void BankSquare::doSomething() {
	if (!(getWorld()->intersection(this, getWorld()->getPeach())) || getWorld()->getPeach()->getState() == walking) {
		activated_peach_intersection = false;
		if (!(getWorld()->overlap(this, getWorld()->getPeach()))) {
			activated_peach_overlap = false;
		}
		else {
			if (activated_peach_overlap) {
				return;
			}
			int to_add = getWorld()->getPeach()->getCoins();
			if (to_add < 5) {
				getWorld()->getPeach()->subtractCoins(to_add);
				getWorld()->addtoBank(to_add);
				getWorld()->playSound(SOUND_DEPOSIT_BANK);
				activated_peach_overlap = true;
			}
			else {
				getWorld()->getPeach()->subtractCoins(5);
				getWorld()->addtoBank(5);
				getWorld()->playSound(SOUND_DEPOSIT_BANK);
				activated_peach_overlap = true;
			}
		}
	}
	else {
		if (activated_peach_intersection) {
			return;
		}
		//otherwise
		getWorld()->getPeach()->addCoins(getWorld()->getBank());
		getWorld()->setBanktoZero();
		getWorld()->playSound(SOUND_WITHDRAW_BANK);
		activated_peach_intersection = true;
	}
	if (!(getWorld()->intersection(this, getWorld()->getYoshi())) || getWorld()->getYoshi()->getState() == walking) {
		if (!(getWorld()->overlap(this, getWorld()->getYoshi()))) {
			activated_yoshi_overlap = false;
		}
		else {
			if (activated_yoshi_overlap) {
				return;
			}
			int to_add = getWorld()->getYoshi()->getCoins();
			if (to_add < 5) {
				getWorld()->getYoshi()->subtractCoins(to_add);
				getWorld()->addtoBank(to_add);
				getWorld()->playSound(SOUND_DEPOSIT_BANK);
				activated_yoshi_overlap = true;
			}
			else {
				getWorld()->getYoshi()->subtractCoins(5);
				getWorld()->addtoBank(5);
				getWorld()->playSound(SOUND_DEPOSIT_BANK);
				activated_yoshi_overlap = true;
			}
		}
	}
	else {
		if (activated_yoshi_intersection) {
			return;
		}
		//otherwise
		getWorld()->getYoshi()->addCoins(getWorld()->getBank());
		getWorld()->setBanktoZero();
		getWorld()->playSound(SOUND_WITHDRAW_BANK);
		activated_yoshi_intersection = true;
	}
}

void PlayerActor::teleport(double& x, double& y) {
	bool check = true;
	while (check) {
		double a = randInt(0, 15);
		double b = randInt(0, 15);
		x = a * 16;
		y = b * 16;
		if (getWorld()->isValidPos(x, y)) {
			moveTo(x, y);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			check = false;
		}
	}
	just_teleported = true;
}

void Actor::teleport(double& x, double& y) {
	bool check = true;
	while (check) {
		double a = randInt(0, 15);
		double b = randInt(0, 15);
		x = a * 16;
		y = b * 16;
		if (getWorld()->isValidPos(x, y)) {
			moveTo(x, y);
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			check = false;
		}
	}
}

void EventSquare::doSomething() {
	if (!(getWorld()->intersection(this, getWorld()->getPeach())) || getWorld()->getPeach()->getState() == walking) {
		activated_peach = false;
		peach_swap = false;
		getWorld()->getPeach()->set_teleport(false);
	}
	else {
		//do nothing if already activated
		if (activated_peach || peach_swap) {
			return;
		}
		//otherwise
		int random = randInt(1, 3);  //REMEMBER TO ADD 3 FOR VORTEX LATER
		switch (random) {
			case 1:
			{
				double x, y;
				getWorld()->getPeach()->teleport(x, y);
				getWorld()->playSound(SOUND_PLAYER_TELEPORT);
				activated_peach = true;
			}
				break;
			case 2:
			{
				//swap positions
				double temp_x = getWorld()->getPeach()->getX();
				double temp_y = getWorld()->getPeach()->getY();
				getWorld()->getPeach()->moveTo(getWorld()->getYoshi()->getX(), getWorld()->getYoshi()->getY());
				getWorld()->getYoshi()->moveTo(temp_x, temp_y);
				//swap ticks
				int temp_ticks = getWorld()->getPeach()->getTicks();
				getWorld()->getPeach()->setTicks(getWorld()->getYoshi()->getTicks());
				getWorld()->getYoshi()->setTicks(temp_ticks);
				//swap sprite
				int temp_sprite = getWorld()->getPeach()->getDirection();
				getWorld()->getPeach()->setDirection(getWorld()->getYoshi()->getDirection());
				getWorld()->getYoshi()->setDirection(temp_sprite);
				//swap walk direction
				int temp_walk = getWorld()->getPeach()->getWalk();
				getWorld()->getPeach()->setWalk(getWorld()->getYoshi()->getWalk());
				getWorld()->getYoshi()->setWalk(temp_walk);
				//swap states
				int temp_state = getWorld()->getPeach()->getState();
				getWorld()->getPeach()->setState(getWorld()->getYoshi()->getState());
				getWorld()->getYoshi()->setState(temp_state);
				getWorld()->playSound(SOUND_PLAYER_TELEPORT);
				activated_peach = true;
				activated_yoshi = true;
				peach_swap = true;
				yoshi_swap = true;
			}
				break;
		case 3:
		{
			if (getWorld()->getPeach()->hasVortex()) {
				return;
			}
			else {
				getWorld()->getPeach()->giveVortex();
				getWorld()->playSound(SOUND_GIVE_VORTEX);
			}
			activated_peach = true;
		}
		}

	}

	if (!(getWorld()->intersection(this, getWorld()->getYoshi())) || getWorld()->getYoshi()->getState() == walking) {
		activated_yoshi = false;
		yoshi_swap = false;
		getWorld()->getPeach()->set_teleport(false);
	}
	else {
		//do nothing if already activated
		if (activated_yoshi || yoshi_swap) {
			return;
		}
		//otherwise
		int random = randInt(1, 3);  //REMEMBER TO ADD 3 FOR VORTEX LATER
		switch (random) {
			case 1:
			{
				double x, y;
				getWorld()->getYoshi()->teleport(x, y);
				getWorld()->playSound(SOUND_PLAYER_TELEPORT);
				activated_yoshi = true;
			}
				break;
			case 2:
			{
				//swap positions
				double temp_x = getWorld()->getYoshi()->getX();
				double temp_y = getWorld()->getYoshi()->getY();
				getWorld()->getYoshi()->moveTo(getWorld()->getPeach()->getX(), getWorld()->getPeach()->getY());
				getWorld()->getPeach()->moveTo(temp_x, temp_y);
				//swap ticks
				int temp_ticks = getWorld()->getYoshi()->getTicks();
				getWorld()->getYoshi()->setTicks(getWorld()->getPeach()->getTicks());
				getWorld()->getPeach()->setTicks(temp_ticks);
				//swap sprite
				int temp_sprite = getWorld()->getYoshi()->getDirection();
				getWorld()->getYoshi()->setDirection(getWorld()->getPeach()->getDirection());
				getWorld()->getPeach()->setDirection(temp_sprite);
				//swap walk direction
				int temp_walk = getWorld()->getYoshi()->getWalk();
				getWorld()->getYoshi()->setWalk(getWorld()->getPeach()->getWalk());
				getWorld()->getPeach()->setWalk(temp_walk);
				//swap states 
				int temp_state = getWorld()->getYoshi()->getState();
				getWorld()->getYoshi()->setState(getWorld()->getPeach()->getState());
				getWorld()->getPeach()->setState(temp_state);
				getWorld()->playSound(SOUND_PLAYER_TELEPORT);
				peach_swap = true;
				yoshi_swap = true;
				activated_yoshi = true;
				activated_peach = true;
			}
			break;
			case 3:
			{
				if (getWorld()->getYoshi()->hasVortex()) {
					return;
				}
				else {
					getWorld()->getYoshi()->giveVortex();
					getWorld()->playSound(SOUND_GIVE_VORTEX);
				}
				activated_yoshi = true;
			}
		}
	}
}

void DroppingSquare::doSomething() {
	if (!(getWorld()->intersection(this, getWorld()->getPeach())) || getWorld()->getPeach()->getState() == walking) {
		activated_peach = false;
	}
	else {
		//do nothing if already activated
		if (activated_peach) {
			return;
		}
		//otherwise
		int random = randInt(1, 2);
		switch (random) {
		case 1:
		{
			if (getWorld()->getPeach()->getCoins() < 10) {
				getWorld()->getPeach()->subtractCoins(getWorld()->getPeach()->getCoins());
				getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			}
			else {
				getWorld()->getPeach()->subtractCoins(10);
				getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			}
			activated_peach = true;
		}
		break;
		case 2:
		{
			if (getWorld()->getPeach()->getStars() < 1) {
				getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			}
			else {
				getWorld()->getPeach()->subtractStar(1);
				getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			}
			activated_peach = true;
		}
		break;
		}

	}
	if (!(getWorld()->intersection(this, getWorld()->getYoshi())) || getWorld()->getYoshi()->getState() == walking) {
		activated_yoshi = false;
	}
	else {
		//do nothing if already activated
		if (activated_yoshi) {
			return;
		}
		//otherwise
		int random = randInt(1, 2);
		switch (random) {
		case 1:
			if (getWorld()->getYoshi()->getCoins() < 10) {
				getWorld()->getYoshi()->subtractCoins(getWorld()->getYoshi()->getCoins());
				getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			}
			else {
				getWorld()->getYoshi()->subtractCoins(10);
				getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			}
			activated_yoshi = true;
			break;
		case 2:
			if (getWorld()->getYoshi()->getStars() < 1) {
				getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			}
			else {
				getWorld()->getYoshi()->subtractStar(1);
				getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			}
			activated_yoshi = true;
			break;
		}

	}
}

void Bowser::doSomething() {
	if (state == paused) {
		if (getWorld()->intersection(this, getWorld()->getPeach()) && getWorld()->getPeach()->getState() == waiting_to_roll) {
			int random_bowser = randInt(1, 2);
			if (random_bowser == 1 && activated_peach == false) {
				int coins_to_subtract = getWorld()->getPeach()->getCoins();
				int stars_to_subtract = getWorld()->getPeach()->getStars();
				getWorld()->getPeach()->subtractCoins(coins_to_subtract);
				getWorld()->getPeach()->subtractStar(stars_to_subtract);
				getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
				activated_peach = true;
			}
		}
		if (getWorld()->intersection(this, getWorld()->getYoshi()) && getWorld()->getYoshi()->getState() == waiting_to_roll) {
			int random_bowser = randInt(1, 2);
			if (random_bowser == 1 && activated_yoshi == false) {
				int coins_to_subtract = getWorld()->getYoshi()->getCoins();
				int stars_to_subtract = getWorld()->getYoshi()->getStars();
				getWorld()->getYoshi()->subtractCoins(coins_to_subtract);
				getWorld()->getYoshi()->subtractStar(stars_to_subtract);
				getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
				activated_yoshi = true;
			}
		}
		pause_counter -= 1;
		if (pause_counter == 0) {
			int curr_dir1 = getWalk();
			vector<int>randoms1;
			int squares_to_move = randInt(1, 10);
			ticks_to_move = squares_to_move * 8;
			for (int i = 0; i <= 270; i += 90) {
				int xx, yy;
				if (i != curr_dir1) {
					getPositionInThisDirection(i, 16, xx, yy);
					if (getWorld()->isValidPos(xx, yy)) {
						randoms1.push_back(i);
					}
				}
			}
			int bowser_choice = randInt(0, randoms1.size() - 1);
			int choice = randoms1[bowser_choice];
			switch (choice) {
			case left:
				walk = left;
				setDirection(left);
				break;
			case right:
				walk = right;
				setDirection(right);
				break;
			case up:
				walk = up;
				setDirection(right);
				break;
			case down:
				walk = down;
				setDirection(right);
			}
			state = walking;
		}
	}
	if(state==walking){
		int curr_dir = getWalk();
		int a, b;
		getPositionInThisDirection(getWalk(), 16, a, b);
		int temp_count = 0;
		for (int i = 0; i <= 270; i += 90) {
			int temp_x1, temp_y1;
			getPositionInThisDirection(i, 16, temp_x1, temp_y1);
			if (getWorld()->isValidPos(temp_x1, temp_y1)) {
				temp_count++;
			}
		}
		if ((a % 16 == 0 && b % 16 == 0) && !(getWorld()->isValidPos(a, b)) && temp_count == 2) {
			if (walk == right || walk == left) {
				int x, y;
				getPositionInThisDirection(up, 16, x, y);
				if (getWorld()->isValidPos(x, y)) {
					setDirection(right);
					walk = up;
				}
				else {
					int c, d;
					getPositionInThisDirection(down, 16, c, d);
					if (getWorld()->isValidPos(c, d)) {
						setDirection(right);
						walk = down;
					}
					else {
						if (walk == right) {
							setDirection(left);
							walk = left;
						}
						else {
							setDirection(right);
							walk = right;
						}
					}
				}
			}
			else {
				int h1, h2;
				getPositionInThisDirection(right, 16, h1, h2);
				if (getWorld()->isValidPos(h1, h2)) {
					setDirection(right);
					walk = right;
				}
				else {
					int g1, g2;
					getPositionInThisDirection(left, 16, g1, g2);
					if (getWorld()->isValidPos(g1, g2)) {
						setDirection(left);
						walk = left;
					}
					else {
						if (walk == up) {
							setDirection(right);
							walk = down;
						}
						else {
							setDirection(right);
							walk = up;

						}
					}
				}
			}

		}
		else if (a % 16 == 0 && b % 16 == 0 && temp_count >= 3) {
			vector<int>randoms;
			for (int i = 0; i <= 270; i += 90) {
				int aa, bb;
				if (abs(curr_dir - i) != 180) {
					getPositionInThisDirection(i, 16, aa, bb);
					if (getWorld()->isValidPos(aa, bb)) {
						randoms.push_back(i);
					}
				}
			}
			int bowser_choice = randInt(0, randoms.size() - 1);
			int choice = randoms[bowser_choice];
			switch (choice) {
			case left:
				walk = left;
				setDirection(left);
				break;
			case right:
				walk = right;
				setDirection(right);
				break;
			case up:
				walk = up;
				setDirection(right);
				break;
			case down:
				walk = down;
				setDirection(right);
			}
		}

		moveAtAngle(getWalk(), 2);
		activated_peach = false;
		activated_yoshi = false;
		ticks_to_move -= 1;
		if (ticks_to_move == 0) {
			state = paused;
			pause_counter = 180;
			int dropping_square = randInt(1, 4);
			if (dropping_square == 1 && ((getWorld()->isCoinSquare(getX(), getY()) || getWorld()->isBankSquare(getX(), getY())
				|| getWorld()->isEventSquare(getX(), getY()) || getWorld()->isStarSquare(getX(), getY()) || getWorld()->isDirectionalSquare(getX(), getY())))) {
					getWorld()->addDroppingSquare(getX() / 16, getY() / 16);
					getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
			}
		}


	}
}


void Boo::doSomething() {
	if (state == paused) {
		if (getWorld()->intersection(this, getWorld()->getPeach()) && getWorld()->getPeach()->getState() == waiting_to_roll && activated_peach==false) {
			int random_boo = randInt(1, 2);
			if (random_boo == 1) {
				int temps = getWorld()->getYoshi()->getStars();
				getWorld()->getYoshi()->setStars(getWorld()->getPeach()->getStars());
				getWorld()->getPeach()->setStars(temps);
				getWorld()->playSound(SOUND_BOO_ACTIVATE);
			}
			if (random_boo == 2) {
				int tempc = getWorld()->getYoshi()->getCoins();
				getWorld()->getYoshi()->setCoins(getWorld()->getPeach()->getCoins());
				getWorld()->getPeach()->setCoins(tempc);
				getWorld()->playSound(SOUND_BOO_ACTIVATE);
			}
			activated_peach = true;
		}
		if (getWorld()->intersection(this, getWorld()->getYoshi()) && getWorld()->getYoshi()->getState() == waiting_to_roll && activated_yoshi==false) {
			int random_boo = randInt(1, 2);
			if (random_boo == 1) {
				int temps = getWorld()->getPeach()->getStars();
				getWorld()->getPeach()->setStars(getWorld()->getYoshi()->getStars());
				getWorld()->getYoshi()->setStars(temps);
				getWorld()->playSound(SOUND_BOO_ACTIVATE);
			}
			if (random_boo == 2) {
				int tempc = getWorld()->getPeach()->getCoins();
				getWorld()->getPeach()->setCoins(getWorld()->getYoshi()->getCoins());
				getWorld()->getYoshi()->setCoins(tempc);
				getWorld()->playSound(SOUND_BOO_ACTIVATE);
			}
			activated_yoshi = true;
		}
		pause_counter -= 1;
		if (pause_counter == 0) {
			int curr_dir1 = getWalk();
			vector<int>randoms1;
			int squares_to_move = randInt(1, 3);
			ticks_to_move = squares_to_move * 8;
			for (int i = 0; i <= 270; i += 90) {
				int xx, yy;
				if (i != curr_dir1) {
					getPositionInThisDirection(i, 16, xx, yy);
					if (getWorld()->isValidPos(xx, yy)) {
						randoms1.push_back(i);
					}
				}
			}
			int boo_choice = randInt(0, randoms1.size() - 1);
			int choice = randoms1[boo_choice];
			switch (choice) {
			case left:
				walk = left;
				setDirection(left);
				break;
			case right:
				walk = right;
				setDirection(right);
				break;
			case up:
				walk = up;
				setDirection(right);
				break;
			case down:
				walk = down;
				setDirection(right);
			}
			state = walking;
		}
	}
	if(state==walking) {
		int curr_dir = getWalk();
		int a, b;
		getPositionInThisDirection(getWalk(), 16, a, b);
		int temp_count = 0;
		for (int i = 0; i <= 270; i += 90) {
			int temp_x1, temp_y1;
			getPositionInThisDirection(i, 16, temp_x1, temp_y1);
			if (getWorld()->isValidPos(temp_x1, temp_y1)) {
				temp_count++;
			}
		}
		if ((a % 16 == 0 && b % 16 == 0) && !(getWorld()->isValidPos(a, b)) && temp_count == 2) {
			if (walk == right || walk == left) {
				int x, y;
				getPositionInThisDirection(up, 16, x, y);
				if (getWorld()->isValidPos(x, y)) {
					setDirection(right);
					walk = up;
				}
				else {
					int c, d;
					getPositionInThisDirection(down, 16, c, d);
					if (getWorld()->isValidPos(c, d)) {
						setDirection(right);
						walk = down;
					}
					else {
						if (walk == right) {
							setDirection(left);
							walk = left;
						}
						else {
							setDirection(right);
							walk = right;
						}
					}
				}
			}
			else {
				int h1, h2;
				getPositionInThisDirection(right, 16, h1, h2);
				if (getWorld()->isValidPos(h1, h2)) {
					setDirection(right);
					walk = right;
				}
				else {
					int g1, g2;
					getPositionInThisDirection(left, 16, g1, g2);
					if (getWorld()->isValidPos(g1, g2)) {
						setDirection(left);
						walk = left;
					}
					else {
						if (walk == up) {
							setDirection(right);
							walk = down;
						}
						else {
							setDirection(right);
							walk = up;

						}
					}
				}
			}

		}
		else if (a % 16 == 0 && b % 16 == 0 && temp_count >= 3) {
			vector<int>randoms;
			for (int i = 0; i <= 270; i += 90) {
				int aa, bb;
				if (abs(curr_dir - i) != 180) {
					getPositionInThisDirection(i, 16, aa, bb);
					if (getWorld()->isValidPos(aa, bb)) {
						randoms.push_back(i);
					}
				}
			}
			int bowser_choice = randInt(0, randoms.size() - 1);
			int choice = randoms[bowser_choice];
			switch (choice) {
			case left:
				walk = left;
				setDirection(left);
				break;
			case right:
				walk = right;
				setDirection(right);
				break;
			case up:
				walk = up;
				setDirection(right);
				break;
			case down:
				walk = down;
				setDirection(right);
			}
		}
		moveAtAngle(getWalk(), 2);
		activated_peach = false;
		activated_yoshi = false;
		ticks_to_move -= 1;
		if (ticks_to_move == 0) {
			state = paused;
			pause_counter = 180;
		}

	}

}

void Vortex::doSomething() {
	int a, b;
	getPositionInThisDirection(firing_direction, 2, a, b);
	moveAtAngle(firing_direction, 2);
	//DESTROY ALL INACTIVE VORTEXES IN STUDENT WORLD
	if (a < 0 || a >= SPRITE_WIDTH*16 || b < 0 || b >= SPRITE_HEIGHT*16) {
		alive = false;
	}
}


