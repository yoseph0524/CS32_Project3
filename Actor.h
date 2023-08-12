#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, int startDirection = right, int depth = 0, double size = 1.0) : GraphObject(imageID, startX, startY), m_world(world), alive(true), impacted(false) {};
    virtual void doSomething() = 0;
    StudentWorld* getWorld(){return m_world;};
    bool canMove(int dir);
    void makeDead(){alive = false;};
    bool isAlive(){return alive;};
    virtual bool isMovingActor(){return false;};
    virtual bool canBeImpacted(){return false;};
    
    bool getImpacted(){return impacted;};
    void impact(){impacted = true;};
    void notImpact(){impacted = false;};
    
    
private:
    StudentWorld* m_world;
    bool alive;
    bool impacted;
    
};

class playerAvatar : public Actor
{
public:
    playerAvatar(StudentWorld* world, int imageID, int now_x, int now_y, int playerNumber)
    : Actor(world, imageID, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y), ticks_to_move(0), state(0), m_coins(0), m_stars(0), m_vortex(0), m_playerNumber(playerNumber), ultimate_direction(right), start(0), teleported(false), m_new(true), m_newForBaddies(false) {};
    
    virtual void doSomething();
    
    int getCoin(){return m_coins;};
    void addcoin(int num);
    int getState(){return state;};
    void subcoin(int num);
    void addStar(int num){m_stars += num;};
    void setUltimateDirection(int dir){ultimate_direction = dir;};
    int getticks(){return ticks_to_move;};
    void decreaseTicks(){ ticks_to_move -= 8;};
    int getStars(){return m_stars;};
    void subStars(int num){m_stars -= num;};
    int getRoll(){return ticks_to_move / 8;};

    int getUltimateDirection(){return ultimate_direction;};
    
    void getOneVortex(){m_vortex = 1;};
    void notStart(){start++;};
    bool isStart(){return start==0;};
    void setTicks(int ticks){ticks_to_move = ticks;};
    void setState(int tstate){state = tstate;};
    
    bool getVortex(){return m_vortex == 1;};
    bool isMoving(){return state == 1;};
    
    bool isTeleported(){return teleported;};
    void teleport(){teleported = true;};
    void notTeleported(){teleported = false;};
    
    bool getNew(){return m_new;};
    void isNew(){m_new = true;};
    void notNew(){m_new = false;};
    
    bool getNewForBaddies(){return m_newForBaddies;};
    void isNewForBaddies(){m_newForBaddies = true;};
    void notNewForBaddies(){m_newForBaddies = false;};
    
    
private:
    int ticks_to_move;
    int state;
    int m_coins;
    int m_stars;
    int m_vortex;
    int m_playerNumber;
    int ultimate_direction;
    int start;
    bool teleported;
    bool m_new;
    bool m_newForBaddies;
    
};



class peach : public playerAvatar
{
public:
    peach(StudentWorld* world, int now_x, int now_y, int playerNumber = 2)
    : playerAvatar(world, IID_PEACH, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y, playerNumber){};
};



class yoshi : public playerAvatar
{
public:
    yoshi(StudentWorld* world, int now_x, int now_y, int playerNumber = 1)
    : playerAvatar(world, IID_YOSHI, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y, playerNumber){};
};










class baddies : public Actor
{
public:
    baddies(StudentWorld* world, int imageID, int startX, int startY) :
        Actor(world, imageID, startX, startY), start(0){};
    void notStart(){start++;};
    bool isStart(){return start==0;};
    

    virtual bool canBeImpacted(){return true;};
    
    
private:
    int start;
};


class bowser : public baddies
{
public:
    bowser(StudentWorld* world, int now_x, int now_y) :
        baddies(world, IID_BOWSER, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y), m_peach(nullptr), m_yoshi(nullptr), m_state(0), initial_travel_distance(0), pause_counter(180), ticks_to_move(0), squares_to_move(0), ultimate_direction(0)  {};
    void doSomething();
    void setUltimateDirection(int dir){ultimate_direction = dir;};
    virtual bool isMovingActor(){return true;};

private:
    peach* m_peach;
    yoshi* m_yoshi;
    
    int m_state;
    int initial_travel_distance;
    int pause_counter;
    int ticks_to_move;
    int squares_to_move;
    int ultimate_direction;
};


