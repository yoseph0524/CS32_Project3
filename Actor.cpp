#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

void vortex::doSomething()
{
    
    if(active == false)
        return;
    
    if(m_dir == right)
        moveTo(getX() + 2, getY());
    else if(m_dir == up)
        moveTo(getX(), getY() + 2);
    else if(m_dir == left)
        moveTo(getX() - 2, getY());
    else if(m_dir == down)
        moveTo(getX(), getY() - 2);
    else
        return;
    
    if(getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT)
    {
        makeDead();
    }
    
    m_actor = getWorld()->isBooOrBowser(getX(), getY());
    
    if(m_actor != nullptr)
    {
        m_actor->impact();
        makeDead();
        getWorld()->playSound(SOUND_HIT_BY_VORTEX);
    }

}



void boo::doSomething()
{
    m_yoshi = getWorld()->getYoshi();
    m_peach = getWorld()->getPeach();
    
    if(getImpacted() == true)
    {
        int x = randInt(0, 15) * SPRITE_WIDTH;
        int y = randInt(0, 15) * SPRITE_HEIGHT;
        
        while(!getWorld()->validPos(x, y)  && (x != getX() || y != getY()))
        {
            x = randInt(0, 15) * SPRITE_WIDTH;
            y = randInt(0, 15) * SPRITE_HEIGHT;
        }
        
        moveTo(x, y);
        setDirection(right);
        ultimate_direction = right;
        m_state  = 0;
        pause_counter = 180;
        notImpact();
    }

    
    if(m_state == 0)
    {
        if(getX() == m_yoshi->getX() && getY() == m_yoshi->getY() && m_yoshi->getState() == 0 && m_yoshi->getNewForBaddies())
        {
            getWorld()->playSound(SOUND_BOO_ACTIVATE);
            int rand = randInt(1,2);
            if(rand == 1 )
            {
                int temp = m_yoshi->getCoin();
                m_yoshi->subcoin(temp);
                m_yoshi->addcoin(m_peach->getCoin());
                m_peach->subcoin(m_peach->getCoin());
                m_peach->addcoin(temp);
            }
            else
            {
                int temp = m_yoshi->getStars();
                m_yoshi->subStars(temp);
                m_yoshi->addStar(m_peach->getStars());
                m_peach->subStars(m_peach->getStars());
                m_peach->addStar(temp);
            }
            m_yoshi->notNewForBaddies();
            getWorld()->playSound(SOUND_BOO_ACTIVATE);

        }
        if(getX() == m_peach->getX() && getY() == m_peach->getY() && m_peach->getState() == 0 && m_peach->getNewForBaddies())
        {
            int rand = randInt(1,2);
            if(rand == 1)
            {
                int temp = m_yoshi->getCoin();
                m_yoshi->subcoin(temp);
                m_yoshi->addcoin(m_peach->getCoin());
                m_peach->subcoin(m_peach->getCoin());
                m_peach->addcoin(temp);
            }
            else
            {
                int temp = m_yoshi->getStars();
                m_yoshi->subStars(temp);
                m_yoshi->addStar(m_peach->getStars());
                m_peach->subStars(m_peach->getStars());
                m_peach->addStar(temp);
            }
            getWorld()->playSound(SOUND_BOO_ACTIVATE);
            m_peach->notNewForBaddies();
        }
        pause_counter--;
        
        if(pause_counter == 0)
        {
            int squares_to_move = randInt(1, 3);
            ticks_to_move = squares_to_move * 8;
            
            int dir = randInt(0, 3);
            while(!canMove(dir * 90))
            {
                dir = randInt(0, 3);
            }
            switch(dir)
            {
                case 1:
                {
                    setDirection(right);
                    setUltimateDirection(up);
                    break;
                }
                case 2:
                {
                    setUltimateDirection(left);
                    setDirection(left);
                    break;
                }
                case 3:
                {
                    setDirection(right);

                    setUltimateDirection(down);
                    break;
                }
                case 0:
                {
                    setDirection(right);

                    setUltimateDirection(right);
                    break;
                }
            }
            m_state = 1;
        }
    }
    
    if(m_state == 1)
    {
        if(isStart())
        {
            if(canMove(right))
            {
                for(int i = 0; i < 8; i++)
                    moveTo(getX() + 2, getY());
                ticks_to_move -= 8;
                ultimate_direction = right;
            }
            else if (canMove(down))
            {
                for(int i = 0; i < 8; i++)
                    moveTo(getX(), getY() - 2);
                ticks_to_move -= 8;
                ultimate_direction = down;
            }
            else if (canMove(up))
            {
                for(int i = 0; i < 8; i++)
                    moveTo(getX(), getY() + 2);
                ticks_to_move -= 8;
                ultimate_direction = up;
            }
            else
            {
                for(int i = 0; i < 8; i++)
                    moveTo(getX() - 2, getY());
                ticks_to_move -= 8;
                ultimate_direction = left;
                setDirection(left);
            }

            notStart();
        }
        
        if(canMove(ultimate_direction) && (canMove(ultimate_direction + 90) || canMove(ultimate_direction + 270)))
        {
            int dir = randInt(1,3);
            if(ultimate_direction == right)
            {
                switch(dir)
                {
                    case 1:
                    {
                        if(canMove(up))
                            setUltimateDirection(up);
                        else
                            return;
                        break;
                    }
                    case 2:
                    {
                        if(canMove(down))
                            setUltimateDirection(down);
                        else
                            return;
                        break;
                    }
                    case 3:
                    {
                        if(canMove(right))
                            setUltimateDirection(right);
                        else
                            return;
                        break;
                    }
                    default:
                        return;
                }
            }
            else if(ultimate_direction == up)
            {
                switch(dir)
                {
                    case 1:
                    {
                        if(canMove(left))
                        {
                            setDirection(left);
                            setUltimateDirection(left);
                        }
                        else
                            return;
                        break;
                    }
                    case 2:
                    {
                        if(canMove(right))
                            setUltimateDirection(right);
                        else
                            return;
                        break;
                    }
                    case 3:
                    {
                        if(canMove(up))
                            setUltimateDirection(up);
                        else
                            return;
                        break;
                    }
                    default:
                        return;
                }
                
            }
            
            else if(ultimate_direction == left)
            {
                switch(dir)
                {
                    case 1:
                    {
                        if(canMove(up))
                        {
                            setDirection(right);
                            setUltimateDirection(up);
                        }
                        else
                            return;
                        break;
                    }
                    case 2:
                    {
                        if(canMove(down))
                        {
                            setDirection(right);
                            setUltimateDirection(down);
                        }
                        else
                            return;
                        break;
                    }
                    case 3:
                    {
                        if(canMove(left))
                        {
                            setDirection(left);
                            setUltimateDirection(left);
                        }
                        else
                            return;
                        break;
                    }
                    default:
                        return;
                }
            }
            else if(ultimate_direction == down)
            {
                switch(dir)
                {
                    case 1:
                    {
                        if(canMove(left))
                        {
                            setDirection(left);
                            setUltimateDirection(left);
                        }
                        else
                            return;
                        break;
                    }
                    case 2:
                    {
                        if(canMove(right))
                            setUltimateDirection(right);
                        else
                            return;
                        break;
                    }
                    case 3:
                    {
                        if(canMove(down))
                            setUltimateDirection(down);
                        else
                            return;
                        break;
                    }
                    default:
                        return;
                }
                
            }
            else
                return;
        }
  
            if(ticks_to_move % 8 == 0 && ticks_to_move != 0)
            {
                if(!canMove(ultimate_direction))
                {
                    if(ultimate_direction == right)
                    {
                        
                        if(canMove(up))
                        {
                            ultimate_direction = up;
                        }
                        else
                        {
                            ultimate_direction = down;
                        }
                    }
                    
                    else if(ultimate_direction == up)
                    {
                        if(canMove(right))
                        {
                            ultimate_direction = right;
                        }
                        else
                        {
                            setDirection(left);
                            ultimate_direction = left;
                        }
                    }
                    
                    else if(ultimate_direction == left)
                    {
                        if(canMove(up))
                        {
                            setDirection(right);
                            ultimate_direction = up;
                            
                            
                        }
                        else
                        {
                            setDirection(right);
                            ultimate_direction = down;
                            
                        }
                    }
                    
                    else if(ultimate_direction == down)
                    {
                        if(canMove(right))
                        {
                            ultimate_direction = right;
                        }
                        else
                        {
                            ultimate_direction = left;
                        }
                    }
                    else{return;}
                    
                }
            }
            if(ticks_to_move == 0)
            {
                m_state = 0;
                pause_counter = 180;
                return;
            }
            
            
            switch (ultimate_direction) {
                case up:
                    setDirection(right);
                    moveTo(getX(), getY() + 2);
                    break;
                case down:
                    setDirection(right);
                    moveTo(getX(), getY() -  2);
                    break;
                case right:
                    setDirection(right);
                    moveTo(getX() + 2, getY());
                    break;
                case left:
                    setDirection(left);
                    moveTo(getX() - 2, getY());
                    break;
                    
            }
            ticks_to_move--;
            
            if(ticks_to_move == 0)
            {
                m_state = 0;
                pause_counter = 180;
                return;
            }
        }
}



