#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  ~StudentWorld();
  bool isValidPos(double x, double y);
  bool addPeach(double x, double y);
  bool addYoshi(double x, double y);
  bool addBowser(double x, double y);
  bool addblueCoin(double x, double y);
  bool addredCoin(double x, double y);
  bool addStarSquare(double x, double y);
  bool addDirectionalSquare(double x, double y, const int sprite_dir);
  bool addBankSquare(double x, double y);
  bool addEventSquare(double x, double y);
  bool addDroppingSquare(double x, double y);
  bool addVortex(double x, double y, int dir);
  bool addBoo(double x, double y);
  bool isDirectional(double x, double y, int& sprite);
  bool isEventSquare(double x, double y);
  bool isCoinSquare(double x, double y);
  bool isStarSquare(double x, double y);
  bool isDirectionalSquare(double x, double y);
  bool isBankSquare(double x, double y);
  bool intersection(Actor* a, Actor* b);
  bool overlap(Actor* a, Actor* b);
  void addtoBank(int x) {
	  *(bank) += x;
  }
  int getBank() {
	  return *bank;
  }
  void setBanktoZero() {
	  *bank = 0;
  }
  Peach* getPeach() {
	  return m_peach;
  }
  Yoshi* getYoshi() {
	  return m_yoshi;
  }
private:
	Peach* m_peach;
	Yoshi* m_yoshi;
	vector<Actor*>actors;
	Board* m_board;
	int* bank;
};

#endif // STUDENTWORLD_H_
