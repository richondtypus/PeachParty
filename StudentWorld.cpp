#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream>
#include <sstream>
#include <iomanip> 
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{   
    actors.clear();
    bank = new int(0);
    m_peach = nullptr;
    m_yoshi = nullptr;
    m_board = new Board();
}

int StudentWorld::init()
{
    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = m_board->loadBoard(board_file);
    if (result == Board::load_fail_file_not_found) {
        cerr << "Could not find board0"<< to_string(getBoardNumber()) <<".txt data file\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_fail_bad_format) {
        cerr << "Your board was improperly formatted\n";
        return GWSTATUS_BOARD_ERROR;
    }
    else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";

        for (int i = 0; i < 16; i++) {
            for (int k = 0; k < 16; k++) {
                Board::GridEntry ge = m_board->getContentsOf(i, k);
                switch (ge) {
                case Board::empty:
                    cout << "Location " << i << ", " << k << " is empty\n";
                    break;
                case Board::boo:
                    addBoo(i, k);
                    addblueCoin(i, k);
                    cout << "Location " << i << ", " << k << " has a Boo and a blue coin square\n";
                    break;
                case Board::bowser:
                    addBowser(i, k);
                    addblueCoin(i, k);
                    cout << "Location " << i << ", " << k << " has a Bowser and a blue coin square\n";
                    break;
                case Board::player:
                    addPeach(i, k);
                    addYoshi(i, k);
                    addblueCoin(i, k);
                    cout << "Location " << i << ", " << k << " has Peach & Yoshi and a blue coin square\n";
                    break;
                case Board::red_coin_square:
                    addredCoin(i, k);
                    cout << "Location " << i << ", " << k << " has a red coin square\n";
                    break;
                case Board::blue_coin_square:
                    addblueCoin(i, k);
                    cout << "Location " << i << ", " << k << " has a blue coin square\n";
                    break;
                case Board::star_square:
                    addStarSquare(i, k);
                    cout << "Location " << i << ", " << k << " has a star square\n";
                    break;
                case Board::up_dir_square:
                    addDirectionalSquare(i, k, 90);
                    cout << "Location " << i << ", " << k << " has a up directional square\n";
                    break;
                case Board::down_dir_square:
                    addDirectionalSquare(i, k, 270);
                    cout << "Location " << i << ", " << k << " has a down directional square\n";
                    break;
                case Board::left_dir_square:
                    addDirectionalSquare(i, k, 180);
                    cout << "Location " << i << ", " << k << " has a left directional square\n";
                    break;
                case Board::right_dir_square:
                    addDirectionalSquare(i, k, 0);
                    cout << "Location " << i << ", " << k << " has a right directional square\n";
                    break;
                case Board::bank_square:
                    addBankSquare(i, k);
                    cout << "Location " << i << ", " << k << " has a bank square\n";
                    break;
                case Board::event_square:
                    addEventSquare(i, k);
                    cout << "Location " << i << ", " << k << " has an event square\n";
                }
            }
        }
        startCountdownTimer(99);
        ostringstream ss;
        ss << "P1 Roll:" << setw(2) << (getPeach()->getTicks()) / 8 << " ";
        ss << "Stars:" << setw(2) << (getPeach()->getStars()) << " ";
        ss << "$$:" << setw(3) << (getPeach()->getCoins()) << " ";
        if (getPeach()->hasVortex()) {
            ss << setw(2) << "VOR";
        }
        ss << "| Time: " << setw(2) << timeRemaining();
        ss << " | Bank:" << setw(2) << (getBank()) << " ";
        ss << "| P2 Roll:" << setw(2) << (getYoshi()->getTicks()) / 8 << " ";
        ss << "Stars:" << setw(2) << (getYoshi()->getStars()) << " ";
        ss << "$$:" << setw(3) << (getYoshi()->getCoins()) << " ";
        if (getYoshi()->hasVortex()) {
            ss << setw(2) << "VOR";
        }
        string display = ss.str();
        setGameStatText(display);
    }


    return GWSTATUS_CONTINUE_GAME;


}

bool overlap();