void bowser::doSomething()
{
    m_yoshi = getWorld()->getYoshi();
    m_peach = getWorld()->getPeach();
    
    if(getImpacted() == true)
    {
        int x = randInt(0, 15) * SPRITE_WIDTH;
        int y = randInt(0, 15) * SPRITE_HEIGHT;
        
        while(!getWorld()->validPos(x, y) && (x != getX() || y != getY()))
        {
            x = randInt(0, 15) * SPRITE_WIDTH;
            y = randInt(0, 15) * SPRITE_HEIGHT;
        }
        
        moveTo(x, y);
        setDirection(right);
        ultimate_direction = right;
        m_state  = 0;
        pause_counter = 180;
        notImpact();
    }
    
    
    
    
    if(m_state == 0)
    {
        int i = randInt(1,2);
        if(getX() == m_yoshi->getX() && getY() == m_yoshi->getY() && m_yoshi->getState() == 0 && m_yoshi->getNewForBaddies() == true)
        {
            if(i == 1)
            {
                m_yoshi->subcoin(m_yoshi->getCoin());
                m_peach->subStars(m_yoshi->getStars());
                getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            }
            m_yoshi->notNewForBaddies();
            
        }
        
        if(getX() == m_peach->getX() && getY() == m_peach->getY() && m_peach->getState() == 0 && m_peach->getNewForBaddies() == true)
        {
            if(i == 1)
            {
                m_peach->subcoin(m_peach->getCoin());
                m_peach->subStars(m_peach->getStars());
                getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
            }
            m_peach->notNewForBaddies();
        }
        if(pause_counter != 0)
            pause_counter--;
        
        if(pause_counter == 0)
        {
            squares_to_move = randInt(1,10);
            ticks_to_move = squares_to_move * 8;
            int dir = randInt(1, 4);
            switch (dir) {
                case 1:
                {
                    if(canMove(up))
                        setDirection(right);
                        ultimate_direction = up;
                    break;
                }
                case 2:
                {
                    if(canMove(down))
                        setDirection(right);
                        ultimate_direction = down;
                    break;
                }
                case 3:
                {
                    if(canMove(right))
                        setDirection(right);
                        ultimate_direction = right;
                    break;
                }
                case 4:
                {
                    if(canMove(left))
                    {
                        ultimate_direction = left;
                        setDirection(left);
                    }
                    break;
                }
            }
            m_state = 1;
        }
        
    }
    
    if(m_state == 1)
    {
        
        if(isStart())
        {
            if(canMove(right))
            {
                for(int i = 0; i < 8; i++)
                    moveTo(getX() + 2, getY());
                ticks_to_move -= 8;
                ultimate_direction = right;
                notStart();
            }
            else
            {
                notStart();
                int dir = randInt(0,3);
                while(!canMove(dir * 90))
                    dir = randInt(0, 3);
                ultimate_direction = dir * 90;
                std::cerr << 1;
                switch (dir) {
                    case right:
                        ticks_to_move -= 8;
                        moveTo(getX() + 16, getY());
                        break;
                    case up:
                        ticks_to_move -= 8;
                        moveTo(getX(), getY() + 16);
                        break;
                    case down:
                        ticks_to_move -= 8;
                        moveTo(getX(), getY() - 16);
                        break;
                    case left:
                        ticks_to_move -= 8;
                        moveTo(getX() + 16, getY());
                        setDirection(left);
                        break;
                }
            }
        }
        
        if(canMove(ultimate_direction) && (canMove(ultimate_direction + 90) || canMove(ultimate_direction + 270)))
        {
            int dir = randInt(1,3);
            if(ultimate_direction == right)
            {
                switch(dir)
                {
                    case 1:
                    {
                        if(canMove(up))
                            setUltimateDirection(up);
                        else
                            return;
                        break;
                    }
                    case 2:
                    {
                        if(canMove(down))
                            setUltimateDirection(down);
                        else
                            return;
                        break;
                    }
                    case 3:
                    {
                        if(canMove(right))
                            setUltimateDirection(right);
                        else
                            return;
                        break;
                    }
                    default:
                        return;
                }
            }
            else if(ultimate_direction == up)
            {
                switch(dir)
                {
                    case 1:
                    {
                        if(canMove(left))
                        {
                            setDirection(left);
                            setUltimateDirection(left);
                        }
                        else
                            return;
                        break;
                    }
                    case 2:
                    {
                        if(canMove(right))
                            setUltimateDirection(right);
                        else
                            return;
                        break;
                    }
                    case 3:
                    {
                        if(canMove(up))
                            setUltimateDirection(up);
                        else
                            return;
                        break;
                    }
                    default:
                        return;
                }
                
            }
            
            else if(ultimate_direction == left)
            {
                switch(dir)
                {
                    case 1:
                    {
                        if(canMove(up))
                        {
                            setDirection(right);
                            setUltimateDirection(up);
                        }
                        else
                            return;
                        break;
                    }
                    case 2:
                    {
                        if(canMove(down))
                        {
                            setDirection(right);
                            setUltimateDirection(down);
                        }
                        else
                            return;
                        break;
                    }
                    case 3:
                    {
                        if(canMove(left))
                        {
                            setDirection(left);
                            setUltimateDirection(left);
                        }
                        else
                            return;
                        break;
                    }
                    default:
                        return;
                }
            }
            else if(ultimate_direction == down)
            {
                switch(dir)
                {
                    case 1:
                    {
                        if(canMove(left))
                        {
                            setDirection(left);
                            setUltimateDirection(left);
                        }
                        else
                            return;
                        break;
                    }
                    case 2:
                    {
                        if(canMove(right))
                            setUltimateDirection(right);
                        else
                            return;
                        break;
                    }
                    case 3:
                    {
                        if(canMove(down))
                            setUltimateDirection(down);
                        else
                            return;
                        break;
                    }
                    default:
                        return;
                }
                
            }
            else
                return;
        }
  
            if(ticks_to_move % 8 == 0 && ticks_to_move != 0)
            {
                if(!canMove(ultimate_direction))
                {
                    if(ultimate_direction == right)
                    {
                        
                        if(canMove(up))
                        {
                            ultimate_direction = up;
                            
                        }
                        else
                        {
                            ultimate_direction = down;
                        }
                    }
                    
                    else if(ultimate_direction == up)
                    {
                        if(canMove(right))
                        {
                            ultimate_direction = right;
                        }
                        else
                        {
                            setDirection(left);
                            ultimate_direction = left;
                        }
                    }
                    
                    else if(ultimate_direction == left)
                    {
                        if(canMove(up))
                        {
                            setDirection(right);
                            ultimate_direction = up;                        }
                        else
                        {
                            setDirection(right);
                            ultimate_direction = down;
                        }
                    }
                    
                    else if(ultimate_direction == down)
                    {
                        if(canMove(right))
                        {
                            ultimate_direction = right;
                        }
                        else
                        {
                            setDirection(left);
                            ultimate_direction = left;
                        }
                    }
                    else{return;}
                }
            }
            if(ticks_to_move == 0)
            {
                m_state = 0;
                pause_counter = 180;
                int chance = randInt(1, 4);
                
                switch (chance) {
                    case 1:
                    {
                        getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
                        getWorld()->createDroppingSquare(getX(), getY());
                        
                        break;
                    }
                    default:
                        break;
                }
                return;
            }
            switch (ultimate_direction) {
                case up:
                    setDirection(right);
                    moveTo(getX(), getY() + 2);
                    break;
                case down:
                    setDirection(right);
                    moveTo(getX(), getY() -  2);
                    break;
                case right:
                    setDirection(right);
                    moveTo(getX() + 2, getY());
                    break;
                case left:
                    setDirection(left);
                    moveTo(getX() - 2, getY());
                    break;
                    
            }
            ticks_to_move--;
            if(ticks_to_move == 0)
            {
                m_state = 0;
                pause_counter = 180;
                int chance = randInt(1, 4);
                
                switch (chance) {
                    case 1:
                    {
                        getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
                        getWorld()->createDroppingSquare(getX(), getY());
                        return;
                        
                        break;
                    }
                    default:
                        break;
                }
            }
        }
}


