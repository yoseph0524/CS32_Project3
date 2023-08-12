#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <string>
#include "Actor.h"
#include <vector>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
    ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();
    
    peach* getPeach(){return m_peach;};
    yoshi* getYoshi(){return m_yoshi;};

    bool validPos(double x, double y);
    Actor* isBooOrBowser(double x, double y);
    
    
    void addDeposit(int num){central_bank_deposit += num;};
    void resetDeposit(){central_bank_deposit = 0;};
    int getDeposit(){return central_bank_deposit;};
    
    void createDroppingSquare(double x, double y);
    void newVortex(double x, double y, int dir);

private:
    peach* m_peach;
    yoshi* m_yoshi;
    std::vector<Actor*> actors;
    int central_bank_deposit;
    
};

#endif // STUDENTWORLD_H_