int StudentWorld::move()
{
    m_peach->doSomething();
    m_yoshi->doSomething();
    for (Actor* a : actors) {
        a->doSomething();
    }
    //remove all dead actors
    for (vector<Actor*>::iterator p = actors.begin(); p != actors.end(); ) {
        if ((*p) == nullptr) {
            continue;
        }
        if ((*p)->isAlive()) {
            p++;
        }
        else {
            delete* p;
            p = actors.erase(p);
        }
    }
    //baddie teleport
    for (vector<Actor*>::iterator p = actors.begin(); p != actors.end(); p++) {
        if ((*p) == nullptr) {
            continue;
        }
        if ((*p)->isBaddie()) {
            for (vector<Actor*>::iterator k = actors.begin(); k != actors.end(); k++) {
                if ((*k)->isVortex()) {
                    if (overlap(*p, *k)) {
                        playSound(SOUND_HIT_BY_VORTEX);
                        double x, y;
                        (*p)->teleport(x, y);
                        (*k)->setAlive(false);
                    }
                }
            }
        }
    }

    ostringstream ss;
    ss << "P1 Roll:" << setw(2) << (getPeach()->getTicks()) / 8<< " ";
    ss << "Stars:" << setw(2) << (getPeach()->getStars())<< " ";
    ss << "$$:" << setw(3) << (getPeach()->getCoins()) << " ";
    if (getPeach()->hasVortex()) {
        ss << setw(2) << "VOR"<<" ";
    }
    else{
        ss << " ";
    }
    ss << "| Time:" << setw(2) << timeRemaining();
    ss << " | Bank:" << setw(2) << (getBank()) << " ";
    ss << "| P2 Roll:" << setw(2) << (getYoshi()->getTicks()) / 8 << " ";
    ss << "Stars:" << setw(2) << (getYoshi()->getStars()) << " ";
    ss << "$$:" << setw(3) << (getYoshi()->getCoins()) << " ";
    if (getYoshi()->hasVortex()) {
        ss << setw(2) << "VOR";
    }
    string display = ss.str();
    setGameStatText(display);

    if (timeRemaining() < 0) {   
        //clean up game
        playSound(SOUND_GAME_FINISHED);
        string winner;
        int p_c = getPeach()->getCoins();
        int p_s = getPeach()->getStars();
        int y_c = getYoshi()->getCoins();
        int y_s = getYoshi()->getStars();
        if (p_s > y_s) {
            winner = "peach";
        }
        else if (p_s < y_s) {
            winner = "yoshi";
        }
        //same stars check coins
        else  {
            if (p_c > y_c) {
                winner = "peach";
            }
            else if (y_c > p_c) {
                winner = "yoshi";
            }
            else {
                int choose = randInt(1, 2);
                if (choose == 1) {
                    winner = "peach";
                }
                else {
                    winner = "yoshi";
                }
            }
        }

        if (winner == "peach") {
            setFinalScore(p_s, p_c);
            return GWSTATUS_PEACH_WON;
        }
        else {
            setFinalScore(y_s, y_c);
            return GWSTATUS_YOSHI_WON;
        }
    }

    

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (Actor* a : actors) {
        delete a;
    }
    actors.clear();
    delete m_peach;
    delete m_yoshi;
    m_peach = nullptr;
    m_yoshi = nullptr;
    delete bank;
    delete m_board;
    m_board = nullptr;
}

StudentWorld::~StudentWorld() { 
    //cleanUp();
}

bool StudentWorld::isValidPos(double x, double y) {
    Board::GridEntry ge = m_board->getContentsOf(int(x / 16), int(y / 16));
    if (ge == Board::GridEntry::empty) {
        return false;
    }
    if ((x >= 0 && x <= VIEW_WIDTH - 1.0) && (y >= 0 && y <= VIEW_HEIGHT - 1.0)) {
        return true;
    }
    return false;
}

bool StudentWorld::isDirectional(double x, double y, int& sprite) {
    Board::GridEntry ge = m_board->getContentsOf(int(x / 16), int(y / 16));
    if (ge == Board::GridEntry::down_dir_square) {
        sprite = 270;
        return true;
    } 
    if (ge == Board::GridEntry::up_dir_square) {
        sprite = 90;
        return true;
    }
    if (ge == Board::GridEntry::left_dir_square) {
        sprite = 180;
        return true;
    }
    if(ge == Board::GridEntry::right_dir_square) {
        sprite = 0;
        return true;
    }
    return false;
}