void blue_coinSquare::doSomething()
{
    m_yoshi = getWorld()->getYoshi();
    m_peach = getWorld()->getPeach();
    
    if(!isAlive())
        return;
    
    if(getX() == m_yoshi->getX() && getY() == m_yoshi->getY() && m_yoshi->getState() == 0 && m_yoshi->getNew() == true)
    {
        m_yoshi->addcoin(3);
        getWorld()->playSound(SOUND_GIVE_COIN);
        m_yoshi->notNew();

    }
    if(getX() == m_peach->getX() && getY() == m_peach->getY() && m_peach->getState() == 0 && m_peach->getNew() == true)
    {
        m_peach->addcoin(3);
        getWorld()->playSound(SOUND_GIVE_COIN);
        m_peach->notNew();
    }
    
    
    
}

void red_coinSquare::doSomething()
{
    m_yoshi = getWorld()->getYoshi();
    m_peach = getWorld()->getPeach();
    
    if(getX() == m_yoshi->getX() && getY() == m_yoshi->getY() && m_yoshi->getState() == 0 && m_yoshi->getNew() == true)
    {
        m_yoshi->subcoin(3);
        getWorld()->playSound(SOUND_TAKE_COIN);
        m_yoshi->notNew();

    }
    if(getX() == m_peach->getX() && getY() == m_peach->getY() && m_peach->getState() == 0 && m_peach->getNew() == true)
    {
        m_peach->subcoin(3);
        getWorld()->playSound(SOUND_TAKE_COIN);
        m_peach->notNew();
    }
}