class boo : public baddies
{
public:
    boo(StudentWorld* world, int now_x, int now_y, int depth = 1) :
        baddies(world, IID_BOO, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y), m_peach(nullptr), m_yoshi(nullptr), m_state(0), initial_travel_distance(0), pause_counter(180), ultimate_direction(0), ticks_to_move(0) {};
    void doSomething();
    void setUltimateDirection(int dir){ultimate_direction = dir;};

    
private:
    peach* m_peach;
    yoshi* m_yoshi;
    
    int m_state;
    int initial_travel_distance;
    int pause_counter;
    int ultimate_direction;
    int ticks_to_move;
};

class squares : public Actor
{
public:
    squares(StudentWorld* world, int imageID, int startX, int startY, int startDirection = right, int depth = 1, double size = 1.0) :
        Actor(world, imageID, startX, startY) {};
    
private:
};

class blue_coinSquare : public squares
{
public:
    blue_coinSquare(StudentWorld* world, int now_x, int now_y)
        : squares(world, IID_BLUE_COIN_SQUARE, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y), m_peach(nullptr), m_yoshi(nullptr) {};
    virtual void doSomething();
    
private:
    peach* m_peach;
    yoshi* m_yoshi;

};

class red_coinSquare : public squares
{
public:
    red_coinSquare(StudentWorld* world, int now_x, int now_y)
    : squares(world, IID_RED_COIN_SQUARE, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y), m_peach(nullptr), m_yoshi(nullptr){};
    virtual void doSomething();
    
private:
    peach* m_peach;
    yoshi* m_yoshi;
};

class eventSquare : public squares
{
public:
    eventSquare(StudentWorld* world, int now_x, int now_y)
    : squares(world, IID_EVENT_SQUARE, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y), m_peach(nullptr), m_yoshi(nullptr){};
    virtual void doSomething();
private:
    peach* m_peach;
    yoshi* m_yoshi;
};

class droppingSquare : public squares
{
public:
    droppingSquare(StudentWorld* world, int now_x, int now_y)
    : squares(world, IID_DROPPING_SQUARE, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y), m_peach(nullptr), m_yoshi(nullptr){};
    virtual void doSomething();
private:
    peach* m_peach;
    yoshi* m_yoshi;
};

class starSquare : public squares
{
public:
   starSquare(StudentWorld* world, int now_x, int now_y) :
    squares(world, IID_STAR_SQUARE, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y), m_peach(nullptr), m_yoshi(nullptr) {};
    virtual void doSomething();
private:
    peach* m_peach;
    yoshi* m_yoshi;
};

class bankSquare : public squares
{
public:
   bankSquare(StudentWorld* world, int now_x, int now_y) :
    squares(world, IID_BANK_SQUARE, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y), m_peach(nullptr), m_yoshi(nullptr) {};
    virtual void doSomething();
    
private:
    peach* m_peach;
    yoshi* m_yoshi;
};

class left_dir_square : public squares
{
public:
   left_dir_square(StudentWorld* world, int now_x, int now_y) :
    squares(world, IID_DIR_SQUARE, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y), m_peach(nullptr), m_yoshi(nullptr) {};
    virtual void doSomething();
    
private:
    peach* m_peach;
    yoshi* m_yoshi;
};

class right_dir_square : public squares
{
public:
   right_dir_square(StudentWorld* world, int now_x, int now_y) :
    squares(world, IID_DIR_SQUARE, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y), m_peach(nullptr), m_yoshi(nullptr) {};
    virtual void doSomething();
private:
    peach* m_peach;
    yoshi* m_yoshi;
};

class up_dir_square : public squares
{
public:
   up_dir_square(StudentWorld* world, int now_x, int now_y) :
    squares(world, IID_DIR_SQUARE, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y), m_peach(nullptr), m_yoshi(nullptr) {};
    virtual void doSomething();
    
private:
    peach* m_peach;
    yoshi* m_yoshi;
};

class down_dir_square : public squares
{
public:
   down_dir_square(StudentWorld* world, int now_x, int now_y) :
    squares(world, IID_DIR_SQUARE, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y), m_peach(nullptr), m_yoshi(nullptr) {};
    virtual void doSomething();
    
private:
    peach* m_peach;
    yoshi* m_yoshi;
};

class vortex : public Actor
{
public:
    vortex(StudentWorld* world, int now_x, int now_y, int dir)
    : Actor(world, IID_VORTEX, SPRITE_WIDTH * now_x, SPRITE_HEIGHT * now_y), m_dir(dir), active(true), m_actor(nullptr){};
    virtual void doSomething();
private:
    int m_dir;
    bool active;
    Actor* m_actor;
};

#endif // ACTOR_H_