bool StudentWorld::isEventSquare(double x, double y) {
    Board::GridEntry ge = m_board->getContentsOf(int(x / 16), int(y / 16));
        if (ge == Board::GridEntry::event_square) {
            return true;
        }
        return false;
}

bool StudentWorld::isCoinSquare(double x, double y) {
        Board::GridEntry ge = m_board->getContentsOf(int(x / 16), int(y / 16));
        if (ge == Board::GridEntry::blue_coin_square || ge == Board::GridEntry::red_coin_square) {
            return true;
        }
    return false;
}

bool StudentWorld::isBankSquare(double x, double y) {
    Board::GridEntry ge = m_board->getContentsOf(int(x / 16), int(y / 16));
    if (ge == Board::GridEntry::bank_square) {
        return true;
    }
    return false;
}

bool StudentWorld::isStarSquare(double x, double y) {
    Board::GridEntry ge = m_board->getContentsOf(int(x / 16), int(y / 16));
    if (ge == Board::GridEntry::star_square) {
        return true;
    }
    return false;
}

bool StudentWorld::isDirectionalSquare(double x, double y) {
    Board::GridEntry ge = m_board->getContentsOf(int(x / 16), int(y / 16));
    if (ge == Board::GridEntry::down_dir_square) {
        return true;
    }
    if (ge == Board::GridEntry::up_dir_square) {
        return true;
    }
    if (ge == Board::GridEntry::left_dir_square) {
        return true;
    }
    if (ge == Board::GridEntry::right_dir_square) {
        return true;
    }
    return false;
}

bool StudentWorld::addPeach(double x, double y) {
    m_peach = new Peach(this, x, y);
    return true;
}

bool StudentWorld::addYoshi(double x, double y) {
    m_yoshi = new Yoshi(this, x, y);
    return true;
}

bool StudentWorld::addblueCoin(double x, double y) {
    actors.push_back(new BlueCoin(this, x, y));
    return true;
}

bool StudentWorld::addredCoin(double x, double y) {
    actors.push_back(new RedCoin(this, x, y));
    return true;
}

bool StudentWorld::addStarSquare(double x, double y) {
    actors.push_back(new StarSquare(this, x, y));
    return true;
}

bool StudentWorld::addDirectionalSquare(double x, double y, const int sprite_dir) {
    actors.push_back(new DirectionalSquare(this, x, y, sprite_dir));
    return true;
}

bool StudentWorld::addBankSquare(double x, double y) {
    actors.push_back(new BankSquare(this, x, y));
    return true;
}

bool StudentWorld::addEventSquare(double x, double y) {
    actors.push_back(new EventSquare(this, x, y));
    return true;
}

bool StudentWorld::addDroppingSquare(double x, double y) {
    for (vector<Actor*>::iterator p = actors.begin(); p != actors.end(); ) {
        if ((*p)->getX() == x && ((*p)->getY() == y)) {  
            delete* p;
            p = actors.erase(p);
        }
        else {
            p++;
        }
    }
    actors.push_back(new DroppingSquare(this, x, y));
    return true;
}

bool StudentWorld::addVortex(double x, double y, int dir) {
    actors.push_back(new Vortex(this, x, y, dir));
    return true;
}

bool StudentWorld::addBowser(double x, double y) {
    actors.push_back(new Bowser(this, x, y));
    return true;
}

bool StudentWorld::addBoo(double x, double y) {
    actors.push_back(new Boo(this, x, y));
    return true;
}

bool StudentWorld::intersection(Actor* a, Actor* b) {
    if ((a->getX() == b->getX())) {
        if (a->getY()== b->getY()){
            return true;
        }

    }
    return false;
}

bool StudentWorld::overlap(Actor* a, Actor* b) {
    if (((abs(a->getX() - b->getX())) == 0) && ((abs(a->getY() - b->getY())) < 16)){
        return true;
    }
    if(((abs(a->getX() - b->getX())) < 16) && ((abs(a->getY() - b->getY())) == 0)) {
        return true;
    }
    return false;
}