void starSquare::doSomething()
{
    m_yoshi = getWorld()->getYoshi();
    m_peach = getWorld()->getPeach();
    
    if(getX() == m_yoshi->getX() && getY() == m_yoshi->getY() && m_yoshi->getNew() == true)
    {
        if(m_yoshi->getCoin() < 20)
            return;
        else
        {
            m_yoshi->subcoin(20);
            m_yoshi->addStar(1);
            getWorld()->playSound(SOUND_GIVE_STAR);
            m_yoshi->notNew();
        }
    }
    
    if(getX() == m_peach->getX() && getY() == m_peach->getY() && m_peach->getNew() == true)
    {
        if(m_peach->getCoin() < 20)
            return;
        else
        {
            m_peach->subcoin(20);
            m_peach->addStar(1);
            getWorld()->playSound(SOUND_GIVE_STAR);
            m_peach->notNew();
        }
    }
    
    
}



void eventSquare::doSomething()
{
    m_yoshi = getWorld()->getYoshi();
    m_peach = getWorld()->getPeach();

    
    if(getX() == m_yoshi->getX() && getY() == m_yoshi->getY() && m_yoshi->getState() == 0 && m_yoshi->getNew() == true)
    {
        int i = randInt(1,3);
        switch (i) {
            case 1: // teleport
            {
                int x = randInt(0, 15);
                int y = randInt(0, 15);
                m_yoshi->notNew();
            while(!getWorld()->validPos(x * SPRITE_WIDTH, y * SPRITE_HEIGHT))
                {
                    x = randInt(0, 15);
                    y = randInt(0, 15);
                }
                
                m_yoshi->moveTo(x * SPRITE_WIDTH, y * SPRITE_HEIGHT);
                m_yoshi->teleport();
                break;
            }
            case 2: // swap
            {
                
                m_yoshi->notNew();
                int tx = m_yoshi->getX();
                int ty = m_yoshi->getY();
                m_yoshi->moveTo(m_peach->getX(), m_peach->getY());
                m_peach->moveTo(tx, ty);
                
                int tticks = m_yoshi->getticks();
                m_yoshi->setTicks(m_peach->getticks());
                m_peach->setTicks(tticks);
                
                int twalk = m_yoshi->getUltimateDirection();
                m_yoshi->setUltimateDirection(m_peach->getUltimateDirection());
                m_peach->setUltimateDirection(twalk);
                
                int tsprite = m_yoshi->getDirection();
                m_yoshi->setDirection(m_peach->getDirection());
                m_peach->setDirection(tsprite);
                
                int tstate = m_yoshi->getState();
                m_yoshi->setState(m_peach->getState());
                m_peach->setState(tstate);
                
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                m_yoshi->teleport();
                break;
            }
                
            case 3: // give vortex
            {
                m_yoshi->notNew();
                m_yoshi->getOneVortex();
                getWorld()->playSound(SOUND_GIVE_VORTEX);
                break;
            }
        }
    }
    if(getX() == m_peach->getX() && getY() == m_peach->getY() && m_peach->getState() == 0 && m_peach->getNew() == true)
    {
        int i = randInt(1,3);
        switch (i) {
            case 1: // teleport
            {
                m_peach->notNew();
                int x = randInt(0, 15);
                int y = randInt(0, 15);
                while(! getWorld()->validPos(x * SPRITE_HEIGHT, y * SPRITE_WIDTH))
                {
                    x = randInt(0, 15);
                    y = randInt(0, 15);
                }
                m_peach->moveTo(x * SPRITE_HEIGHT, y * SPRITE_WIDTH);
                m_peach->teleport();
                break;
            }
            case 2: // swap
            {
                m_peach->notNew();
                int tx = m_yoshi->getX();
                int ty = m_yoshi->getY();
                m_yoshi->moveTo(m_peach->getX(), m_peach->getY());
                m_peach->moveTo(tx, ty);
                
                int tticks = m_yoshi->getticks();
                m_yoshi->setTicks(m_peach->getticks());
                m_peach->setTicks(tticks);
                
                int twalk = m_yoshi->getUltimateDirection();
                m_yoshi->setUltimateDirection(m_peach->getUltimateDirection());
                m_peach->setUltimateDirection(twalk);
                
                int tsprite = m_yoshi->getDirection();
                m_yoshi->setDirection(m_peach->getDirection());
                m_peach->setDirection(tsprite);
                
                int tstate = m_yoshi->getState();
                m_yoshi->setState(m_peach->getState());
                m_peach->setState(tstate);
                
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
                m_peach->teleport();
                break;
            }
                
            case 3: // give vortex
            {
                m_peach->notNew();
                m_peach->getOneVortex();
                getWorld()->playSound(SOUND_GIVE_VORTEX);
                break;
            }
        }
    }
    
};

