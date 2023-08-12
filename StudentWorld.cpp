#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include "Board.h"
#include "Actor.h"
#include <vector>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_peach = nullptr;
    m_yoshi = nullptr;
    actors.clear();
    central_bank_deposit = 0;
}

int StudentWorld::init()
{
	startCountdownTimer(99);  
    
    Board m_board;
    string m_boardFile = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    
    Board::LoadResult result = m_board.loadBoard(m_boardFile);
    if (result == Board::load_fail_file_not_found)
        cerr << "Cannot find data file" << endl;
    else if (result == Board::load_fail_bad_format)
        cerr << "Bad Format" << endl;
    else if (result == Board::load_success)
    {
        cerr << "Successfully loaded" << endl;
        for(int x = 0; x < 16; x++)
            for(int y = 0; y < 16; y++)
            {
                Board::GridEntry ge = m_board.getContentsOf(x, y);
                
                switch (ge) {
                    case Board::player:
                    {
                        m_peach = new peach(this, x, y);
                        actors.push_back(new blue_coinSquare(this, x, y));
                        m_yoshi = new yoshi(this, x, y);
                    }
                        break;
                        
                    case Board::blue_coin_square:
                        actors.push_back(new blue_coinSquare(this, x, y));
                        break;
                    case Board::event_square:
                        actors.push_back(new eventSquare(this, x, y));
                        break;
                    case Board::star_square:
                        actors.push_back(new starSquare(this, x, y));
                    case Board::empty:
                        break;
                    case Board::red_coin_square:
                        actors.push_back(new red_coinSquare(this, x, y));
                        break;
                    case Board::left_dir_square:
                        actors.push_back(new left_dir_square(this, x, y));

                        break;
                    case Board::right_dir_square:
                        actors.push_back(new right_dir_square(this, x, y));
                        break;
                    case Board::up_dir_square:
                        actors.push_back(new up_dir_square(this, x, y));
                        break;
                    case Board::down_dir_square:
                        actors.push_back(new down_dir_square(this, x, y));
                        break;

                    case Board::bank_square:
                        actors.push_back(new bankSquare(this,x,y));
                        break;
                        
                    case Board::bowser:
                        actors.push_back(new bowser(this,x,y));
                        actors.push_back(new blue_coinSquare(this,x,y));

                        break;
                    case Board::boo:
                        actors.push_back(new blue_coinSquare(this, x, y));
                        actors.push_back(new boo(this,x,y));
                        break;
                }
            }
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::createDroppingSquare(double x, double y)
{
    for(Actor* a: actors)
    {
        if(x == a->getX() && y == a->getY() && a->isMovingActor() == false)
        {
            a->makeDead();
            actors.push_back(new droppingSquare(this, x / SPRITE_WIDTH, y / SPRITE_HEIGHT));

            break;
        }
    }
}

void StudentWorld::newVortex(double x, double y, int dir)
{
    switch (dir) {
        case 0:
            x = x + 16;
            break;
        case 90:
            y = y + 16;
            break;
        case 180:
            x = x - 16;
            break;
        case 270:
            y = y - 16;
            break;
        default:
            break;
    }

    actors.push_back(new vortex(this, x / SPRITE_WIDTH, y / SPRITE_WIDTH, dir));
    
}

bool StudentWorld::validPos(double x, double y)
{
    
    for(Actor* a : actors)
    {
        
        if(x == a->getX() && y == a->getY())
        {
            return true;
        }
    }
    return false;
    
}

Actor* StudentWorld::isBooOrBowser (double x, double y)
{
    int sprite_x = x / 16;
    int sprite_y = y / 16;
    
    int a_x, a_y;
    
    for(Actor* a : actors)
    {
        if(a->canBeImpacted())
        {
            if(a->getX() > x && a->getX() % 16 == 0)
                a_x = (a->getX() / 16 ) - 1;
            else
                a_x = a->getX() / 16;
            
            if(a->getY() > x && a->getY() % 16 == 0)
                a_y = (a->getX() / 16 ) - 1;
            else
                a_y = a->getY() / 16;
            
            
            if(a_x == sprite_x && a_y == sprite_y)
                return a;
        }
    }
    return nullptr;
}





int StudentWorld::move()
{
    if(m_peach != nullptr)
        m_peach->doSomething();
    if(m_yoshi != nullptr)
        m_yoshi->doSomething();
    
    for (auto it = actors.begin(); it != actors.end(); it++) {
        if ((*it)->isAlive()) {
            (*it)->doSomething();

        }
    }

    for (auto it = actors.begin(); it != actors.end(); it++) {
        if (!(*it)->isAlive()) {
            delete *it;
            actors.erase(it);
            it = actors.begin();
        }
    }
    if(m_peach != nullptr && m_yoshi != nullptr)
    {
        if(m_peach->getVortex() && m_yoshi->getVortex()){
            setGameStatText("P1 Roll: " + to_string(m_peach->getRoll()) + " Stars: " + to_string(m_peach->getStars()) + " $$: " + to_string(m_peach->getCoin()) + " VOR | Time: " + to_string(timeRemaining()) + " | Bank: " + to_string(getDeposit()) + " | P2 Roll: " + to_string(m_yoshi->getRoll()) + " Stars: " + to_string(m_yoshi->getStars()) + " $$: " + to_string(m_yoshi->getCoin()) + " VOR");
        }
        else if(m_peach->getVortex())
            setGameStatText("P1 Roll: " + to_string(m_peach->getRoll()) + " Stars: " + to_string(m_peach->getStars()) + " $$: " + to_string(m_peach->getCoin()) + " VOR | Time: " + to_string(timeRemaining()) + " | Bank: " + to_string(getDeposit()) + " | P2 Roll: " + to_string(m_yoshi->getRoll()) + " Stars: " + to_string(m_yoshi->getStars()) + " $$: " + to_string(m_yoshi->getCoin()));
        else if(m_yoshi->getVortex())
            setGameStatText("P1 Roll: " + to_string(m_peach->getRoll()) + " Stars: " + to_string(m_peach->getStars()) + " $$: " + to_string(m_peach->getCoin()) + " | Time: " + to_string(timeRemaining()) + " | Bank: " + to_string(getDeposit()) + " | P2 Roll: " + to_string(m_yoshi->getRoll()) + " Stars: " + to_string(m_yoshi->getStars()) + " $$: " + to_string(m_yoshi->getCoin()) + " VOR");
        else
            setGameStatText("P1 Roll: " + to_string(m_peach->getRoll()) + " Stars: " + to_string(m_peach->getStars()) + " $$: " + to_string(m_peach->getCoin()) + " | Time: " + to_string(timeRemaining()) + " | Bank: " + to_string(getDeposit()) + " | P2 Roll: " + to_string(m_yoshi->getRoll()) + " Stars: " + to_string(m_yoshi->getStars()) + " $$: " + to_string(m_yoshi->getCoin()));
    }
    
    
    
    
    if(timeRemaining() <= 0)
    {
        playSound(SOUND_GAME_FINISHED);
        if(m_yoshi == nullptr){
            setFinalScore(m_yoshi->getStars(), m_yoshi->getCoin());
            return GWSTATUS_PEACH_WON;
        }
        if(m_yoshi->getStars() > m_peach->getStars())
        {
            setFinalScore(m_yoshi->getStars(), m_yoshi->getCoin());
            return GWSTATUS_YOSHI_WON;
        }
        else if(m_yoshi->getStars() < m_peach->getStars())
        {
            setFinalScore(m_yoshi->getStars(), m_yoshi->getCoin());
            return GWSTATUS_PEACH_WON;
        }
        else
        {
            if(m_yoshi->getCoin() > m_peach->getCoin())
            {
                setFinalScore(m_yoshi->getStars(), m_yoshi->getCoin());
                return GWSTATUS_YOSHI_WON;
            }
            else if(m_yoshi->getCoin() < m_peach->getCoin())
            {
                setFinalScore(m_yoshi->getStars(), m_yoshi->getCoin());
                return GWSTATUS_PEACH_WON;
            }
            else
            {
                int winner = randInt(1,2);
                if(winner == 1)
                {
                    setFinalScore(m_yoshi->getStars(), m_yoshi->getCoin());
                    return GWSTATUS_YOSHI_WON;
                }
                else
                {
                    setFinalScore(m_yoshi->getStars(), m_yoshi->getCoin());
                    return GWSTATUS_PEACH_WON;
                }
                
            }
        }
    }
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if(m_peach != nullptr)
    {
        delete m_peach;
        m_peach = nullptr;
    }
    
    if(m_yoshi != nullptr)
    {
        delete m_yoshi;
        m_yoshi = nullptr;
    }
    
    for(Actor* a : actors)
        delete a;
    actors.clear();
}

StudentWorld::~StudentWorld()
{
    cleanUp();
};
