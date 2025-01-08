#ifndef LINKLOGGAME_HPP
#define LINLLOGGAME_HPP

#include "elements.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

#define FALL_POS 800
#define BASE_LIFE 3

enum level {
    MAX_LEVEL=3,
    FIRST_LEVEL=1
};

enum side {
    TOP,
    BOT,
    LEFT,
    RIGHT
};

class Linker 
{
private:
    Racket* racket_ = new Racket();
    Ball* ball_ = new Ball();
    Blocs* blocs_ = new Blocs();
    std::vector<Bonus*> bonus_vect_;
    unsigned short score_=0;
    unsigned short life_=3;
    unsigned short level_=1;
    unsigned short pb_=0;
    char act_bonus=NOBONUS;
    char prev_bonus=NOBONUS;
    bool collision_done=false;
    bool catch_ball=false;
    time_t timer_catch=-1;
    time_t timer_speed=-1;
    time_t timer_laser=-1;
    virtual void enlargementBonus();
    virtual void unenlargementBonus();
    virtual void healBonus();
    virtual void catchBonus();
    virtual void unCatchBonus();
    virtual void slowBonus();
    virtual void unSlowBonus();
    virtual void checkSpeed();

public:
    Linker();
    virtual ~Linker();
    virtual void checkInteractionRB();
    virtual void checkInteractionBB();
    virtual void checkInteractionCB(int,int);
    virtual unsigned short int getScore();
    virtual unsigned short getLife();
    virtual int checkLoosedBall();
    virtual void resetBall();
    virtual void moveBall();
    virtual void moveRacketRight();
    virtual void moveRacketLeft();
    virtual void resetLife();
    virtual Blocs* getBlocs();
    virtual Ball* getBall();
    virtual Racket* getRacket();
    virtual void resetBlocs();
    virtual void setScore(int);
    virtual void resetScore();
    virtual void moveRacketTo(int);
    virtual unsigned getMaxScore();
    virtual void loadLevel(unsigned short,short);
    virtual unsigned short getLevel();
    virtual void nextLevel();
    virtual void prevLevel();
    virtual void setLevel(unsigned short);
    virtual unsigned short getMaxLevel();
    virtual unsigned short getPB();
    virtual void setPB(unsigned short,unsigned);
    virtual void resetPB(unsigned short);
    virtual void getPBfromFile(unsigned short);
    virtual void updatePB(unsigned short);
    virtual int getNbrBonus();
    virtual Bonus* getBonus(unsigned short);
    virtual void checkBonusPos();
    virtual void checkInteractionRBonus();
    virtual void handleBonus();
    virtual void unHandleBonus();
    virtual void resetTimerCatch();
    virtual char getActBonus();
    virtual int shootLaser();
    virtual bool startMovingBall();
};

#endif