void bankSquare::doSomething()
{
    m_yoshi = getWorld()->getYoshi();
    m_peach = getWorld()->getPeach();
    
    if(getX() == m_yoshi->getX() && getY() == m_yoshi->getY() && m_yoshi->getState() == 1)
    {
        if(m_yoshi->getCoin() > 5)
        {
            m_yoshi->subcoin(5);
            getWorld()->addDeposit(5);
            getWorld()->playSound(SOUND_DEPOSIT_BANK);
        }
        else{
            m_yoshi->subcoin(m_yoshi->getCoin());
            getWorld()->addDeposit(m_yoshi->getCoin());
            getWorld()->playSound(SOUND_DEPOSIT_BANK);
        }
    }
    if(getX() == m_peach->getX() && getY() == m_peach->getY() && m_peach->getState() == 1)
    {
        if(m_peach->getCoin() > 5)
        {
            m_peach->subcoin(5);
            getWorld()->addDeposit(5);
            getWorld()->playSound(SOUND_DEPOSIT_BANK);
        }
        else{
            m_peach->subcoin(m_peach->getCoin());
            getWorld()->addDeposit(m_peach->getCoin());
            getWorld()->playSound(SOUND_DEPOSIT_BANK);
        }
    }
    
    
    if(getX() == m_yoshi->getX() && getY() == m_yoshi->getY() && m_yoshi->getState() == 0)
    {
        m_yoshi->addcoin(getWorld()->getDeposit());
        getWorld()->resetDeposit();
        getWorld()->playSound(SOUND_WITHDRAW_BANK);
    }
    if(getX() == m_peach->getX() && getY() == m_peach->getY() && m_peach->getState() == 0)
    {
        m_peach->addcoin(getWorld()->getDeposit());
        getWorld()->resetDeposit();
        getWorld()->playSound(SOUND_WITHDRAW_BANK);

    }
};

void droppingSquare::doSomething()
{
    m_yoshi = getWorld()->getYoshi();
    m_peach = getWorld()->getPeach();
    
    int i = randInt(1,2);
    if(getX() == m_yoshi->getX() && getY() == m_yoshi->getY() && m_yoshi->getState() == 0 && m_yoshi->getNew() == true)
    {
        switch (i) {
            case 1:
            {
                if(m_yoshi->getCoin() >= 10)
                    m_yoshi->subcoin(10);
                else
                    m_yoshi->subcoin(m_yoshi->getCoin());
                break;
            }
                
            case 2:
            {
                if(m_yoshi->getStars() >= 1)
                    m_yoshi->subStars(1);
                break;
            }
                
        }
        getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
        m_yoshi->notNew();
    }
    if(getX() == m_peach->getX() && getY() == m_peach->getY() && m_peach->getState() == 0 && m_peach->getNew() == true)
    {
        switch (i) {
            case 1:
            {
                if(m_peach->getCoin() >= 10)
                    m_peach->subcoin(10);
                else
                    m_peach->subcoin(m_peach->getCoin());
                break;
            }
                
            case 2:
            {
                if(m_peach->getStars() >= 1)
                    m_peach->subStars(1);
                break;
            }
                
        }
        getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
        m_peach->notNew();
    }
    
}


void left_dir_square::doSomething()
{
    setDirection(left);
    m_yoshi = getWorld()->getYoshi();
    m_peach = getWorld()->getPeach();
    if(getX() == m_yoshi->getX() && getY() == m_yoshi->getY())
    {
        m_yoshi->setUltimateDirection(left);
        if(m_yoshi->getState() == 1)
        {
            m_yoshi->moveTo(m_yoshi->getX() - 16, m_yoshi->getY());
            m_yoshi->decreaseTicks();
        }
    }
    if(getX() == m_peach->getX() && getY() == m_peach->getY())
    {
        m_peach->setUltimateDirection(left);
        if(m_peach->getState() == 1)
        {
            m_peach->moveTo(m_peach->getX() - 16, m_peach->getY());
            m_peach->decreaseTicks();
        }
    }
    
};

void right_dir_square::doSomething()
{
    setDirection(right);
    m_yoshi = getWorld()->getYoshi();
    m_peach = getWorld()->getPeach();
    if(getX() == m_yoshi->getX() && getY() == m_yoshi->getY())
    {
        m_yoshi->setUltimateDirection(right);
        if(m_yoshi->getState() == 1){
            m_yoshi->moveTo(m_yoshi->getX() + 16, m_yoshi->getY());
            m_yoshi->decreaseTicks();
        }
    }
    if(getX() == m_peach->getX() && getY() == m_peach->getY())
    {
        m_peach->setUltimateDirection(right);
        if(m_peach->getState() == 1)
        {
            m_peach->moveTo(m_peach->getX() + 16, m_peach->getY());
            m_peach->decreaseTicks();
        }
    }
    
};

void up_dir_square::doSomething()
{
    setDirection(up);
    m_yoshi = getWorld()->getYoshi();
    m_peach = getWorld()->getPeach();
    if(getX() == m_yoshi->getX() && getY() == m_yoshi->getY())
    {
        m_yoshi->setUltimateDirection(up);
        if(m_yoshi->getState() == 1)
        {
            m_yoshi->moveTo(m_yoshi->getX(), m_yoshi->getY() + 16);
            m_yoshi->decreaseTicks();
        }
    }
    if(getX() == m_peach->getX() && getY() == m_peach->getY())
    {
        m_peach->setUltimateDirection(up);
        if(m_peach->getState() == 1)
        {
            m_peach->moveTo(m_peach->getX(), m_peach->getY() + 16);
            m_peach->decreaseTicks();
        }
    }
    
};

void down_dir_square::doSomething()
{
    setDirection(down);
    m_yoshi = getWorld()->getYoshi();
    m_peach = getWorld()->getPeach();
    if(getX() == m_yoshi->getX() && getY() == m_yoshi->getY())
    {
        m_yoshi->setUltimateDirection(down);
        if(m_yoshi->getState() == 1){
            m_yoshi->moveTo(m_yoshi->getX(), m_yoshi->getY() - 16);
            m_yoshi->decreaseTicks();
        }
    }
    if(getX() == m_peach->getX() && getY() == m_peach->getY())
    {
        m_peach->setUltimateDirection(down);
        if(m_peach->getState() == 1)
        {
            m_peach->moveTo(m_peach->getX(), m_peach->getY() - 16);
            m_peach->decreaseTicks();
        }
    }
};





void playerAvatar::addcoin(int num)
{
    m_coins += num;
}

void playerAvatar::subcoin(int num)
{
    
    if(m_coins - num > 0)
    {
        m_coins -= num;
    }
    else
        m_coins = 0;
    
}

bool Actor::canMove(int dir) {
    
    dir = dir % 360;
    
    double dest_x = getX(), dest_y = getY();
    switch (dir) {
    case up:
        dest_y += SPRITE_HEIGHT;
        break;
    case down:
        dest_y -= SPRITE_HEIGHT;
        break;
    case left:
        dest_x -= SPRITE_WIDTH;
        break;
    case right:
        dest_x += SPRITE_WIDTH;
        break;
    }
    return getWorld()->validPos(dest_x, dest_y);
}

void playerAvatar::doSomething()
{

    if(ticks_to_move < 0)
        return;
    
    if(state == 0)
    {
        
        if(isTeleported())
        {
            if(!canMove(ultimate_direction))
            {
                int dir = randInt(0, 4) * 90;
                while(!canMove(dir))
                    dir = randInt(0,4) * 90;
                ultimate_direction = dir;
            }
            notTeleported();
            std::cerr << 3<< std::endl;
            if(isStart())
                notStart();
        }
        
        switch (getWorld()->getAction(m_playerNumber))
        {
            case ACTION_NONE:
                return;
                break;
            case ACTION_FIRE:
                if(m_vortex == 1)
                {
                    getWorld()->newVortex(getX(), getY(), ultimate_direction);
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    m_vortex = 0;
                }
                break;
            case ACTION_ROLL:
                int die_roll = randInt(1, 10);
                ticks_to_move = die_roll * 8;
                state = 1;
                
                break;
        }
    }
    
    if(state == 1)
    {
        if(isStart())
        {
            if(canMove(right))
            {
                for(int i = 0; i < 8; i++)
                    moveTo(getX() + 2, getY());
                ticks_to_move -= 8;
                ultimate_direction = right;
                notStart();
            }
            else
            {
                notStart();
                int dir = randInt(0,3);
                while(!canMove(dir * 90))
                    dir = randInt(0, 3);
                ultimate_direction = dir * 90;
                std::cerr << 1;
                switch (dir) {
                    case right:
                        ticks_to_move -= 8;
                        moveTo(getX() + 16, getY());
                        break;
                    case up:
                        ticks_to_move -= 8;
                        moveTo(getX(), getY() + 16);
                        break;
                    case down:
                        ticks_to_move -= 8;
                        moveTo(getX(), getY() - 16);
                        break;
                    case left:
                        ticks_to_move -= 8;
                        moveTo(getX() + 16, getY());
                        setDirection(left);
                        break;
                }
            }
        }
        
        if(ticks_to_move == 0)
        {
            state = 0;
            return;
        }
            
        

        if(canMove(ultimate_direction) && (canMove(ultimate_direction + 90) || canMove(ultimate_direction + 270)))
        {
            if(ultimate_direction == right)
            {
                switch(getWorld()->getAction(m_playerNumber))
                {
                    case ACTION_UP:
                    {
                        if(canMove(up))
                            setUltimateDirection(up);
                        else
                            return;
                        break;
                    }
                    case ACTION_DOWN:
                    {
                        if(canMove(down))
                            setUltimateDirection(down);
                        else
                            return;
                        break;
                    }
                    case ACTION_RIGHT:
                    {
                        if(canMove(right))
                            setUltimateDirection(right);
                        else
                            return;
                        break;
                    }
                    default:
                        return;
                }
            }
            else if(ultimate_direction == up)
            {
                switch(getWorld()->getAction(m_playerNumber))
                {
                    case ACTION_LEFT:
                    {
                        if(canMove(left))
                        {
                            setDirection(left);
                            setUltimateDirection(left);
                        }
                        else
                            return;
                        break;
                    }
                    case ACTION_RIGHT:
                    {
                        if(canMove(right))
                            setUltimateDirection(right);
                        else
                            return;
                        break;
                    }
                    case ACTION_UP:
                    {
                        if(canMove(up))
                            setUltimateDirection(up);
                        else
                            return;
                        break;
                    }
                    default:
                        return;
                }

            }
            
            else if(ultimate_direction == left)
            {
                switch(getWorld()->getAction(m_playerNumber))
                {
                    case ACTION_UP:
                    {
                        if(canMove(up))
                        {
                            setDirection(right);
                            setUltimateDirection(up);
                        }
                        else
                            return;
                        break;
                    }
                    case ACTION_DOWN:
                    {
                        if(canMove(down))
                        {
                            setDirection(right);
                            setUltimateDirection(down);
                        }
                        else
                            return;
                        break;
                    }
                    case ACTION_LEFT:
                    {
                        if(canMove(left))
                        {
                            setDirection(left);
                            setUltimateDirection(left);
                        }
                        else
                            return;
                        break;
                    }
                    default:
                        return;
                }
            }
            else if(ultimate_direction == down)
            {
                switch(getWorld()->getAction(m_playerNumber))
                {
                    case ACTION_LEFT:
                    {
                        if(canMove(left))
                        {
                            setDirection(left);
                            setUltimateDirection(left);
                        }
                        else
                            return;
                        break;
                    }
                    case ACTION_RIGHT:
                    {
                        if(canMove(right))
                            setUltimateDirection(right);
                        else
                            return;
                        break;
                    }
                    case ACTION_DOWN:
                    {
                        if(canMove(down))
                            setUltimateDirection(down);
                        else
                            return;
                        break;
                    }
                    default:
                        return;
                }
            }
            else
                return;
            
        }
          
        else {
            
            if(ticks_to_move % 8 == 0)
            {
                if(!canMove(ultimate_direction))
                {
                    if(ultimate_direction == right)
                    {
                        
                        if(canMove(up))
                            ultimate_direction = up;
                        else
                            ultimate_direction = down;
                    }
                    
                    else if(ultimate_direction == up)
                    {
                        if(canMove(right))
                            ultimate_direction = right;
                        else
                        {
                            setDirection(left);
                            ultimate_direction = left;
                        }
                    }
                    
                    else if(ultimate_direction == left)
                    {
                        if(canMove(up))
                        {
                            setDirection(right);
                            ultimate_direction = up;
                        }
                        else
                        {
                            setDirection(right);
                            ultimate_direction = down;
                        }
                    }
                    
                    else if(ultimate_direction == down)
                    {
                        if(canMove(right))
                            ultimate_direction = right;
                        else
                        {
                            setDirection(left);
                            ultimate_direction = left;
                        }
                    }
                    else{return;}
                }
            }
        }
        m_new = true;
        m_newForBaddies = true;
        if(ticks_to_move == 0)
        {
            state = 0;
            return;
        }
        
        switch (ultimate_direction) {
            case up:
                moveTo(getX(), getY() + 2);
                break;
            case down:
                moveTo(getX(), getY() -  2);
                break;
            case right:
                moveTo(getX() + 2, getY());
                break;
            case left:
                moveTo(getX() - 2, getY());
                break;
                
        }
        ticks_to_move--;
        m_new = true;
        m_newForBaddies = true;
        
        if(ticks_to_move == 0)
            state = 0;
        
    }
}
